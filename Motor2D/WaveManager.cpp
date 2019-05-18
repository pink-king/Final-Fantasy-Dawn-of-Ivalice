#include "WaveManager.h"
#include "j1EntityFactory.h"
#include "Enemy.h"
#include "j1App.h"
#include "j1BuffManager.h"

WaveManager::WaveManager(const SDL_Rect & zone, uint numWaves) : spawnZone(zone), maxWaves(numWaves), j1Entity(ENTITY_TYPE::WAVE_MANAGER, zone.x, zone.y, "WaveManager") 
{
}

WaveManager::~WaveManager()
{
}

bool WaveManager::Start()
{
	gen.seed(rd()); //Standard mersenne_twister_engine seeded with rd()
	return true;
}

bool WaveManager::CleanUp()
{
	return true;
}

bool WaveManager::PreUpdate()
{
	if (isWaveOver())
	{
		// Add sfx of wave over
		if (actualWave <= maxWaves)
		{
			toCreateNextWave = true;
			nextWaveData = LoadNextWaveData(actualWave); 
			timer.Start(); 
		}
		else
			to_finish = true; 
	}

	return true;
}

bool WaveManager::Update(float dt)
{
	if (toCreateNextWave)  
	{
		if (timer.ReadSec() > 3) // Some delay to give the player time to breathe before next wave
		{
			// TODO: Add sfx and labels
			CreateNextWave(nextWaveData); 
			toCreateNextWave = false; 
			actualWave++;
		}
	}

	return true;
}

bool WaveManager::PostUpdate()
{
	if (to_finish)
	{
		Finish(); 
		to_delete = true; 
	}

	return true;
}

WaveData WaveManager::LoadNextWaveData(uint waveNumber)
{
	WaveData data; 
	switch (waveNumber)
	{
	case 0:
		data.enemiesNumber = 5;
		data.types.push_back(EnemyType::TEST);

		data.bombChances = 0;
		data.golemChances = 0;
		data.zombieChances = 10;
		break;

	case 1: 
		data.enemiesNumber = 10;
		data.types.push_back(EnemyType::TEST);

		data.bombChances = 0;
		data.golemChances = 0;
		data.zombieChances = 10;
		break; 

	case 2: 
		data.enemiesNumber = 15;
		data.types.push_back(EnemyType::TEST);
		data.types.push_back(EnemyType::BOMB);

		data.bombChances = 2;
		data.golemChances = 0;
		data.zombieChances = 8;
		break;

	case 3:
		data.enemiesNumber = 20;
		data.types.push_back(EnemyType::TEST);
		data.types.push_back(EnemyType::BOMB);
		data.types.push_back(EnemyType::ARCHER);

		data.bombChances = 5;
		data.golemChances = 1;
		data.zombieChances = 4;		
		break;

	case 4:
		data.enemiesNumber = 20;
		data.types.push_back(EnemyType::BOMB);
		data.types.push_back(EnemyType::ARCHER);

		data.bombChances = 5;
		data.golemChances = 5;
		data.zombieChances = 0;	
		break;

	case 5:
		data.enemiesNumber = 25;
		data.types.push_back(EnemyType::TEST);
		data.types.push_back(EnemyType::BOMB);
		data.types.push_back(EnemyType::ARCHER);

		data.bombChances = 2;
		data.golemChances = 1;
		data.zombieChances = 5;		
		break;

	case 6:
		data.enemiesNumber = 25;
		data.types.push_back(EnemyType::TEST);
		data.types.push_back(EnemyType::BOMB);
		data.types.push_back(EnemyType::ARCHER);

		data.bombChances = 2;
		data.golemChances = 6;
		data.zombieChances = 2;
		break;

		// Add some more if we'd like
	default:
		break; 
	}

	return data; 
}

void WaveManager::CreateNextWave(WaveData waveData)
{
	uint enemyCount = 0; 
	uint maxEnemies = waveData.enemiesNumber;


	while (enemyCount < maxEnemies)
	{
		for (std::vector<EnemyType>::iterator typeIter = waveData.types.begin(); typeIter != waveData.types.end(); typeIter++)
		{
			Enemy* enemy = nullptr;
			iPoint spawnPos = { spawnZone.x + (int)CreateRandomBetween(0, spawnZone.w), spawnZone.y + (int)CreateRandomBetween(0,spawnZone.h) }; 
			// No need to check for valid spawn pos since its assumed the rect will be in valid place

			switch (*typeIter)
			{

			case EnemyType::TEST:
				if (CreateRandomBetween(1, 10) <= waveData.zombieChances && enemyCount < maxEnemies)
				{
					enemy = App->entityFactory->CreateEnemy(EnemyType::TEST, spawnPos);
					enemy->inWave = true; 

					alive.push_back(enemy);	// Add it to the wave enemy vector
					if (App->entityFactory->player != nullptr)
					{
						enemy->level = App->entityFactory->player->level + CreateRandomBetween(0, 2);
					}
					if (enemy != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(5, 15) + 5 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(7, 17) + 5 * enemy->level);
						enemyCount++;
					}
				}
				break; 

			case EnemyType::BOMB:
				if (CreateRandomBetween(1, 10) <= waveData.bombChances && enemyCount < maxEnemies)
				{
					enemy = App->entityFactory->CreateEnemy(EnemyType::BOMB, spawnPos);
					enemy->inWave = true;

					alive.push_back(enemy);	// Add it to the wave enemy vector

					if (App->entityFactory->player != nullptr)
					{
						enemy->level = App->entityFactory->player->level + CreateRandomBetween(0, 2);
					}
					if (enemy != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(5, 15) + 5 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(7, 17) + 5 * enemy->level);
						enemyCount++;
					}
				}
				break;

			case EnemyType::ARCHER:
				if (CreateRandomBetween(1, 10) <= waveData.golemChances && enemyCount < maxEnemies)
				{
					// Last paramater is dummy
					enemy = App->entityFactory->CreateEnemy(EnemyType::ARCHER, spawnPos, false);
					enemy->inWave = true;

					alive.push_back(enemy);	// Add it to the wave enemy vector

					if (App->entityFactory->player != nullptr)
					{
						enemy->level = App->entityFactory->player->level + CreateRandomBetween(0, 2);
					}
					if (enemy != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(5, 15) + 5 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(7, 17) + 5 * enemy->level);
						enemyCount++;
					}
				}
				break;

			default: 
				break; 
			}
		}
	}

}


bool WaveManager::isWaveOver() const
{
	return alive.empty();
}

void WaveManager::Finish()
{
	// What happens when the waves are completed - i. e. open doors, spawn legendary loot etc...
}

bool WaveManager::Load(pugi::xml_node &)
{
	return true;
}

bool WaveManager::Save(pugi::xml_node &) const
{
	return true;
}

uint WaveManager::CreateRandomBetween(uint min, uint max)
{
	std::uniform_real_distribution<float> dis(min, max);

	return (uint)dis(gen);
}

