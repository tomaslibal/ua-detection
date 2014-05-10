#include "dbh.h"
#include "mongo.h"
#include <string.h>

uint16_t          port = 27017;
char              *host_and_port;
char              *username;
char              *password;

static void test_empty_query( mongo *conn ) {
    mongo_cursor cursor[1];
    mongo_cursor_init( cursor, conn, "ua_detection.ua_strings" );

    while( mongo_cursor_next( cursor ) == MONGO_OK ) {
        bson_iterator iterator[1];

        if ( bson_find(iterator, mongo_cursor_bson(cursor), "ua") ) {
            printf("User-Agent: %s\n", bson_iterator_string(iterator));
        }
    }

    mongo_cursor_destroy( cursor );
}

static void test_query( mongo *conn ) {
    bson query[1];
    mongo_cursor cursor[1];

    bson_init( query );

    bson_append_start_object( query, "count" );
        bson_append_int( query, "$gt", 3);
    bson_append_finish_object( query );

    bson_finish( query );

    mongo_cursor_init( cursor, conn, "ua_detection.keywords" );
    mongo_cursor_set_query( cursor, query );

    while( mongo_cursor_next( cursor ) == MONGO_OK ) {
        bson_iterator keyword[1];
        bson_iterator cnt[1];
        if ( bson_find(keyword, mongo_cursor_bson(cursor), "value") &&
        bson_find(cnt, mongo_cursor_bson(cursor), "count") ) {
            printf("Keyword: %s, count: %d\n", bson_iterator_string(keyword), bson_iterator_int(cnt));
        }
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );
}

// dbh_get_double(conn, "ua_detection.keywords", "value", "Mozilla/5.0", "count")
// dbh_get_double(conn, "ua_detection.keywords", "value", "Mozilla/5.0", "avg_position")
double dbh_get_double(
mongo *conn,
const char *db_and_coll,
const char *fkey,
const char *fval,
const char *s)
{
    double val = 0.0;
    bson query[1];
    mongo_cursor cursor[1];

    bson_init( query );
        bson_append_string( query, fkey, fval);
    bson_append_finish_object( query );

    bson_finish( query );

    mongo_cursor_init( cursor, conn, db_and_coll );
    mongo_cursor_set_query( cursor, query );

    while( mongo_cursor_next( cursor ) == MONGO_OK ) {
        bson_iterator value[1];
        if ( bson_find(value, mongo_cursor_bson(cursor), s) ) {
            printf("Found %f\n", bson_iterator_double(value));
            val = bson_iterator_double(value);
        }
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );
    return val;
}


int dbh_test()
{
    mongo *conn = dbh_get_conn();
    // test_empty_query(conn);
    // test_query(conn);
    // char *coll;
    // coll = (char*) malloc(sizeof(char)*9);
    // strcpy(coll, "weights");
    // double cnt = get_doc_cnt(coll);
    // printf("doc count = %f\n", cnt);
    // free(coll);
    mongo_destroy(conn);
    return 0;
}

// Creates a new database handle pointer and returns it
// Exits on any connection problem
mongo* dbh_get_conn()
{
    static mongo conn[1];
    int status = mongo_client( conn, "127.0.0.1", port );

    if (status != MONGO_OK) {
        switch ( conn->err ) {
            case MONGO_CONN_NO_SOCKET:  printf("No socket\n"); exit(1);
            case MONGO_CONN_FAIL:       printf("Connection failed\n"); exit(1);
            case MONGO_CONN_NOT_MASTER: printf("Not master\n"); exit(1);
            default: printf("Error connecting to MongoDB\n"); exit(1);
        }
    }

    return conn;
}

// Counts the number of documents in a collection
// @param const char * coll - the collection name
unsigned int get_doc_cnt(const char* coll)
{
    mongo dbh = *dbh_get_conn();
    unsigned int cnt = mongo_count( &dbh, "ua_detection", coll, NULL);
    if (cnt == MONGO_ERROR) {
        mongo_destroy(&dbh);
        return -1;
    }
    mongo_destroy(&dbh);
    return cnt;
}

int dbh_init_config(uint16_t *port)
{
  char line[80];
  char *param;
  char *val;

  char username[53];
  char password[53];

  FILE *config_file;

  param = (char*) malloc (26); // 25 max chars for the param
  val   = (char*) malloc (54);  // 53 max chars for the value

  // Get the working directory first and then locate the config file
  config_file = fopen ("../data/db_config.txt", "r");

  if (config_file == NULL) {
    return 1;
  }

  // Read the config file
  while(fgets(line, 80, config_file) != NULL) {
     sscanf(line, "%s = %s", param, val);

     if (strcmp(param, "username") == 0)
       strcpy(username, val);

     if (strcmp(param, "password") == 0)
       strcpy(password, val);

  }
  fclose(config_file);

  free(param);
  free(val);

  return 0;
}
