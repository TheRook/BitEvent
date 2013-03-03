#ifndef DO_H
#define DO_H
#include <evhttp.h>
#include <leveldb/c.h>

void view_peer_list(leveldb_t * store, struct evhttp_request *req, char * info_hash, int numwant, int compact);

#endif
