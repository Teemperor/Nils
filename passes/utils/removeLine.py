#!/usr/bin/env python

import sys
import random

argfile = open(sys.argv[1], "r")

lines = [line.strip() + "\n" for line in argfile]
argfile.close()

if len(lines) == 0:
  exit(0)
lines.pop(random.randint(0,len(lines)-1))

argfile = open(sys.argv[1], "w")
argfile.writelines(lines)
