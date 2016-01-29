#include "utils.h"

#include <string>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

void error(string msg) {
	perror(msg.data());
    exit(EXIT_FAILURE);
}
