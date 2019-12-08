#ifndef UI_VIEW_CONTROLLER_H
#define UI_VIEW_CONTROLLER_H

#include "Scene.h"
#include "UITextField.h"

class UIViewController : public Scene {

public:

	UIViewController();
	~UIViewController() override;

	void Awake() override;

protected:

	UITextField* CreateTextField(const std::string& _prompt);

};

#endif
