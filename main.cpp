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

void print_usage();

struct UadetSettings {
    string ua;
    string category;
};

class InvalidArgsException: virtual public exception{
public:
    InvalidArgsException(string m="Invalid Arguments!") : msg(m) {}
    ~InvalidArgsException() throw() {}
    const char* what() const throw() { return msg.c_str(); }
private:
    string msg;
};

void parse_args(int argc, char** argv, UadetSettings& settings);

/*
 * 
 */
int main(int argc, char** argv) {
    
    if (argc < 2) {
        print_usage();
        exit(1);
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

void foreach(vector<string>* vec, function<void (string)>& callback) {
    for(vector<string>::iterator it = vec->begin(); it != vec->end(); ++it) {
        callback(*it);
    }
}

void print_usage() {
    char tab = '\t';
    
    cout << "uadet2 User-agent String Classifier" << endl << endl;
    cout << tab << "uadet2 --ua='Mozilla/5.0 ...'" << endl;
    // cout << tab << "uadet2 --ua='Mozilla/5.0 ...' --category=mobile" << endl;
}

void parse_args(int argc, char** argv, UadetSettings& settings) {
    // user-agent
    string ua_arg;
    ua_arg = argv[1];
    
    string::size_type n = ua_arg.find("--ua=");
    if (n != string::npos) {
        settings.ua = ua_arg.substr(n+1);
    } else {
        throw InvalidArgsException("Cannot parse out the user-agent string");
    }
}