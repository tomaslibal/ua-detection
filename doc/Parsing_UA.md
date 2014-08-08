# Parsing the `user-agent` HTTP header field

This document describes how the program tokenizes and parses the `user-agent`
header field. All references to `user-agent` highlighted like this refer to 
the HTTP/1.1 Header Field as specified in the section 14.43 of the [standard](http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.43).

   > more info on the [user-agent field](/ua_detection/docs/user_agent.md)
   
## Tokenizer

Example User-Agent string

    Mozilla/5.0 (Linux; U; en-us; Android 4.4) Firefox/30.0
    
Sigma: 

    E = { a, b }

1. a (token) = 1*<any CHAR except CTLs or separators>
2. b (separators) = { ( ) < > @ , ; : \ " / [ ] ? = "{" "}" SP HT }

* That `1*` in the tokens probably means one or more
* CTLs probably means control characters, SP probably space, HT = ?
* I remove "/" from the separators for now

Pseudo-code to tokenize the string

    char separators[18] = { "(", ")", "<", ..., H };
    
    /*
     user_agent is the input string
     buffer is the array of tokens
     length is the length of the array (number of the tokens)
     */
    int tok(char* user_agent, char** buffer, int* length) {
        i = 0
        *length = 0
        token_started = 0
        string current_token = \0
        char c = \0
        // read the user-agent string character by character until the end
        // of the string
        while(c = char(user_agent, i) != \0) {
            // a separator means that a new token starts right after this char
            // and therefore the current string of characters has to be pushed
            // onto the buffer
            // (but only if there is actually any token - this will prevent from
            // having a series of empty tokens if there are more separator chars
            // next to each other - like spaces)*can this happen according to the
            // specification?
            if(in_array(separators, c) && strlen(current_token)>0) {
                // add the token to the buffer, increment the array length and
                // clear the current token
                array_push(buffer, current_token)
                *length++
                current_token = \0
            } else {
                // if c is any other character, just continue building the 
                // string into the current_token
                current_token += c
            }
            i++;
        }
        // after jumping out of the loop, the current_token variable may contain
        // the last token so we have to check the contents
        if(strlen(current_token)>0) {
            array_push(buffer, current_token);
            *length++;
        }
        return 0;
    }





   
