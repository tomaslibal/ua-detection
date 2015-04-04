## Device detection

### Introduction

This is an experimental repository about device detection for the World Wide
Web. I'm mostly interested in device detection for the user agents connecting
to web servers through the HTTP requests.

There are two end goals that I would like achieve:

1. Determine if a device belongs to a given group

   Say, you define a group 'Smart TVs' and you get a user-agent string, this
   detection should tell if the string comes possibly from a device that belongs to
   the group or not.

2. Uniquely determine a device for a given user-agent string

   This detection should tell what device(s) likely belong to the supplied user-agent
   string.
   
### ua-detection

`uadet` is the implementation of a classification program that uses supervised
learning method to learn from data and makes predictions about new data as to which
class the input may belong. 

#### dependencies

Needed to build the binaries:

- make utility
- GCC or CLANG (c99)

Use the download script for data:

- UNIX like system with shell

#### build

    make all

#### usage

*default usage:*

    bin/uadet --uas "Mozilla/5.0 ..." --group mobile
    
Prints to stdout the output of the `mobile` classifier.

run a user-agent string against all classifiers (that have been seen in the training data):

    bin/uadet --uas "Mozilla/5.0 ..." --cmp_all

### Data

More in the [data section](data/)

> You can contribute by sending additional user-agent string data either 
> labelled or unlabelled. 
>  
> All content must be publicly available and compatible with the [GNU License](LICENSE)

Only sample and test data are part of this repository. Some datasets are
available online and can be downloaded using [the download script](data/download-data.sh).

    cd data/
    /bin/sh download-data.sh

#### Data format

The program can work with ascii (8-bit) encoded data of the following format:

1. one entry per line terminated by a new line character
2. each entry has a one word label separated by a space character from the user-agent string following the label

A user-agent string with a class "mobile" would look like this:

    mobile Mozilla/5.0 (Linux; U; Android 4.0; en-us) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

See the [example](data/uas_with_class.txt) 

#### uadb

`uadb` is a storing mechanism for <user-agent-string, [classes]>, e.g.

    Mozilla/5.0 (Android; en_us... [Mobile, Android, en_us]
    Mozilla/5.0 (Linux; X11... [Desktop, Linux] 
      
To see usage, execute `uadb --help`

### Test

`make test` compiles the test suite and runs it. If stdout prints "ALL TESTS
PASSED" the suite returned with no errors. Otherwise, it will stop on the first
error printing the test file and corresponding line number where the error
occured.

### Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

[Full license](https://github.com/tomaslibal/ua-detection/blob/master/LICENSE)

---

- March 2015: wrote and added a probability based algorithm
- August 2014: added a user-agent string tokenizer
- April-May 2014: first attempt to create a model for predicting the class of an UA input

Tomas <tomas@libal.eu>
