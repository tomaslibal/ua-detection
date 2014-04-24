#ifndef DBH_HEADER
#define DBH_HEADER

#include "mongo.h"
#include <stdio.h>

static void test_query( mongo *conn );
static void test_empty_query(mongo *conn);
extern mongo* dbh_get_conn();

unsigned int get_doc_cnt(const char* coll);

int dbh_init(uint16_t *port);
int dbh_test();

#endif
