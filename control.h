#ifndef DO_H
#define DO_H
#include <evhttp.h>
#include <leveldb/c.h>
#include <time.h>
#include "store.h"
#include "model.h"
#include "main.h"

//#define PEER6_SIZE 8 todo
#define PEER_SIZE 8
#define META_SIZE 2
#define MAX_PEER_LEN 200
#define MAX_META_LEN MAX_PEER_LEN/4

void control_remove_peer(struct config *conf, uint32_t client_ip, int client_port, char *  info_hash);
void control_add_peer(struct config *conf, int worth, uint32_t client_ip, int client_port, char *  info_hash);

#endif DO_H
