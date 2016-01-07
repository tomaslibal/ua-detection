#include "sockets.h"
#include "utils.h"
#include "protocol.h"

#include <iostream>

using std::cout;
using std::endl;
using std::function;
using std::mutex;
using std::unique_lock;

int create_socket_inet_stream() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR Opening Socket");
    }
    return sockfd;
}

void wait_and_accept(sockaddr_in* cli_addr, int insockfd, function<void ()>& exit_callback, unique_lock<mutex>& signal_exit, NaiveBayessClassifier& nbc) {
    socklen_t clilen;
    char buffer[256];
    int n;
    
    cout << "Connection established (" << insockfd << ")" << endl;

    bzero(buffer,256);
    n = read(insockfd,buffer,255);
	if (n < 0) perror("ERROR reading from socket");
    std::vector<std::string>* output = process_message(buffer);
    
    if (strcmp(buffer, "exit\n") == 0) {
        signal_exit.try_lock();
        delete output;
        close(insockfd);
        exit_callback();
        return;
    }
    
    n = write(insockfd, "OK", 2);
    if (n < 0) perror("ERROR writing to socket");
    close(insockfd);
      
    bzero(buffer, 256);
    delete output;
    return;
}
