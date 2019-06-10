#include "j1App.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1DialogSystem.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1TransitionManager.h"
#include "Boss_Flower.h"
#include "Brofiler/Brofiler.h"


j1DialogSystem::j1DialogSystem()
{
}
j1DialogSystem::~j1DialogSystem()
{
}

bool j1DialogSystem::Start()
{
	bool ret = true;
	LoadDialogue("Dialog.xml");
	//currentNode = dialogTrees[treeid]->dialogNodes[0];   // load it but do not play it yet
	//PerformDialogue(treeid);

	


	return ret;
}

bool j1DialogSystem::Update(float dt)
{
	BROFILER_CATEGORY("Update DialogSystem", Profiler::Color::LawnGreen);
	bool ret = true;




	if (App->scene->inGamePanel->enable)
	{

		// fake devug keys to test different dialog triggers


		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			SetCurrentDialog("VENDOR");
		}

		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			SetCurrentDialog("SAVEGAME");
		}

		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		{
			SetCurrentDialog("STRANGER");
		}
		

		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		{
			SetCurrentDialog("BOSS");
		}


		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		{
			SetCurrentDialog("TUTORIAL");
		}

		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			SetCurrentDialog("PREBOSS");
		}


		if (spawnDialogSequence) 
		{
			input = -1;                     
			PerformDialogue(treeid);
			spawnDialogSequence = false;
			isDialogSequenceActive = true; 
			
			//App->entityFactory->player->selectedCharacterEntity->isParalize = true;

			App->entityFactory->player->LockPlayerInput(); 
			
			App->entityFactory->player->selectedCharacterEntity->currentAnimation = &App->entityFactory->player->selectedCharacterEntity->idle[App->entityFactory->player->selectedCharacterEntity->pointingDir];
			//App->entityFactory->player->selectedCharacterEntity->inputReady = false; 
		

			App->scene->dialogueBox->hide = false; 
		}
		

		if (isDialogSequenceActive)   // capture input only in this case
		{
			//App->entityFactory->player->selectedCharacterEntity->isParalize = true; 
			//App->entityFactory->player->selectedCharacterEntity->currentAnimation = &App->entityFactory->player->selectedCharacterEntity->idle[App->entityFactory->player->selectedCharacterEntity->pointingDir]; 

			if (isDialogInScreen)    // if dialog sequence is active AND inventory is NOT openned
			{
				if (waitForNPCTalking)
				{
					checkIfNPCFinishedTalking(); 

				}
				else if(App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // || App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
				{

					doDialogTypeLogic();


				}
				

			}


		}
		else
		{
			//App->entityFactory->player->Update(0); 
		}


	}
	
	
		
	return ret;
}


void j1DialogSystem::checkIfNPCFinishedTalking()
{

	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();
	for (; iter != App->gui->ListItemUI.end(); ++iter)     // discover which of the dialog options is actually the selected object
	{
		if ((*iter)->guiType == GUI_TYPES::LABEL)
		{
			if (dynamic_cast<UiItem_Label*>(*iter)->isDialog && dynamic_cast<UiItem_Label*>(*iter)->finishedWriting)
			{
				spawnPlayerLabelAfterNPCFinishesTalking();
				break;
			}
		}

	}

}

void j1DialogSystem::spawnPlayerLabelAfterNPCFinishesTalking()
{
	std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

	for (; iter != App->gui->ListItemUI.end(); ++iter)     
	{
		if ((*iter)->guiType == GUI_TYPES::LABEL)
		{
			if (dynamic_cast<UiItem_Label*>(*iter)->isDialog && dynamic_cast<UiItem_Label*>(*iter)->dialogPos != 666) // player options
			{
				dynamic_cast<UiItem_Label*>(*iter)->hide = false; 
				dynamic_cast<UiItem_Label*>(*iter)->tabbable = true;     // unhide and make it available to press A
				waitForNPCTalking = false; 

				App->gui->resetHoverSwapping = false; 
			}
		}
	}

}


