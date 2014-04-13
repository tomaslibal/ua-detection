# Database Model

## phones collection

    {
      "_id": <ObjectId1>,
      "group_id": <ObjectId3>,
      "name": "One",
      "brand": "HTC"
    }

## ua_strings collection

    {
      "_id": <ObjectId2>,
      "phone_id": <ObjectId1>,
      "ua": "Mozilla/5.0 (Linux; en-us; Android/4.4;) Firefox/28.0"
    }

## groups collection

    {
      "_id": <ObjectId3>,
      "short_name": "android-phones",
      "name": "Android phones",
      "description": "All phones with Android OS"
    }

## keywords_weights

This collection is populated by an algorithm that works like this:

- choose a 'group' for which we will calculate the weights
- take each User-Agent and parse it into separate keywords
- find the current weight of the keyword for the given group
- if the keyword is new, assign it a weight of 0.0
- for each keyword decrease or increase its weight by 0.01x depending if the
User-Agent string was of a phone from the given group (then increase the weight)
or decrease the weight if the phone was not in the target group

The learning algorithm will use this collection as its training set.

{
  "_id": <ObjectId4>,
  "group_id": <ObjectId3>,
  "keyword": "Mozilla/5.0",
  "count": 3129,
  "weight": 2.09
}

If we then assume a group 'Android phones' the idea is that keywords that often
appear in User-Agent strings from Android phones will have a positive weight
values, keywords that equally often appear in Android phones' User-Agent strings
as well as in User-Agents strings from other devices will have a neutral weight
values and keywords that are found in non Android devices (e.g. Windows) will
have negative weights.

### Example:

Group: Android phones

User-Agent 1 of n: "Mozilla/5.0 (Linux; en-us; Android/4.4;) Firefox/28.0"

Is this User-Agent from a phone that belongs to the group: TRUE

Keywords: Mozilla/5.0, Linux, en-us, Android/4.4, Firefox/28.0

Mozilla/5.0: current weight 0.0, increase by 0.01 to 0.01

...

Firefox/28.0: current weight 0.0, increase by 0.01 to 0.01
