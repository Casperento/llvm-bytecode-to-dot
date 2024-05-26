#!/bin/bash
# This script assumes it is being executed inside the llvm-bytecode-to-dot repo folder
#
DOT_FILES_FOLDER="tests/Stanford"

SRC_FILES=()
mapfile -t SRC_FILES < <(find "$DOT_FILES_FOLDER" -type f -regex ".*\.ll$")
if [[ "${#SRC_FILES[@]}" -eq 0 ]]; then
  printf "No source files (.ll) found...\n"
  exit
fi

for i in "${SRC_FILES[@]}"
do
  DOT_FILES=()
  mapfile -t DOT_FILES < <(find "$DOT_FILES_FOLDER" -type f -regex "\($i\).*\.dot$")
  if [[ "${#DOT_FILES[@]}" -eq 0 ]]; then
    printf "No related dot files (.dot) found to %s...\n" "$i"
  fi

  for j in "${DOT_FILES[@]}"
  do
    printf "Converting %s to PDF...\n" "$j"
    dot -Tpdf "$j" -o "$j.pdf"
  done
done
