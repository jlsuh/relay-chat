#ifndef SRC_USER_H_
#define SRC_USER_H_

#include "utils/utils.h"

void start_chatting(int);
void recv_distributed_msg(void*);
void display_deserialized_msg(int);
void send_user_info(int);
void send_room_info(int);
void send_serialized_package(int, t_buffer*, op_code, char*);
void exchange_info(int, void(*info_sender)(int));
void free_sended_info(t_buffer*, void*, char*);
int connect_to_server(char*, char*);

#endif
