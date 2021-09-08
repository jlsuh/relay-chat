#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include "common_utils.h"

#define SERVER_CONFIG "cfg/server.config"
#define SERVER_IP "IP"
#define SERVER_PORT "PORT"

void handle_connections(int serverSocket);
void* suscription_handler(void* socket);
void handle_chat_room(t_user* user);
void lead_chat_room(t_user* user);
void send_msg_to_all_users(t_user* pivot, char* msg);
int get_user_index(t_user* target);
t_user* recv_user_info(int userSocket);
t_chat_room* find_room(uint32_t roomID);
void sig_int_handler(int _);
int init_server(char* ip, char* port);

#endif
