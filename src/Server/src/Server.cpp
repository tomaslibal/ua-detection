#include "Server.h"
#include "../../common/src/uadet2.h"
#include "sockets.h"

#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <csignal>
#include <vector>
#include <fstream>
#include <sstream>

Server::Server()
{
    ProgramConfig confCtrl(configFilePath);

    confCtrl.update(config);
    
    log("Initializing Server");
}

Server::~Server()
{

}

void Server::learn()
{
    /*
     * Start the learning phase: open the data file and add the data to the naive
     * bayess classifier
     */
    
    /*
     * The learning data file
     */
    const std::string dataFile = config.datafile;

    log("Using datafile: " + dataFile);
   
    NaiveBayessClassifier* nbl = &nb;
    
    /* 
     * A lambda function that takes a string line. It expects the line to have
     * two columns: categories and a user-agent string, separated by a tab
     * character.
     * The function then calls NaiveBayesClassifier.add_data to add the new 
     * user agent string with its category(ies) to the memory.
     */
    std::function<void (std::string)> add_line = [nbl](std::string line) {
        std::string category, uas;
        std::string::size_type n = line.find('\t');
        std::vector<std::string> categories;
        
        if (n != std::string::npos) {
            category = line.substr(0, n);
            uas = line.substr(n+1);

            strsplit(category, ',', categories);
            if (categories.size() > 1) {
                for(std::vector<std::string>::iterator it = categories.begin(); it != categories.end(); ++it) {
                    nbl->add_data(uas, *it);
                }
            } else {
                nbl->add_data(uas, category);
            }
        }
    };
    
    /*
     * FileInputReader for reading in the data file, and passing each file line
     * to the previous lambda.
     */
    FileInputReader reader;
    
    log("Start: reading the datafile");
    
    reader.readLines(dataFile, add_line);
    
    log("Finish: reading the datafile");
    
    /*
     * Prints summary of number of categories and n-grams that have been read.
     */
    nb.stats();
}

 
void Server::set_config(const ProgramConfigObject& config)
{
    this->config = config;
}

void Server::set_logger(const FileLog& logger)
{
    this->logger = logger;
}

void Server::log(const std::string& msg)
{
    logger.log(msg);   
}


void Server::create_thread()
{

}

void Server::start()
{
    /*
     * Start the network server:
     */
    
    sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    
    network.set_port_no(config.portno);
    
    sockfd = network.addr_listen();
    
    logger.log("Using port number " + std::to_string(config.portno));
    
    logger.log("Now listening to incoming connections");
        
    NaiveBayessClassifier * nbl = &nb;
    
    /* 
     * Worker is started as a new thread, it processes the incoming request
     * in a detached state so it should not block other requests.
     */
    std::function<void (int)> worker2 = [nbl](int in_sockfd) {
        evaluate_incoming_request(in_sockfd, *nbl);
    };
    
    while(1) {
        /*
         * worker2 calls wait_and_accept (in sockets.h) which may lock the unique
         * lock 'lck' if it receives a signal to exit. This lock will interrupt
         * this while loop.
         */
        if (lck) {
            std::cout << "mutex locked, must exit" << std::endl;
            break;
        }
        /*
         * waits until an incoming connection is made
         */
        log("Waiting for a connection");
        
        int in_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (in_sockfd < 0) { perror("in_sockfd < 0"); }
        
        log("Incoming connection...Creating a new thread to handle it");
 
        std::thread a_thread (worker2, in_sockfd);
        a_thread.detach();
    }

    close(sockfd);
}

int Server::sockfd = -1;
