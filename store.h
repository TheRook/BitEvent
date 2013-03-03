#ifndef DB_H
#define DB_H

void store_peer_list(leveldb_t * db, struct evhttp_request *req, char * info_hash, int numwant, int compact);
void store_remove_peer(leveldb_t * db, uint32_t client_ip, int client_port, char *  info_hash);
void store_add_peer(leveldb_t * db, int worth, uint32_t client_ip, int client_port, char *  info_hash);

#endif
