
#include "Tool.hpp"
#include<stdio.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
Tool::Tool(): texture(NULL), player(0), exist(0),effect1(1), effect2(1),x(300),y(400){
    imageSurface = SDL_LoadBMP("image/greenliquid.bmp");
    
    //std::cout<<"constructor";
}

void Tool::tool_reset(){
	effect1=1;
	effect2=1;
	exist=1;
	
}

void Tool::settool(SDL_Renderer* renderer){
	if(exist==1)
	{
        printf("aaaaa");
	texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_Rect destinationRect = {x,y, imageSurface->w, imageSurface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
	}
}
 
Tool::~Tool(){}

ToolB::ToolB() {
    imageSurface = SDL_LoadBMP("image/purpleliquid.bmp");
    x=rand() % 641;
    y=200+rand() % 201;
    //std::cout<<"constructor";
}
void ToolB::rand_xy(){
	x=rand() % 641;
    y=200+rand() % 201;	
}

