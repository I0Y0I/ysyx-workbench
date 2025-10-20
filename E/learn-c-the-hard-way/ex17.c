#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA (conn->db->max_data)
#define MAX_ROWS (conn->db->max_rows)
#define ID(i) (conn->db->id[i])
#define SET(i) (conn->db->set[i])
#define NAME(i) (conn->db->name + i * MAX_DATA)
#define EMAIL(i) (conn->db->email + i * MAX_DATA)

typedef struct Database {
  int max_data;
  int max_rows;
  int *id;
  int *set;
  char *name;
  char *email;
} Database;

typedef struct Connection {
  FILE *file;
  Database *db;
} Connection;

// 使用全局变量后，函数不再需要传入conn参数
Connection *conn;

void Database_close();
void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  Database_close();
  exit(1);
}

void Address_print(int id) {
  printf("%d %s %s\n", ID(id), NAME(id), EMAIL(id));
}

void Database_load() {
  int rc = fread(&MAX_ROWS, sizeof(int), 1, conn->file);
  if (rc != 1)
    die("Failed to load database.");

  rc = fread(&MAX_DATA, sizeof(int), 1, conn->file);
  if (rc != 1)
    die("Failed to load database.");

  conn->db->id = malloc(MAX_ROWS * sizeof(int));
  rc = fread(conn->db->id, sizeof(int), MAX_ROWS, conn->file);
  if (rc != MAX_ROWS)
    die("Failed to load database.");

  conn->db->set = malloc(MAX_ROWS * sizeof(int));
  rc = fread(conn->db->set, sizeof(int), MAX_ROWS, conn->file);
  if (rc != MAX_ROWS)
    die("Failed to load database.");

  conn->db->name = malloc(MAX_ROWS * MAX_DATA * sizeof(char));
  rc = fread(conn->db->name, sizeof(char), MAX_ROWS * MAX_DATA, conn->file);
  if (rc != MAX_ROWS * MAX_DATA)
    die("Failed to load database.");

  conn->db->email = malloc(MAX_ROWS * MAX_DATA * sizeof(char));
  rc = fread(conn->db->email, sizeof(char), MAX_ROWS * MAX_DATA, conn->file);
  if (rc != MAX_ROWS * MAX_DATA)
    die("Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode) {
  conn = malloc(sizeof(struct Connection));
  if (!conn)
    die("Memory error");

  conn->db = malloc(sizeof(struct Database));
  if (!conn->db)
    die("Memory error");

  if (mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if (conn->file) {
      Database_load();
    }
  }

  if (!conn->file)
    die("Failed to open the file");

  return conn;
}

void Database_close() {
  if (conn) {
    if (conn->file)
      fclose(conn->file);
    if (conn->db) {
      if (conn->db->id) {
        free(conn->db->id);
      }
      if (conn->db->set) {
        free(conn->db->set);
      }
      if (conn->db->name) {
        free(conn->db->name);
      }
      if (conn->db->email) {
        free(conn->db->email);
      }
      free(conn->db);
    }
    free(conn);
  }
}

void Database_write() {
  rewind(conn->file);

  int rc = fwrite(&MAX_ROWS, sizeof(int), 1, conn->file);
  if (rc != 1)
    die("Failed to write database.");

  rc = fwrite(&MAX_DATA, sizeof(int), 1, conn->file);
  if (rc != 1)
    die("Failed to write database.");

  rc = fwrite(conn->db->id, sizeof(int), MAX_ROWS, conn->file);
  if (rc != MAX_ROWS)
    die("Failed to write database.");

  rc = fwrite(conn->db->set, sizeof(int), MAX_ROWS, conn->file);
  if (rc != MAX_ROWS)
    die("Failed to write database.");

  rc = fwrite(conn->db->name, sizeof(char), MAX_ROWS * MAX_DATA, conn->file);
  if (rc != MAX_ROWS * MAX_DATA)
    die("Failed to write database.");

  rc = fwrite(conn->db->email, sizeof(char), MAX_ROWS * MAX_DATA, conn->file);
  if (rc != MAX_ROWS * MAX_DATA)
    die("Failed to write database.");

  rc = fflush(conn->file);
  if (rc == -1)
    die("Cannot flush database.");

  // printf("DB file should be %lu Bytes.\n",
  //        2 * sizeof(int) + 2 * MAX_ROWS * sizeof(int) +
  //            2 * MAX_DATA * MAX_ROWS * sizeof(char));
}

void Database_create(int max_rows, int max_data) {

  MAX_ROWS = max_rows;
  MAX_DATA = max_data;

  conn->db->id = malloc(MAX_ROWS * sizeof(int));
  conn->db->set = malloc(MAX_ROWS * sizeof(int));
  conn->db->name = malloc(MAX_ROWS * MAX_DATA * sizeof(char));
  conn->db->email = malloc(MAX_ROWS * MAX_DATA * sizeof(char));

  int i = 0;
  for (i = 0; i < MAX_ROWS; i++)
    ID(i) = i;

  memset(conn->db->set, 0, MAX_ROWS * sizeof(int));
  memset(conn->db->name, 0, MAX_ROWS * MAX_DATA * sizeof(char));
  memset(conn->db->email, 0, MAX_ROWS * MAX_DATA * sizeof(char));
}

void Database_set(int id, const char *name, const char *email) {
  if (SET(id))
    die("Already set, delete it first");

  SET(id) = 1;
  // WARNING: bug, read the "How To Break It" and fix this
  char *res = strncpy(NAME(id), name, MAX_DATA);
  // demonstrate the strncpy bug
  if (!res)
    die("Name copy failed");
  *(NAME(id) + MAX_DATA - 1) = '\0';

  res = strncpy(EMAIL(id), email, MAX_DATA);
  if (!res)
    die("Email copy failed");
  *(EMAIL(id) + MAX_DATA - 1) = '\0';
}

void Database_get(int id) {
  if (SET(id)) {
    Address_print(id);
  } else {
    die("ID is not set");
  }
}

void Database_find(const char *pattern) {
  for (int i = 0; i < MAX_ROWS; i++) {
    if (SET(i)) {
      if (strncmp(NAME(i), pattern, MAX_DATA)) {
        Address_print(i);
        break;
      }
      if (strncmp(EMAIL(i), pattern, MAX_DATA)) {
        Address_print(i);
        break;
      }
    }
  }
  die("pattern is not set");
}

void Database_delete(int id) { SET(id) = 0; }

void Database_list() {
  int i = 0;

  for (i = 0; i < MAX_ROWS; i++) {
    if (SET(i)) {
      Address_print(i);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3)
    die("USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  conn = Database_open(filename, action);
  int id = 0, max_rows, max_data;

  switch (action) {
  case 'c':
    if (argc != 5)
      die("Need an id to get");
    max_rows = atoi(argv[3]);
    max_data = atoi(argv[4]);
    Database_create(max_rows, max_data);
    Database_write();
    break;

  case 'g':
    if (argc != 4)
      die("Need an id to get");
    id = atoi(argv[3]);
    if (id >= MAX_ROWS)
      die("There's not that many records.");

    Database_get(id);
    break;

  case 'f':
    if (argc != 4)
      die("Need a patter to find");
    Database_find(argv[3]);
    break;

  case 's':
    if (argc != 6)
      die("Need id, name, email to set");
    id = atoi(argv[3]);
    if (id >= MAX_ROWS)
      die("There's not that many records.");

    Database_set(id, argv[4], argv[5]);
    Database_write();
    break;

  case 'd':
    if (argc != 4)
      die("Need id to delete");
    id = atoi(argv[3]);
    if (id >= MAX_ROWS)
      die("There's not that many records.");

    Database_delete(id);
    Database_write();
    break;

  case 'l':
    Database_list();
    break;
  default:
    die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
  }

  Database_close();

  return 0;
}
