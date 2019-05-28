#ifndef _J1DIALOGSYSTEM_H
#define _J1DIALOGSYSTEM_H

#include "j1Module.h"
#include <string>
#include <vector>
#include "j1App.h"
#include "p2Log.h"
#include <map>



class DialogOption
{
public:
	DialogOption() {};
	~DialogOption() {};
public:
	std::string text;
	int nextnode;
	//int karma;  no need for karma in shop right now
};

class DialogNode
{
public:
	DialogNode() {};
	~DialogNode() {};
public:
	std::string text;
	std::vector <DialogOption*> dialogOptions;
	int id/*, karma*/;
};
class DialogTree
{
public:
	DialogTree() {};
	~DialogTree() {};

public:
	std::vector <DialogNode*> dialogNodes;
	int treeid/*, karma*/;
	std::string treeName; 
	int maxNodes; 
};

class UiItem_Label;

class j1DialogSystem : public j1Module
{
public:
	j1DialogSystem();
	~j1DialogSystem();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void PerformDialogue(int tr_id, bool createLabels = true);
	bool LoadDialogue(const char*);
	bool LoadTreeData(pugi::xml_node& trees, DialogTree* oak);
	bool LoadNodesDetails(pugi::xml_node& text_node, DialogNode* npc);
	void BlitDialog();
	void doDialogTypeLogic(); 
	void SetCurrentDialog(std::string callback); 
	void spawnDialoguesAfterInventory(); 

	void checkIfNPCFinishedTalking(); 
	void spawnPlayerLabelAfterNPCFinishesTalking(); 


	// - - - - - - - - - - - - - - - - - - - - for inventory logic 

	bool isDialogInScreen = false; 
	bool isDialogSequenceActive = false;  // TODO: it should be false, then true when arriving to the store


	bool spawnDialogSequence = false; 

	bool waitForNPCTalking = true; 

private:
	std::vector <DialogTree*> dialogTrees;
	DialogNode* currentNode;
	int input = 7;
	int treeid = 0;
	pugi::xml_document	tree_file;	


	std::string currentDialogType = "DEFAULT";
};

#endif