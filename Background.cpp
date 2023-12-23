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
    bg_surface = IMG_Load(a);
    
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);if (!bg_surface) {
        printf("IMG_Load: %s\n", IMG_GetError());}
}

void Background::destruct() const{
    
        SDL_FreeSurface(bg_surface);
        SDL_DestroyTexture(bg_texture);}


void Background::render(SDL_Renderer* renderer)const {
    
    //if(bg_texture) printf("AA");
    SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
    //SDL_RenderPresent(renderer);
}
