#!/bin/bash

rm -f my_out.txt

OUT_FILE="my_out.txt"
TA_OUT="ta_out.txt"
EXEC_NAME="pagesim"

for i in {1..3}; do
    for j in {3,4,5}; do
        echo "--------------Running test$i.txt with $j frames--------------" >> $OUT_FILE
        ./$EXEC_NAME $j < test"$i".txt >> $OUT_FILE
    done
done

DIFF=$(diff -w $OUT_FILE $TA_OUT)

if [ $? -eq 0 ] && [ "$DIFF" = "" ]
then
    echo "Same outputs✌. But do more tests to make sure that it is bug-free"
else
    echo "Seems like we have different outputs☠"
fi

