#include "utils/chat_room.h"

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
