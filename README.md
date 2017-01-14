## Device detection

### Introduction

This is an experimental repository for device detection on the World Wide
Web. I'm mostly interested in device detection for the user agents connecting
to web servers through the HTTP(S) requests.

There are two end goals that I would like achieve:

1. Determine if a device belongs to a given group

   Say, you define a group 'Smart TVs' and you get a user-agent string, this
   detection should tell if the string comes possibly from a device that belongs to
   the group or not.

2. Uniquely determine a device for a given user-agent string

   This detection should tell what device(s) likely belong to the supplied user-agent
   string.
   
### ua-detection

`uadet2` is an implementation of a generative model using the naive bayess classifier
to answer the two questions from above.

There were a few reasons that favored the use of a generative model:

- to learning process is simpler for the generative model which will do with counting and averaging
- each class conditional density is estimated separately so it is possible to add classes without retraining the whole model

A major disadvantage of the nb classifier is its independence assumption. This independence
is believed to invalid for the user-agent strings since it is possible to have the same keyword,
like `linux` in both `desktop` and `mobile` classes (the agents running on Android OS often 
include the word linux in their user agents as well as ordinary agents running on desktop
version of Linux OS include the same).

To overcome this issue, `uadet2` tokenizes the input into n-grams and use that data
in the training of the classifier.

### Architecture

This is a server-client program. The server implementation is also accompanied 
by a sample client implementation. Hence, the makefile has two targets: the server
`uadet2d` and the client `uadet2cli`.

### Usage

*default usage:*

    $1 dist/uadet2d # start this process in one shell and keep it running
    $2 dist/uadet2cli "eval desktop Mozilla/5.0 (Linux..."

Prints to stdout the output of classification of the given user agent string 
against all known categories present in the data file.

[See a Manual: basic usage of uadet2](doc/uadet.md)


### Classification

The output from the server is a `json` document with softmax values (classes with values that are 0 are omitted in the output).

![Sample result](/doc/sample_uadet.png)

In the figure above you can see a sample result of evaluating a user-agent string constrained to the non zero results. This data set had 23 features in total and only three features evaluated with probabilities higher than zero for the given input.


```
Mozilla/5.0 (Linux; Android 5.1; XT1526 Build/LPI23.29-18-S.2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.94 Mobile

{
    "android": 0.900142,
    "mobile": 0.099804,
    "windowsphone": 0.000054
}

```

### Learning model

Let *x* be new input, and let *d* be all previously classified data in *N* different classes. This program then calculates *N* probabilities *p_i=(x|d_n)* where *d_n* are all datapoints in the given class.

The algorithm for classification is similar to this.

```
    classify(String userAgent, String category)
        Array<String> ngrams = getNgramsFromUserAgentString(userAgent)
        double probability = 0
        double category_freq = getCategoryFreq(category)
        
        for (String ngram in ngrams)
            p_ngram = getGlobalNgramProbability(ngram)
            p_ngram_in_category = getNgramProbability(ngram, category)
            if (p_ngram_in_category > 0)
                probability += log(p_ngram_in_category / p_ngram)
        
        return exp( probability + log( category_freq ) )
```

This algorithm can be found in [double NaiveBayessClassifier::classify()](/src/NaiveBayessClassifier/src/NaiveBayessClassifier.cpp#L177).

### Build the project from source

Currently no binaries are distributed. Here's a manual how to build it from the source code.

#### Dependencies

Most *nix systems should have these tools available.

- CMake 2.6+
- GNU Make utility
- C++11 Compiler
- [CppUnit for tests] `yum install cppunit cppunit-devel`
- [CTest 2.6+ for tests]

#### Build process

In the root of the repository execute the following commands

    cmake .
    make

This outputs the background service (`uadet2d`) and the client CLI program (`uadet2cli`) binaries in `dist/`.

There is no installation as of now.


## Data

Data file is consumed by the background service which will read it and train the model on it. Currently the data format is plain text with columns separated by a tab character.

### Data format

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
    
### Config

By default the client and server programs look for config files in the `config/` directory (named `client.txt` and `server.txt` respectivelly). 
The amount of the settings that are possible to configure is quite limited for now and mainly include these options:

- port number (`port=10128`)
- hostname (`hostname=localhost`)
- log file names (`logfile=server.log.txt`)
- data input file name (`datafile=data_in.txt`, only server)
- output type ("json" or "plaintext")

### Limitations

Header fields can be easily set to an arbitrary value and it is up to the agent to choose its user-agent string. Therefore, this value should be taken with a grain of salt as a general rule.

The supervised learning method used by `uadet2` relies on having labelled data as its input. Firstly, the labels need to be assigned to each data point which means that either the user-agent string is obtained from the agent itself and categorized, or some other expert assigns a label to the user-agent string. The latter brings the possibility of mislabelling the string and lowering the ability of the program to successfully predict the class on new data.

Then the method itself has its own limitations in what it can learn from the data and how successful it can be in predicting the class of the input string.

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
