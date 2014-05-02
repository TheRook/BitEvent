#ifndef KEY_H
#define KEY_H
#include <evhttp.h>
#define MAX_PEER_KEY 256

#define DB_TORRENT_HEADER "0"
#define DB_TORRENT_PEERS "1"
#define DB_TORRENT_META "2"
#define DB_TORRENT_SMALL "3"
#define DB_PEER_QUALITY "4"
#define DB_ACCESS_CONTROL "5"

char *model_torrent_peers(int worth, char *  info_hash);
char *model_torrent_header(char *  info_hash);
char *model_torrent_meta(int worth, char *  info_hash);
char *model_peer(uint32_t client_ip, int client_port, char *  info_hash);

#endif KEY_H
