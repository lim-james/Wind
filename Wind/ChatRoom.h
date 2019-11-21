#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "TCP.h"

#include "Scene.h"
#include "Sprite.h"

#include "UITextField.h"

#include <Math/Vectors.hpp>

class ChatRoom : public Scene {


	Sprite* cursor;

	TCP* client;

public:

	ChatRoom();
	~ChatRoom() override;

	void Awake() override;

private:

	void ReturnHandler(UITextField* const target);

};

#endif
