#include "dbh.h"

mongoc_database_t *database;
mongoc_cursor_t   *cursor;
mongoc_client_t   *client;
const bson_t      *reply;
uint16_t          port;
bson_error_t      error;
bson_t            ping;
char              *host_and_port;
char              *str;
int               init = 0;

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

  //*host_and_port = bson_strdup_printf("mongodb://root:root@localhost:%du", port);
  //host_and_port = bson_strdup("mongodb://root:root@localhost:27017");

  free(param);
  free(val);

  return 0;
}

int dbh_test()
{
  if (init == 0)
    dbh_init(&port);

  // mongoc_init();
  // client = mongoc_client_new(host_and_port);
  // if(!client) {
  //   fprintf(stderr, "Invalid hostname or port: %s\n", host_and_port);
  //   return 2;
  // }

  // bson_init(&ping);
  // bson_append_int32(&ping, "ping", 4, 1);
  // database = mongoc_client_get_database(client, "ua_detection");
  // cursor = mongoc_database_commnad(database, 0, 0, 1, 0, &ping, NULL, NULL);
  // if (mongoc_cursor_next(cursor, &reply)) {
  //   str = bson_as_json(reply, NULL);
  //   fprintf(stdout, "%s\n", str);
  //   bson_free(str);
  // } else if (mongoc_cursor_error(cursor, &error)) {
  //   fprintf(stderr, "Ping failure: %s\n", error.message);
  //   return 3;
  // }
  //
  // mongoc_cursor_destroy(cursor);
  // bson_destroy(&ping);
  // mongoc_client_destroy(client);
  // bson_free(host_and_port);
  //

  return 0;
}
