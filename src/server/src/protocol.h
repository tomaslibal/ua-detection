/* 
 * File:   protocol.h
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:35 PM
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdlib>
#include <string>
#include <vector>

struct Token {
    std::string data;
    std::string arg;
    std::string val;
};

std::vector<std::string>* tok(const char* msg);
std::vector<std::string>* process_message(const char* msg);

#endif /* PROTOCOL_H */

