#ifndef DBH_HEADER
#define DBH_HEADER

#include "mongo.h"

extern mongo* dbh_get_conn();
unsigned int  get_doc_cnt(const char* coll);
char          *dbh_get_string(mongo *conn,const char *ns,const char *fkey,const char *fval,const char *s);
mongo_cursor  *dbh_search_by_string(mongo *conn,const char *ns,const char *fkey,const char *fval);
double        dbh_get_double(mongo *conn,const char *db_and_coll,const char *fkey,const char *fval,const char *s);
int           dbh_insert(mongo *conn, const char *ns, bson *doc);
int           dbh_init(uint16_t *port);
int           dbh_test();
static void   test_query( mongo *conn );
static void   test_empty_query(mongo *conn);

#endif
