#include "sockets.h"
#include "utils.h"
#include "protocol.h"

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

void wait_and_accept(sockaddr_in* cli_addr, int insockfd, function<void ()>& exit_callback, unique_lock<mutex>& signal_exit, NaiveBayessClassifier& nbc) {
    socklen_t clilen;
    char buffer[256];
    int n;

	assert(insockfd >= 0);
    
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
    
    /*
     * If token[0] == "eval_one" then evaluate the given user-agent (token[2]) using
     * the naive bayess classifier for the specified category (token[1])
     */
    if (output->at(0) == "eval_one") {
        
        double p = nbc.classify(output->at(2), output->at(1));
        
        std::ostringstream pstrs;
        pstrs << p;
        std::string str = pstrs.str();
        n = write(insockfd, str.data(), str.length());
    // If token[0] == "eval" then:
    // Evaluate the UA (output->at(2)) against all classes and decides if the given class 'mobile' (output->at(1))
    // was the most probable or not
    } else if (output->at(0) == "eval") {
        std::vector<std::string>* categories = nbc.get_categories();
	
	/*
	 * Keeps evalution results for each category:
	 *     cateogory, eval_result
	 */
	std::map<double, std::string, std::greater<double>> results;
        std::ostringstream pstrs;
        double threshold = 0.75;
        double highest_prob = 0;
        double p_category = 0;
        bool has_highest_prob = false;
        
        for (std::vector<std::string>::iterator it = categories->begin(); it != categories->end(); ++it) {
            std::string category = *it;
            double p = nbc.classify(output->at(2), category);
        
            if (p > highest_prob) {
                highest_prob = p;
                
                if (category == output->at(1)) {
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
	 * Now results are in order by the highest probability descending
	 * 
	 */
	for (auto& item: results) {
	    pstrs << item.second << ":" << item.first << std::endl;
	}
                
        // If the given category (output->at(1)) has highest probability, then
        // is_in_category(UA, category) = maybe
        // If is_in_category == maybe and p(category|UA) > threshold then is_in_category = true
        std::string result;
        if (has_highest_prob == true) {
            result = "maybe";
            
            if (p_category > threshold) {
                result = "true";
            }
        } else {
            // alternatively, if p(category|UA) is not the highest but it is
            // over the threshold, is_in_category = maybe, otherwise false.
            result = "false";
            
            if (p_category > threshold) {
                result = "maybe";
            }
        }
        
        pstrs << "UA is in " << output->at(1) << ": " << result << std::endl;
        
        std::string str = pstrs.str();
        n = write(insockfd, str.data(), str.length());
        
        delete categories;
    } else if (output->at(0) == "add") {
        std::string category = output->at(1);
        std::string ua_agent = output->at(2);
        
        nbc.add_data(ua_agent, category);
        n = write(insockfd, "added OK", 8*sizeof(char));
    } else {
        n = write(insockfd, "\nCommand not understood", 23);
    }
    
    if (n < 0) perror("ERROR writing to socket");
    close(insockfd);
      
    bzero(buffer, 256);
    delete output;
    return;
}
