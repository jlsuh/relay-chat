#include "utils/chat_room.h"

void chat_room_destroy(void* aChatRoom) {
	t_chat_room* chatRoom = (t_chat_room*) aChatRoom;
	free(chatRoom->roomName);
	list_destroy_and_destroy_elements(chatRoom->users, user_destroy);
	free(chatRoom);
}

void user_destroy(void* aUser) {
	t_user* user = (t_user*) aUser;
	free(user->userName);
	free(user);
}
