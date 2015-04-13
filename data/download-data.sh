#!/bin/bash

curl --output mobile.cls.txt -O http://libal.eu/eph/uastrings/sample1.txt 

if [ "$?" -eq 0 ]; then
	echo "Downloaded data files";
else
    echo "Error downloading the data file";
fi

exit 0;
