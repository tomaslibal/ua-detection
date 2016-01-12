/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
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

