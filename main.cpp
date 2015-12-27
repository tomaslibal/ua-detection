/* 
 * File:   main.cpp
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:02 PM
 */

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

#include "FileInputReader/src/FileInputReader.h"
#include "NaiveBayessClassifier/src/NaiveBayessClassifier.h"

using namespace std;

/*
 * Function foreach applies the callback function to each element of the vector
 */
void foreach(vector<string>* vec, function<void (string)> &callback);

/*
 * 
 */
int main(int argc, char** argv) {
    NaiveBayessClassifier nb;
    string dataFile = "data_in.txt";
    string ua = "Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0";
    cout << "Using user-agent " + ua << endl;
    
    
    /* 
     * Open data_in and read the <category, user-agent-string> pairs. The columns
     * are assumed to be separated by a \t character.
     */
    function<void (string)> add_line = [&nb](string line) {
        string category, uas;
        string::size_type n;
        
        n = line.find('\t');
        
        if (n != string::npos) {
            category = line.substr(0, n);
            uas = line.substr(n+1);
    
            nb.add_data(uas, category);            
        }
    };
    
    FileInputReader reader;
    reader.readLines(dataFile, add_line);
    
    /*
     * Prints summary of number of categories and n-grams that have been read.
     */
    nb.stats();
    
    function<void (string)> eval_ua_in_category = [&ua, &nb](string category) {
        cout.precision(5);        
        cout << "P(ua|" + category << ")=" << nb.classify(ua, category) << endl;
        cout << "P(category)=" << nb.prob_category(category) << endl;
    };
    
    vector<string>* categories = nb.get_categories();
    
    foreach(categories, eval_ua_in_category);
    
    delete categories;
    
    return 0;
}

void foreach(vector<string>* vec, function<void (string)>& callback) {
    for(vector<string>::iterator it = vec->begin(); it != vec->end(); ++it) {
        callback(*it);
    }
}