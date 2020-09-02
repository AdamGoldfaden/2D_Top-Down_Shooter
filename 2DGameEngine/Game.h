#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Vector2D.h"

class ColliderComponent;
class AssetManager;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; }
	Vector2D randomPos(float maxX, float maxY);

	static bool isRunning;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static AssetManager* assets;

	int playerHealth = 110;
	int score = 0;
	bool playerDead = false;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders, 
		groupProjectiles
	};

private:
	SDL_Window* window;
	int cnt = 0;
};

