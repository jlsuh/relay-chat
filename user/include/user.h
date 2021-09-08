#ifndef SRC_USER_H_
#define SRC_USER_H_

#include "common_utils.h"
#include <readline/readline.h>

#define USER_CONFIG "cfg/user.config"
#define USER_IP "IP"
#define USER_PORT "PORT"
#define MAX_USERNAME_LENGTH 32

void start_chatting(int serverSocket);
void recv_distributed_msg(void* socket);
void display_deserialized_msg(int serverSocket);
void send_user_info(int serverSocket);
void send_room_info(int serverSocket);
void send_serialized_package(int serverSocket, t_buffer* buffer, op_code opCode, char* str);
void exchange_info(int serverSocket, void(*info_sender)(int));
void free_sended_info(t_buffer* buffer, void* toSend, char* str);
int connect_to_server(char* ip, char* port);

#endif
