# UA Detection

There are two binaries: 

- a background service `uadet2d`
- a command line program `uadet2cli`

The development is done on Scientific Linux 7.1 (similar to RHEL or CentOS or maybe other RPM based Linux systems) where it should be possible to build the project and run it, dependent on the requirements criteria.

- C++ Compiler with -std=c++11 support
- CMake 2.6+ (ideally 2.8)
- CTest 2.6+ (ideally 2.8)

## Learning part and the background service

First, start `uadet2d` in a console. This program will look for a data file named `data_in.txt` in the current working directory. So if you start the program in the command line as 

```bash
    ./dist/uadet2d
```

then the current working directory might be the current directory `.`. This background service will go through the data and learn the Naive Bayess Classifier, and then open port 10128 to incoming connections.

## CLI Program

This is the program to query the background service using a simple text based command line interface.

### Invokation template

```bash
    ./dist/uadet2cli <hostname> <port> <command>
```

The first two arguments in the angle brackets can go without quotation marks as ASCII text. With the command argument, assuming a standard Bash shell, wrap it in quotes if it is more than one word. By default `<hostname>` is `localhost` and the port number is `10128`.

### Add new data

```bash
    ./dist/uadet2cli localhost 10128 "add mobile Mozilla/5.0 (Linux ..."
```

This command will add a user agent string beginning with *Mozilla/5.0 (Linux ...* belonging to a category *mobile* to the classifier. The data added from the command line stay in memory and are not persisted if the background service is stopped.

### Classify/evaluate data

```bash
    ./dist/uadet2cli localhost 10128 "eval desktop Mozilla/5.0 (Linux ..."
```

This command will classify a user agent string beginning with *Mozilla/5.0 (Linux ...* as if it belonged to the group *desktop* and will print the resulting probability of the fact, based on the previous data.
