#include <stdlib.h>
#include <stdio.h>
#include "mongo.h"

int num_tests = 0;
int p = 0;
#include "suite.h"

#include "../src/dbh.h"

/* clang ../src/dbh.c suite.c test_dbh.c */

int main()
{
    printf("Executing test case 'test_dbh'\n");

    /* test connection */
    mongo *testconn = dbh_get_conn();
    ASSERT("Expected testconn->connected to be 1", 1==testconn->connected);

    /* test delete */

    bson cond[1];
    bson_init(cond);
    bson_finish(cond);
    int rem = mongo_remove(testconn, "ua_detection.test", cond, NULL);
    bson_destroy(cond);
    ASSERT("Expect rem = MONGO_OK", rem==0);

    /* test insert */
    bson doc[1];
    bson_init(doc);
        bson_append_string(doc, "hello", "world");
        bson_append_int(doc, "int", 1);
        bson_append_double(doc, "double", 1.0);
    bson_finish(doc);
    int val = dbh_insert(testconn, "ua_detection.test", doc);
    ASSERT("Expect dbh_insert to return MONGO_OK", val==MONGO_OK);

    /* test the inserted documented - dbh_get_double */
    double dbl = dbh_get_double(testconn, "ua_detection.test", "hello", "world", "double");
    ASSERT("Expect dbl = 1.0", 1.0==dbl);

    /* test the inserted documented - dbh_get_string */
    char *res;
    res = dbh_get_string(testconn, "ua_detection.test", "hello", "world", "hello");
    ASSERT("Expect res = \"world\"", 0==strcmp(res, "world"));
    free(res);

    /* test count */
    unsigned int cnt = get_doc_cnt("test");
    printf("cnt = %d\n", cnt);
    ASSERT("Expect cnt = 1", 1==cnt);

    //mongo_destroy(testconn);

    return done(num_tests, p);
}
