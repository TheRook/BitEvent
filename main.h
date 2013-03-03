
#ifndef MAIN_H
#define MAIN_H

#include <getopt.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <event2/event.h>
//#include <event2/http.h>
#include <evhttp.h>
#include <event2/buffer.h>
#include <fcntl.h>
#include <leveldb/c.h>

struct config{
	leveldb_t * store;
	int interval;
	int min_interval;
	int max_peer_return;
	char *client_whitelist;
	char *client_blacklist;
	int private_tracker;
};

#endif
