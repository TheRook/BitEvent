#ifndef DO_H
#define DO_H
#include <evhttp.h>
#include <leveldb/c.h>
#include "store.h"
#include "model.h"
#include "main.h"

#define MAX_

void control_remove_peer(struct config *conf, uint32_t client_ip, int client_port, char *  info_hash);
void control_add_peer(struct config *conf, int worth, uint32_t client_ip, int client_port, char *  info_hash);

#endif DO_H
