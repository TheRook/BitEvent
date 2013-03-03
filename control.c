#import "control.h"

void control_remove_peer(struct config *conf, uint32_t client_ip, int client_port, char *  info_hash){
	char *err = NULL;
	leveldb_writeoptions_t *woptions;
	char * peer_key;
	//= get_peer_key(client_ip, client_port, info_hash);
	woptions = leveldb_writeoptions_create();
	leveldb_delete(conf->store, woptions, peer_key, strlen(peer_key), &err);
}

void control_add_peer(struct config *conf, int worth, uint32_t client_ip, int client_port, char * info_hash){
	char *err = NULL;
	int peer_len;
	char *meta_key = model_torrent_meta(worth, info_hash);
	char *meta_list=store_get(conf->store, meta_key, strlen(meta_key), &peer_len);
	char *torrent_key = model_torrent_peers(worth, info_hash);
	char *peer_list=store_get(conf->store, torrent_key, strlen(torrent_key), &peer_len);


	store_put(conf->store, torrent_key, strlen(torrent_key), peer_list, strlen(peer_list));

	free(meta_key);
	free(meta_list);
	free(torrent_key);
	free(peer_list);
}
