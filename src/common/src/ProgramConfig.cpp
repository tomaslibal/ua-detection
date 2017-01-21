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

#include <cstring>

std::string get_value(const std::string& config_line, const std::string& parameter)
{
    return std::string(config_line.substr(config_line.find(parameter) + 1 + parameter.size()));
}

ProgramConfig::ProgramConfig() {
}

ProgramConfig::ProgramConfig(const std::string& configFilePath) {
    path = configFilePath;
}

ProgramConfig::ProgramConfig(const ProgramConfig& orig) {
}

ProgramConfig::~ProgramConfig() {
}

void ProgramConfig::update(ProgramConfigObject& confObj)
{
    std::function<void (std::string const&)> onReadLine = [&confObj](std::string const& line) {
        if (line.find("hostname") != std::string::npos) {
            confObj.hostname = get_value(line, "hostname");
        }
        if (line.find("port") != std::string::npos) {
            confObj.portno = std::stoi(get_value(line, "port"));
        }
        if (line.find("datafile") != std::string::npos) {
            confObj.datafile = get_value(line, "datafile");
        }
        if (line.find("logfile") != std::string::npos) {
            confObj.logfile = get_value(line, "logfile");
        }
        if (line.find("output") != std::string::npos) {
            std::string val = get_value(line, "output");
            if (val == "json") {
                confObj.outputType = OutputType::json;
            } else {
                confObj.outputType = OutputType::plaintext;
            }
        }
    };

    r.readLines(path, onReadLine);
}
