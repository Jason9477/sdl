//
//  Point.cpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/23.
//
#include <stdio.h>
#include "Point.hpp"
Point::Point(){};
Point::Point(SDL_Renderer* renderer,const char* path){

    
    if (TTF_Init() == -1) {
    SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    exit(1);}

    font = TTF_OpenFont(path, 15);
    if (font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    sprintf(score_char,"%d",score);
    surface = TTF_RenderText_Blended(font, score_char, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    text_rect.x = 10;
    text_rect.y = 40;
    text_rect.w = 50;
    text_rect.h = 50;
    

}
Point::Point(const Point& old_point){
    score=old_point.score;
    font=old_point.font;
    surface=old_point.surface;
    texture=old_point.texture;
    text_rect=old_point.text_rect;
   
    
}

Point& Point::operator++() {
        score++;  // Increment score
        sprintf(score_char,"%d",score);
        surface = TTF_RenderText_Blended(font, score_char, color);
        //texture = SDL_CreateTextureFromSurface(renderer, surface);

        return *this;
    }
void Point::set_xpos(int x){
    text_rect.x=x;
    
}
void Point::render(SDL_Renderer* renderer){
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
}
void Point::reset(){
    score=0;
    sprintf(score_char,"%d",score);
    surface = TTF_RenderText_Blended(font, score_char, color);
}
Point::~Point(){};
