/* 
 * File:   FileInputReader.h
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:13 PM
 */

#ifndef FILEINPUTREADER_H
#define	FILEINPUTREADER_H

#include <string>
#include <functional>

class FileInputReader {
public:
    FileInputReader();
    virtual ~FileInputReader();
    
    void readLines(std::string filename, std::function<void (std::string)>& f);
private:

};

#endif	/* FILEINPUTREADER_H */

