#ifndef _UA_NGRAM_BUILDER_H_
#define _UA_NGRAM_BUILDER_H_

#include "NgramBuilder.h"

#include <string>
#include <vector>

class UaNgramBuilder : public NgramBuilder 
{
public:
    UaNgramBuilder() : NgramBuilder() {};
    int fromUserAgentString(std::string const& sentence, std::vector<Ngram> *ngrams);
private:
    
};

#endif /* _UA_NGRAM_BUILDER_H_ */