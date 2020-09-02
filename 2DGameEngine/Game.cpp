#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include <stdlib.h>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = {0,0,800,640};
AssetManager* Game::assets = new AssetManager(&manager);
bool Game::isRunning = false;

auto& player(manager.addEntity());

auto& label(manager.addEntity());
auto& scoreLabel(manager.addEntity());
auto& winLabel(manager.addEntity());
auto& endScoreLabel(manager.addEntity());

Game::Game() 
{
}

Game::~Game() 
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	} 

	if (TTF_Init() == -1)
	{
		std::cout << "failed to init SDL_ttf" << std::endl;
	}

	assets->AddTexture("terrain", "Assets/terrain_ss.png");
	assets->AddTexture("player", "Assets/player_anims.png");
	assets->AddTexture("player2", "Assets/player.png");
	assets->AddTexture("projectile", "Assets/proj.png");
	assets->AddTexture("enemy", "Assets/ColTex.png");

	assets->AddFont("arial", "Assets/arial.ttf", 16);
	assets->AddFont("arialBig", "Assets/arial.ttf", 32);

	map = new Map("terrain", 1, 32);
	map->LoadMap("Assets/map.map", 25, 20);
	
	player.addComponent<TransformComponent>(400.0f, 320.0f, 32, 32, 2);
	player.addComponent<SpriteComponent>("player2", false);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);
	scoreLabel.addComponent<UILabel>(10, 50, "Test String", "arial", white);
	winLabel.addComponent<UILabel>(300, 288, "GAME OVER", "arialBig", white);
	endScoreLabel.addComponent<UILabel>(320, 340, "Test String", "arialBig", white);

	assets->CreateEnemy(randomPos(800.0f, 640.0f), "enemy");
	assets->CreateEnemy(randomPos(800.0f, 640.0f), "enemy");
	assets->CreateEnemy(randomPos(800.0f, 640.0f), "enemy");
}

auto& tiles(manager.GetGroup(Game::groupMap));
auto& players(manager.GetGroup(Game::groupPlayers));
auto& colliders(manager.GetGroup(Game::groupColliders));
auto& projectiles(manager.GetGroup(Game::groupProjectiles));
auto& enemies(manager.GetGroup(Game::groupEnemies));

void Game::handleEvents() 
{
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default: break;
	}
}

void Game::update() 
{
	if (playerDead)
		return;

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	
	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& e : enemies)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider))
		{
			e->destroy();
			playerHealth -= 10;
			if (playerHealth < 0)
				playerHealth = 0;
			assets->CreateEnemy(randomPos(800.0f, 640.0f), "enemy");
		}
	}

	for (auto& p : projectiles)
	{
		for (auto& e : enemies)
		{
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider,
				p->getComponent<ColliderComponent>().collider))
			{
				p->destroy();
				e->destroy();
				assets->CreateEnemy(randomPos(800.0f, 640.0f), "enemy");
				score += 100;
			}
		}
		
	}

	std::stringstream ss;
	std::stringstream scoreStream;
	ss << "Health: " << playerHealth;
	scoreStream << "Score: " << score;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");
	scoreLabel.getComponent<UILabel>().SetLabelText(scoreStream.str(), "arial");

	if (playerHealth <= 0)
	{
		playerDead = true;
		player.destroy();
		for (auto& e : enemies)
		{
			e->destroy();
		}
		for (auto& p : projectiles)
		{
			p->destroy();
		}

		std::stringstream endScoreStream;
		endScoreStream << "Score: " << score;
		endScoreLabel.getComponent<UILabel>().SetLabelText(endScoreStream.str(), "arialBig");
	}



	/*camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > camera.w)
	{
		camera.x = camera.w;
	}
	if (camera.y > camera.h)
	{
		camera.y = camera.h;
	}*/


}

void Game::render() 
{
	SDL_RenderClear(renderer);


	for (auto& t : tiles)
	{
		t->draw();
	}

	if (playerDead)
	{
		winLabel.draw();
		endScoreLabel.draw();
		SDL_RenderPresent(renderer);
		return;
	}

	/*for (auto& c : colliders)
	{
		c->draw();
	}*/
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}

	if (playerDead)
	{
		winLabel.draw();
	}
	else {
		label.draw();
		scoreLabel.draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean() 
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

Vector2D Game::randomPos(float maxX, float maxY)
{
	Vector2D retVec;
	float randomX = ((float)rand()) / (float)RAND_MAX;
	float randomY = ((float)rand()) / (float)RAND_MAX;

	randomX = randomX * maxX;
	randomY = randomY * maxY;

	retVec.x = randomX;
	retVec.y = randomY;
	return retVec;
}

