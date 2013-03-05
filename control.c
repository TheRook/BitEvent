#import "control.h"

void control_remove_peer(struct config *conf, uint32_t client_ip, int client_port, char *  info_hash){
	char *err = NULL;
	leveldb_writeoptions_t *woptions;
	char * peer_key;
	//= get_peer_key(client_ip, client_port, info_hash);
	woptions = leveldb_writeoptions_create();
	leveldb_delete(conf->store, woptions, peer_key, strlen(peer_key), &err);
}

int16_t current_time(){
	time_t current_time = time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime(&current_time);
	int16_t timestamp = (int16_t)((timeinfo->tm_mday << 11) | (timeinfo->tm_hour << 6) | timeinfo->tm_min);
	return timestamp;
}

void control_add_peer(struct config *conf, int worth, uint32_t client_ip, int client_port, char * info_hash){
	char new_peer_list[MAX_PEER_LEN];
	char new_meta_list[MAX_META_LEN];
	size_t peer_len=0;
	int meta_len=0;
	int i;
	int16_t cur_timestamp = current_time();
	char *meta_key = model_torrent_meta(worth, info_hash);
	char *torrent_key = model_torrent_peers(worth, info_hash);
	int16_t *meta_list=store_get(conf->store, meta_key, strlen(meta_key), &peer_len);
	char *peer_list=store_get(conf->store, torrent_key, strlen(torrent_key), &meta_len);

	//cull old records
	for(i=meta_len/2;i>0;i-=2){
		//if the peer hasn't announced in min_interval,  then remove it.
		//todo,  allow for an interval larger than 60 minutes...
		if(meta_list[i]<cur_timestamp+(conf->min_interval/60)){
			//The peer at the end of this list has expired...
			peer_len-PEER_SIZE;
		}else{
			break;
		}
	}
	//if we are at the max,  remove the last peer in the list.
	if(peer_len>MAX_PEER_LEN-8){
		peer_len=MAX_PEER_LEN-8;
	}
	if(meta_len>MAX_META_LEN-2){
		meta_len=MAX_META_LEN-2;
	}

	//prepend the new client
	memcpy(new_peer_list,&client_ip,6);
	memcpy(new_peer_list+6,&client_port,2);
	//peer_len is no larger than MAX_PEER_LEN-8.
	memcpy(new_peer_list+8,peer_list,peer_len);
	store_put(conf->store, torrent_key, strlen(torrent_key), new_peer_list, strlen(new_peer_list));

	//prepend the new client's timestamp
	memcpy(new_meta_list,&cur_timestamp,2);
	memcpy(new_meta_list+2,meta_list,meta_len);
	store_put(conf->store, meta_key, strlen(meta_key), new_meta_list, strlen(new_meta_list));

	free(meta_key);
	free(meta_list);
	free(torrent_key);
	free(peer_list);
}
