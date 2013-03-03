#import "model.h"

char *model_peer(uint32_t client_ip, int client_port, char *  info_hash){
	char * peer_key;
	peer_key=malloc(MAX_PEER_KEY+1);
	snprintf(peer_key, MAX_PEER_KEY, "%i%i%s",client_ip, client_port, info_hash);
	return peer_key;
}

char *model_torrent_peers(int worth, char *  info_hash){
	char * peer_key;
	peer_key=malloc(MAX_PEER_KEY+1);
	snprintf(peer_key, MAX_PEER_KEY, DB_TORRENT_PEERS"%i%s", worth, info_hash);
	return peer_key;
}

char *model_torrent_meta(int worth, char *  info_hash){
	char * peer_key;
	peer_key=malloc(MAX_PEER_KEY+1);
	snprintf(peer_key, MAX_PEER_KEY, DB_TORRENT_META"%i%s", worth, info_hash);
	return peer_key;
}
