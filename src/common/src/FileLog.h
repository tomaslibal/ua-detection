#ifndef FILELOG_H
#define FILELOG_H

#include <string>

class FileLog
{
public:
    FileLog();
    FileLog(FileLog const& orig);
    virtual ~FileLog();

    void setPath(std::string const& path);
    void log(std::string const& msg);
private:
    std::string path = "./uadet2.log.txt";
};

#endif /* FILELOG_H */
