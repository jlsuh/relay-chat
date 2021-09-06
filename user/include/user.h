#ifndef SRC_USER_H_
#define SRC_USER_H_

#include "common_utils.h"
#include <readline/readline.h>

#define USER_CONFIG "cfg/user.config"
#define USER_IP "IP"
#define USER_PORT "PORT"
#define MAX_USERNAME_LENGTH 32

void start_chatting(int);
void recv_distributed_msg(void*);
void display_deserialized_msg(int);
void send_user_info(int);
void send_room_info(int);
void send_serialized_package(int, t_buffer*, op_code, char*);
void exchange_info(int, void(*info_sender)(int));
void free_sended_info(t_buffer*, void*, char*);
int connect_to_server(char*, char*);

static bool deserializeNext = true;

#endif
