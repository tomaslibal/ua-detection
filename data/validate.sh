#!/bin/bash

while read line
do
    bin/uadet --uas "$line" --cmp_all
    echo -e "\n\n"
done < data/uas_validation_no_class.txt
