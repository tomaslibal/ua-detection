#include "Server.h"
#include "../../common/src/uadet2.h"
#include "../../common/src/FileLog.h"
#include "./protocol.h"
#include "./utils.h"

#include <functional>
#include <utility>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <csignal>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>

#define BUFFERSIZE 512

Server::Server()
{
    ProgramConfig confCtrl(configFilePath);

    confCtrl.update(config);
    
    logger.setPath(config.logfile);
    log("*********** Initializing Server");
    
    sockfd = -1;
}

Server::~Server()
{
    log("*********** Server destroy");
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
    std::function<void (std::string const&)> add_line = [nbl](std::string const& line) {
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
    NetworkConfig const networkConfig(config.portno, config.hostname);
    Network const network(networkConfig);
    
    sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    
    //network.set_port_no(config.portno);
    
    sockfd = network.addr_listen();
    
    logger.log("Using port number " + std::to_string(config.portno));
    
    logger.log("Now listening to incoming connections");
        
    NaiveBayessClassifier * nbl = &nb;
    
    /* 
     * Worker is started as a new thread, it processes the incoming request
     * in a detached state so it should not block other requests.
     */
    std::function<void (int, std::atomic<int> &)> worker2 = [nbl, this](int in_sockfd, std::atomic<int> & nIncomingReq) {
        evaluate_incoming_request(in_sockfd, *nbl, nIncomingReq);
    };
    
    while(1) {
        /*
         * waits until an incoming connection is made
         */
        log("Waiting for a connection");
        
        int in_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (in_sockfd < 0) { perror("in_sockfd < 0"); }
        
        log("Incoming connection...Creating a new thread to handle it");
 
        std::thread a_thread (worker2, in_sockfd, std::ref(nIncomingMsg));
        a_thread.detach();
        log("Connection #" + std::to_string(nIncomingMsg++) + ", thread created");
    }

    close(sockfd);
}

void Server::stop()
{
    close(sockfd);
    
    log("Server stopping...");
    log("Num connections was " + std::to_string(nIncomingMsg));
}


/**
 * 
 */
void Server::evaluate_incoming_request(int insockfd, NaiveBayessClassifier& nbc, std::atomic<int> & nIncomingMsg) {
  
    FileLog logger;
    logger.setPath(std::string("eval.") + std::to_string(insockfd) + std::string(".log.txt"));
    logger.log("Got a request");
    
    /**
     * 
     */
    socklen_t clilen;
    /**
     * Input buffer. The incoming connection data is read into it.
     */
    char buffer[BUFFERSIZE];
    /**
     * 
     */
    int n;

    /**
     * Check that we have a valid socket descriptor
     */
    assert(insockfd >= 0);

    logger.log("Connection established (" + std::to_string(insockfd) + ")");

    bzero(buffer,BUFFERSIZE);
    n = read(insockfd,buffer,BUFFERSIZE);

    if (n < 0) perror("ERROR reading from socket");
    
    logger.log(std::string("Request was: ") + std::string(buffer));

    std::vector<std::string>* input = process_message(buffer);
    
    std::string* output = classify_data(*input, nbc);
    
    logger.log(std::string("Output was: " + *output));
    
    n = write(insockfd, output->data(), output->length() * sizeof(char));
    
    delete output;
    
    if (n < 0) perror("ERROR writing to socket");
    close(insockfd);
      
    bzero(buffer, BUFFERSIZE);
    delete input;
    return;
}

std::string* Server::classify_data(std::vector<std::string>& input, NaiveBayessClassifier& nbc)
{
    std::string* p_str = nullptr;
    std::string aux;

    std::ostringstream pstrs;

    /*
     * If token[0] == "eval_one" then evaluate the given user-agent (token[2]) using
     * the naive bayess classifier for the specified category (token[1])
     */
    if (input.at(0) == "eval_one") {
        double p = nbc.classify(input.at(2), input.at(1));

        pstrs << p;

        std::vector<double> results;
        results.push_back(p);
        std::vector<std::string> labels;
        labels.push_back(input.at(1));

        if (config.outputType == OutputType::json) {
            p_str = json_output(results, labels);
        } else {
            p_str = plaintext_output(results, labels);
        }
        return p_str;
    // If token[0] == "eval" then:
    // Evaluate the UA (output->at(2)) against all classes and decides if the given class 'mobile' (output->at(1))
    // was the most probable or not
    } else if (input.at(0) == "eval") {
        std::vector<std::string>* categories = nbc.get_categories();
    
        /*
         * Keeps evalution results for each category:
         *     cateogory, eval_result
         */
        std::map<double, std::string, std::greater<double>> results;
        
        for (std::vector<std::string>::iterator it = categories->begin(); it != categories->end(); ++it) {
            std::string category = *it;
            double p = nbc.classify(input.at(2), category);
            /*
             * Use the activation function to produce the likelyhood of the user-agent 
             * being in the given class.
             */
            double pct = sigm(p) * 100;

            results.insert(std::pair<double, std::string>(pct, category));
        }

        /* get softmax values */
        std::vector<double> non_normalized;
        std::vector<std::string> labels;

        for (auto& item: results) {
	    non_normalized.push_back(item.first);        
            labels.push_back(item.second);
        }

        std::vector<double> normalized = softmax(non_normalized);
        
        /*
         * Now print the normalized values together with their respective labels
         * from the buffer.
         * 
         */ 	
        if (config.outputType == OutputType::json) {
            p_str = json_output(normalized, labels);	    	
        } else {
            p_str = plaintext_output(normalized, labels);
        }    
        delete categories;
    
        return p_str;
    } else if (input.at(0) == "add") {
        std::string category = input.at(1);
        std::string ua_agent = input.at(2);
        
        nbc.add_data(ua_agent, category);
        return new std::string("added OK");
    } else {
        return new std::string("Command not understood");
    }
}

std::string*  Server::json_output(std::vector<double>& values, std::vector<std::string>& labels)
{
    std::ostringstream json;

    json << "{\n"; 

    assert(values.size() == labels.size());

    int size = values.size();

    for (size_t i = 0; i < size; ++i) {
        json << "    \"" << labels[i] << "\": " << std::fixed << values[i];
        // if not the last key-value pair, add a trailing comma
        if (i < (size - 1)) {
            json << ",";
        }

        json << std::endl;
    }
 
    json << "}";
  
    return new std::string(json.str());
}

std::string* Server::plaintext_output(std::vector<double>& values, std::vector<std::string>& labels)
{
    std::ostringstream output;

    assert(values.size() == labels.size());

    int size = values.size();

    for (size_t i = 0; i < size; ++i) {
        output << labels[i] << ":" << std::fixed << values[i] << std::endl;
    }
 
    return new std::string(output.str());
}
