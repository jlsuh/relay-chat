#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include "utils/utils.h"

void handle_connections(int serverSocket);
void* suscription_handler(void* socket);
void handle_chat_room(t_user* user);
void lead_chat(t_user* user);
void send_msg_to_all_users(t_user* pivot, char* msg);
int get_user_index(t_user* target);
t_user* get_user_info(int userSocket);
t_chat_room* find_room(uint32_t roomID);
void sig_int_handler(int _);
void free_chatroom(void* aChatRoom);
void free_user(void* aUser);
int init_server(char *ip, char *port);

static pthread_t threads[FD_SETSIZE];
static pthread_mutex_t userIDLock;
static pthread_mutex_t roomIDLock;

static t_list* rooms;
static pthread_mutex_t roomsLock;
static pthread_mutex_t usersLock;
static pthread_mutex_t existentUsersLock;

#endif
