/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sockets.h
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:32 PM
 */

#ifndef SOCKETS_H
#define SOCKETS_H

#include <functional>
#include <mutex>

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../../NaiveBayessClassifier/src/NaiveBayessClassifier.h"

int create_socket_inet_stream();

void wait_and_accept(sockaddr_in* cli_addr, int insockfd, std::function<void ()>& exit_callback, std::unique_lock<std::mutex>& signal_exit, NaiveBayessClassifier& nbc);

#endif /* SOCKETS_H */

