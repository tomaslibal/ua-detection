#ifndef _UA_NGRAM_BUILDER_H_
#define _UA_NGRAM_BUILDER_H_

#include "NgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"

#include <string>
#include <vector>

class UaNgramBuilder : public NgramBuilder 
{
public:
    UaNgramBuilder() : NgramBuilder() {};
    int fromUserAgentString(std::string const& sentence, std::vector<NgramSimple> *ngrams);
private:
    UATokenizer tok;
};

#endif /* _UA_NGRAM_BUILDER_H_ */