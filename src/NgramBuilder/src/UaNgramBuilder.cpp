#include "UaNgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"


int UaNgramBuilder::fromUserAgentString(std::string& sentence, std::vector< Ngram >* ngrams)
{
    UATokenizer tok;
    std::vector<std::string> uaTokens;
    
    tok.tokenize(sentence, &uaTokens);
    
    return this->fromTokenList(uaTokens, ngrams);
}
