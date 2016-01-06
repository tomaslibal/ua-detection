/* 
 * File:   server.cpp
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:08 PM
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <chrono>
#include <mutex>
#include <cstring>
#include <csignal>

#include "server/src/utils.h"
#include "server/src/sockets.h"

using std::function;
using std::cout;
using std::cerr;
using std::endl;
using std::mutex;
using std::thread;

static int sockfd;

/*
 * 
 */
int main(int argc, char** argv) {
    int insockfd;
    int portno;
    sockaddr_in serv_addr, cli_addr;
    
    socklen_t clilen;
    
    char buffer[256];
    /*
     * Get a new Internet socket
     */
    sockfd = create_socket_inet_stream();

	/*
	 * catch the ctrl+c interrupt signal
	 */
	signal(SIGINT, [] (int signum) {
		close(sockfd);
		exit(signum);
	});
    
	bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 10128;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    int retries = 3;
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
    listen(sockfd,backlogsize);
    
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
    function<void (int)> worker2 = [&cli_addr, &exitCallback, &lck](int in_sockfd) {
        wait_and_accept(&cli_addr, in_sockfd, exitCallback, lck);
    };
    
    while(1) {
        if (lck) {
            cout << "mutex locked, must exit" << endl;
            break;
        }
        int in_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        thread a_thread (worker2, in_sockfd);
        a_thread.detach();
    }

    close(sockfd);
    
    return 0;
}

