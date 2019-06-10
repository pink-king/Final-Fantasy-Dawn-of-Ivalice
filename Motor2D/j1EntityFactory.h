#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Enemy.h"
#include "Trigger.h"
#include "PlayerEntityManager.h"
#include "WaveManager.h"
#include <vector>
#include <list>
#include "Color.h"
#include "j1Map.h"
#include "ConsumableLoot.h"
#include "EquipableLoot.h"

#include "Projectile.h"

class DialogTriggerVolatile;
enum class ChestType; 

struct GroupInfo {
	GroupInfo(std::vector<EnemyType> types, SDL_Rect zone, uint minEnemies, uint maxEnemies, uint level)
		: types(types), zone(zone), minEnemies(minEnemies), maxEnemies(maxEnemies), groupLevel(level) {}

	std::vector<EnemyType> types;
	SDL_Rect zone = { 0, 0, 0, 0 };
	uint minEnemies;
	uint maxEnemies;
	uint groupLevel; 
};

enum class ChestType {
	NORMAL,
	SILVER,
	BOSS,
	// ---
	NOCHEST
};

enum class EnvironmentAssetsTypes
{
	NO_TYPE,
	WALL,
	TRIGGERWALL,
	WALL1,
	BREAKABLE_ASSET,
	CHEST,
	TORCH,
	// ---
	MAX
};

enum class BreakableType
{
	JAR,
	JARFULL,
	BARREL_1,
	BARREL_2,
	//---
	NO_BREAKABLE_TYPE
};

struct entityDataMap
{
	std::vector<j1Entity*> entities;

	bool isEmpty()
	{
		return entities.size() > 0;
	}
};

struct currentLootBuffRanges
{
	iPoint mainStat = { -1, -1 };
	iPoint subStatMajor = { -1, -1 };
	iPoint subStatMinor = { -1., -1 };
};


class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool LoadPortal(pugi::xml_node&);
	bool SavePortal(pugi::xml_node&) const;

	// entities constructors -------
	PlayerEntityManager* CreatePlayer(iPoint position);
	WaveManager* CreateWave(const SDL_Rect& zone, uint numWaves, WAVE_TYPE wave, j1Entity* linkedTrigger = nullptr);
	j1Entity* CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name);
	Enemy* CreateEnemy(EnemyType etype, iPoint pos, bool dummy = false);
	std::vector<j1Entity*> CreateEnemiesGroup(std::vector<EnemyType> enemyTypes, SDL_Rect zone, uint minNum, uint maxNum, uint groupLevel = 0);
	void LoadSpawnGroups();
	
	j1Entity* CreateArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, PROJECTILE_TYPE type, uint lifeTime = 0);
	j1Entity* CreateBossEmitter(fPoint pos, uint radius, uint spawnRatio, const j1Entity* owner, uint lifeTime);

	LootEntity* CreateLoot( int posX, int posY);
	LootEntity* CreateGold(int posX, int posY);

	Trigger* CreateTrigger(TRIGGER_TYPE type, float posX, float posY, SceneState scene = SceneState::MAX_STATES, Color color = Black, uint nSubtiles = 0, bool lookLeft = true);
	Trigger* CreateWaveTrigger(const iPoint& pos, const SDL_Rect& zone, uint level); 
	Trigger* CreateDialogTrigger(float posX, float posY, std::string Dtrigger,iPoint posState, uint nSubtiles = 0, bool pressA = true);
	Trigger* CreateDialogTriggerVolatile(float posX, float posY, std::string Dtrigger, uint nSubtiles = 0, DialogTriggerVolatile* parent = nullptr);

	uint CreateRandomBetween(uint min, uint max); 
	void Debug(j1Entity* ent);

	void CreateEntitiesDataMap(int width, int height);
	bool isThisSubtileEmpty(const iPoint position) const;
	bool isThisSubtileEnemyFree(const iPoint pos) const; 
	j1Entity* isThisSubtileTriggerFree(const iPoint pos) const;
	j1Entity* isThisSubtileLootFree(const iPoint pos) const;

	bool BoolisThisSubtileTriggerFree(const iPoint pos) const;
	bool isThisSubtilePlayerFree(const iPoint pos) const; 
	int GetSubtileEntityIndexAt(const iPoint position) const;
	bool areAllSubtilesReserved() const; 

	std::vector<j1Entity*>* GetSubtileEntityVectorAt(const iPoint pos) const;

	std::list<LootEntity*> GetConsumableListFromSubtile(const iPoint pos) const;

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

	void AssignEntityToSubtile(j1Entity* entity) const;
	void AssignEntityToSubtilePos(j1Entity* entity, iPoint subtile);
	bool DeleteEntityFromSubtilePos(j1Entity* entity, iPoint subtile); 
	bool DeleteEntityFromSubtile( j1Entity* entity) const;


	void setCurrentEnemiesToAGivenState(EnemyState state); 
	
	// Subyacent subtiles functions

	bool isPlayerAdjacent(const iPoint& pos) const; 
	iPoint TranslateToRelativePlayerPos(const iPoint& pos) const; 
	int GetAdjacentIndex(const iPoint& pos) const; 
	void ReserveAdjacent(const iPoint& pos);
	void FreeAdjacent(const iPoint& pos);
	bool isThisSubtileReserved(const iPoint& pos) const;
	void ReleaseAllReservedSubtiles();
	// ---------
	j1Entity* CreateAsset(EnvironmentAssetsTypes type, iPoint worldPos, SDL_Rect atlasRect, BreakableType breakableType = BreakableType::NO_BREAKABLE_TYPE, bool isBroken = false, ChestType chestType = ChestType::NORMAL, bool containsLoot = true);
