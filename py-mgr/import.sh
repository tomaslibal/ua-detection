#!/usr/bin/env bash

function import_one {
    ua=$1
    label1=$2
    curl -G --data-urlencode "dp=${ua}" --data-urlencode "cat1=${label1}" localhost:8080/add &> /dev/null
}

filename=data_in.txt
while read -r cat ua
do
    echo "read ua=${ua} with category=${cat}"
    import_one "${ua}" "${cat}"
done < "$filename"

# import_one "mozilla/5.0 test4" "desktop"
