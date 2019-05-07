#include "j1App.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1DialogSystem.h"
#include "j1Input.h"
#include "j1Scene.h"

j1DialogSystem::j1DialogSystem()
{
}
j1DialogSystem::~j1DialogSystem()
{
}

bool j1DialogSystem::Start()
{
	bool ret = true;
	LoadDialogue("ShopDialog.xml");
	currentNode = dialogTrees[treeid]->dialogNodes[0];   // load it but do not play it yet
	//PerformDialogue(treeid);

	

	return ret;
}

bool j1DialogSystem::Update(float dt)
{
	bool ret = true;

	if (App->scene->inGamePanel->enable)
	{

		if (spawnDialogSequence)    // put the "isDialogSequenceactive to True"
		{
			PerformDialogue(treeid);
			spawnDialogSequence = false;
		}
		

		if (isDialogSequenceActive)   // capture input only in this case
		{

			if (isDialogInScreen)    // if dialog sequence is active AND inventory is NOT openned
			{

				if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
				{
                  
					bool enterInventory = false; 

					std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

					for (; iter != App->gui->ListItemUI.end(); ++iter)     // discover which of the dialog options is actually the selected object
					{

							if (App->gui->selected_object == (*iter))
							{
								input = (*iter)->dialogPos;   // capture the dialog option number

								if (currentNode->dialogOptions.at(input)->text.find("I will show you") != std::string::npos) // open player inventory
								{
									enterInventory = true;     
									App->scene->inventoryItem->isVendorInventory = false; 
								}
								else if (currentNode->dialogOptions.at(input)->text.find("Sure") != std::string::npos) // open vendor inventory
								{

									enterInventory = true;   
									App->scene->inventoryItem->isVendorInventory = true; 
								}
							
									
								

							}
							

					}

		
					App->gui->selected_object = nullptr;          // first put the selected dialog label to nullptr 
					App->gui->deleteCurrentDialogs();             // then delete the active dialog labels                 

					isDialogInScreen = false;    // the dialog labels are no longer in screen

					if (!enterInventory)
						PerformDialogue(treeid);  
					else
					{
						PerformDialogue(treeid, false); // TODO 1: don't create dialog if the inventory has to be oppened
					    App->scene->DoOpenInventory(false, App->scene->inventoryItem->isVendorInventory);  // actually open it
					}
						 


				}

				// TODO 2: perform dialog when the inventory is openned: 
				// A)Put selected to nullptr 
				// B) Delete dialogs 
				// C) DON'T PLAY THE DIALOGUE (done in TODO 1)  
				// D) put "isDialogInScreen" to False 

				// TODO 3: perform dialog when the inventory is closed:   
				// A)Put "resetHoverSwapping" to false
				// B) NOW, play the last on hold dialogue  
				// C) put "isDialogInScreen" to True


				// TODO 4: When "farewell":
				// A)Put selected to nullptr  
				// B) Delete dialogs 
				// C)Restore default dialogue tree
				// D) put  "isDialogSequenceactive and "isDialogInScreen" to False



			}


		}


	}
	
	
		
	return ret;
}

bool j1DialogSystem::CleanUp()
{
	bool ret = true;

	for (int j = 0; j < dialogTrees.size(); j++)
	{
		for (int i = 0; i < dialogTrees[j]->dialogNodes.size(); i++)
			delete dialogTrees[j]->dialogNodes[i];

		dialogTrees[j]->dialogNodes.clear();
		delete dialogTrees[j];
	}
	dialogTrees.clear();

	return ret;
}

void j1DialogSystem::PerformDialogue(int tr_id, bool CreateLabels)
{
	if (dialogTrees.empty())
		LOG("TreeEmpty");

	
		//Find the next node 
		if (input >= 0 && input < currentNode->dialogOptions.size()) //Only if the input is valid
		{
			for (int j = 0; j < dialogTrees[tr_id]->dialogNodes.size(); j++)
			{
				if (currentNode->dialogOptions[input]->nextnode == dialogTrees[tr_id]->dialogNodes[j]->id) //If the option id is the same as one of the nodes ids in the tree
				{
					currentNode = dialogTrees[tr_id]->dialogNodes[j]; // we assign our node pointer to the next node in the tree				
					break;
				}
			}
		}
	
	// Print the dialog in the screen

	if(CreateLabels)
	BlitDialog();
}

void j1DialogSystem::BlitDialog()
{
	isDialogInScreen = true; 

	UiItem_Label* vendorLabel = App->gui->AddLabel(currentNode->text.c_str(), {255, 255, 255, 255}, App->font->openSansBold18, iPoint(500, 500), App->scene->inGamePanel);
	vendorLabel->isDialog = true; 
	vendorLabel->tabbable = false; 

	int space = 500;
	for (int i = 0; i < currentNode->dialogOptions.size(); i++)
	{
		UiItem_Label* characterLabel = App->gui->AddLabel(currentNode->dialogOptions[i]->text.c_str(), { 0, 255, 0, 255 }, App->font->openSansBold18, iPoint(500, space += 50), App->scene->inGamePanel);
		
		
		characterLabel->isDialog = true;     // player labels are dialogs, tabbable, and have a pos (0, 1 or 2)
		characterLabel->tabbable = true; 
		characterLabel->dialogPos = i; 

		if(i == 0)
		App->gui->resetHoverSwapping = false;   // assign the current UI selected object to the player first choice
	}
		

}

void j1DialogSystem::spawnDialoguesAfterInventory()
{

	//currentNode = dialogTrees[treeid]->dialogNodes[4]; // recover the node before the inventory openned (could be cleaner code) 

	//input = 1; // "I changed my mind" (faked to go back to dialogue) 
	
	//PerformDialogue(treeid);        // last node was stored, it is "Anything else?" 


	BlitDialog();    

}


bool j1DialogSystem::LoadDialogue(const char* file)
{
	bool ret = true;

	pugi::xml_parse_result result = tree_file.load_file(file);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}
	else
		LOG("XML was loaded succesfully!");

	for (pugi::xml_node t = tree_file.child("dialogue").child("dialogtree"); t != NULL; t = t.next_sibling("dialogtree"))
	{
		DialogTree* tr = new DialogTree;
		tr->treeid = t.attribute("treeid").as_int();
	//	tr->karma = t.attribute("karma").as_int();
		LoadTreeData(t, tr);
		dialogTrees.push_back(tr);	
	}
	return ret;
}

bool j1DialogSystem::LoadTreeData(pugi::xml_node& trees, DialogTree* oak)
{
	bool ret = true;

	//Filling the dialogue tree information
	for (pugi::xml_node n = trees.child("node");n != NULL; n = n.next_sibling("node"))
	{
		DialogNode* node = new DialogNode;
		node->text.assign(n.attribute("line").as_string());
		node->id = n.attribute("id").as_int();
	//	node->karma = n.attribute("karma").as_int();
		LoadNodesDetails(n, node);
		oak->dialogNodes.push_back(node);
		
	}
	return ret;
}

bool j1DialogSystem::LoadNodesDetails(pugi::xml_node& text_node, DialogNode* npc)
{
	bool ret = true;
	for (pugi::xml_node op = text_node.child("option"); op != NULL; op = op.next_sibling("option"))
	{
		DialogOption* option = new DialogOption;
		option->text.assign(op.attribute("line").as_string());
		option->nextnode = op.attribute("nextnode").as_int();
	//	option->karma = op.attribute("karma").as_int();
		npc->dialogOptions.push_back(option);
	}
	return ret;
}