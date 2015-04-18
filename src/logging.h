#ifndef LOGGING_H
#define LOGGING_H

struct logline {
    char *line; // message
    char *time; // timestamp
    char *src;  // source file from which the message was logged
}

#endif