void j1DialogSystem::doDialogTypeLogic()
{
			if (currentDialogType == "VENDOR")
			{
				bool enterInventory = false;
				std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

				for (; iter != App->gui->ListItemUI.end(); ++iter)     // discover which of the dialog options is actually the selected object
				{

					if (App->gui->selected_object == (*iter))
					{
						input = (*iter)->dialogPos;   // capture the dialog option number


						if (input >= 0 && input < currentNode->dialogOptions.size())
						{


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
				}

				App->gui->selected_object = nullptr;          // first put the selected dialog label to nullptr 
				App->gui->deleteCurrentDialogs();             // then delete the active dialog labels                 

				isDialogInScreen = false;    // the dialog labels are no longer in screen


					if (!enterInventory)
					{
						if (input >= 0 && input < currentNode->dialogOptions.size())
						{

							PerformDialogue(treeid);

						}


					}

					else
					{
						PerformDialogue(treeid, false); // TODO 1: don't create dialog if the inventory has to be oppened
						App->scene->DoOpenInventory(false, App->scene->inventoryItem->isVendorInventory);  // actually open it
					}
			}	

			else if(currentDialogType == "SAVEGAME" || currentDialogType == "STRANGER" || currentDialogType == "TUTORIAL" || currentDialogType == "BOSS" || currentDialogType == "PREBOSS")
			{
				bool enterInventory = false;
				std::list<UiItem*>::iterator iter = App->gui->ListItemUI.begin();

				for (; iter != App->gui->ListItemUI.end(); ++iter)     // discover which of the dialog options is actually the selected object
				{

					if (App->gui->selected_object == (*iter))
					{
						input = (*iter)->dialogPos;   // capture the dialog option number

					
					}
				}

				App->gui->selected_object = nullptr;          // first put the selected dialog label to nullptr 
				App->gui->deleteCurrentDialogs();

				isDialogInScreen = false;    // the dialog labels are no longer in screen


				if (input >= 0 && input < currentNode->dialogOptions.size())
				{

					PerformDialogue(treeid);

					if (currentDialogType == "SAVEGAME")
					{
						if (currentNode->dialogOptions.at(input)->text.find("Yes") != std::string::npos)
						{

							App->SaveGame("save_game.xml");
							App->HPManager->callSaveLabelSpawn(App->render->WorldToScreen(App->entityFactory->player->selectedCharacterEntity->GetPosition().x + 5, App->entityFactory->player->selectedCharacterEntity->GetPosition().y - 20));
							
							App->audio->PlayFx(App->scene->savedSFX, 0);
						}
					}
				
				}

			}








}

void j1DialogSystem::SetCurrentDialog(std::string callback) 
{


	if (!isDialogSequenceActive)
	{

		for (int j = 0; j < dialogTrees.size(); j++)
		{
			if (dialogTrees[j]->treeName == callback)
			{
				treeid = dialogTrees[j]->treeid;

				currentDialogType = dialogTrees[j]->treeName;


				currentNode = dialogTrees[j]->dialogNodes[0];
			}

		}

		spawnDialogSequence = true;
	}


	if (currentDialogType == "PREBOSS")
		App->entityFactory->setCurrentEnemiesToAGivenState(EnemyState::MENTAL_EMBOLIA); 
	

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

	bool exit = false; 
	
		//Find the next node 
		if (input >= 0 && input < currentNode->dialogOptions.size()) //Only if the input is valid
		{
			for (int j = 0; j < dialogTrees[tr_id]->dialogNodes.size(); j++)
			{
				if (currentNode->dialogOptions[input]->nextnode > dialogTrees[tr_id]->maxNodes - 1)  // if greater, exit dialogue
				{
					exit = true; 
				}
				

				if (currentNode->dialogOptions[input]->nextnode == dialogTrees[tr_id]->dialogNodes[j]->id) //If the option id is the same as one of the nodes ids in the tree
				{
					currentNode = dialogTrees[tr_id]->dialogNodes[j]; // we assign our node pointer to the next node in the tree				
					break;
				}
			}
		}
	
	// Print the dialog in the screen

		if (CreateLabels)
		{
			if(!exit)
			BlitDialog(tr_id);
			else                                       // Stop dialogue if input is greater than max nodes ("Farewell")
			{


				isDialogSequenceActive = false; 
				spawnDialogSequence = false;
				
				
				//App->entityFactory->player->selectedCharacterEntity->isParalize = false;
				//App->entityFactory->player->selectedCharacterEntity->inputReady = true;
				//App->pause = false; 


				App->scene->dialogueBox->hide = true;
				if (currentDialogType == "BOSS")
				{

					for (auto& item : App->entityFactory->entities)
					{
						if (item != nullptr)
						{
							if (item->type == ENTITY_TYPE::FLOWERBOSS)
							{
								dynamic_cast<FlowerBossEntity*>(item)->myState = Boss1State::READY;
								dynamic_cast<FlowerBossEntity*>(item)->myBossLifeBar->ShowBossBarWhenDialogIsOver();
							}
						}
					}
				}
			
				if (currentDialogType == "STRANGER")
				{
					App->scene->lobbyState = LobbyState::TALKSTRANGER;
					//TODO: put audio
				}
				if (dialogTrees[tr_id]->firstInteraction)                       // show npc name after first interaction
				{
					if(dialogTrees[tr_id]->hasNPCLabel)
						dialogTrees[tr_id]->myNPCLabels.nameLabel->hide = false;
			
					dialogTrees[tr_id]->firstInteraction = false;
				}


				if (currentDialogType == "PREBOSS")
					App->entityFactory->setCurrentEnemiesToAGivenState(EnemyState::IDLE);




				App->entityFactory->player->UnlockPlayerInput();
			}
			
	    }
	
		
}

void j1DialogSystem::destroyNPCNameLabels(SceneState sc)
{

	if (sc == SceneState::LOBBY)
	{
		for (int j = 0; j < treeCount; j++)
		{
			if (dialogTrees[j]->NPCscene == "LOBBY")
			{
				//App->gui->destroyElement(dialogTrees[j]->myNPCLabels.nameLabel);  // TODO: talk label
				if(dialogTrees[j]->hasNPCLabel)
					dialogTrees[j]->myNPCLabels.nameLabel->hide = true;
			
			}

		}
		
	}
	else if (sc == SceneState::FIRINGRANGE)
	{
		for (int j = 0; j < treeCount; j++)
		{
			if (dialogTrees[j]->NPCscene == "FIRINGRANGE")
			{
				//App->gui->destroyElement(dialogTrees[j]->myNPCLabels.nameLabel);  // TODO: talk label
				if (dialogTrees[j]->hasNPCLabel)
					dialogTrees[j]->myNPCLabels.nameLabel->hide = true;
			}

		}
	}

}

void j1DialogSystem::createNPCNameLabels(SceneState sc)
{
	if (sc == SceneState::LOBBY)
	{
		for (int j = 0; j < treeCount; j++)
		{
			if (dialogTrees[j]->NPCscene == "LOBBY")
			{
				//dialogTrees[j]->myNPCLabels.nameLabel = App->gui->AddLabel(dialogTrees[j]->NPCName, { 255, 255, 255, 255 }, App->font->knightsQuest24, { 0,0 }, App->scene->inGamePanel);
				if (!dialogTrees[j]->firstInteraction && dialogTrees[j]->hasNPCLabel)
					dialogTrees[j]->myNPCLabels.nameLabel->hide = false;   // show npc name after first interaction
			
				
			}

		}
	}

	else if (sc == SceneState::FIRINGRANGE)
	{
		for (int j = 0; j < treeCount; j++)
		{
			if (dialogTrees[j]->NPCscene == "FIRINGRANGE")
			{
				if (!dialogTrees[j]->firstInteraction && dialogTrees[j]->hasNPCLabel)
					dialogTrees[j]->myNPCLabels.nameLabel->hide = false;   // show npc name after first interaction

			}


		}
	}

}

void j1DialogSystem::hideAllNPCLabels(bool hide)
{
	bool doIt = false;

	for (auto& dialogTree : dialogTrees)                      // no need to hide "Godo" label when you open inventory in lobby, or the other labels when ypu open it in firing range
	{
		if (App->scene->state == SceneState::LOBBY && dialogTree->NPCscene == "LOBBY")
		{
			doIt = true;
		}
		else if (App->scene->state == SceneState::FIRINGRANGE && dialogTree->NPCscene == "FIRINGRANGE")
		{
			doIt = true;
		}


		if (doIt)
		{
			if (!dialogTree->firstInteraction && dialogTree->hasNPCLabel)   // if it hasn't interacted yet the label is hidden already
			{
				dialogTree->myNPCLabels.nameLabel->hide = hide;
			}
		}


	}

}

void j1DialogSystem::BlitDialog(int tr_id)
{
	isDialogInScreen = true; 

	waitForNPCTalking = true; 

	UiItem_Label* npcLabel = App->gui->AddLabel(currentNode->text.c_str(), {100, 200, 100, 255}, App->font->knightsQuest24, iPoint(515, 510), App->scene->inGamePanel, true);
	npcLabel->isDialog = true;
	npcLabel->tabbable = false;
	npcLabel->isNPCLabel = true;

	
	int space = 513;
	for (int i = 0; i < currentNode->dialogOptions.size(); i++)
	{
		UiItem_Label* characterLabel = App->gui->AddLabel(currentNode->dialogOptions[i]->text.c_str(), { 255, 255, 255, 255 }, App->font->knightsQuest24, iPoint(515, space += 21), App->scene->inGamePanel);
		
		
		characterLabel->isDialog = true;     // player labels are dialogs, tabbable, and have a pos (0, 1 or 2)
		characterLabel->tabbable = false; 
		characterLabel->dialogPos = i; 
		characterLabel->hide = true;  // hide until npc finishes talking
		/*if(i == 0)
		App->gui->resetHoverSwapping = false;   // assign the current UI selected object to the player first choice*/

	}
	
	
	/*if (tr_id != 666)
	{
		dialogTrees[tr_id]->myNPCLabels.nameLabel = App->gui->AddLabel(dialogTrees[tr_id]->NPCName, { 255, 255, 255, 255 }, App->font->knightsQuest24, { 0,0 }, App->scene->inGamePanel);
	}*/




}

void j1DialogSystem::spawnDialoguesAfterInventory()
{
	BlitDialog(666);    
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
		tr->treeName = t.attribute("treeName").as_string();
	//	tr->karma = t.attribute("karma").as_int();

		tr->NPCName = t.attribute("NPCname").as_string();
		tr->NPCscene = t.attribute("NPCscene").as_string();
		tr->NPCtriggerPos.x = t.attribute("NPCposX").as_int();
		tr->NPCtriggerPos.y = t.attribute("NPCposY").as_int();
		
		iPoint pos = App->render->WorldToScreen(tr->NPCtriggerPos.x, tr->NPCtriggerPos.y, true);
		
		if (tr->NPCName == "Stranger")
		{
		    pos.x -= 20; 
			pos.y += 50; 
		}
		else if (tr->NPCName == "Vendor")
		{
			pos.x -= 290; 
			pos.y += 120;
		}
		else if (tr->NPCName == "Godot")
		{
			pos.x -= 125;
			pos.y += 373;
		}
	
		// TODO: tutorial npc, Godo 
	
		if (tr->NPCName != "")
		{
			tr->myNPCLabels.nameLabel = App->gui->AddLabel(tr->NPCName, { 255, 255, 255, 255 }, App->font->knightsQuest24, pos, App->scene->inGamePanel);
			tr->myNPCLabels.nameLabel->hide = true;
			tr->myNPCLabels.nameLabel->useCamera = false;
			tr->hasNPCLabel = true;


			tr->myNPCLabels.nameLabel->isNPCLabel = true; 
		}
		else
			tr->hasNPCLabel = false; 


		
		LoadTreeData(t, tr);
		dialogTrees.push_back(tr);	
		treeCount++; 
	}
	return ret;
}

bool j1DialogSystem::LoadTreeData(pugi::xml_node& trees, DialogTree* oak)
{
	bool ret = true;

	uint nodesCount = 0; 
	//Filling the dialogue tree information
	for (pugi::xml_node n = trees.child("node");n != NULL; n = n.next_sibling("node"))
	{
		DialogNode* node = new DialogNode;
		node->text.assign(n.attribute("line").as_string());
		node->id = n.attribute("id").as_int();
	//	node->karma = n.attribute("karma").as_int();
		LoadNodesDetails(n, node);
		oak->dialogNodes.push_back(node);
		
		nodesCount++; 
	}

	oak->maxNodes = nodesCount;   // Will be needed if nextnode is greater than the max nodes. Then, stop dialogue


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