#ifndef LOGGING_H
#define LOGGING_H

const char separator = '\n';

struct logline {
    char *message; // message
    char *time; // timestamp
    char *src;  // source file from which the message was logged
};

void log(char *message, char *src);

#endif