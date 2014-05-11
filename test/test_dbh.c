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
    mongo_destroy(testconn);
    /* test insert */

    /* test the inserted documented */

    /* test count */

    return done(num_tests, p);
}
