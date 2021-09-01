#include "server.h"

#define SERVER_CONFIG "cfg/server.config"
#define SERVER_IP "IP"
#define SERVER_PORT "PORT"

static int tid = 0;
static uint32_t userID = 0;
static uint32_t roomID = 0;

int main(int argc, char* argv[]) {
	t_config* config = config_create(SERVER_CONFIG);
	char* ip = config_get_string_value(config, SERVER_IP);
	char* port = config_get_string_value(config, SERVER_PORT);

	int serverSocket = init_server(ip, port);

	config_destroy(config);

	rooms = list_create();

	struct sigaction sigInt;
	sigInt.sa_handler = sig_int_handler;
	sigemptyset(&sigInt.sa_mask);
	sigInt.sa_flags = 0;
	sigaction(SIGINT, &sigInt, NULL);

	handle_connections(serverSocket);

	return EXIT_SUCCESS;
}

void handle_connections(int serverSocket) {
	struct sockaddr sockAddr;
	socklen_t addrSize = sizeof(sockAddr);
	int userSocket;
	do {
		userSocket = accept(serverSocket, &sockAddr, &addrSize);
		if (userSocket > 0) {
			pthread_create(&threads[tid], NULL, suscription_handler, (void*) &userSocket);
			pthread_detach(threads[tid]);
			tid += 1;
		} else {
			perror(strerror(errno));
		}
	} while(1);
}

void* suscription_handler(void* socket) {
	int userSocket = *(int*) socket;
	t_user* user = get_user_info(userSocket);

	printf("[ID: %d] %s connected to the server\n", user->userID, user->userName);

	handle_chat_room(user);

	return NULL;
}

void handle_chat_room(t_user* user) {
	send_str(user->socket, "[Room-to-join  RoomID]: ");
	t_chat_room* newChatRoom = (t_chat_room*) deserialize_package(user->socket, true);
	t_chat_room* existentRoom = find_room(newChatRoom->roomID);
	t_chat_room* current = NULL;
	char* msg;
	if(existentRoom != NULL) {
		msg = string_from_format("%s [ID: %d] already exists. Joining room\n", existentRoom->roomName, existentRoom->roomID);
		printf(msg);
		send_str(user->socket, msg);

		pthread_mutex_lock(&existentUsersLock);
		list_add(existentRoom->users, user);
		pthread_mutex_unlock(&existentUsersLock);

		current = existentRoom;

		free(newChatRoom->roomName);
		free(newChatRoom);
	} else {
		msg = "There isn't any room by this name and ID\n";
		printf(msg);
		send_str(user->socket, msg);

		// TODO: We are supposing that there will be always someone in the chatroom
		// TODO: make sure to destroy chatroom when there aren't any users connected
		newChatRoom->users = list_create();

		pthread_mutex_lock(&roomIDLock);
		newChatRoom->roomID = roomID++;
		pthread_mutex_unlock(&roomIDLock);

		pthread_mutex_lock(&roomsLock);
		list_add(rooms, newChatRoom);
		pthread_mutex_unlock(&roomsLock);

		pthread_mutex_lock(&usersLock);
		list_add(newChatRoom->users, user);
		pthread_mutex_unlock(&usersLock);

		msg = string_from_format("%s [ID: %d] created\n", newChatRoom->roomName, newChatRoom->roomID);
		printf(msg);
		send_str(user->socket, msg);

		current = newChatRoom;
	}
	user->currentChatRoom = current;

	printf("%d user(s) connected to %s [ID: %d]\n", list_size(current->users), current->roomName, current->roomID);

	char* toDistribute = string_from_format("%s joined %s!\n", user->userName, current->roomName);
	send_msg_to_all_users(user, toDistribute);

	free(toDistribute);
	free(msg);

	lead_chat(user);
}

void lead_chat(t_user* user) {
	do {
		char* msg = (char*) deserialize_package(user->socket, true);
		if(strcmp(msg, "/exit") == 0) {
			free(msg);
			break;
		}
		char* chatLog = string_from_format("%s says: %s\n", user->userName, msg);
		printf("%s", chatLog);
		send_msg_to_all_users(user, chatLog);
		free(msg);
		free(chatLog);
	} while(1);
	char* disconnMsg = string_from_format("%s disconnected from %s\n", user->userName, user->currentChatRoom->roomName);
	send_msg_to_all_users(user, disconnMsg);
	printf(disconnMsg);
	free(disconnMsg);

	int index = get_user_index(user);
	list_remove(user->currentChatRoom->users, index);
	free(user->userName);
	free(user);
}

void send_msg_to_all_users(t_user* pivot, char* msg) {
	t_chat_room* currentChatRoom = pivot->currentChatRoom;
	t_link_element* destUser = currentChatRoom->users->head;
	while(destUser != NULL) {
		send_str(((t_user*) destUser->data)->socket, msg);
		destUser = destUser->next;
	}
}

int get_user_index(t_user* target) {
	int index = 0;
	t_link_element* aux = target->currentChatRoom->users->head;
	while(aux != NULL) {
		uint32_t userID = ((t_user*) aux->data)->userID;
		if(userID == target->userID) {
			return index;
		}
		aux = aux->next;
		index++;
	}
	return -1;
}

t_user* get_user_info(int userSocket) {
	send_str(userSocket, "Input username: ");
	char* userName = (char*) deserialize_package(userSocket, true);

	t_user* user = malloc(sizeof(t_user));
	pthread_mutex_lock(&userIDLock);
	user->userID = userID++;
	pthread_mutex_unlock(&userIDLock);
	user->userName = strdup(userName);
	user->socket = userSocket;

	free(userName);
	return user;
}

t_chat_room* find_room(uint32_t roomID) {
	t_link_element* aux = rooms->head;
	while(aux != NULL) {
		uint32_t currentRoomID = ((t_chat_room*) aux->data)->roomID;
		if(currentRoomID == roomID) {
			return (t_chat_room*) aux->data;
		}
		aux = aux->next;
	}
	return NULL;
}

void sig_int_handler(int _) {
	list_destroy_and_destroy_elements(rooms, free_chatroom);
	exit(EXIT_SUCCESS);
}

void free_chatroom(void* aChatRoom) {
	t_chat_room* chatRoom = (t_chat_room*) aChatRoom;
	free(chatRoom->roomName);
	list_destroy_and_destroy_elements(chatRoom->users, free_user);
	free(chatRoom);
}

void free_user(void* aUser) {
	t_user* user = (t_user*) aUser;
	free(user->userName);
	free(user);
}

int init_server(char* ip, char* port) {
	int optVal = 1;
	struct addrinfo hints;
	struct addrinfo* serverInfo;

	memset(&hints, 0, sizeof(hints));	// make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;		// fill in my IP for me

	int rv = getaddrinfo(ip, port, &hints, &serverInfo);
	if (rv != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(rv));
		return EXIT_FAILURE;
	}

	int serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (serverSocket == -1) {
		printf("Socket creation error\n%s", strerror(errno));
		return EXIT_FAILURE;
	}

	int sockOpt = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(int));
	if (sockOpt == -1) {
		printf("Sockopt error\n%s", strerror(errno));
		return EXIT_FAILURE;
	}

	int bindVal = bind(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	if (bindVal == -1) {
		printf("Mismatched bind\n%s", strerror(errno));
		return EXIT_FAILURE;
	}

	int listenVal = listen(serverSocket, SOMAXCONN);
	if (listenVal == -1) {
		printf("Listen error\n%s", strerror(errno));
		return EXIT_FAILURE;
	}

	freeaddrinfo(serverInfo);

	return serverSocket;
}
