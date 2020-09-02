#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* text = SDL_CreateTextureFromSurface(Game::renderer , tempSurface);
	SDL_FreeSurface(tempSurface);

	return text;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}
