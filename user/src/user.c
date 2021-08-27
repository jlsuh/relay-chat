#include "user.h"

#define USER_CONFIG_NAME "user.config"
#define USER_IP "IP"
#define USER_PORT "PORT"

bool deserializeNext = true;

int main(int argc, char *argv[]) {
	t_config* config = config_create(USER_CONFIG_NAME);
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
	pthread_t msg_recvr;
	pthread_create(&msg_recvr, NULL, (void*)recv_distributed_msg, (void*)&serverSocket);
	char* read;
	do {
		read = readline("");
		if(strcmp(read, "/exit") == 0) {
			send_str_msg(serverSocket, "/exit");
			free(read);
			puts("Disconnected from chatting room");
			deserializeNext = false;
			pthread_join(msg_recvr, NULL);
			break;
		}
		send_str_msg(serverSocket, read);
		free(read);
	} while (1);
}

void recv_distributed_msg(void* socket) {
	int serverSocket = *(int*)socket;
	do {
		t_string* msg = (t_string*)deserialize_package(serverSocket, deserializeNext);
		if(msg == NULL) {
			free(msg);
			close(serverSocket);
			break;
		} else {
			printf("%s", msg->content);
			free(msg->content);
			free(msg);
		}
	} while(1);
	pthread_exit(NULL);
}

void display_deserialized_msg(int serverSocket) {
	t_string* msg = (t_string*)deserialize_package(serverSocket, true);
	printf("%s", msg->content);
	free(msg->content);
	free(msg);
}

void send_user_info(int serverSocket) {
	t_string str;
	str.content = malloc(sizeof(char) * 32);
	scanf("%s", str.content);
	str.length = strlen(str.content) + 1;
	t_buffer* buffer = serialize_string(str);

	send_serialized_package(serverSocket, buffer, STRING, str);
}

void send_room_info(int serverSocket) {
	t_string roomName;
	uint32_t roomID;
	roomName.content = malloc(sizeof(char) * 32);
	scanf("%s %d", roomName.content, &roomID);
	roomName.length = strlen(roomName.content) + 1;
	t_buffer* buffer = serialize_chat_room(roomName, roomID);

	send_serialized_package(serverSocket, buffer, ROOMINFO, roomName);
}

void send_serialized_package(int serverSocket, t_buffer* buffer, op_code opCode, t_string str) {
	void* toSend = serialize_package(opCode, buffer);
	send_package(serverSocket, toSend, buffer);
	free_sended_info(buffer, toSend, str);
}

void exchange_info(int serverSocket, void(*info_sender)(int)) {
	display_deserialized_msg(serverSocket);
	info_sender(serverSocket);
}

void free_sended_info(t_buffer* buffer, void* toSend, t_string str) {
    free(buffer->stream);
    free(buffer);
    free(str.content);
    free(toSend);
}

int connect_to_server(char *ip, char *port) {
	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	int rv = getaddrinfo(ip, port, &hints, &serverInfo);
	if (rv != 0) {
		fprintf(stderr, "getaddrinfo error: %s", gai_strerror(rv));
		return EXIT_FAILURE;
	}

	int serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (serverSocket == -1) {
		perror(strerror(errno));
		return EXIT_FAILURE;
	}

	int connectVal = connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	if (connectVal == -1) {
		printf("Mismatched connection with server\n%s", strerror(errno));
		return EXIT_FAILURE;
	}

	freeaddrinfo(serverInfo);

	return serverSocket;
}
