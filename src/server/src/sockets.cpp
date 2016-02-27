#include "sockets.h"
#include "utils.h"
#include "protocol.h"
#include "../../common/src/FileLog.h"

#include <iostream>
#include <sstream>
#include <cassert>

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

#define BUFFERSIZE 512

/**
 * 
 */
void evaluate_incoming_request(int insockfd, function< void () >& exit_callback, unique_lock< mutex >& signal_exit, NaiveBayessClassifier& nbc) {
  
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

    cout << "Connection established (" << insockfd << ")" << endl;

    bzero(buffer,BUFFERSIZE);
    n = read(insockfd,buffer,BUFFERSIZE);

    if (n < 0) perror("ERROR reading from socket");
    
    if (strcmp(buffer, "exit\n") == 0) {
        signal_exit.try_lock();
        close(insockfd);
        exit_callback();
        return;
    }
    
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

std::string* classify_data(std::vector<std::string>& input, NaiveBayessClassifier& nbc)
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
        
	p_str = new std::string(pstrs.str());
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
        
        double threshold = 0.75;
        double highest_prob = 0;
        double p_category = 0;
        bool has_highest_prob = false;
        
        for (std::vector<std::string>::iterator it = categories->begin(); it != categories->end(); ++it) {
            std::string category = *it;
            double p = nbc.classify(input.at(2), category);
        
            if (p > highest_prob) {
                highest_prob = p;
                
                if (category == input.at(1)) {
                    has_highest_prob = true;
                    p_category = p;
                } else {
                    has_highest_prob = false;
                }
            }
            
            //pstrs << category << ":" << p << std::endl;
	    results.insert(std::pair<double, std::string>(p, category));
        }     
        
        /*
	 * Now results are in order by the highest probability descending. Print them with true/false flags
	 * signifying if the value exceeded the threshold or not.
	 * 
	 */	
	for (auto& item: results) {
	    aux = item.first > threshold ? "true" : "false";
	    
	    pstrs << item.second << ":" << item.first << "," << aux << std::endl;
	}

	p_str = new std::string(pstrs.str());
	        
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
