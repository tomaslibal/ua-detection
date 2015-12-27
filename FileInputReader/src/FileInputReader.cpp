/* 
 * File:   FileInputReader.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:13 PM
 */

#include <fstream>

#include "FileInputReader.h"

using namespace std;

FileInputReader::FileInputReader() {
}

FileInputReader::~FileInputReader() {
}

void FileInputReader::readLines(string filename, function<void (string)>& f) {
    ifstream infile(filename);
        
    string line;
    
    while (getline(infile, line)) {
        f(line);
    }
}

