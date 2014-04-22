#include "dbh.h"
#include "mongo.h"

uint16_t          port;
char              *host_and_port;
char              *str;
int               init = 0;

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
            printf("User-Agent: %s, count: %d\n", bson_iterator_string(keyword), bson_iterator_int(cnt));
        }
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );
}

int dbh_test()
{
    mongo conn[1];
    int status = mongo_client( conn, "127.0.01", 27017 );

    if (status != MONGO_OK) {
        switch ( conn->err ) {
            case MONGO_CONN_NO_SOCKET:  printf("no socket\n"); return 1;
            case MONGO_CONN_FAIL:       printf("connection failed\n"); return 1;
            case MONGO_CONN_NOT_MASTER: printf("not master\n"); return 1;
        }
    }

    //test_empty_query(conn);
    //test_query(conn);

    mongo_destroy(conn);
    return 0;
}

int dbh_init(uint16_t *port)
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
