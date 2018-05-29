#!/bin/bash

nils_exe="$1"
source_dir="$2"
tmp_dir="$3"

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export NILS_PASS_DIRS="$DIR/../passes"

mkdir -p "$tmp_dir"

rm -rf "$tmp_dir"

cp -r "$source_dir" "$tmp_dir"
cd "$tmp_dir"
"$nils_exe" --batch

rm -rf "$tmp_dir/.git"

diff -r -U3 "$source_dir.expected" "$tmp_dir"