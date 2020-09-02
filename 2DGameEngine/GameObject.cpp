#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) 
{
	objTexture = TextureManager::LoadTexture(textureSheet);

	xpos = x;
	ypos = y;
}

GameObject::~GameObject() 
{}

void GameObject::Update() 
{
	xpos++;

	SDL_QueryTexture(objTexture, NULL, NULL, &srcRect.w, &srcRect.h);
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = 64;
	destRect.h = 64;
}

void GameObject::Render() 
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
