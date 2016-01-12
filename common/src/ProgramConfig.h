/*
 * This class reads the config file and sets appropriate variables to the values
 * from the configuration.
 *  
 * File:   ProgramConfig.h
 * Author: tlibal
 *
 * Created on January 12, 2016, 7:58 PM
 */

#ifndef PROGRAMCONFIG_H
#define PROGRAMCONFIG_H

#include <string>

#include "../../FileInputReader/src/FileInputReader.h"

class ProgramConfig {
public:
    ProgramConfig();
    ProgramConfig(const ProgramConfig& orig);
    virtual ~ProgramConfig();
private:
    FileInputReader r;
    std::string path = "common/config/server.txt";
};

#endif /* PROGRAMCONFIG_H */

