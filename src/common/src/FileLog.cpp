#include "FileLog.h"

#include <fstream>
#include <ctime>
#include <string>

FileLog::FileLog()
{

}

FileLog::FileLog(const FileLog& orig)
{

}

FileLog::~FileLog()
{

}

void FileLog::setPath(const std::string& path)
{
    this->path = path;
}

void FileLog::log(const std::string& msg)
{
	std::ofstream ofile(path, std::fstream::app);

	if (!ofile.good()) {
		return;
	}

    std::time_t now = std::time(nullptr);
	char timestamp[60];
	std::strftime(timestamp, sizeof(timestamp), "%c ", std::gmtime(&now));
	
	std::string logLine = std::string(timestamp) + msg + "\n";

	ofile.write(logLine.c_str(), logLine.length());
}
