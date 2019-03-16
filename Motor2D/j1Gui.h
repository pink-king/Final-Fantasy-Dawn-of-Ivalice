#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include <list>
#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"
/*#include "UiItem_Button.h"
#include "UiItem_Bar.h"*/

class j1Gui : public j1Module
{
public:

	j1Gui();
	~j1Gui();

	//Override funtions from class j1Module
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	std::list<UiItem*>	ListItemUI;
};

#endif