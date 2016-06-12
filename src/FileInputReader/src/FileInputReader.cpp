/* 
 * File:   FileInputReader.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:13 PM
 */

#include <fstream>
#include <iostream>

#include "FileInputReader.h"

using std::ifstream;
using std::cerr;
using std::string;
using std::endl;
using std::function;

FileInputReader::FileInputReader() {
}

FileInputReader::~FileInputReader() {
}

void FileInputReader::readLines(string filename, function<void (string)>& f) {
    ifstream infile(filename);
    
    if (!infile.good()) {
        cerr << "Cannot find datafile " << filename << endl;
        return;
    }
        
    string line;
    
    while (getline(infile, line)) {
        f(line);
    }
}

