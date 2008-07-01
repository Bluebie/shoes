require 'irb/ruby-lex'
require 'stringio'

class MimickIRB < RubyLex
  attr_accessor :started

  class Continue < StandardError; end
  class Empty < StandardError; end

  def initialize
    super
    set_input(StringIO.new)
  end

  def run(str)
    obj = nil
    @io << str
    @io.rewind
    unless l = lex
      raise Empty if @line == ''
    else
      case l.strip
      when "reset"
        @line = ""
      when "time"
        @line = "puts %{You started \#{IRBalike.started.since} ago.}"
      else
        @line << l << "\n"
        if @ltype or @continue or @indent > 0
          raise Continue
        end
      end
    end
    unless @line.empty?
      obj = eval @line, TOPLEVEL_BINDING, "(irb)", @line_no
    end
    @line_no += @line.scan(/\n/).length
    @line = ''
    @exp_line_no = @line_no

    @indent = 0
    @indent_stack = []

    $stdout.rewind
    output = $stdout.read
    $stdout.truncate(0)
    $stdout.rewind
    [output, obj]
  rescue Object => e
    case e when Empty, Continue
    else @line = ""
    end
    raise e
  ensure
    set_input(StringIO.new)
  end

end

CURSOR = ">>"
IRBalike = MimickIRB.new
$stdout = StringIO.new

Shoes.app do
  @str, @cmd = [CURSOR + " "], ""
  @buffers = {:next => [], :previous => []}
  stack :width => 1.0, :height => 1.0 do
    background "#555"
    stack :width => 1.0, :height => 50 do
      para "Interactive Ruby ready.", :fill => white, :stroke => red
    end
    @scroll =
      stack :width => 1.0, :height => -50, :scroll => true do
        background "#555"
        @console = para @str, :font => "Monospace 12px", :stroke => "#dfa"
        @console.cursor = -1
      end
  end
  keypress do |k|
    case k
    when "\n"
      begin
        out, obj = IRBalike.run(@cmd)
        @str += ["#@cmd\n",
          span("#{out}=> #{obj.inspect}\n", :stroke => "#fda"),
          "#{CURSOR} "]
        @buffers[:previous] << @cmd
        @console.cursor = -1
        @cmd = ""
      rescue MimickIRB::Empty
      rescue MimickIRB::Continue
        @str += ["#@cmd\n.. "]
        @cmd = ""
      rescue Object => e
        @str += ["#@cmd\n", span("#{e.class}: #{e.message}\n", :stroke => "#fcf"),
          "#{CURSOR} "]
        @cmd = ""
      end
    when String
      pos = @cmd.length + @console.cursor + 1
      @cmd = @cmd[0...pos].to_s + k + @cmd[pos...@cmd.length].to_s
    when :backspace
      @cmd[@console.cursor] = '' unless @console.cursor < -@cmd.length
    when :tab
      @cmd += "  "
    when :alt_q
      quit
    when :alt_c
      self.clipboard = @cmd
    when :alt_v
      pos = @cmd.length + @console.cursor + 1
      @cmd = @cmd[0...pos].to_s + self.clipboard + @cmd[pos...@cmd.length].to_s
    when :up
      unless @buffers[:previous].empty?
        @buffers[:next].unshift @cmd
        @cmd = @buffers[:previous].pop
      end
    when :down
      unless @buffers[:next].empty?
        @buffers[:previous] << @cmd
        @cmd = @buffers[:next].shift
      end
    when :left
      @console.cursor -= 1 unless @console.cursor < -@cmd.length
    when :right
      @console.cursor += 1 unless @console.cursor == -1
    when :home
      @console.cursor = -@cmd.length - 1
    when :end
      @console.cursor = -1
    end
    
    @console.replace *(@str + [@cmd])
    @scroll.scroll_top = @scroll.scroll_max
  end
end
