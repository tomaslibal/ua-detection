#!/bin/sh

curl -O http://libal.eu/eph/uastrings/sample1.txt

if [ "$?" -eq 0 ]; then
	echo "Downloaded data files";
fi

exit 0;