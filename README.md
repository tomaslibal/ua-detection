## Device detection

### Introduction

This is an experimental repository about device detection for the World Wide
Web. I am mostly interested in device detection for the user agents connecting
to web servers through the HTTP requests.

Two end goals that I would like achieve:

1. Determine if the device belong to a given group

   Say, you define a group 'Smart TVs' and you get a user-agent string, this
   detection tells if the string comes possibly from a device that belongs to
   the group or not.

2. Uniquely determine a device for a given user-agent string

   This detection tells what device(s) likely belong to the supplied user-agent
   string.

### Data

More in the [data section](data/)

> You can contribute by adding more user-agent strings data  
> All content must be publicly available and compatible with the [GNU License](LICENSE)
   
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

- April-May 2014
- August 2014

Tomas <tomas@libal.eu>
