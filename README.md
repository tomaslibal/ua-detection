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

![Sample result](/doc/ua_eval_test.png)

In the figure above you can see a sample result of evaluating a user-agent string constrained to top 10 labels. Features 3-10 were evaluated extremely unlikely. This data set had 17 features in total and only two features evaluated positively as expected for the given input.

The scores for each of the first 10 classes which are shown in the figure, were as follows:

```
android:43.3779
mobile:6.62649
windows:0.0973025
macos:0.0525497
linux:0.0281889
opera:0.0221725
ipad:0.0079283
unknown:0.00355648
bot:0.00210765
blackberry:0.000911062
```

### Build the project from source

Currently no binaries are distributed. Here's a manual how to build it from the source code.

#### Dependencies

Most *nix systems should have these tools available.

- CMake 2.6+
- GNU Make utility
- C++11 Compiler
- [CppUnit for tests]
- [CTest 2.6+ for tests]

#### Build process

In the root of the repository execute the following commands

    cmake .
    make

This outputs the background service (`uadet2d`) and the client CLI program (`uadet2cli`) binaries in `dist/`.

There is no installation as of now.

### Usage

*default usage:*

    $1 dist/uadet2d # start this process in one shell and keep it running
    $2 dist/uadet2cli "eval desktop Mozilla/5.0 (Linux..."

Prints to stdout the output of classification of the given user agent string 
against all known categories present in the data file.

[See a Manual: basic usage of uadet2](doc/uadet.md)

### Data

Data file is consumed by the background service which will read it and train a model on it. Currently the data format is plain text with columns separated by a tab character.

#### Data format

The program can work with ascii (8-bit) encoded data of the following format:

1. one entry per line terminated by a new line character
2. each entry has two columns separated by a tab character: first colum is the label (or multiple labels separated by comma) and the second column is the user-agent string

A user-agent string with a class "mobile" would look like this:

    mobile  Mozilla/5.0 (Linux; U; Android 4.0; en-us) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1
    
Example with multiple labels:

    mobile,android,safari Mozilla/5.0 (Linux; U; Android 4.0; en-us) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

See the [example](/data_in.txt) 

### Test

To run all tests and print the output to STDOUT execute the next commands:

    cmake .
    make
    ctest .

### Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

[Full license](https://github.com/tomaslibal/ua-detection/blob/master/LICENSE)

---

- February 2016: added basic multiclass features
- December 2015: ported to a C++ version
- March 2015: wrote and added a probability based algorithm
- August 2014: added a user-agent string tokenizer
- April-May 2014: first attempt to create a model for predicting the class of an UA input

Tomas <tomas@libal.eu>
