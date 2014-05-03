### A system to attribute user-agent string to its respective user-agent or device

#### A case study

---------------------------




| Column                              | Description                                  |
|:----------------------------------- |:-------------------------------------------- |
| Introduction                        | Explains the goal of `ua_detection` and how the detection works         |
| The User-Agent request-header       | Defines the set of user-agents that this case study works with |
| The method in brief                 | How the method can be used (by human users)  |
| The steps for a computerized method | The method written for computers             |
| Resources                           | List of references and bibliography          |



#### Introduction

This memo describes a method that can be used to identify certain user-agent strings and the devices/software or whatever other user-agent sent that string in a HTTP/1.1 request [[1]](http://tools.ietf.org/html/rfc2616).

It was first used by a human user and in this paper it will be formalized into a method that can be used by a computer device. 

The program `ua-detection` has the goal to adopt the process and by using
machine learning algorithms it should be able to dynamically learn about the
user-agent strings and when trained, it should be able to attain the skill
 (or feature, at least) of recognising which user-agent sent the user-agent
string.

#### The User-Agent request-header

Let's have a look at the definion of this request-header field [[2]](http://tools.ietf.org/html/rfc2616#section-14.43):

	User-Agent     = "User-Agent" ":" 1*( product | comment )

The definition says that the User-Agent field consists of product tokens ([[3] section 3.8 of the specification](http://tools.ietf.org/html/rfc2616#section-3.8)) and comments ([[4] section 2.2 of the specification](http://tools.ietf.org/html/rfc2616#page-17)) in order or significance.

>In my experience the user-agents that are more of a software tool character, such as web crawlers (e.g. Indexing Bots) or the cURL library, these user-agents send information enough to identify the user-agent themselve but they do not send additional information about the operating system they were run on or any information about the platform.
>
><br>
>
>On the other hand, user-agents that run as web clients for human users, notably web browsers, do send additional information identifying the operating system, platform, language settings and even the model of the hardware (often in the case of web browsers run on smartphones).

This case study works with the following set of user-agent string:

<pre>
[1]: Mozilla/5.0 (Linux; U; Android 4.1; en-us; HTC_PN071 Build/JZO54K) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30
[2]: Mozilla/5.0 (iPhone; CPU iPhone OS 5_0 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9A334 Safari/7534.48.3
[3]: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.131 Safari/537.36
</pre>

#### The method in brief

Let's take the first user-agent string from the set:

    Mozilla/5.0 (Linux; U; Android 4.1; en-us; HTC_PN071 Build/JZO54K) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30

Just by looking at it we can observe a couple of facts:

- we can see the product tokens and the comments
- reading the keyword left to right we can see:
   - Linux is a keyword for the operating system
   - Android is a mobile operating system
   - en-us is a language code for the US English
   - Mobile may signify this has something to do with a mobile user-agent
   - HTC_PN071 might be a keyword for a product of HTC the mobile manufacturer company
   - AppleWebKit is a HTML rendering technology
   - Safari is a Web browser - user-agent
   - etc.

##### The keywords and their position

We can decouple the keywords using the whitespace character class as a delimiter. Also, it seems that some keywords are more important than others for determining the user-agent and the device.

> The practice is telling me that for smartphones, the most important piece of information for establishing what device sent the string is the first comment section. This comment usually contains the platform, i.e. whether it is a iOS device, Android devices, etc. and in most cases it also contains the product name or the product codename.

<pre>
(Linux; U; <span style="background:lime;">Android 4.1</span>; en-us; <span style="background:lime;">HTC_PN071</span> Build/JZO54K)
</pre>


1. This suggests that by observation we can learn where the important keywords are usually positioned within the user-agent string.
2. Some keywords like `Mozilla/5.0` are used by a lot of different devices and user-agents and on a fine granular level seems not to be so important

These observations are later used in the computer version of the method to bias towards less and more important keywords.

##### Local variations

Another observation, especially if you examine a larger amount of user-agent strings, is that the string from one particular user-agent or device will have more forms which are "similar" to each other but differ in product version numbers, build numbers, locale versions (e.g. en-us, en-gb, es-es, etc.). 

This is quite important information because that means that the matching for a device is of a type `1:m (one-to-many)`.

> This assumes that no two devices share exactly the same user-agent string.



#### The steps for a computerized method

1. save_in_db( user_agent_strings[], devices[])
2. decouple keywords --> save_in_db(keywords[])
3. calculate weights --> save_in_db(weights[])
4. prepare training_set (data pre-processing) --> save_in_db(training_set[])
5. train a perceptron
6. run (user-agent-string, device) through the trained perceptron --> RESULT

##### 1) save the user-agent strings and a table of devices in the database

In addition to our set of user-agent strings a list of 2 devices will be used in this case study:

<pre>
[1] { .type="Smartphone", .model="One (M7)", .manufacturer="HTC" }
[2] { .type="Smartphone", .model="iPhone 5", .manufacturer="Apple" }
</pre>

These are saved in a database of choice.

> I use MongoDB for a couple of reasons: my parsing scripts are in JavaScript for the node.js environment and it is easy to work with MongoDB with node.js and JavaScript. It also has a flexible document structure, powerfull and easy lookup queries and I think it has a low overhead in terms of setting up the database and administering it.

**Important note: the user-agent[1] was linked to the device [1], and user-agent[2] was linked to device [2] manually in the database. You may optionally assign a confidence weight to this assignment.**

##### 2) decouple keywords and save them in the database

All keywords from the user-agent string set have been decouple by this rule:

- each keyword is either a product token (i.e. `product["/" version]`) or a word (`[[:alphanum::]]` character class) delimited by whitespace.

> In my regular expression that I used to decouple the keywords I actually take only the alphanum characters so for instance "en-us" becomes two keywords "en" and "us". Maybe I should change this.

|  Keyword             |   Count      |
|:-------------------- |:------------ |
| U                    | 1            |
| us                   | 1            |
| en                   | 1            |
| Gecko                | 3            |
| KHTML                | 3            |
| AppleWebKit/534.30   | 1            |
| like                 | 4            |
| K                    | 1            |
| iPhone               | 2            |
| Safari/534.30        | 1            |
| Mobile               | 1            |
| Version/4.0          | 1            |
| OS                   | 3            |
| CPU                  | 1            |
| 5_0                  | 1            |
| Mac                  | 2            |
| X                    | 2            |
| AppleWebKit/534.46   | 1            |
| Version/5.1          | 1            |
| Mobile/9             | 1            |
| A334                 | 1            |
| Safari/7534.48.3     | 1            |
| Macintosh            | 1            |
| Intel                | 1            |
| 10_9_2               | 1            |
| AppleWebKit/537.36   | 1            |
| Chrome/34.0.1847.131 | 1            |
| Safari/537.36        | 1            |
| Linux                | 1            |
| Mozilla/5.0          | 3            |
| 4.1                  | 1            |
| Android              | 1            |
| JZO54                | 1            |
| HTC_PN071            | 1            |
| Build                | 1            |


##### 3) calculate weights and save them in the database

Each keyword will have a weight assigned to it for each device. This means that for our 2 devices 2 weights will be created for each keyword. This is a high-level description of the algorithm:

```
rate      = 0.1;
devices[] = get_all_devices();

foreach devices[] as device_item
do
	user_agents[] = get_all_user_agent_strings();
	foreach user_agents[] as ua_item
	do
		if(ua_item._id == NULL) continue;
		keywords[]   = parse_keywords(ua_item);
		adj          = (device_item._id == ua_item._id) ? rate : -1*rate;
		foreach keywords[] as keyword
		do
			db.update_keywords_weight(keyword, device_item._id, adj);
		end;
	end;
end;
```
> This algorithm doesn't consider the position of the keywords or anything more complicated than simply looking what's the keywords usually occuring in the user-agent strings for the given device. This way, the weights should be
> 
> - positive: for the keywords that often appear in the user-agent strings for the given device
> - around zero: 
> - negative

> If you applied the 'confidence' weight on the assignemnt of devices to their respective user-agent strings, you may want to apply this weight when adjust the individual keyword's weight.


The next table shows results when the algorithm run through the set of devices and the keywords:

|  Keyword             |   Count      | Weight[1] (HTC One) | Weight[2] (iPhone 5) |
|:-------------------- |:------------ |:------------------- |:-------------------- |
| U                    | 1            | 0.1                 | -0.1                 |
| us                   | 1            | 0.1                 | -0.1                 |
| en                   | 1            | 0.1                 | -0.1                 |
| Gecko                | 3            | 0.0                 | 0.0                  |
| KHTML                | 3            | 0.0                 | 0.0                  |
| AppleWebKit/534.30   | 1            | 0.1                 | -0.1                 |
| like                 | 4            | -0.1                | 0.1                  |
| K                    | 1            | 0.1                 | -0.1                 |
| iPhone               | 2            | -0.2                | 0.2                  |
| Safari/534.30        | 1            | 0.1                 | -0.1                 |
| Mobile               | 1            | 0.1                 | -0.1                 |
| Version/4.0          | 1            | 0.1                 | -0.1                 |
| OS                   | 3            | -0.2                | 0.2                  |
| CPU                  | 1            | -0.1                | 0.1                  |
| 5_0                  | 1            | -0.1                | 0.1                  |
| Mac                  | 2            | -0.1                | 0.1                  |
| X                    | 2            | -0.1                | 0.1                  |
| AppleWebKit/534.46   | 1            | 0.1                 | -0.1                 |
| Version/5.1          | 1            | -0.1                | 0.1                  |
| Mobile/9             | 1            | -0.1                | 0.1                  |
| A334                 | 1            | -0.1                | 0.1                  |
| Safari/7534.48.3     | 1            | 0.1                 | -0.1                 |
| Macintosh            | 1            | -                   | -                    |
| Intel                | 1            | -                   | -                    |
| 10_9_2               | 1            | -                   | -                    |
| AppleWebKit/537.36   | 1            | -                   | -                    |
| Chrome/34.0.1847.131 | 1            | -                   | -                    |
| Safari/537.36        | 1            | -                   | -                    |
| Linux                | 1            | 0.1                 | -0.1                 |
| Mozilla/5.0          | 3            | 0.0                 | 0.0                  |
| 4.1                  | 1            | 0.1                 | -0.1                 |
| Android              | 1            | 0.1                 | -0.1                 |
| JZO54                | 1            | 0.1                 | -0.1                 |
| HTC_PN071            | 1            | 0.1                 | -0.1                 |
| Build                | 1            | 0.1                 | -0.1                 |

Note: the dash symbol `-` signifies that no weights were calculated (this is due to the fact that these keywords did not appear in any user-agent string of the two devices) but for the program that value is the same as `0.0`. 

Note 2: the symetricity of the weights is given by the fact that we only had 2 devices and 2 assigned user-agent strings. In a larger set the keywords would probably overlap in more instances and the weights will be assymetric.

##### 4) preprocess the data and save that as a training_set in the database

...

A set of inputs is determined in this step. This is probably the most experimental part of the method and the selection of the input might be refined in the future. Currently these are calculated for the input user-agent string:

- sample mean of the weights
- sample standard deviation of the weights
- character count of the user-agent string

Resulting training set:

<pre>
{
	_id: <ObjectId>,
	device_id: <ObjectId>,
	ua_string: "Mozilla/5.0 (Linux; U; Android 4.1; en-us; HTC_PN071 Build/JZO54K) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30",
	ua_string_id: <ObjectId>,
	input_vector: [ 0.072222, 0.057451, 138 ],
	expected_output: 1,
	vector_calculated: 1
},
{
	_id: <ObjectId>,
	device_id: <ObjectId>,
	ua_string: "Mozilla/5.0 (iPhone; CPU iPhone OS 5_0 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9A334 Safari/7534.48.3",
	ua_string_id: <ObjectId>,
	input_vector: [ 0.093333, 0.059362, 134 ],
	expected_output: 1,
	vector_calculated: 1
}
</pre>


##### 5) train a perceptron

...

##### 6) run a touple (user_agent_string, device) through the trained perceptron

...




### Resources 

References

1. HTTP/1.1 Specification at [http://tools.ietf.org/html/rfc2616](http://tools.ietf.org/html/rfc2616)
1. User-Agent request-header at [http://tools.ietf.org/html/rfc2616#section-14.43](http://tools.ietf.org/html/rfc2616#section-14.43)
2. Product tokens in HTTP headers at [http://tools.ietf.org/html/rfc2616#section-3.8](http://tools.ietf.org/html/rfc2616#section-3.8)
3. Comments in HTTP headers at [http://tools.ietf.org/html/rfc2616#page-17](http://tools.ietf.org/html/rfc2616#page-17)


Bibliography

* Weisstein, Eric W. "Standard Deviation." From MathWorld--A Wolfram Web Resource. http://mathworld.wolfram.com/StandardDeviation.html 