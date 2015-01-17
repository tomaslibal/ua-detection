# Documentation

- [Parsing the User-Agent string](Parsing_UA.md)
- [User-Agent string definition](user_agent.md)

## Hashing function for User-Agent strings

- [Node.js implementation](../tools/ua_string_hash.js)

In the file above I have described the hashing function in terms of its
pseudocode and I also implemented it in node.js.

Function prototype in C would look something like this:

````c
    /*
        Returns a number in the interval <0, 10000> for any given array
        of keywords, e.g. ["Mozilla", "Intel", "OS", "X", "10_9", ...]

        param String keywordsArray* is pointer to the array of keywords
        param int numKeywords is the number of keywords in the array
    */
    float ua_string_hash(String keywordsArray*, int numKeywords)
````
