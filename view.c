/*
 * The bittorrent tracker protocol has a number of "views",
 * which includes all of the formats that it can be returned as
 */
#include "view.h"


//A function private to the view for building a list of peers.
char *
rebuild_response(char * info_hash, int numwant){
	char * response;
	//char * header = "d8:completei%zde10:downloadedi%zde10:incompletei%zde8:intervali%ie12:min intervali%iepeers%zd:";
	//snprintf(response,MAX_RESPONSE,header,);

	return response;
}

void view_peer_list(leveldb_t * store, struct evhttp_request *req, char * info_hash, int numwant, int compact){
	size_t read_len;
	char *err = NULL;
	char * resp;
	struct evbuffer *resp_buffer;
	resp_buffer = evbuffer_new();;
	resp = store_get(store, info_hash, strlen(info_hash), &read_len);
	if(!resp || !strlen(resp)){
		//resp = rebuild_response(info_hash, numwant);
		if(!resp){
			//nothing found...
		}
	}
	//val and len should come from leveldb!
	evbuffer_add(resp_buffer, resp, strlen(resp));
	//evbuffer_add_printf(buf, "Requested: %s\n", key);
	evhttp_send_reply(req, HTTP_OK, "OK", resp_buffer);
}
