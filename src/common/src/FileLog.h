#ifndef FILELOG_H
#define FILELOG_H

#include <string>

class FileLog
{
public:
    FileLog();
    FileLog(const FileLog& orig);
    virtual ~FileLog();

    void setPath(const std::string& path);
    void log(const std::string& msg);
private:
    std::string path = "./uadet2.log.txt";
};

#endif /* FILELOG_H */
