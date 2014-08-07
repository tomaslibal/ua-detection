## Device detection

### Introduction

This is an experimental repository about device detection for the World Wide 
Web. I am mostly interested in device detection for the user agents connecting 
to web servers through the HTTP requests.

Two methods that I would like achieve:

1. Determine if the device belong to a given group

   Say, you define a group 'Smart TVs' and you get a user-agent string, this 
   detection tells if the string comes possibly from a device that belongs to 
   the group or not.

2. Uniquely determine a device for a given user-agent string

   This detection tells what device(s) likely belong to the supplied user-agent 
   string.
   
### The program
  
The two methods are to be implemented in the program `ua_detection`.

#### Usage

By executing the program with a user-agent string and a group name supplied, it 
tries to determine if the user-agent string belongs to the given group.

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

When the program is executed with a user-agent string only, it returns a list of 
devices that may have likely use the supplied user-agent string.

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

### Branching strategy

- master (stable) will be used for releases
- develop (unstable) will have feature branches merged into it
- feature/{feature-branch} will be used for developing new features
- hotfix/{hotfix} hotfixes that may go directly into the master

### Disclaimers

There is no warranty for this free software. All mentioned trademarks are property
of their owners. I hope that this repository or its part will be of use to someone.

[Full license](https://github.com/tomaslibal/ua-detection/blob/master/LICENSE)

---

- April-May 2014
- August 2014

Tomas <tomas@libal.eu>
