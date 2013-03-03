/*
 * Simplify leveldb access and error reporting.
 */

#include <leveldb/c.h>
#include "bencode.h"
#include "store.h"

leveldb_t * store_open(char * name){
	leveldb_t *db = 0x00;
    leveldb_options_t *options;
    leveldb_readoptions_t *roptions;
    leveldb_writeoptions_t *woptions;
    char *err = 0x00;
	options = leveldb_options_create();
    leveldb_options_set_create_if_missing(options, 1);
    db = leveldb_open(options, name, &err);

    if (err) {
      free(err);
      db = 0x00;
    }
    return db;
}

char * store_get(leveldb_t * store, char * key,size_t key_size, size_t *read_len){
	char * err;
	char * resp = leveldb_get(store,  leveldb_readoptions_create(), key, key_size, &read_len, &err);
	if(err){
		printf(key_size);
		free(err);
	}
	return resp;
}

void store_put(leveldb_t * store, char * key, size_t key_size, char * value, size_t *value_len){
	char * err;
	//char * resp = leveldb_put(store,  leveldb_readoptions_create(), key, key_size, &read_len, &err);
	leveldb_put(store, leveldb_writeoptions_create(), key, key_size, value, value_len, &err);
	if(err){
		printf(key_size);
		free(err);
	}
}

void store_delete(leveldb_t * store, char * key, size_t key_size){
	char * err;
	leveldb_delete(store, leveldb_writeoptions_create(), key, key_size, &err);
	if(err){
		printf(key_size);
		free(err);
	}
}
