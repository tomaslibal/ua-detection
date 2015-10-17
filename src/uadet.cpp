/*
 * User-agent Detection 2
 *  
 * File:   uadet.cpp
 * Author: tomaslibal
 *
 * Created on 17 October 2015, 21:28
 */

#include <cstdlib>
#include <iostream>

#include "ngram/NgramBuilder.h"

#include "util/bitmask.h"
#include "util/logging.h"
#include "util/memutil.h"

using namespace std;

/*
 * Some options:
 * 
 * --training-set <path> path to the training set file
 * --training-set-delimiter <char> delimits columns in the training set file
 * --uas <string> user-agent string
 * 
 */
int main(int argc, char** argv) {

    NgramBuilder *ngb = new NgramBuilder();
    
    Ngram *ngram;
    
    const char *uas = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:43.0) Gecko/20100101 Firefox/43.0";
    
    int l = ngb->fromUserAgentString(uas, ngram);
    
    std::cout << ">>> UA Detection 2\n";
    std::cout << "User-agent=" << uas << "\n";
    std::cout << ngb->level << "-level N-grams=" << l << "\n";
    
    for(int i = 0; i < l; i++) {
        if (ngram[i].len != NULL) {
            ngb->print(&ngram[i]);
        }
    }
    
    return 0;
}

