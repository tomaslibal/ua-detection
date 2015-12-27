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
#include "common/src/uadet2.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    /* 
     * The program expects to be run with at least one argument specifying 
     * the user agent string to be tested. If this test fails, exit with 
     * a failure status.
     */
    if (argc < 2) {
        print_usage();
        exit(EXIT_FAILURE);        
    }
    
    UadetSettings settings;
    
    try {
        parse_args(argc, argv, settings);
    } catch (const InvalidArgsException& e) {
        cout << "Error handling the program arguments!" << endl;
        cout << e.what() << endl;
        exit(1);
    }
    
    NaiveBayessClassifier nb;
    string dataFile = "data_in.txt";
    string ua = settings.ua;
    cout << "Using user-agent " + ua << endl;
    
    /* 
     * A lambda function that takes a string line, expecting it to be a pair
     * of <categories, user-agent string> separated by a tab character.
     * The function then calls NaiveBayesClassifier.add_data to add the new 
     * user agent string with its category(ies) to the memory.
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
    
    /*
     * A lambda function that takes the supplied user agent string and a category
     * and evaluates the probability P(ua|category).
     */
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