# Device detection

## Introduction

This is an experimental repository about device detection for the World Wide
Web. I am mostly interested in device detection for the user agents connecting to web servers through the HTTP requests.

HTTP/1.1 specifies that a user agent should send its identifying string but the form of that string is left to the user agent. As a result the web server has no means to validate the correctness (user agent can send any string it likes) and secondly, the individual strings often varies one from each other so there is a vast number of different User-Agent strings being send in HTTP requests.

The approach/content of this repository is two-fold: the practical part aims to provide a program that
can successfully and to a known degree of reliability determine the device that sent the HTTP request and
the theoretical part is a set of essays or notes on the topic.

## The program

The `detection` program has a goal to identify devices by their User-Agent strings that they send in HTTP requests. This software started to meet one specific need which was to identify and categorize the User-Agent strings of smartphone devices into separate groups like Android devices vs. Non Android devices and groups for particular manufacturers.

And so if we assume that we have defined the groups, this program determines the confidence with which we can say that a given User-Agent string belongs to one of the devices of the group (for a demonstration see the `usage` column).

The main functionality for device detection relies on a perceptron implemented in the program and this has several implications:

- machine learning must be possible
- data must be available
- training set is used to train the ANN*

*Currently we have a single layer feed-forward neural network but in the future implementations this may change to a multilayer neural network with eventual back-propagation.

### Usage

This first example shows the only usage that is currently implemented. By querying the program with a User-Agent string and a group name it gives an estimate if the device is from the chosen group.

    > ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android/4.1.1)' --group android-devices
    > User-Agent: Mozilla/5.0 (Linux; U; en-us; Android/4.1.1)
    > Group: android-devices
    > Group description: All devices with Android OS
    > Resolution: 1
    > Confidence: 0.99945

For this usage to work we assume that the program's ANN has been trained and the groups have been defined in the database.

*Following usage examples are not yet implemented.*

Give a list of devices that may use the supplied user-agent string

    > ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android/4.4)'
    > User-Agent: Mozilla/5.0 (Linux; U; en-us; Android/4.4)
    > All devices with result > 0.00100
    > Generic Android Device: 0.99998
    > HTC One (M7): 0.00312
    > Sony Xperia Z1: 0.00135


The underlying code is written in C. Depending on the intended usage this fact has some implications. If you would like to call this from a web application it will probably make most sense to run it as a service (daemon) so that the program is not 'started, loaded to memory, used and discarded' for each call.

If the program runs as a daemon it would be good to have a wrapper API or API(s) in JavaScript or more languages that can query it.

### The database

The raw data are stored in Mongo database and the program also uses a proprietary binary format for storing intermediate data.


For more information about the data models and the part that mongodb has in this program package see `data_model.md`.

## The notes on device detection on the WWW

My goal is to critically discuss the topic, review the latest developments as
we as things from the past and to make research on the matter.

## Branching strategy

  ** NOT IN PLACE NOW - MASTER IS EXPERIMENTAL **

- master (stable) will be used for releases
- develop (unstable) will have feature branches merged into it
- feature/{feature-branch} will be used for developing new features
- hotfix/{hotfix} hotfixes that may go directly into the master

## Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

## Footnotes

Dependencies

- node.js (tested with v0.10.21)
- mongodb (node.js module)
- mongojs (node.js module)
- node-simple-router (node.js module)
- mongod  (mongodb daemon service)
- C compiler

---
April 2014

Tomas <tomas@libal.eu>
