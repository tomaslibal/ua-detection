## Device detection

### Introduction

This is an experimental repository about device detection for the World Wide
Web. I am mostly interested in device detection for the user agents connecting to web servers through the HTTP requests.

1. Does the device belong to a group

   Say, you define a group 'Smart TVs' and you get a user-agent string, this detection tells if the string comes possibly from a device that belongs to the group or not.

2. Lookup a device for a given user-agent string

   This detection tells what device(s) likely belong to the supplied user-agent string

| Document                       | Description                  |
|:------------------------------ |:---------------------------- |
| [Product keyword lookup](https://github.com/tomaslibal/ua-detection/blob/master/doc/keywords_lookup.md)         | A method to determine which keyword in the user-agent string is the product keyword |
| [User-agent header and tokenizing it](https://github.com/tomaslibal/ua-detection/blob/master/doc/user_agent.md) | Describes the HTTP User-Agent header field and also talks a little about tokenizing the header |
| Group inclusivity: a case study | Overview of a method to determine if a given user-agent string belongs to a group or not |
| Device detection from user-agent | Overview of a method to determine a device from a given user-agent string |
| [Data models](https://github.com/tomaslibal/ua-detection/blob/master/doc/data_model.md) | MongoDB data models with which `ua_detection` works |

The approach/content of this repository is two-fold: the practical part aims to provide a program that
can successfully and to a known degree of reliability determine the device that sent the HTTP request and
the theoretical part is a set of essays or notes on the topic.

### The program

The `ua_detection` program has 2 goals:

1. to identify group inclusiveness for a given pair (user_agent_string, group_name)

2. to identify which device may have possibly sent the user-agent string

#### Usage

*The program is not yet fully implemented*

By executing the program with a user-agent string and a group name, it gives an estimate if the device is from the chosen group.


    > admin:~ $ ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android 4.1.1)' --group android-devices

```json
    { "status": 0,
        "query": {
            "ua": "Mozilla/5.0 (Linux; U; en-us; Android 4.1.1)",
            "group": "android-devices"
        }
        "result": {
            "resolve": "OK",
            "resolution": 1,
            "confidence": 0.00045
        }
    }
```

Give a list of devices that may have sent the supplied user-agent string


    > admin:~ $ ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android 4.4)'

```json
    { "status": 0,
        "query": {
            "ua": "Mozilla/5.0 (Linux; U; en-us; Android 4.4)"
        },
        "result": {
            "resolve": "OK",
            "resolution": [
                {
                    "device":"HTC One (M8)",
                    "confidence": 0.00009
                },
                {
                    "device":"Sony Xperia Z2",
                    "confidence": 0.00008}
            ]
        }
    }
```

The underlying code is written in C. Depending on the intended usage this has some implications. If you would like to call this from a web application it will probably make most sense to run it as a service (daemon) so that the program is not 'started, loaded to memory, used and discarded' for each call.

If the program runs as a daemon it would be good to have a wrapper API or API(s) in some common languages that can query it.

#### The database

The raw data are stored in Mongo database and the program may store some binary data in the file system.


For more information about the data models and the part that mongodb has in this program package see `doc/data_model.md`.

### The notes on device detection on the WWW

My goal is to critically discuss the topic and give arguments why we may need a reliable device detection method.

[Read more](http://github.com/tomaslibal/ua-detection/doc)

### Branching strategy

  ** NOT IN PLACE NOW - MASTER IS EXPERIMENTAL **

- master (stable) will be used for releases
- develop (unstable) will have feature branches merged into it
- feature/{feature-branch} will be used for developing new features
- hotfix/{hotfix} hotfixes that may go directly into the master

### Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

[Full license](https://github.com/tomaslibal/ua-detection/blob/master/LICENSE)

### Footnotes

Dependencies

- node.js (tested with v0.10.21) for aux. tools
- mongodb (node.js module) for aux. tools
- node-simple-router (node.js module) for data API
- mongod  (mongodb daemon service)
- [MongoDB C driver 0.7](http://api.mongodb.org/c/0.7)
- C compiler

---
April-May 2014

Tomas <tomas@libal.eu>
