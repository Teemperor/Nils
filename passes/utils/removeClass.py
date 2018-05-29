#!/usr/bin/env python

import sys
import random

argfile = open(sys.argv[1], "r")

lines = [l for l in argfile]
argfile.close()

if len(lines) == 0:
  exit(0)

start_index = random.randint(0,len(lines)-1)

def removeClass():
  for offset in range(0, len(lines)):
    index = (offset + start_index) % len(lines)
    line = lines[index]
    if line.lstrip().startswith("};"):
      class_end = index + 1
      indentation = len(line) - len(line.lstrip())
      while index >= 0:
        index -= 1
        line = lines[index]
        print("LINE:" + line)
        if line.lstrip().startswith("public:"):
          continue
        if line.lstrip().startswith("private:"):
          continue
        if line.lstrip().startswith("protected:"):
          continue
        new_indentation = len(line) - len(line.lstrip())
        print("Checking")
        print(new_indentation)
        print(indentation)

        if indentation == new_indentation:
          return lines[0:index] + lines[class_end:]
  return lines

lines = removeClass()

argfile = open(sys.argv[1], "w")
argfile.writelines(lines)
