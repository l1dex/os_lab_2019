#!/bin/bash
MAXCOUNT=150
count=1
while [ "$count" -le $MAXCOUNT ]
do
head -c1 /dev/urandom | hexdump -d | sed -E '1!d;s/.+ +0+//' >> numbers.txt
let "count += 1"
done