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

Currently the program is both the backend and the client in one executable. This also means that each time the program is invoked, the learning process has to be completed. The learning part would be ideally separated from the querying to optimize resources and to lower the latency.

#### dependencies

Needed to build the binaries:

- GNU Make utility
- C++11 Compiler
- CppUnit for tests

#### build

    make

This outputs an executable binary in `dist/uadet2`.

#### usage

*default usage:*

    dist/uadet2 --ua="Mozilla/5.0 ..."
    
Prints to stdout the output of classification of the given user agent string 
against all known categories present in the data file.

### Data

#### Data format

The program can work with ascii (8-bit) encoded data of the following format:

1. one entry per line terminated by a new line character
2. each entry has a one word label separated by a tab character from the user-agent string following the label

A user-agent string with a class "mobile" would look like this:

    mobile  Mozilla/5.0 (Linux; U; Android 4.0; en-us) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

See the [example](/data_in.txt) 

### Test

> Tests are currently not runnable from make! I switched to CMake build tool because it 
> supports multiple binary targets in the project (client & server binaries). This caused
> a loss of NetBeans generated `build-tests` target which was used to compile test runners.
> I will add test compilation back and a test runner in the future.

`make test` compiles the test suite and runs it. There are multiple test runners for each
class and they print OK(###) if all tests within that runner passed (### is the number of
passing tests). Otherwise, it prints the test errors with the message (if the message was
defined).

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
