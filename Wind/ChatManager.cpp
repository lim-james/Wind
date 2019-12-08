#include "ChatManager.h"

#include <Helpers/File/FileHelpers.h>
#include <Events/EventsManager.h>

#include <thread>

ChatManager::ChatManager() {
	rooms = {};
	//Events::EventsManager::GetInstance()->Subscribe("T_STEP", &ChatManager::Step, this);
}

ChatManager::~ChatManager() {
	for (auto& s : server)
		delete s;
	server.clear();

	for (auto& pair : rooms)
		delete pair.first, pair.second;
	rooms.clear();
}

void ChatManager::Host(const USHORT & port, const std::string & title) {
	TCP* socket = new TCP;
	socket->Initialize(port);
	server.push_back(socket);

	std::thread read([socket]() {
		while (true) {
			socket->ServerStep();
		}
	});
	read.detach();
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
	if (message.content.type != UPDATE_CONTENT) {
		rooms[socket]->messages.push_back(message);
	}

	if (message.content.type != TEXT_CONTENT) {
		if (message.profile.ip != profile->ip || message.profile.name != profile->name) {
			Helpers::WriteFile(message.content.title, message.content.body);
			if (message.content.type == JOIN_CONTENT) {
				if (!profile->picture.empty()) {
					std::cout << "new user.... UPdating profil\n";
					Events::EventsManager::GetInstance()->Trigger("NEW_MESSAGE", new Events::AnyType<TCP*>(socket));
					Content content;
					content.type = UPDATE_CONTENT;
					content.title = Helpers::GetFileName(profile->picture);
					content.body = Helpers::ReadFile(profile->picture);
					socket->Send(Codable::Join(2, profile, &content));
					return;
				}
			}
		}
	}

	Events::EventsManager::GetInstance()->Trigger("NEW_MESSAGE", new Events::AnyType<TCP*>(socket));
}
