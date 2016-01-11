## Device detection

### Introduction

This is an experimental repository for device detection on the World Wide
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

`uadet2` is an implementation of a classification program that uses supervised
learning method to learn from data and makes predictions about new data as to which
class the input may belong.

#### dependencies

Needed to build the binaries:

- CMake 2.6+
- GNU Make utility
- C++11 Compiler
- CppUnit for tests
- CTest 2.6+ for tests

#### build

    cmake .
    make

This outputs the background service (`uadet2d`) and the client CLI program (`uadet2cli`) binaries in `dist/`.

#### usage

*default usage:*

    $1 dist/uadet2d # start this process in one shell and keep it running
    $2 dist/uadet2cli localhost 10128 "eval desktop Mozilla/5.0 (Linux..."

Prints to stdout the output of classification of the given user agent string 
against all known categories present in the data file.

[See a Manual: basic usage of uadet2](doc/uadet.md)

### Data

The background service will look for a file named `data_in.txt` in the current working directory. This file will be read prior to the evaluation of the user-agent string.

#### Data format

The program can work with ascii (8-bit) encoded data of the following format:

1. one entry per line terminated by a new line character
2. each entry has a one word label separated by a tab character from the user-agent string following the label

A user-agent string with a class "mobile" would look like this:

    mobile  Mozilla/5.0 (Linux; U; Android 4.0; en-us) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

See the [example](/data_in.txt) 

### Test

Run all tests and print output

    ctest .

### Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

[Full license](https://github.com/tomaslibal/ua-detection/blob/master/LICENSE)

---

- December 2015: ported to a C++ version
- March 2015: wrote and added a probability based algorithm
- August 2014: added a user-agent string tokenizer
- April-May 2014: first attempt to create a model for predicting the class of an UA input

Tomas <tomas@libal.eu>
