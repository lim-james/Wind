#ifndef LOGIN_SCENE_H
#define LOGIN_SCENE_H

#include "LobbyScene.h"
#include "UIViewController.h"

#include "UITextField.h"
#include "UIButton.h"
#include "Sprite.h"

#include <Events/Event.h>

class LoginScene : public UIViewController {

	Profile* profile;

	Sprite* imageView;
	UIButton* confirm;

	UITextField* nameField;
	UITextField* statusField;

public:

	LoginScene();
	~LoginScene() override;

	void Reset() override;
	void Start() override;

	void PrepareForSegue(Scene* destination) override;

private:

	void DropHandler(Events::Event* event);

	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);

	UITextField* CreateTextField(const std::string& _prompt);

	void Login();

};

#endif
