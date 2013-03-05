#ifndef DO_H
#define DO_H
#include <evhttp.h>
#include <leveldb/c.h>
#include "main.h"

void view_peer_list(struct config * conf, struct evhttp_request *req, char * info_hash, int numwant, int compact);

#endif
