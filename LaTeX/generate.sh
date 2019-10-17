#!/bin/bash 

filename="paper-wallet"

if [ "$#" -ne 2 ]; then
  echo "$0 requires 2 arguments, $# provided"
  exit 1
fi

xelatex "\\def\\address{$1}\\def\\private{$2}\\include{$filename}" > /dev/null 2>&1
if [ "$?" -ne 0 ]; then
  echo "Unable to compile $filename.tex"
  exit 1
fi

rm -f $filename.aux
rm -f $filename.log
xdg-open $filename.pdf
