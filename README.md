# Device detection

## Introduction

This is an experimental repository about device detection for the World Wide
Web. I am mostly interested in device detection for the user agents connecting to web servers through the HTTP requests.

HTTP/1.1 specifies that a user agent should send its identifying string but the form of that string is left to the user agent. As a result the web server has no means to validate the correctness (user agent can send any string it likes) and secondly, the individual strings often varies one from each other so there is a vast number of different User-Agent strings being send in HTTP requests.

The approach/content of this repository is two-fold: the practical part aims to provide a program that
can successfully and to a known degree of reliability determine the device that sent the HTTP request and
the theoretical part is a set of essays or notes on the topic.

## The program

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
