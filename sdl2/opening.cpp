//
//  opening.cpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/23.
//
#include <SDL2/sdl.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "opening.hpp"
#include "Background.hpp"



void Open::start(SDL_Renderer* renderer,Background* bg){
    SDL_Window* gWindow = NULL;
        
    //The surface contained by the window
    SDL_Surface* gScreenSurface = NULL;


    //Current displayed image
    SDL_Surface* gCurrentSurface =NULL;
    SDL_Texture* gCurrentTexture=NULL;
    SDL_Rect rect;
    
    
    
    if (TTF_Init() == -1) {
    SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    exit(1);}

    TTF_Font* font = TTF_OpenFont("ttf/open.ttf", 100);
    if (font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    SDL_Rect text_rect;
    
    const SDL_Color color = {255, 255, 255,255};
 
    text_rect.x = 10;
    text_rect.y = 40;
    text_rect.w = 50;
    text_rect.h = 50;
    
    SDL_Rect srcrect={ 0,0,60,60};

    /*choose ball*/
    bool quit = false;
    
    //Event handler
    SDL_Event e;
    int i=1;
    rect.x=160;rect.y=200;rect.w=320;rect.h=240;
    int n=0;
    while( !quit )
        
    {
        
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                //User presses a key
                else if( e.type == SDL_KEYDOWN )
                {
                    //Select surfaces based on key press
                    switch( e.key.keysym.sym )
                    {
                            
                        case SDLK_LEFT:
                            if(i==1)i=4;
                            else i--;
                            break;
                            
                        case SDLK_RIGHT:
                            if(i==4)
                                i=1;
                            else
                                i++;
                            break;
                            
                        case SDLK_SPACE:
                            printf("aa");
                            
                            n++;
                            
                            break;
                        case SDLK_RETURN:
                            arr[n]=i;
                            
                            if(n==4) quit=1;
                            n++;
                            break;
                        case SDLK_KP_ENTER:
                            arr[n]=i;
                            
                            if(n==4) quit=1;
                            n++;
                            break;
                        default:
                            
                            break;
                    }
                    
                }
                
                
                //Apply the current image
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, getbgtexture(bg[0]), NULL, NULL);
                printf("%d",n);
                if(n==0){
                    text_rect.x = 120;
                    text_rect.y = 40;
                    text_rect.w = 400;
                    text_rect.h = 150;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "NTU VOLLEYBALL", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    text_rect.y = 170;
                    textsurface = IMG_Load("image/start.bmp");
                    texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    text_rect.x = 0;
                    text_rect.y = 350;
                    text_rect.w = 200;
                    text_rect.h = 50;
                    textsurface = TTF_RenderText_Blended(font, "B12901050", color);
                    texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    text_rect.y = 390;
                    textsurface = TTF_RenderText_Blended(font, "B12901051", color);
                    texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    text_rect.y = 430;
                    textsurface = TTF_RenderText_Blended(font, "B12901184", color);
                    texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                }
                else if(n==1){
                    //player 1 choose
                    text_rect.x = 120;
                    text_rect.y = 40;
                    text_rect.w = 300;
                    text_rect.h = 100;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "Choose PLAYER1", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    char a[50];
                    sprintf(a,"image/player1%d.bmp",i);
                    gCurrentSurface=IMG_Load(a);
                    SDL_SetColorKey(gCurrentSurface, SDL_TRUE, SDL_MapRGB(gCurrentSurface->format, 0, 0, 5));
                    gCurrentTexture = SDL_CreateTextureFromSurface(renderer, gCurrentSurface);
                    
                    SDL_RenderCopy(renderer, gCurrentTexture, &srcrect, &rect);
                }
                else if(n==2){
                    //player 2 vhoose
                    //n=2;
                    text_rect.x = 120;
                    text_rect.y = 40;
                    text_rect.w = 300;
                    text_rect.h = 100;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "Choose PLAYER2", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    char a[50];
                    sprintf(a,"image/player2%d.bmp",i);
                    gCurrentSurface=IMG_Load(a);
                    SDL_SetColorKey(gCurrentSurface, SDL_TRUE, SDL_MapRGB(gCurrentSurface->format, 0, 0, 5));
                    gCurrentTexture = SDL_CreateTextureFromSurface(renderer, gCurrentSurface);
                    
                    SDL_RenderCopy(renderer, gCurrentTexture, &srcrect, &rect);
                    
                }
                else if (n==3){
                    //choose ball
                    //n=3;
                    text_rect.x = 120;
                    text_rect.y = 40;
                    text_rect.w = 300;
                    text_rect.h = 100;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "Choose BALL", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    char a[50];
                    sprintf(a,"image/ball%d.bmp",i);
                    gCurrentSurface=IMG_Load(a);
                    gCurrentTexture = SDL_CreateTextureFromSurface(renderer, gCurrentSurface);
                    
                    SDL_RenderCopy(renderer, gCurrentTexture, &srcrect, &rect);
                }
                else{
                    //choose bg
                    text_rect.x = 120;
                    text_rect.y = 40;
                    text_rect.w = 300;
                    text_rect.h = 100;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "Choose BACKGROUND", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                    gCurrentTexture = getbgtexture(bg[i]);
                    
                    SDL_RenderCopy(renderer, gCurrentTexture, NULL, &rect);
                    
                }
                if(n!=0){
                    text_rect.x = 120;
                    text_rect.y = 130;
                    text_rect.w = 400;
                    text_rect.h = 70;
                    SDL_Surface* textsurface = TTF_RenderText_Blended(font, "LEFT and RIGHT to pick    ENTER to select", color);
                    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                    SDL_RenderCopy(renderer, texttexture, NULL, &text_rect);
                }
                
                
                    
                //Update the surface
                SDL_RenderPresent(renderer);
            }
        }
    
}
