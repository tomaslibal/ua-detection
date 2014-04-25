# Database Model

## devices collection

    {
      "_id": <ObjectId1>,
      "group_id": <ObjectId3>,
      "type": "Smartphone",
      "name": "One (M7)",
      "brand": "HTC",
      "aka": ["One"]
    }

## ua_strings collection

Each document is one distinct user agent string. The `ua` property holds the
full string. This property could have a 'text index' on it if you would like
to use fulltext search (MongoDB 2.6+).

Confidence should signify what is the confidence that the given user agent string
belongs to the device.

    {
      "_id": <ObjectId2>,
      "device_id": <ObjectId1>,
      "ua": "Mozilla/5.0 (Linux; en-us; Android/4.4;) Firefox/28.0",
      "confidence": 1.0
    }

## groups collection

    {
      "_id": <ObjectId3>,
      "short_name": "android-phones",
      "name": "Android phones",
      "description": "All phones with Android OS"
    }

## keywords collection

keywords collection

One of the JavaScript tools loops each user agent string and parses their individual
keywords and stores them in this collection.

    {
      "_id": <ObjectId4>,
      "value": "Mozilla/5.0",
      "count": 3129
    }


## weights collection

This collection is populated by an algorithm that works like this:

- one_group = <chosen_group_id>
- foreach ua_string document
- do
-     keywords = ua_string.ua.match(/regex/)
-     if [device.find({ua_string.device_id}).group_id -eq one_group ]
-     then
-         adj = 0.01
-     else
-         adj = -0.01
-     weights.update({ keyword : { $in : keywords }, { $inc : { adj } })
- done

The learning algorithm uses this collection when preparing the training set.

    weights collection

    {
        "_id": <ObjectId5>,
        "keyword_id": <ObjectId4>,
        "keyword": "Mozilla/5.0",
        "group_id": <ObjectId3>,
        "value": -4.31
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
