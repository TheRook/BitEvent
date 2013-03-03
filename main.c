#include "main.h"

int getInt(struct evkeyvalq *GET, char * key){
	char * tmp;
	int ret=0;
	tmp=evhttp_find_header(GET, key);
	if(tmp){
		ret=atoi(tmp);
	}
	return ret;
}

void tracker(struct evhttp_request *req, struct config *confg){
	char * info_hash=0x00,
		 * client_ip=0x00,
		 * event=0x00;
	int    client_port=0,
		   left = 0,
		   compact = 0,
		   numwant = 0,
		   peer_worth = 0,
		   uploaded = 0,
		   downloaded = 0;
	ev_uint16_t scoket_client_port;
	uint32_t client_ip_addr=0;

	//evhttp is strange sometimes...
	struct evkeyvalq GET;
	const struct evhttp_uri *uri = evhttp_request_get_evhttp_uri(req);
	char *query = evhttp_uri_get_query(uri);
	evhttp_parse_query_str(query, &GET);

	info_hash=evhttp_find_header(&GET, "info_hash");
	if(info_hash){
		event=evhttp_find_header(&GET, "event");
		left = getInt(&GET,"left");
		//the bt client must have something left and not completed or stopped.
		if(left && (event && strcmp(event, "completed") == 0 && strcmp(event, "stopped") == 0)){
			//Build a list of peers for the response:
			compact = getInt(&GET,"compact");
			numwant = getInt(&GET,"numwant");
			view_peer_list(confg, req, info_hash, numwant, compact);
		}

		client_ip = evhttp_find_header(&GET, "Ip");
		if(client_ip){
			client_ip_addr=inet_addr(client_ip);
		}else{
			//try a non-standard case
			client_ip = evhttp_find_header(&GET, "ip");
			if(client_ip){
				client_ip_addr=inet_addr(client_ip);
			}
		}

		if(client_ip_addr<=0){
			//Looks like we got an invalid ip address as a GET param,  recovering...
			evhttp_connection_get_peer(req->evcon,
									   &client_ip,
									   &scoket_client_port);
			client_ip_addr=inet_addr(client_ip);
		}

		client_port = getInt(&GET,"port");
		//port 0 is valid...  and is used by no one, ever.
		if(client_port > 0 && client_port < 65536){
			uploaded = getInt(&GET,"uploaded");
			downloaded = getInt(&GET,"downloaded");
			//Process event:
			if(event){
				if(strcmp(event, "stopped") == 0 ){
					control_remove_peer(confg, client_ip_addr, client_port, info_hash);
					peer_worth=-1;
				}else if(strcmp(event, "completed") == 0){
					//This peer has every chunk,  they are valuable
					peer_worth=2;
				}else if(strcmp(event, "started") == 0){
					//This peer has nothing
					peer_worth=0;
				}else{
					//Probably better than nothing.
					peer_worth=1;
				}
			}else{
				peer_worth=1;
			}
			//make sure the peer has uploaded something and is worth something.
			//otherwise it doesn't help anyone to burn the resources to track them.
			//When they come back with an announce that is worth while,  then we'll add them to the list.
			if(peer_worth >= 1 && uploaded > 0){
				if(uploaded>downloaded){
					//This peer is healthy! (Or lying...)
					peer_worth++;
				}
				//This peer is worth something,  we should track it.
				//The recorded peer_worth is from 1-3
				control_add_peer(confg, peer_worth, client_ip_addr, client_port, info_hash);
			}
		}else{
			//todo error invalid port
		}
	}else{
		//todo error; no info_hash
	}
}

BindSocket(int port) {
  int r;
  int nfd;
  nfd = socket(AF_INET, SOCK_STREAM, 0);
  if (nfd < 0) return -1;

  int one = 1;
  r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
  if (r < 0) return -1;
  r = listen(nfd, 10240);
  if (r < 0) return -1;

  int flags;
  if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
      || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
    return -1;

  return nfd;
}

int main(int argc, char **argv) {
	int cli;
	int sock = BindSocket(12345);
	struct config conf;
	conf.store = store_open("tracker.db");
	conf.max_peer_return=50;
	conf.min_interval=300;
	conf.max_peer_return=48;
	struct event_base *base = event_init();
	struct evhttp *httpd;
	httpd = evhttp_new(base);
	cli = evhttp_accept_socket(httpd, sock);
	evhttp_set_gencb(httpd, tracker, (void *)&conf);
	event_base_dispatch(base);

	return 0;
}
