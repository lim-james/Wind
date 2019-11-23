#ifndef UI_TABLE_VIEW_CELL_H
#define UI_TABLE_VIEW_CELL_H

#include "Sprite.h"
#include "UILabel.h"

class UITableViewCell : public Sprite {

public:

	UILabel* title;
	UILabel* subtitle;

	virtual void Build();

};

#endif
