/*
 * The bittorrent tracker protocol has a number of "views",
 * which includes all of the formats that it can be returned as
 */
#include "view.h"


//A function private to the view for building a list of peers.
char *
rebuild_response(struct config * conf, char * info_hash, int numwant){
	char * response;
	size_t peer_len;
	char * peer_key;
	/*char * header = "d8:completei%zde10:downloadedi%zde10:incompletei%zde8:intervali%ie12:min intervali%iepeers%zd:";
	char * header_key = model_torrent_header(info_hash);
	size_t header_len;
	char * torrent_header = store_get(store, header_key, strlen(header_key), &header_len);
    */
	//check to see if this info_hash is below the number of nodes needed for optimization:
	peer_key = model_torrent_peers(0, info_hash);
	response=store_get(conf->store, peer_key, strlen(peer_key), &peer_len);
	//snprintf(response,MAX_RESPONSE,header,);
	free(peer_key);
	return response;
}

void view_peer_list(struct config * conf, struct evhttp_request *req, char * info_hash, int numwant, int compact){
	size_t read_len;
	char *err = NULL;
	char * resp = NULL;
	struct evbuffer *resp_buffer;
	resp_buffer = evbuffer_new();
	//resp = store_get(store, info_hash, strlen(info_hash), &read_len);
	if(!resp || !strlen(resp)){
		resp = rebuild_response(conf, info_hash, numwant);
		if(!resp){
			//error
			//nothing found...
		}else{
			//val and len should come from leveldb!
			evbuffer_add(resp_buffer, resp, strlen(resp));
			//evbuffer_add_printf(buf, "Requested: %s\n", key);
			evhttp_send_reply(req, HTTP_OK, "OK", resp_buffer);
			free(resp);
		}
	}
}
