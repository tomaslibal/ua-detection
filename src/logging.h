#ifndef LOGGING_H
#define LOGGING_H

#ifdef DEBUG_BUILD
#define LOGM(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOGM(...) do {} while (0)
#endif

struct logline {
    char *message; // message
    char *time; // timestamp
    char *src;  // source file from which the message was logged
};

void logmsg(char *message, char *src);

#endif