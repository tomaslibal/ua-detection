/* 
 * File:   server.cpp
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:08 PM
 */
 
 // Todo - add a flag which will not start the background service, like --no-tcp

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <chrono>
#include <mutex>
#include <cstring>
#include <csignal>
#include <vector>
#include <fstream>
#include <sstream>

#include "server/src/utils.h"
#include "server/src/sockets.h"

#include "FileInputReader/src/FileInputReader.h"
#include "NaiveBayessClassifier/src/NaiveBayessClassifier.h"
#include "common/src/uadet2.h"
#include "common/src/ProgramConfig.h"
#include "common/src/FileLog.h"

using std::function;
using std::cout;
using std::cerr;
using std::endl;
using std::mutex;
using std::thread;
using std::string;

/**
 * Static socket filedescriptor variable so that it can be referenced from the signal handler function.
 * 
 * @param int sockfd
 */
static int sockfd;

/*
 * 
 */
int main(int argc, char** argv) {
    int insockfd;
    int portno;
    sockaddr_in serv_addr, cli_addr;
    
    socklen_t clilen;
    clilen = sizeof(cli_addr);

    FileLog logger;
    logger.log("Initializing Server");

    /*
     * Get the program config
     */
    string serverConfigFile = "src/common/config/server.txt";
    ProgramConfigObject conf;
    ProgramConfig confCtrl(serverConfigFile);

    confCtrl.update(conf);
    logger.log("Config file read and updated");

    char buffer[256];
    /*
     * Get a new Internet socket
     */
    sockfd = create_socket_inet_stream();

    if (sockfd < 0) {
        error("Creating a socket failed");
        logger.log("Creating a socket failed");
    }

    /*
     * catch the ctrl+c interrupt signal
     */
    signal(SIGINT, [] (int signum) {
        close(sockfd);
        exit(signum);
    });
    
    /*
     * Start the learning phase: open the data file and add the data to the naive
     * bayess classifier
     */
    /*
     * The program uses a Naive Bayess Classifier to predict a posterior of 
     * a given user-agent string.
     */
    NaiveBayessClassifier nb;
    
    /*
     * The learning data file
     */
    string dataFile = conf.datafile;


    /* 
     * A lambda function that takes a string line. It expects the line to have
     * two columns: categories and a user-agent string, separated by a tab
     * character.
     * The function then calls NaiveBayesClassifier.add_data to add the new 
     * user agent string with its category(ies) to the memory.
     */
    function<void (string)> add_line = [&nb](string line) {
        string category, uas;
        string::size_type n = line.find('\t');
        std::vector<std::string> categories;
        
        if (n != string::npos) {
            category = line.substr(0, n);
            uas = line.substr(n+1);

            strsplit(category, ',', categories);
            if (categories.size() > 1) {
                for(std::vector<std::string>::iterator it = categories.begin(); it != categories.end(); ++it) {
                    nb.add_data(uas, *it);
                }
            } else {
                nb.add_data(uas, category);
            }
        }
    };
    
    /*
     * FileInputReader for reading in the data file, and passing each file line
     * to the previous lambda.
     */
    FileInputReader reader;
    reader.readLines(dataFile, add_line);
    
    /*
     * Prints summary of number of categories and n-grams that have been read.
     */
    nb.stats();
    
    /*
     * Start the network server:
     */
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = conf.portno;
    
    serv_addr.sin_family = AF_INET;
    // host address: INADDR_ANY = 0.0.0.0?
    // this should be read from the config as well
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    int retries = 12;
    int result = -1;
    int wait = 3;
    
    while (retries > 0 && (result = ::bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
        retries--;
        cerr << "error binding, retrying in " << wait << "s..." << endl;
        std::this_thread::sleep_for(std::chrono::seconds(wait));
    }
    
    if (result < 0) {
        error("binding failed");
    }
    
    cout << "binding to the socket successful on port " << portno << endl;
    
    int backlogsize = 5;
    /*
     * Accept incoming connections
     */
    int ls = listen(sockfd,backlogsize);

    if (ls < 0) {
            error("cannot accept connection");
    }    

    mutex signal_exit;
    std::unique_lock<mutex> lck (signal_exit);
    lck.unlock();
    
    function<void ()> exitCallback = []() {
        cout << "exiting now..." << endl;
        close(sockfd);
        exit(EXIT_SUCCESS);
    };
    
    /* 
     * Worker is started as a new thread, it processes the incoming request
     * in a detached state so it should not block other requests.
     */
    function<void (int)> worker2 = [&cli_addr, &exitCallback, &lck, &nb](int in_sockfd) {
        wait_and_accept(&cli_addr, in_sockfd, exitCallback, lck, nb);
    };
    
    while(1) {
        /*
         * worker2 calls wait_and_accept (in sockets.h) which may lock the unique
         * lock 'lck' if it receives a signal to exit. This lock will interrupt
         * this while loop.
         */
        if (lck) {
            cout << "mutex locked, must exit" << endl;
            break;
        }
        /*
         * waits until an incoming connection is made
         */
        int in_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (in_sockfd < 0) { perror("in_sockfd < 0"); }       
 
        thread a_thread (worker2, in_sockfd);
        a_thread.detach();
    }

    close(sockfd);
    
    return 0;
}

