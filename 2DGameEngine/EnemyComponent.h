#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Game.h"
#include <cmath>

class EnemyComponent : public Component
{
public:
	EnemyComponent(Manager* man)
	{
		manager = man;
	}
	~EnemyComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->speed = 2;
		playerTransform = &manager->GetGroup(Game::groupPlayers)[0]->getComponent<TransformComponent>();
	}

	void update() override
	{
		transform->velocity.x = playerTransform->position.x - transform->position.x;
		transform->velocity.y = playerTransform->position.y - transform->position.y;
		
		float magnitude = sqrt((transform->velocity.x * transform->velocity.x) + 
			(transform->velocity.y * transform->velocity.y));
		if (magnitude != 0)
		{
			transform->velocity.x /= magnitude;
			transform->velocity.y /= magnitude;
		}
	}

private:
	TransformComponent* transform;
	TransformComponent* playerTransform;
	Manager* manager;
	int speed = 0;
	Vector2D velocity;
};