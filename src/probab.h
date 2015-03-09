#ifndef PROBAB
#define PROBAB

#include "reader.h"
#include "htable_int.h"
#include "tokenizer.h"

void count_words(struct uas_record *datapoint, struct htable_int *store);

#endif
