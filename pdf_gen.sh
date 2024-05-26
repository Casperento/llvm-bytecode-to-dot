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

  IMG_FILES=()
  for j in "${DOT_FILES[@]}"
  do
    printf "Converting %s to PDF...\n" "$j"
    dot -Tpng "$j" -o "$j.pdf"
    if which img2pdf >/dev/null 2>&1; then
      dot -Tpng "$j" -o "$j.png"
      IMG_FILES+=("$j.png")
    fi
  done

  if which img2pdf >/dev/null 2>&1; then
    printf "Merging PDFs, for %s, into a single one...\n" "$i"
    img2pdf "${IMG_FILES[@]}" -o "${i}_merged.pdf"
  fi

done
