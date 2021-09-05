#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include "common_utils.h"

void handle_connections(int);
void* suscription_handler(void*);
void handle_chat_room(t_user*);
void lead_chat_room(t_user*);
void send_msg_to_all_users(t_user*, char*);
int get_user_index(t_user*);
t_user* recv_user_info(int);
t_chat_room* find_room(uint32_t);
void sig_int_handler(int);
int init_server(char*, char*);

static pthread_t threads[FD_SETSIZE];
static pthread_mutex_t userIDLock;
static pthread_mutex_t roomIDLock;

static t_list* rooms;
static pthread_mutex_t roomsLock;
static pthread_mutex_t usersLock;
static pthread_mutex_t existentUsersLock;

#endif
