#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Enemy.h"
#include "Trigger.h"
#include "PlayerEntityManager.h"
#include "WaveManager.h"
#include <vector>
#include "Color.h"
#include "j1Map.h"
#include "ConsumableLoot.h"
#include "EquipableLoot.h"

#include "Projectile.h"

struct GroupInfo {
	GroupInfo(std::vector<EnemyType> types, SDL_Rect zone, uint minEnemies, uint maxEnemies)
		: types(types), zone(zone), minEnemies(minEnemies), maxEnemies(maxEnemies) {}

	std::vector<EnemyType> types;
	SDL_Rect zone = { 0, 0, 0, 0 };
	uint minEnemies;
	uint maxEnemies;
};

enum class EnvironmentAssetsTypes
{
	NO_TYPE,
	WALL,
	TRIGGERWALL,
	WALL1,
	// ---
	MAX
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
	WaveManager* CreateWave(const SDL_Rect& zone, uint numWaves); 
	j1Entity* CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name);
	Enemy* CreateEnemy(EnemyType etype, iPoint pos, bool dummy = false);
	void CreateEnemiesGroup(std::vector<EnemyType> enemyTypes, SDL_Rect zone, uint minNum, uint maxNum);
	void LoadSpawnGroups();
	
	j1Entity* CreateArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, PROJECTILE_TYPE type, uint lifeTime = 0);

	LootEntity* CreateLoot( int posX, int posY);
	LootEntity* CreateGold(int posX, int posY);

	Trigger* CreateTrigger(TRIGGER_TYPE type, float posX, float posY,SceneState scene = SceneState::MAX_STATES, Color color = Black);

	uint CreateRandomBetween(uint min, uint max); 
	void Debug(j1Entity* ent);

	void CreateEntitiesDataMap(int width, int height);
	bool isThisSubtileEmpty(const iPoint position) const;
	bool isThisSubtileEnemyFree(const iPoint pos) const;
	j1Entity * isThisSubtileTriggerFree(const iPoint pos) const;

	j1Entity * isThisSubtileLootFree(const iPoint pos) const;

	bool BoolisThisSubtileTriggerFree(const iPoint pos) const;

	bool isThisSubtilePlayerFree(const iPoint pos) const; 
	int GetSubtileEntityIndexAt(const iPoint position) const;
	bool areAllSubtilesReserved() const; 

	std::vector<j1Entity*>* GetSubtileEntityVectorAt(const iPoint pos) const;

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

	void AssignEntityToSubtile(j1Entity* entity) const;
	void AssignEntityToSubtilePos(j1Entity * entity, iPoint subtile);
	bool DeleteEntityFromSubtile( j1Entity* entity) const;


	// Subyacent subtiles functions

	bool isPlayerAdjacent(const iPoint& pos) const; 
	iPoint TranslateToRelativePlayerPos(const iPoint& pos) const; 
	int GetAdjacentIndex(const iPoint& pos) const; 
	void ReserveAdjacent(const iPoint& pos);
	void FreeAdjacent(const iPoint& pos);
	bool isThisSubtileReserved(const iPoint& pos) const;
	void ReleaseAllReservedSubtiles();
	// ---------
	j1Entity* CreateAsset(EnvironmentAssetsTypes type, iPoint worldPos, SDL_Rect atlasRect);
//private:
	bool CheckSubtileMapBoundaries(const iPoint pos) const;

	bool LoadLootData(LootEntity* lootEntity, pugi::xml_node& config);
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

public:
	fPoint					dmg_vec;
	bool					pushEF = false;
	bool					dmgInTimeFdbck = false;
	bool					loadEnemies = false;
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
	SDL_Texture*			portalTex = nullptr;
	std::vector<GroupInfo> spawngroups;
	std::vector<j1Entity*>	entities;
	bool justGold;
	j1Timer alphaTimer;
	//----SFX-----//
	unsigned int lootGroundSFX;
	unsigned int potionGroundSFX;
	unsigned int coinGroundedSFX;
	unsigned int swapCharSFX;
	unsigned int stepSFX;
	unsigned int enemySpawn;
	unsigned int goblinDetection;
	unsigned int marcheDamaged;
	unsigned int marcheBasic;
	unsigned int marcheTornadoExplosion;
	unsigned int marcheBasic2;
	unsigned int RitzDamaged;
	unsigned int RitzBasic;
	unsigned int RitzBasicHit;
	unsigned int RitzMedusa;
	unsigned int RitzAbility2;
	unsigned int RitzAbility1;
	unsigned int RitzUltimate;
	unsigned int sharaBasic;
	unsigned int basicBodyImp;
	unsigned int strech_Shoot;
	unsigned int emitter_explodeFire;
	unsigned int SharaUltimateWoosh;
	unsigned int SharaDamaged;
	unsigned int goblinDamaged;
	unsigned int goblinDeath;
	unsigned int goblinAttack;
	unsigned int goblinLaugh;
	unsigned int marcheUltimateScream;
	unsigned int marcheAbility1;
	unsigned int marcheEarthShakeSFX;
	unsigned int marcheAbility2; //tornado
	unsigned int dash;
	unsigned int sharaAbility1;
	unsigned int sharaAbility2_shoot;
	unsigned int sharaBasic_ImpactsWall; 
	unsigned int sharaAbility1_ImpactsWall; 
	unsigned int sharaAbility2_ImpactsWall;
	unsigned int BombDeathSFX;
	unsigned int bombgetHitSFX;
private:
	std::vector<j1Entity*>	draw_entities;
	// subtile data map, associated entities to subtile
	entityDataMap* entitiesDataMap = nullptr;
	bool reservedAdjacentSubtiles[9];

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()

	int subtileWidth = 0; // stores the size in subtiles scale
	int subtileHeight = 0;
	int equipableChance = 35;
	int baseItemChance = 50; 
	int goldChance = 50; 

	uint minKillsToDeserveLoot = 5;
	uint minKillsWithPerfectGoldToDeserveMaxPrice = 15; 
	int goldMin = 0; 
	int goldMax = 0; 
	
	currentLootBuffRanges currentLootEntityBuffRanges; 

};




#endif
