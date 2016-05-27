#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>

#include "../../common/src/FileLog.h"
#include "../../common/src/ProgramConfig.h"
#include "../../Network/src/Network.h"
#include "../../NaiveBayessClassifier/src/NaiveBayessClassifier.h"

class Server {
public:
    Server();
    virtual ~Server();
    
    static int sockfd;
    
    void set_logger(const FileLog& logger);
    void set_config(const ProgramConfigObject& config);
    void learn();
    void start();
private:
    FileLog logger;
    ProgramConfigObject config;
    Network network;
    NaiveBayessClassifier nb;
    
    const std::string configFilePath = "src/common/config/server.txt";
    
    void create_thread();
    void log(const std::string& msg);
};

#endif /* _SERVER_H_ */