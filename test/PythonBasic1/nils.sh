#!/bin/bash

set -e
set -o pipefail

echo "$PWD"

output=$(timeout 0.3s python prog.py) 
echo "$output" | grep -q "90 bottles of beer on the wall"
