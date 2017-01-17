#include "UaNgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"


int UaNgramBuilder::fromUserAgentString(std::string const& sentence, std::vector< Ngram >* ngrams)
{
    std::vector<std::string> uaTokens;
    
    tok.tokenize(sentence, &uaTokens);
    
    return this->fromTokenList(uaTokens, ngrams);
}

int UaNgramBuilder::fromUserAgentString(std::string const& sentence, std::vector< NgramSimple >* ngrams)
{
    std::vector<std::string> uaTokens;
    
    tok.tokenize(sentence, &uaTokens);
    
    return this->fromTokenList(uaTokens, ngrams);
}
