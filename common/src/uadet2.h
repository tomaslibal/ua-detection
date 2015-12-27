/* 
 * File:   uadet2.h
 * Author: tlibal
 *
 * Created on December 27, 2015, 8:33 PM
 */

#ifndef UADET2_H
#define	UADET2_H

#include <string>
#include <vector>
#include <functional>

/*
 * Command line arguments are read and stored in a UadetSettings struct.
 */
struct UadetSettings {
    std::string ua;
    std::string category;
};

/*
 * Function foreach applies the callback function to each element of the vector
 */
void foreach(std::vector<std::string>* vec, std::function<void (std::string)> &callback);

/*
 * The common stdout helper that prints a summary of available command line
 * argumets for the program.
 */
void print_usage();

void parse_args(int argc, char** argv, UadetSettings& settings);

class InvalidArgsException: virtual public std::exception{
public:
    InvalidArgsException(std::string m="Invalid Arguments!") : msg(m) {}
    ~InvalidArgsException() throw() {}
    const char* what() const throw() { return msg.c_str(); }
private:
    std::string msg;
};


#endif	/* UADET2_H */

