#!/bin/bash

nils_exe="$1"
source_dir="$2"
tmp_dir="$3"

set -e

mkdir -p "$tmp_dir"

rm -rf "$tmp_dir"

cp -r "$source_dir" "$tmp_dir"
cd "$tmp_dir"
"$nils_exe"

diff -r "$source_dir.expected" "$tmp_dir"