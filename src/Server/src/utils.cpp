#include "utils.h"

#include <string>
#include <iostream>
#include <cmath>

using std::string;
using std::cerr;
using std::endl;

void error(string msg)
{
    perror(msg.data());
    exit(EXIT_FAILURE);
}

double get_percentile(double value, std::map<double, std::string, std::greater<double>>& population)
{
    int size = population.size();
    int num_higher_values = 0;
    int num_smaller_or_equal_values = 0;
    
    for (auto& item: population) {    
	if (item.first <= value) {
	  num_smaller_or_equal_values++;
	}
    }
    
    num_higher_values = size - num_smaller_or_equal_values;
    
    return std::round( num_smaller_or_equal_values / size );
}

double sigm(double const x)
{
    return 1 / (1 + exp(-x));
}