//private:
	bool CheckSubtileMapBoundaries(const iPoint pos) const;

	void RandomAmountofLoot(LOOT_TYPE LootType, int amount, fPoint pos); //create the amount and type of randomLoot 
	void RepeatAmountofConsumables(int amount, fPoint pos, OBJECT_TYPE type);//Create the amount of the same Consumable with random values for gold
	void RepeatAmountofEquipable(int amount, fPoint pos, EQUIPABLE_TYPE type);//Create the amount of the same Equipable with random values for each entity
	void CreateLegendariEquipable(fPoint pos, EQUIPABLE_TYPE type);
	bool LoadLegendariData(LootEntity* lootEntity, pugi::xml_node& config);
	bool LoadLootData(LootEntity* lootEntity, pugi::xml_node& config, bool preventLegendary = false);
	void CreateLootStats(LootEntity* lootEntity);

	int GetRandomValue(int min, int max);
	j1Entity* CreateLootType(int x, int y, LOOT_TYPE definedType = LOOT_TYPE::NO_LOOT);
	LOOT_TYPE WillDrop();
	iPoint GetEnemySubtile(j1Entity* enemy);
	iPoint SetLootPos(int x, int y);

	void GenerateDescriptionForLootItem(LootEntity* lootItem);
	void DoDescriptionComparison(LootEntity* lootItem);

	void AddExp(Enemy* enemy);
	
	fPoint getplayerDamagevec();
	void setPlayerDmageVec(fPoint unitari);
	
	void MagicPriceCalculator(LootEntity*);

	void UnloadEntitiesWithoutPlayer();

public:
	fPoint					dmg_vec;
	bool					pushEF = false;
	bool					dmgInTimeFdbck = false;
	bool					loadEnemies = false;
	bool					loadPlayer = true;
	//j1Entity*				Player = nullptr;
	PlayerEntityManager*	player = nullptr;
	WaveManager*			waveManager = nullptr;

	SDL_Texture*			texture = nullptr;
	SDL_Texture*			assetsAtlasTex = nullptr;
	// TO DO Atlas of enemies
	SDL_Texture*			enemyZombieTex = nullptr; 
	SDL_Texture*			enemyBombTex = nullptr; 
	SDL_Texture*			enemyGolemTex = nullptr;
	SDL_Texture*			debugsubtileTex = nullptr; 
	SDL_Texture*			arrowsTexture = nullptr; 
	SDL_Texture*			ritzUltimateTex = nullptr; 
	SDL_Texture*			ritzBasicTex = nullptr; 
	SDL_Texture*			marcheTornadoTex = nullptr;
	SDL_Texture*			lootItemsTex = nullptr;
	SDL_Texture*			lootShadowTex = nullptr;
	SDL_Texture*			portalTex = nullptr;
	SDL_Texture*			campFireTex = nullptr;
	SDL_Texture*			interactiveStatesTex = nullptr;
	SDL_Texture*			hallTex = nullptr;
	SDL_Texture*			ButtonAtex = nullptr;
	SDL_Texture*			iceTornadoTex = nullptr; 

	std::vector<GroupInfo> spawngroups;
	std::list<j1Entity*>	entities;
	bool justGold;
	j1Timer alphaTimer;
	

private:
	std::vector<j1Entity*>	draw_entities;
	// subtile data map, associated entities to subtile
	entityDataMap* entitiesDataMap = nullptr;
	bool reservedAdjacentSubtiles[9];

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()

	int subtileWidth = 0; // stores the size in subtiles scale
	int subtileHeight = 0;
	int equipableChance = 10;
	int baseItemChance = 80; 
	int goldChance = 16; 

	uint minKillsToDeserveLoot = 10;
	uint minKillsWithPerfectGoldToDeserveMaxPrice = 20; 
	int goldMin = 0; 
	int goldMax = 0; 
	
	currentLootBuffRanges currentLootEntityBuffRanges; 

};




#endif
