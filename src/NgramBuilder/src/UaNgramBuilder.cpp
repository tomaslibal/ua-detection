#include "UaNgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"

int UaNgramBuilder::fromUserAgentString(std::string const& sentence, std::vector< NgramSimple >* ngrams)
{
    char** uaTokens = (char**)malloc(sizeof(char*)*128);
    int num = 0;
    
    tok.tokenize(sentence, uaTokens, &num);
    
    return this->fromTokenList(uaTokens, num, ngrams);
}
