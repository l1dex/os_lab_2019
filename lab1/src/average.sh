#!/bin/bash

sum=0
count=0
for var in $(cat $1)
do
(( count += 1 ))
(( sum += var))
done
echo $count count
echo $sum sum
let "res = (($sum/$count |bc -l))"
echo $res answer