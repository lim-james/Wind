#ifndef LOBBY_SCENE_H
#define LOBBY_SCENE_H

#include "TCP.h"

#include "Scene.h"

#include "UITextField.h"
#include "UIButton.h"
#include "Sprite.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class LobbyScene : public Scene {

	TCP* client;

	Sprite* imageView;
	UIButton* confirm;

	UITextField* ipField;
	UITextField* portField;

public:

	LobbyScene();
	~LobbyScene() override;

	void Awake() override;
	void Reset() override;
	void Start() override;

	void PrepareForSegue(Scene* destination) override;

private:
	
	void DropHandler(Events::Event* event);

	void DidChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);

	UITextField* CreateTextField(const std::string& _prompt = "");

};

#endif
