#!/bin/bash

DOT_FILES_FOLDER="tests/Stanford"
DOT_FILES=()

mapfile -t DOT_FILES < <(find "$DOT_FILES_FOLDER" -type f -iregex ".*\.\(dot\)$")

if [[ "${#DOT_FILES_FOLDER[@]}" -eq 0 ]]; then
	printf "No dot files (.dot) found...\n"
	exit
fi

for i in "${DOT_FILES_FOLDER[@]}"
do
  dot -Tpdf "$i" -o "$i.pdf"
done
