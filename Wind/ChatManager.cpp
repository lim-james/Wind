#include "ChatManager.h"

#include <Helpers/File/FileHelpers.h>
#include <Events/EventsManager.h>

#include <thread>

ChatManager::ChatManager() {
	rooms = {};
	//Events::EventsManager::GetInstance()->Subscribe("T_STEP", &ChatManager::Step, this);
}

ChatManager::~ChatManager() {
}

void ChatManager::Host(const USHORT & port, const std::string & title) {
	TCP* socket = new TCP;
	socket->Initialize(port);

	Room* room = new Room;
	room->title = title;
	rooms[socket] = room;
}

TCP* ChatManager::Join(const std::string & ip, const USHORT & port, std::function<void(TCP*)> completion) {
	TCP* socket = new TCP;
	if (socket->Connect(ip, port, completion)) {
		socket->BindReceiveHandler(&ChatManager::ReceiveHandler, this);

		Room* room = new Room;
		rooms[socket] = room;

		std::thread read([socket]() {
			while (true) {
				socket->ClientStep();
			}
		});
		read.detach();
		return socket;
	}

	delete socket;
	return nullptr;
}

void ChatManager::SetProfile(Profile * const _profile) {
	profile = _profile;
}

Profile * ChatManager::GetProfile() const {
	return profile;
}

Room * ChatManager::GetRoom(TCP* const socket) {
	return rooms[socket];
}

std::map<TCP*, Room*>& ChatManager::GetRooms() {
	return rooms;
}

void ChatManager::ReceiveHandler(std::string result, TCP* socket) {
	Message message;
	message.Decode(result);
	rooms[socket]->messages.push_back(message);

	if (message.content.type != TEXT_CONTENT) {
		if (message.profile.name != profile->name) {
			Helpers::WriteFile(message.content.title, message.content.body);
		}
	}

	Events::EventsManager::GetInstance()->Trigger("NEW_MESSAGE", new Events::AnyType<TCP*>(socket));
}
