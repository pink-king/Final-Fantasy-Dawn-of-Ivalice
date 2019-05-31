#include "WaveManager.h"
#include "j1EntityFactory.h"
#include "Enemy.h"
#include "j1App.h"
#include "j1BuffManager.h"

WaveManager::WaveManager(const SDL_Rect& zone, uint numWaves, WAVE_TYPE type, j1Entity* associatedTrigger) : associatedTrigger(associatedTrigger),spawnZone(zone), maxWaves(numWaves + 1), currentWave(1), type(type), j1Entity(ENTITY_TYPE::WAVE_MANAGER, zone.x, zone.y, "WaveManager")
{
	App->scene->wave_label->hide = false;
	//Add SFX spawn
	App->audio->PlayFx(App->scene->wave_start, 0);
}

WaveManager::~WaveManager()
{
	App->scene->wave_label->hide = true;
	App->audio->PlayFx(App->scene->wave_end, 0);

	// associate trigger delete
	if (associatedTrigger != nullptr)
		associatedTrigger->to_delete = true;

	// deletes any enemy

	for (std::vector<Enemy*>::iterator iter = alive.begin(); iter != alive.end(); ++iter)
	{
		(*iter)->inWave = false;
		dynamic_cast<j1Entity*>(*iter)->to_delete = true;
	}


	alive.clear();

	App->entityFactory->waveManager = nullptr;
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
		if (currentWave <= maxWaves && !toCreateNextWave)
		{
			App->audio->PlayFx(App->scene->wave_respawn, 0);
			toCreateNextWave = true;
			nextWaveData = LoadNextWaveData(currentWave);
			timer.Start();
		}
		else
			to_finish = true;
	}

	return true;
}

bool WaveManager::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		to_delete = true;
	}

	if (toCreateNextWave)
	{
		SpawnCurrentWaveLabel();
		ChangeStaticLabel();

		CreateNextWave(nextWaveData);
		toCreateNextWave = false;
		currentWave++;

		// TODO: Add sfx

		//LOG("Current Wave: %i/%i", currentWave, maxWaves);
	}

	return true;
}

void WaveManager::SpawnCurrentWaveLabel()
{
	iPoint targetLabelPos = App->render->WorldToScreen(App->entityFactory->player->selectedCharacterEntity->GetPosition().x - 75,
		App->entityFactory->player->selectedCharacterEntity->GetPosition().y - 135, true);

	App->HPManager->callWaveLabelSpawn(targetLabelPos, currentWave);
}

void WaveManager::ChangeStaticLabel()
{
	str_wave = "wave " + std::to_string(currentWave) + "/" + std::to_string(maxWaves);
	App->scene->wave_label->ChangeTextureIdle(str_wave, NULL, NULL);
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
		/*case 0:	// Starts at 1
			data.enemiesNumber = 5;
			data.types.push_back(EnemyType::TEST);
			data.bombChances = 0;
			data.golemChances = 0;
			data.zombieChances = 10;
			break;*/

	case 1:
		data.enemiesNumber = 5;
		data.types.push_back(EnemyType::TEST);

		data.bombChances = 0;
		data.golemChances = 0;
		data.zombieChances = 10;
		break;

	case 2:
		data.enemiesNumber = 10;
		data.types.push_back(EnemyType::TEST);

		data.bombChances = 0;
		data.golemChances = 0;
		data.zombieChances = 10;
		break;

	case 3:
		{
		if (type == WAVE_TYPE::LEVEL_1)
		{
			data.enemiesNumber = 12;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);

			data.zombieChances = 8;
			data.golemChances = 0;
			data.bombChances = 2;
		}
		else
		{
			data.enemiesNumber = 12;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);

			data.bombChances = 2;
			data.golemChances = 0;
			data.zombieChances = 8;
		}
		}
		break;

	case 4:
		if (type == WAVE_TYPE::LEVEL_1)
		{
			data.enemiesNumber = 14;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);

			data.bombChances = 3;
			data.golemChances = 0;
			data.zombieChances = 8;
		}
		else
		{
			data.enemiesNumber = 14;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);
			data.types.push_back(EnemyType::ARCHER);

			data.bombChances = 5;
			data.golemChances = 1;
			data.zombieChances = 4;
		}
		break;

	case 5:
		if (type == WAVE_TYPE::LEVEL_1)
		{
			data.enemiesNumber = 14;
			data.types.push_back(EnemyType::BOMB);

			data.bombChances = 4;
			data.golemChances = 0;
			data.zombieChances = 8;
		}
		else
		{
			data.enemiesNumber = 14;
			data.types.push_back(EnemyType::BOMB);
			data.types.push_back(EnemyType::ARCHER);

			data.bombChances = 5;
			data.golemChances = 5;
			data.zombieChances = 0;
		}
		break;

	case 6:
		if (type == WAVE_TYPE::LEVEL_1)
		{
			data.enemiesNumber = 16;
			data.types.push_back(EnemyType::TEST);

			data.bombChances = 1;
			data.golemChances = 0;
			data.zombieChances = 10;
		}
		else
		{
			data.enemiesNumber = 18;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);
			data.types.push_back(EnemyType::ARCHER);

			data.bombChances = 2;
			data.golemChances = 1;
			data.zombieChances = 5;
		}
		break;

	case 7:
		{
		if (type == WAVE_TYPE::LEVEL_1)
		{
			data.enemiesNumber = 20;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);

			data.bombChances = 3;
			data.golemChances = 0;
			data.zombieChances = 7;
		}
		else
		{
			data.enemiesNumber = 22;
			data.types.push_back(EnemyType::TEST);
			data.types.push_back(EnemyType::BOMB);
			data.types.push_back(EnemyType::ARCHER);

			data.bombChances = 2;
			data.golemChances = 6;
			data.zombieChances = 2;
		}
		}
		break;
		// Add some more if we'd like
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
			spawnPos = App->map->IsoToWorld(spawnPos.x, spawnPos.y);
			spawnPos.x = spawnPos.x * 2;

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
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(2, 3) + 2 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(4, 8) + 2 * enemy->level);
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
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(3, 5) + 2 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(4, 8) + 2 * enemy->level);
						enemyCount++;
					}
				}
				break;

			case EnemyType::ARCHER:

				if (CreateRandomBetween(1, 10) <= waveData.bombChances && enemyCount < maxEnemies)
				{	// Last paramater is dummy
					enemy = App->entityFactory->CreateEnemy(EnemyType::ARCHER, spawnPos, false);
					enemy->inWave = true;

					alive.push_back(enemy);	// Add it to the wave enemy vector

					if (App->entityFactory->player != nullptr)
					{
						enemy->level = App->entityFactory->player->level + CreateRandomBetween(0, 2);
					}
					if (enemy != nullptr)
					{
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, enemy, "\0", CreateRandomBetween(4, 6) + 3 * enemy->level);
						App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, enemy, "\0", CreateRandomBetween(20, 25) + 3 * enemy->level);
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

bool WaveManager::Load(pugi::xml_node&)
{
	return true;
}

bool WaveManager::Save(pugi::xml_node&) const
{
	return true;
}

uint WaveManager::CreateRandomBetween(uint min, uint max)
{
	std::uniform_real_distribution<float> dis(min, max);

	return (uint)dis(gen);
}
