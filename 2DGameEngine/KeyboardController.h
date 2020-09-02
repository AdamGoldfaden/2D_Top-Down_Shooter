#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
private:
	int fireDelay;
	int maxFireDelay;
public: 
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		fireDelay = 0;
		maxFireDelay = 20;
	}

	void update() override
	{
		if (fireDelay == maxFireDelay)
		{
			fireDelay = 0;
		}
		else if (fireDelay != 0)
		{
			fireDelay++;
		}

		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_w:
					transform->velocity.y = -1;
					//sprite->Play("Walk");
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					//sprite->Play("Walk");
					break;
				case SDLK_d:
					transform->velocity.x = 1;
					//sprite->Play("Walk");
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					//sprite->Play("Walk");
					break;
				case SDLK_RIGHT:
					if (fireDelay == 0)
					{
						Game::assets->CreateProjectile(transform->position, Vector2D(2, 0), 200, 2, "projectile");
						fireDelay++;
					}
					break;
				case SDLK_LEFT:
					if (fireDelay == 0)
					{
						Game::assets->CreateProjectile(transform->position, Vector2D(-2, 0), 200, 2, "projectile");
						fireDelay++;
					}
					break;
				case SDLK_UP:
					if (fireDelay == 0)
					{
						Game::assets->CreateProjectile(transform->position, Vector2D(0, -2), 200, 2, "projectile");
						fireDelay++;
					}
					break;
				case SDLK_DOWN:
					if (fireDelay == 0)
					{
						Game::assets->CreateProjectile(transform->position, Vector2D(0, 2), 200, 2, "projectile");
						fireDelay++;
					}
					break;
				default:
					break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->spriteFlip = SDL_FLIP_NONE;
				//sprite->Play("Idle");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}
	}
};
