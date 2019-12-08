#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include "TCP.h"
#include "Message.h"

#include <map>

struct Room {
	std::string title;
	std::vector<Message> messages;
};

class ChatManager {

	Profile* profile;
	std::map<TCP*, Room*> rooms;

public:

	ChatManager();
	~ChatManager();

	void Host(const USHORT& port, const std::string& title);
	TCP* Join(const std::string& ip, const USHORT& port, std::function<void(TCP*)> completion);

	void SetProfile(Profile* const _profile);
	Profile* GetProfile() const;

	Room* GetRoom(TCP* const socket);
	std::map<TCP*, Room*>& GetRooms();

private:

	void ReceiveHandler(std::string result, TCP* socket);

};

#endif
