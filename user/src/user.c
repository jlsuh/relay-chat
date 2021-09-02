#include "user.h"

#define USER_CONFIG "cfg/user.config"
#define USER_IP "IP"
#define USER_PORT "PORT"
#define MAX_USERNAME_LENGTH 32

bool deserializeNext = true;

int main(int argc, char* argv[]) {
	t_config* config = config_create(USER_CONFIG);
	char* ip = config_get_string_value(config, USER_IP);
	char* port = config_get_string_value(config, USER_PORT);

	int serverSocket = connect_to_server(ip, port);

	config_destroy(config);

	exchange_info(serverSocket, send_user_info);
	exchange_info(serverSocket, send_room_info);

	start_chatting(serverSocket);
	return EXIT_SUCCESS;
}

void start_chatting(int serverSocket) {
	pthread_t msg_receiver;
	pthread_create(&msg_receiver, NULL, (void*) recv_distributed_msg, (void*) &serverSocket);
	char* read;
	do {
		read = readline("");
		if(strcmp(read, "/exit") == 0) {
			send_str(serverSocket, "/exit");
			free(read);
			puts("Disconnected from chatting room");
			deserializeNext = false;
			pthread_join(msg_receiver, NULL);
			break;
		}
		send_str(serverSocket, read);
		free(read);
	} while (1);
}

void recv_distributed_msg(void* socket) {
	int serverSocket = *(int*) socket;
	do {
		char* msg = (char*) deserialize_package(serverSocket, deserializeNext);
		if(msg == NULL) {
			free(msg);
			close(serverSocket);
			break;
		} else {
			printf("%s", msg);
			free(msg);
		}
	} while(1);
	pthread_exit(NULL);
}

void display_deserialized_msg(int serverSocket) {
	char* msg = (char*) deserialize_package(serverSocket, true);
	printf("%s", msg);
	free(msg);
}

void send_user_info(int serverSocket) {
	char* userName;
	userName = malloc(MAX_USERNAME_LENGTH);
	scanf("%s", userName);
	t_buffer* buffer = buffer_create();
	buffer_pack_string(buffer, userName);
	send_serialized_package(serverSocket, buffer, STRING, userName);
}

void send_room_info(int serverSocket) {
	char* roomName;
	uint32_t roomID;
	roomName = malloc(MAX_USERNAME_LENGTH);
	scanf("%s %d", roomName, &roomID);
	t_buffer* buffer = buffer_create();
	buffer_pack_chat_room(buffer, roomID, roomName);
	send_serialized_package(serverSocket, buffer, ROOMINFO, roomName);
}

void send_serialized_package(int serverSocket, t_buffer* buffer, op_code opCode, char* str) {
	void* toSend = serialize_package(opCode, buffer);
	send_package(serverSocket, toSend, buffer);
	free_sended_info(buffer, toSend, str);
}

void exchange_info(int serverSocket, void(*info_sender)(int)) {
	display_deserialized_msg(serverSocket);
	info_sender(serverSocket);
}

void free_sended_info(t_buffer* buffer, void* toSend, char* str) {
	buffer_destroy(buffer);
	free(str);
	free(toSend);
}

int connect_to_server(char* ip, char* port) {
	int conn;
	struct addrinfo hints;
	struct addrinfo* serverInfo;
	struct addrinfo* p;

	memset(&hints, 0, sizeof(hints));	// make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;		// fill in my IP for me

	int rv = getaddrinfo(ip, port, &hints, &serverInfo);
	if (rv != 0) {
		fprintf(stderr, "getaddrinfo error: %s", gai_strerror(rv));
		return EXIT_FAILURE;
	}
	for(p = serverInfo; p != NULL; p = p->ai_next) {
		conn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(conn == -1) {
			continue;
		}
		if(connect(conn, p->ai_addr, p->ai_addrlen) != -1) {
			break;
		}
		close(conn);
	}
	freeaddrinfo(serverInfo);
	if(conn != -1 && p != NULL) {
		return conn;
	}
	return -1;
}
