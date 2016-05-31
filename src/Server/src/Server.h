#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <atomic>

#include "../../common/src/FileLog.h"
#include "../../common/src/ProgramConfig.h"
#include "../../Network/src/Network.h"
#include "../../NaiveBayessClassifier/src/NaiveBayessClassifier.h"

class Server {
public:
    Server();
    virtual ~Server();
    
    static int sockfd;
    
    void set_logger(FileLog const& logger);
    void set_config(ProgramConfigObject const& config);
    void learn();
    void start();
    void stop();
private:
    FileLog logger;
    ProgramConfigObject config;
    NaiveBayessClassifier nb;
    
    /*
     * Incremental counter for each new incoming connection
     */
    std::atomic<int> nIncomingMsg{0};
    
    const std::string configFilePath = "config/server.txt";
    
    /*
     * Each new connection is handled in its own thread
     */
    void create_thread();
    
    /*
     * Writes the message to the log
     */
    void log(std::string const& msg);
};

#endif /* _SERVER_H_ */