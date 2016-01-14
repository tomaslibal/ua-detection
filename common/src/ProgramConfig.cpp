/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProgramConfig.cpp
 * Author: tlibal
 * 
 * Created on January 12, 2016, 7:58 PM
 */

#include "ProgramConfig.h"

#include <iostream>

std::string* get_value(const std::string& config_line, const std::string& parameter)
{
    std::string* str = new std::string(config_line.substr(config_line.find(parameter) + 1 + parameter.size()));
    return str;
}

ProgramConfig::ProgramConfig() {
}

ProgramConfig::ProgramConfig(const ProgramConfig& orig) {
}

ProgramConfig::~ProgramConfig() {
}

void ProgramConfig::update(ProgramConfigObject& confObj)
{

    std::function<void (std::string)> onReadLine = [&confObj](std::string line) {
        std::cout << line << std::endl;
        std::string* p_str;
        if (line.find("hostname") != std::string::npos) {
            p_str = get_value(line, "hostname");
            confObj.hostname = *p_str;
            delete p_str;
        }
        if (line.find("port") != std::string::npos) {
            p_str = get_value(line, "port");
            confObj.portno = std::atoi(p_str->c_str());
            delete p_str;
        }
    };

    r.readLines(path, onReadLine);
}
