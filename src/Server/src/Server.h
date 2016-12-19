#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <atomic>

#include <netinet/in.h>
#include <unistd.h>

#include "../../common/src/FileLog.h"
#include "../../common/src/ProgramConfig.h"
#include "../../Network/src/Network.h"
#include "../../NaiveBayessClassifier/src/NaiveBayessClassifier.h"

class Server {
public:
    Server();
    virtual ~Server();
    
    int sockfd;
    
    void set_logger(FileLog const& logger);
    void set_config(ProgramConfigObject const& config);
    void learn();
    void start();
    void stop();

    /**
     * Reads the incoming socket connection and parses it using protocol::process_message().
     * 
     * @param sockaddr_in* cli_addr seems unused; delete this
     * @param int insockfd socket descriptor of the incoming request
     * @param std::function<void ()>& exit_callback is a callable object that is called if function exits early; rename to early_exit_callback
     * @param std::unique_lock<std::mutex>& signal_exit is used to signal to the main thread that the user wishes to terminate the server program; remove this functionality
     * @param NaiveBayessClassifier& nbc is an instance of learned NaiveBayessClassifier
     * 
     * @return void
     */
    void evaluate_incoming_request(int insockfd, NaiveBayessClassifier& nbc, std::atomic<int> & nIncomingMsg);

    /**
     * Classifies the given data using NaiveBayessClassifier
     * 
     */
    std::string* classify_data(std::vector<std::string>& input, NaiveBayessClassifier& nbc);

    /**
     * Prints two vectors of doubles and strings (<0.42, 1.337, ...>, <"mobile", "desktop", ...>)
     * as a json string.
     *
     */
    std::string* json_output(std::vector<double>& values, std::vector<std::string>& labels);
private:
    FileLog logger;
    ProgramConfigObject config;
    NaiveBayessClassifier nb;
    
    /**
     * Incremental counter for each new incoming connection
     */
    std::atomic<int> nIncomingMsg{0};
    
    const std::string configFilePath = "config/server.txt";
    
    /**
     * Each new connection is handled in its own thread
     */
    void create_thread();
    
    /**
     * Writes the message to the log
     */
    void log(std::string const& msg);
};

#endif /* _SERVER_H_ */
