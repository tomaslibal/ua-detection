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
void evaluate_incoming_request(int insockfd, NaiveBayessClassifier& nbc);

/**
 * Classifies the given data using NaiveBayessClassifier
 * 
 */
std::string* classify_data(std::vector<std::string>& input, NaiveBayessClassifier& nbc);

#endif /* SOCKETS_H */

