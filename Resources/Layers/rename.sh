#!/bin/sh
prefix=$1

count=0
for f in *.jpg; do
    mv "$f" "$prefix$count".jpg
    count=`expr $count + 1`
done