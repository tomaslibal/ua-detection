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

void strsplit(const std::string& str, char delimiter, std::vector<std::string>& tokens);

/*
 * Prints the error message and exits the program with the implementation
 * dependent EXIT_FAILURE exit code.
 */
void error(const char *msg);

/*
 *
 *
 */
std::vector<double> softmax(std::vector<double>& z);


/*
 * Logistic function f(x) maps x to [0, 1] with f(0) = 0.5
 */
std::vector<double> logistic(std::vector<double>& z);

#endif	/* UADET2_H */

