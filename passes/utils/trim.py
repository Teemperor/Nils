#!/usr/bin/env python

import sys

argfile = open(sys.argv[1], "r")

lines = [line.strip() + "\n" for line in argfile]

argfile.close()
argfile = open(sys.argv[1], "w")
argfile.writelines(lines)
