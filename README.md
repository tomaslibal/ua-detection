# Device detection

## Introduction

This is an experimental repository about device detection for the World Wide
Web. I am mostly interested in device detection for the user agents connecting to web servers through the HTTP requests.

HTTP/1.1 specifies that a user agent should send its identifying string but the form of that string is left to the user agent. As a result the web server has no means to validate the correctness (user agent can send any string it likes) and secondly, the individual strings often varies one from each other so there is a vast number of different User-Agent strings being send in HTTP requests.

The approach/content of this repository is two-fold: the practical part aims to provide a program that
can successfully and to a known degree of reliability determine the device that sent the HTTP request and
the theoretical part is a set of essays or notes on the topic.

## The program

The `detection` program can be used for two things: for a given user-agent string
it can give a result set of the closest matching devices that use that particular
user-agent.

Secondly, it can be used to return true or false estimate if the user-agent
string belongs to a device from a particular group. This given group has to be
predefined (examples of this can be 'Android devices', 'iPhones', 'Smart TVs'
and many more groups could be thought of).

### Example

Give a list of devices that may use the supplied user-agent string

    > ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android/4.4)'
    > User-Agent: Mozilla/5.0 (Linux; U; en-us; Android/4.4)
    > All devices with result > 0.00100
    > Generic Android Device: 0.99998
    > HTC One (M7): 0.00312
    > Sony Xperia Z1: 0.00135

Give an estimate if the device is from a particular group

    > ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android/4.1.1)' --group android-devices
    > User-Agent: Mozilla/5.0 (Linux; U; en-us; Android/4.1.1)
    > Group: android-devices
    > Group description: All devices with Android OS
    > Resolution: 1
    > Confidence: 0.99945

The underlying code is written in C.

** Description of the program to be written here **

It would be good to have a wrapper or access the program through API like a JavaScript API.



## The notes

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


---
April 2014

Tomas <tomas@libal.eu>
