//
//  Background.cpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/23.
//

#include "Background.hpp"
#include<stdio.h>
Background::Background(){};
Background::Background(SDL_Renderer* renderer,char *a)  {
    surface = IMG_Load(a);
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);if (!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());}
    
}
Background::~Background(){};
void Background::destruct() {
    
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);}


void Background::render(SDL_Renderer* renderer){
    
    //if(bg_texture) printf("AA");
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    //SDL_RenderPresent(renderer);
}
SDL_Texture* getbgtexture(Background& e){ return e.texture;}

