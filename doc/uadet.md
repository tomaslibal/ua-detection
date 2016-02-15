# UA Detection

There are two binaries: 

- a background service `uadet2d` which trains a model based on the data and receives commands over TCP
- a command line program `uadet2cli` for issuing commands to the background service and printing out the server's response

The development is done on Scientific Linux 7.1 (similar to RHEL or CentOS or maybe other RPM based Linux systems) where it should be possible to build the project and run it, dependent on the requirements criteria.

- C++ Compiler with -std=c++11 support
- CMake 2.6+ (ideally 2.8)
- CTest 2.6+ (ideally 2.8)

## Learning part and the background service

When `uadet2d` starts it will look for a text data file which contains labels and data, so that it can train the model. There is currently no persistence in the training process so everytime the program starts it will train from scratch.

By default, this data file is `data_in.txt` in the current working diretory but it can be set in the configuration file to be in a different location in the local filesystem.

As the next step the program opens port 10128 to incoming connections. This port can be also set in the configuration file.

## CLI Program

This is the program to query the background service using a simple text based command line interface.

### Invokation template

```bash
    ./dist/uadet2cli [<hostname> <port>] <command>
```

If no hostname and port specified, the values from the config file will be used. By default these are `localhost` and `10128`. For the "command" argument, assuming a standard Bash shell, wrap it in quotes if it is more than one word.

### Add new data

```bash
    ./dist/uadet2cli "add mobile Mozilla/5.0 (Linux ..."
```

This command will add a user agent string beginning with *Mozilla/5.0 (Linux ...* belonging to a category *mobile* to the classifier. The data added from the command line stay in memory and are not persisted if the background service is stopped.

### Classify/evaluate data

```bash
    ./dist/uadet2cli "eval desktop Mozilla/5.0 (Linux ..."
```

This command will evaluate the probability of the user-agent string belonging for each known class (printing the class probabilities) and then evaluate if the supplied user-agent belongs to the specified class "desktop" (and printing the result).

```bash
    ./dist/uadet2cli "eval_one smart_tv Mozilla/5.0 (Linux ..."
```

This command will evaluate the probability only for the given class *smart_tv* given the user-agent string.
