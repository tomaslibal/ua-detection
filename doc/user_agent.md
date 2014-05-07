### HTTP and the User-Agent header field

The HTTP specification refers to the RFC-2616 (Hypertext Transfer Protocol -- [HTTP/1.1](http://tools.ietf.org/html/rfc2616))

User-Agent header field is specified in section [14.43](http://tools.ietf.org/html/rfc2616#section-14.43) as:

    User-Agent     = "User-Agent" ":" 1*( product | comment )

Product tokens are specified in [section 3.8](http://tools.ietf.org/html/rfc2616#section-3.8) as:

    product         = token ["/" product-version]
    product-version = token

Tokens are specified as:

    token          = 1*<any CHAR except CTLs or separators>
    separators     = "(" | ")" | "<" | ">" | "@"
                  | "," | ";" | ":" | "\" | <">
                  | "/" | "[" | "]" | "?" | "="
                  | "{" | "}" | SP | HT

Comments are specified as:

    comment        = "(" *( ctext | quoted-pair | comment ) ")"
    ctext          = <any TEXT excluding "(" and ")">

### Tokenizing the keywords

For the purposes of this repository, when we say "keyword" we esentially mean a token from the user-agent string (and by saying "user-agent string" we mean the value of the user-agent header field).

So keyword for us mean not only an individual product token but we accept that the comments contain one or more keywords too.

I will give an example:

    (Linux; U; en-us; Android 4.4)

The comment above, for us, contains 5 keywords: `["Linux;", "U;", "en-us;", "Android", "4.4"]` One may argue that 4.4 goes together with Android which might be true but for keeping things simple we assume that the keywords are delimited by white-space (spacebar characters).

### Finite-state-machine for the User-Agent header

![The states](https://31.media.tumblr.com/e03186d80216603b72dfb19a4c20a2c7/tumblr_inline_n5807cuv1i1snsz0s.png "The states")

I posted the above image on my Tumblr as a working draft for states in a finite-state-machine for tokenizing the user-agent string.

The goal is to write a tokenizer that takes the user-agent string as an input

    Mozilla/5.0 (Linux; U; en-us; Android 4.4) Firefox/30.0

and produces something like the following output:

```lisp
    (user-agent
        (keyword Mozilla/5.0),
        (keyword Linux;),
        (keyword U;),
        (keyword en-us;),
        (keyword Android),
        (keyword 4.4),
        (keyword Firefox/30.0)
    )
```
