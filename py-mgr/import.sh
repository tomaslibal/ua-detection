#!/usr/bin/env bash

#
# import.sh    Import script that reads an ASCII text data file containing two
#              columns of data separated by a whitespace character. The first
# column is one or more categories separated by a comma and the second column
# is the datapoint (user-agent string). The data is then processed in this
# manner and send to a URL on the localhost in a GET HTTP request. This end
# point is assumed to be the 'httpend.py' server from py-mgr.
#
# <example>
#     echo "mobile Mozilla/5.0 (Linux; Phone Android Cn-cn) Firefox/45.0" > data_in.txt
#     ./import.sh
# </example>

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
