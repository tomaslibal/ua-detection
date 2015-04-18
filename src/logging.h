#ifndef LOGGING_H
#define LOGGING_H

struct logline {
    char *message; // message
    char *time; // timestamp
    char *src;  // source file from which the message was logged
};

void logm(char *message, char *src);

#endif