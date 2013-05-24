#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_rows;
    int max_data;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void die(const char *message, struct Connection *conn) {
    void Database_close(struct Connection *);
    if (conn) Database_close(conn);
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addr) {
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
    void Database_set(struct Connection *, int, const char *, const char *); 
    void Database_create(struct Connection *, int, int);
    //int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    int max_rows, max_data, id;
    char *name, *email;
    int rc = fscanf(conn->file, "%d %d", &max_rows, &max_data);
    if (rc != 2) die("Failed to load database.", conn);
    Database_create(conn, max_rows, max_data);
    name = malloc(max_data);
    email = malloc(max_data);
    while (fscanf(conn->file, "%d %s %s", &id, name, email) == 3) {
        Database_set(conn, id, name, email);
    }
    free(name);
    free(email);
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db) die("Memory error", conn);

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn) {
    int i = 0;
    if (conn->file) {
        if (conn->file) fclose(conn->file);  
        for (i = 0; i < conn->db->max_rows; i++) {
            if (conn->db->rows[i].name) free(conn->db->rows[i].name);
            if (conn->db->rows[i].email) free(conn->db->rows[i].email);
        }
        if (conn->db->rows) free(conn->db->rows);
        if (conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn) {
    int i = 0;
    freopen(NULL, "w+", conn->file);

    //int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    int rc = fprintf(conn->file, "%d %d\n", conn->db->max_rows, conn->db->max_data);
    if (rc < 0) die("Failed to write database.", conn);
    for (i = 0; i < conn->db->max_rows; i++) {
        if (conn->db->rows[i].set) {
            rc = fprintf(conn->file, "%d %s %s\n", conn->db->rows[i].id, 
                conn->db->rows[i].name, conn->db->rows[i].email);
            if (rc < 0) die("Failed to write database.", conn);
        }
    }

    rc = fflush(conn->file);
    if (rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int row_num, int data_size) {
    int i = 0;
    
    conn->db->max_rows = row_num;
    conn->db->max_data = data_size;

    conn->db->rows = malloc(row_num*sizeof(struct Address));
    for(i = 0; i < row_num; i++) {
        struct Address addr = {.id = i, .set = 0, .name = NULL, .email = NULL};
        conn->db->rows[i] = addr;
    }
}

void Database_find(struct Connection *conn, const char *search_word) {
    int i = 0;
    struct Address *addr;
    for(i = 0; i < conn->db->max_rows; i++) {
        addr = &conn->db->rows[i];
        if (addr->set) {
            if (!strcmp(addr->name, search_word)
                || !strcmp(addr->email, search_word)) {
                Address_print(addr);
            }
        }
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) die("Already set, delete it first", conn);

    addr->set = 1;

    addr->name = malloc(conn->db->max_data);
    if (!addr->name) die("Memory error", conn);
    char *res = strncpy(addr->name, name, conn->db->max_data);
    addr->name[conn->db->max_data-1] = '\0';
    if (!res) die("Name copy failed", conn);

    addr->email = malloc(conn->db->max_data);
    if (!addr->email) die("Memory error", conn);
    res = strncpy(addr->email, email, conn->db->max_data);
    addr->email[conn->db->max_data-1] = '\0';
    if (!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id) {
    char *name = conn->db->rows[id].name;
    char *email =  conn->db->rows[id].email;
    if (name) free(name);
    if (email) free(email);
    conn->db->rows[id].set = 0;
    conn->db->rows[id].name = NULL;
    conn->db->rows[id].email = NULL;
}

void Database_list(struct Connection *conn) {
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) die("USAGE: ex17 <dbfile> <action> <action params>", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;
    int rows_num;
    int max_data;

    switch(action) {
        case 'c':
            if (argc != 5) die("Need row and data size to create", conn);
            rows_num = atoi(argv[3]);
            max_data = atoi(argv[4]);
            Database_create(conn, rows_num, max_data);
            Database_write(conn);
            break;
        case 'g':
            if (argc != 4) die("Need an id to get", conn);
            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There'n not that many records.", conn);
            Database_get(conn, id);
            break;
        case 's':
            if (argc != 6) die("Need id, name, email to set", conn);
            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There'n not that many records.", conn);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        case 'd':
            if (argc != 4) die("Need id to delete", conn);
            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There'n not that many records.", conn);
            Database_delete(conn, id);
            Database_write(conn);
            break;
        case 'l':
            Database_list(conn);
            break;
        case 'f':
            if (argc != 4) die("Need a search word to find", conn);
            Database_find(conn, argv[3]);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }
  
    Database_close(conn);

    return 0;
}


