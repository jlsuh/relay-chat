#ifndef SRC_USER_H_
#define SRC_USER_H_

#include "utils/utils.h"

void start_chatting(int serverSocket);
void recv_distributed_msg(void* socket);
void display_deserialized_msg(int serverSocket);
void send_user_info(int serverSocket);
void send_room_info(int serverSocket);
void send_serialized_package(int serverSocket, t_buffer* buffer, op_code opCode, t_string str);
void exchange_info(int serverSocket, void(*info_sender)(int));
void free_sended_info(t_buffer* buffer, void* toSend, t_string str);
int connect_to_server(char *ip, char *port);

#endif
