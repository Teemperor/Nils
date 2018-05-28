#!/bin/bash

set -e
set -o pipefail

echo "$PWD"

output=$(timeout 0.3s python prog.py 2>&1)
echo "$output" | grep -q "^4 bottles of beer on the wall,$"
