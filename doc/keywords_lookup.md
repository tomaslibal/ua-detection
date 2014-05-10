## Product keywords lookup without a product keyword table

### Introduction

When reading a user agent string, how can you pick up the keyword which is the
product keyword that can identify the device which send the user agent? If you
knew all product keywords in advance, you could scan a table of all product keywords
for every keyword from the user agent but what if you don't have such a table?

This method is inspired from the nature and it simulates, in an essence, this
reasoning behaviour:

```
Scan the user agent string
Look at the place where the product keywords usually are located
Look at each keyword from that location
Evaluate the keyword
```

### Example

    Mozilla/5.0 (Linux; U; Android 2.3.3; de-ch; HTC Desire Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

Scan the user agent string

```
Tokenize each keyword
```

Look at the place where the product keywords usually are located

```
It looks like a user agent string of an Android device -> product keywords are
usually in the first comment section
```

Look at each keyword from that location

```
Linux   - occurs in user agents of multiple devices; high relative occurrence
U       - same as above
Android - same as above
2.3.3   - same as above
de-ch   - same as above
HTC     - occurs in user agents of multiple devices; lower relative occurrence
Desire  - single device occurence; low relative occurrence
Build/FRF91 - single device occurence; low relative occurence
```

Evaluate the keyword

```
[Linux, U, Android, 2.3.3 de-ch] low chance to be a product keyword
[HTC] higher chance to be a product keyword
[Desire, Build/FRF91] high chance to be a product keyword
```
