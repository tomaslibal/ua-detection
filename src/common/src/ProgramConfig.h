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
#include <functional>

#include "../../FileInputReader/src/FileInputReader.h"

enum class OutputType { json, plaintext };

struct ProgramConfigObject {
    int portno;
    std::string hostname;
    std::string datafile;
    std::string logfile;
    OutputType outputType;

    ProgramConfigObject(
        int portno=10128, 
        std::string hostname="localhost", 
        std::string datafile="common/config/server.txt", 
        std::string logfile="log.txt",
        OutputType outputType = OutputType::json
    )
        : portno(portno), hostname(hostname), datafile(datafile), logfile(logfile), outputType(outputType)
    {
        
    }
};

class ProgramConfig {
public:
    ProgramConfig();
    ProgramConfig(std::string const& configFilePath);
    ProgramConfig(ProgramConfig const& orig);
    virtual ~ProgramConfig();
    /*
     * Reads the config file and sets the read values into the config object
     */
    void update(ProgramConfigObject& conf);
    std::string path = "common/config/server.txt";
private:
    FileInputReader r;
};

#endif /* PROGRAMCONFIG_H */

