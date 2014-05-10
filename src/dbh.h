#ifndef DBH_HEADER
#define DBH_HEADER

#include "mongo.h"
#include <stdio.h>

extern mongo* dbh_get_conn();
unsigned int  get_doc_cnt(const char* coll);
double        dbh_get_double(mongo *conn,const char *db_and_coll,const char *fkey,const char *fval,const char *s);
int           dbh_init(uint16_t *port);
int           dbh_test();
static void   test_query( mongo *conn );
static void   test_empty_query(mongo *conn);

#endif
