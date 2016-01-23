CREATE TABLE IF NOT EXISTS datapoints (
    id serial PRIMARY KEY,
    value varchar(512)
);

CREATE TABLE IF NOT EXISTS labels (
    id serial PRIMARY KEY,
    value varchar(128)
);

CREATE TABLE IF NOT EXISTS datapoint_labels (
    id serial PRIMARY KEY,
    datapoint_id integer NOT NULL,
    label_id integer NOT NULL,
    -- confidence is a floating point number in the interval [0, 1] 
    -- representing the confidence with which the label is assigned to the 
    -- datapoint, with 1 being "certainty" and 0 "uncertainty". Whether these
    -- are relative values or not is not specified.
    confidence real DEFAULT 1
);

