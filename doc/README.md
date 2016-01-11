# Documentation

- [Parsing the User-Agent string](Parsing_UA.md)
- [User-Agent string definition](user_agent.md)
- [Manual: basic usage of the program](uadet.md)

## Hashing function for User-Agent strings

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
