#!/usr/bin/ruby
# -*- coding: utf-8 -*-

#
#     iva - Invader VM Assembler
#
#     Original Copyright (C) 2013 - 2014 Ron Hashimoto.
#                                         http://h2so5.net/
#                                         mail@h2so5.net
#     Portions Copyright (C) <year> <author>
#                                         <website>
#                                         <e-mail>
#     All rights reserved.
#
#     ------------------------------------------------------------------------
#
#     GNU GENERAL PUBLIC LICENSE (GPLv3)
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License,
#     or (at your option) any later version.
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.ß
#     See the GNU General Public License for more details.
#     You should have received a copy of the GNU General Public License
#     along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#     このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
#     GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
#     再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
#     *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
#     詳しくは GNU 一般公衆利用許諾書をご覧ください。
#     あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
#     もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
#

require 'yaml'
require 'csv'

begin
  require 'fastlz'
rescue LoadError
  puts "warning: fastlz gem not found"
end

class Iva
  class ParseError < Exception; end

  def initialize(conf)
    @registers = Hash.new
    conf['registers'].each_with_index do |val, i|
      @registers[val.intern] = i
    end
    
    @instructions = Hash.new
    conf['instructions'].each_with_index do |val, i|
      name = val.shift.intern
      @instructions[name] = {idx: i, args: val.map {|v| v.intern}}
    end
    
    @constants = Hash.new
    conf['constants'].each do |enum|
      if enum.is_a? Array
        enum.each_with_index do |val, i|
          @constants[val.intern] = i
        end
      elsif enum.is_a? Hash
        enum.keys.each do |key|
          @constants[key.intern] = enum[key]
        end
      end
    end

    conf['functions'].each_with_index do |val, i|
      @constants[val.intern] = i + 1
    end

    @code_size = conf['code_size'].to_i

    @constants.merge! @registers
    @code = []
  end

  def get_const(const)
    begin
      n = Integer(const)
    rescue
      sym = const.intern
      raise ParseError.new("unknown constant: #{const}") unless @constants.key? sym
      n = @constants[sym]
    ensure
      n
    end
  end

  def parse_arg(type, arg)
    case type
    when :R
      sym = arg.intern
      raise ParseError.new("unknown register: #{arg}") unless @registers.key? sym
      n = [@registers[sym], 'C', 1]
    when :L
      n = get_const(arg)
      if (0..4294967295).include? n
        # little endian unsigned 32bit
        n = [n, 'V', 4]
      elsif (-2147483648..-1).include? n
        # little endian signed 32bit
        n = [(-n - 1 | (1 << 31)), 'V', 4]
      else
        raise ParseError.new("out of range: #{n}")
      end
    when :I
      n = get_const(arg)
      raise ParseError.new("out of range(0..65535): #{n}") unless (0..65535).include? n
      # little endian unsigned 16bit
      n = [n, 'v', 2]
    when :C
      n = get_const(arg)
      raise ParseError.new("out of range(0..255): #{n}") unless (0..255).include? n
      # unsigned 8bit
      n = [n, 'C', 1]
    when :V
      n = [arg, "A#{arg.size}", arg.size]
    when :O
      n = arg.intern
      # signed 8bit
      n = [n, 'c', 1]
    end
    n
  end

  def parse(filename)
    code = []
    labels = {}
    line = 1
    bytes = 0
    CSV.foreach(filename, "r") do |row|
      row = row.map {|item| item.strip}
      unless row.empty?
        next if row[0].start_with? '#' # skip comment

        # label
        if row.size == 1 && row[0].end_with?(":")
          labels[row[0].intern] = bytes
          next
        end

        begin
          # check instruction
          inst = row.shift.intern
          unless @instructions.key? inst
            raise ParseError.new("unknown instruction: #{inst}")
          end
          
          # check arguments
          inst_spec = @instructions[inst]
          args = inst_spec[:args]
          if row.size != args.size
            raise ParseError.new("wrong number of arguments: #{inst} expects #{args.size}")
          end
          
          code << [inst_spec[:idx], 'C', 1]
          bytes += 1
          for i in 0..(args.size - 1)
            c = parse_arg(args[i], row[i])
            bytes += c[2]
            if args[i] == :O
              c[0] = [c[0], bytes]
            end
            code << c
          end
        rescue Iva::ParseError => e
          puts "[#{line}] parse error: #{e.message}"
          return
        end
      end
      line += 1
    end

    code = code.map do |c|
      if c[0].is_a? Array
        [labels[c[0][0]] - c[0][1],c[1], c[2]]
      else
        c
      end
    end
    @code = code
  end

  def dump
    val = []
    fmt = ""
    @code.each do |b|
      val << b[0]
      fmt += b[1]
    end
    bin = val.pack fmt
    if bin.size > @code_size
      puts "warning: the code size exceeds #{@code_size}bytes"
    end
    if defined? FastLZ
      comp = FastLZ.compress(bin)
      if comp.size + 1 < bin.size
        bin = [@instructions[:COMPRESSED][:idx]].pack('C') + comp
      end
    end
    bin
  end
end

vmconf = YAML.load_file(File.dirname(__FILE__) + '/vm.yml')

iva = Iva.new(vmconf)
iva.parse(ARGV[0])
print iva.dump.unpack('H*').first + "\n"
