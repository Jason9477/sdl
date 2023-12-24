#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//#include <SDL.h>
//#include <SDL_image.h>
//include <SDL_ttf.h>
//#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include<iostream>
#include <cstring>
#include "opening.hpp"
#include "Background.hpp"
#include "Point.hpp"
#include "Tool.hpp"
#include "Points.hpp"
const Uint32 ANIMATION_FRAME_TIME = 1000000/25*3;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int FRAME_INIT_X = 0;
const int FRAME_INIT_Y = 0;
const int ANIMATION_FRAME_WIDTH = 60;
const int ANIMATION_FRAME_HEIGHT = 60;
int ENDTURN = 0;
int balltype=1;
bool runagain=0;
bool game_finish=0;



//The sound effects that will be used
Mix_Music *gMusic=NULL;
Mix_Music *gWin=NULL;
Mix_Music *gMain=NULL;
Mix_Chunk* gButton = NULL;
Mix_Chunk* gBall = NULL;
Mix_Chunk* gScore = NULL;
Mix_Chunk* gJump = NULL;
//Mix_Chunk* gMusic = NULL;


bool loadMedia()
{
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    } else {
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }
    gWin = Mix_LoadMUS( "sounds/Win.mp3" );
    gMusic = Mix_LoadMUS( "sounds/bgm.mp3" );
    gMain = Mix_LoadMUS( "sounds/main.mp3" );
    gBall = Mix_LoadWAV( "sounds/ball.mp3" );
    gButton = Mix_LoadWAV( "sounds/button.mp3" );
    gScore = Mix_LoadWAV( "sounds/score.mp3" );
    gJump = Mix_LoadWAV( "sounds/jump.mp3" );
    
    return true;
}


/** Names of sprites
* Names for array indexes to simulate associative array
*/
enum Actors
{
  BALL = 0,
  PLAYER1 = 1,
  PLAYER2 = 2,
  NET = 3
};

/** Sprite
* Holds all informations about sprite.
*/
class Sprite
{
public:
  SDL_Rect srcrect; /**< current animation frame from sprite sheet */
  SDL_Rect dstrect; /**< where to render sprite */
  SDL_Texture *texture;
  SDL_FPoint d; /**< change in position to apply */
  double angle; /**< rotation to apply, used only by BALL */
  Uint32 last_anim_frame_change; /**< time of last change of animation frame*/
};
typedef class Sprite Sprite;

Sprite load_sprite(SDL_Renderer *renderer, const char *filename)
{
  Sprite sprite;
  SDL_Surface *surface = SDL_LoadBMP(filename);
  SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 5));
  sprite.texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  /*SDL_SetTextureBlendMode(sprite.texture, SDL_BLENDMODE_BLEND);*/
  sprite.last_anim_frame_change = ANIMATION_FRAME_TIME;
  sprite.srcrect.x = FRAME_INIT_X;
  sprite.srcrect.y = FRAME_INIT_Y;
  sprite.srcrect.w = ANIMATION_FRAME_WIDTH;
  sprite.srcrect.h = ANIMATION_FRAME_HEIGHT;
  sprite.dstrect.w = ANIMATION_FRAME_WIDTH;
  sprite.dstrect.h = ANIMATION_FRAME_HEIGHT;
  sprite.d.x = 0;
  sprite.d.y = 0;
  sprite.angle = 0;
  return sprite;
}




void load_sprites(SDL_Renderer *renderer, Sprite *sprite,Open open)
{
    char a[50];
    sprintf(a,"image/player1%d.bmp",open.arr[1]);
  sprite[PLAYER1] = load_sprite(renderer, a);
    sprintf(a,"image/player2%d.bmp",open.arr[2]);
  sprite[PLAYER2] = load_sprite(renderer, a);
  sprite[NET] = load_sprite(renderer, "image/net.bmp");
    int balltype=open.arr[3];
    sprintf(a,"image/ball%d.bmp",balltype);

         sprite[BALL] = load_sprite(renderer, a);


  sprite[PLAYER1].dstrect.y = WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT;
  sprite[PLAYER2].dstrect.y = WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT;

  sprite[NET].srcrect.x = 0;
  sprite[NET].srcrect.y = 0;
  sprite[NET].srcrect.w = 10;
  sprite[NET].srcrect.h = 200;
  sprite[NET].dstrect.x = WINDOW_WIDTH / 2 - sprite[NET].srcrect.w / 2;
  sprite[NET].dstrect.y = WINDOW_HEIGHT - sprite[NET].srcrect.h;
  sprite[NET].dstrect.w = sprite[NET].srcrect.w;
  sprite[NET].dstrect.h = sprite[NET].srcrect.h;

}

void place_sprites_on_start(Sprite *sprite, int delivery)
{
    sprite[PLAYER1].dstrect.x = 50;
    sprite[PLAYER2].dstrect.x = 520;
    
    if (delivery == PLAYER2)
    {
        sprite[BALL].dstrect.x = 50;
        sprite[BALL].dstrect.y = 50;
    }
    else if (delivery == PLAYER1)
    {
        sprite[BALL].dstrect.x = 520;
        sprite[BALL].dstrect.y = 50;
    }
    else
    {
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    sprite[BALL].d.x = 0;
    sprite[BALL].d.y = 0;
}
SDL_bool process_events(unsigned int *is_npc)
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {

    switch(event.type)
    {

      case SDL_WINDOWEVENT_CLOSE:
        return SDL_TRUE;

      case SDL_QUIT:
        return SDL_TRUE;
        case SDL_MOUSEBUTTONDOWN:
            
            runagain=1;
           // std::cout<<"lll";
            return SDL_FALSE;
            
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_F1)
        {
          *is_npc = *is_npc ^ 1; /* toggle first bit in numer for player 1, used in control_oponent */
        }
        if (event.key.keysym.sym == SDLK_F2)
        {
          *is_npc = *is_npc ^ 2; /* toggle second bit in numer for player 2, used in control_oponent */
        }
        return SDL_FALSE;

      default:
        return SDL_FALSE;

    }
  }
  return SDL_FALSE;
}

void control_player(Sprite *sprites)
{
  const int step = 5;
  const int jump = 16;
  const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

  /* PLAYER 1 */
  if (keyboard_state[SDL_SCANCODE_A])
  {
    sprites[PLAYER1].d.x = -step;
  }
  else if (keyboard_state[SDL_SCANCODE_D])
  {
     sprites[PLAYER1].d.x = step;
  }
  else
  {
    sprites[PLAYER1].d.x = 0;
  }

  if (keyboard_state[SDL_SCANCODE_W])
  {
      Mix_PlayChannel( -1, gJump, 0 );
    if (sprites[PLAYER1].dstrect.y == WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT) /* IS standing on the ground */
    {
      sprites[PLAYER1].d.y -= jump;
    }
  }

  /* PLAYER 2 */
  if (keyboard_state[SDL_SCANCODE_LEFT])
  {
    sprites[PLAYER2].d.x = -step;
  }
  else if (keyboard_state[SDL_SCANCODE_RIGHT])
  {
    sprites[PLAYER2].d.x = step;
  }
  else
  {
    sprites[PLAYER2].d.x = 0;
  }

  if (keyboard_state[SDL_SCANCODE_UP])
  {
      Mix_PlayChannel( -1, gJump, 0 );
    if (sprites[PLAYER2].dstrect.y == WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT) /* IS standing on the ground */
    {
      sprites[PLAYER2].d.y-=jump;
    }
  }

}

void control_oponent(Sprite *sprites, unsigned int is_npc)
{
  if (ENDTURN) {return;}
  const int step = 5;
  const int jump = 16;
  const int max_jump = 150;
  int x_diff;
  float x_ratio;
  int npc;

  if (sprites[BALL].dstrect.x > sprites[NET].dstrect.x && (is_npc == 2 || is_npc == 3)) /* binary 10 OR 11 */
  {
    npc = PLAYER2;
  }
  else if (sprites[BALL].dstrect.x < sprites[NET].dstrect.x && (is_npc == 1 || is_npc == 3)) /* binary 01 or 11 */
  {
    npc = PLAYER1;
  }
  else
  {
  return;
  }
  x_diff = sprites[BALL].dstrect.x - sprites[npc].dstrect.x;
  x_ratio = (float) x_diff / 55; /**< 1.0..-1.0 player left positive, player right negative*/

  if (x_ratio < -1) /* go forward or backward if not uder ball */
  {
    sprites[npc].d.x = -step;
  }
  else if (x_ratio > 1)
  {
    sprites[npc].d.x = step;
  }

  else /* if under ball */
  {
    sprites[npc].d.x = 0;

    if (x_ratio == 0) /* if just in the center of ball, step sideways to avoid 90 degree ball movement */
    {
      if(SDL_GetTicks() % 2 == 0)
      {
        sprites[npc].d.x = step;
      }
      else
      {
        sprites[npc].d.x = -step;
      }
    }

    if (sprites[BALL].dstrect.y >  max_jump && sprites[npc].dstrect.y == WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT) /* if ball close enough, jump */
    {
      sprites[npc].d.y -= jump;
    }
  }
}


void apply_gravity(Sprite *sprites)
{
  for (int i = BALL; i < PLAYER2+1; i++)
  {
    if (sprites[i].dstrect.y < WINDOW_HEIGHT - sprites[i].dstrect.h)
    {
    
      sprites[i].d.y += (i==BALL?0.3:0.7);
      
    }
  }
}

int bounce_ball(Sprite *ball)
{
  if (ball->dstrect.y >= WINDOW_HEIGHT - ball->dstrect.h) /* hit GROUND*/
  {
    ball->d.y = ball->d.y * -0.5;
    ball->d.x *= 0.5;
    if (ball->dstrect.x > WINDOW_WIDTH / 2)
      {return PLAYER1;}
    else
      {return PLAYER2;}
  }

  if (ball->dstrect.y <= 0) /*hit CEILING */
  {
    ball->d.y = ball->d.y * -0.6;
  }

  if (ball->dstrect.x >= WINDOW_WIDTH - ball->dstrect.w) /* hit RIGHT wall */
  {
    ball->d.x = ball->d.x * -0.6;
  }

  if  (ball->dstrect.x <= 0) /* hit LEFT wall */
  {
    ball->d.x = ball->d.x * -0.6;
  }
  return 0;
}

void hit_ball(Sprite *sprites)
{
  if (ENDTURN) {return;}
  int x_diff = 0;
  float x_ratio = 0;
  for (int i = PLAYER1; i < NET; i++)
  {
    if (SDL_HasIntersection(&sprites[i].dstrect, &sprites[BALL].dstrect))
    {
        Mix_PlayChannel( -1, gBall, 0 );
      sprites[BALL].dstrect.y = sprites[i].dstrect.y - sprites[BALL].dstrect.h;
      x_diff = sprites[BALL].dstrect.x - sprites[i].dstrect.x;
      x_ratio = (float) x_diff / 55; /**< 1.0..-1.0 player left positive, player right negative*/
      if (sprites[BALL].d.y >= 0)
      {
        sprites[BALL].d.y = sprites[BALL].d.y * -.6;
        sprites[BALL].d.x = sprites[BALL].d.x * -.6;
      }
      else
      {sprites[BALL].d.y += sprites[i].d.y;
       sprites[BALL].d.x = abs(sprites[BALL].d.y) * x_ratio;}


    }
  }


}

void hit_net(Sprite *sprites)
{
if (SDL_HasIntersection(&sprites[BALL].dstrect, &sprites[NET].dstrect))
  {
      //std::cout<<"a";
      
    if (sprites[BALL].dstrect.x < sprites[NET].dstrect.x)
    {
      sprites[BALL].dstrect.x = sprites[NET].dstrect.x - sprites[BALL].dstrect.w;
    }
    else
    {
      sprites[BALL].dstrect.x = sprites[NET].dstrect.x + sprites[NET].dstrect.w;
    }
    sprites[BALL].d.x *= -0.8;
  }
}

void animate_players(Sprite *sprites)
{
  for (int i = PLAYER1; i < NET; i++)
  {
    bool moves_verticaly = (sprites[i].d.y < 0);
    if (moves_verticaly)
    {
      sprites[i].srcrect.y = 60; /* animation row */
    }
    else
    {
      sprites[i].srcrect.y = 0;
    }


    /* animation col */
    bool moves_horizontaly = (sprites[i].d.x != 0);
    if (SDL_TICKS_PASSED(SDL_GetTicks(), sprites[i].last_anim_frame_change) && moves_horizontaly)
    {
      if (sprites[i].srcrect.x < ANIMATION_FRAME_WIDTH) {sprites[i].srcrect.x+=ANIMATION_FRAME_WIDTH;}
      else {sprites[i].srcrect.x = FRAME_INIT_X;}
      sprites[i].last_anim_frame_change = SDL_GetTicks() + ANIMATION_FRAME_TIME;
    }
    else
    {
      if (SDL_TICKS_PASSED(SDL_GetTicks(), sprites[i].last_anim_frame_change))
      {
        sprites[i].srcrect.x = FRAME_INIT_X;
      }
    }
  }
}

void animate_ball(Sprite *ball)
{
  if (ball->d.x != 0)
  {
    if (ball->angle <  360)
      {ball->angle += 6;}
    else
      {ball->angle = 0;}
  }
}

void apply_delta(Sprite *sprites,Tool * green,ToolB * purple)
{
    sprites[BALL].dstrect.x += balltype*sprites[BALL].d.x*0.8;
        sprites[BALL].dstrect.y += balltype*sprites[BALL].d.y*0.8;
          sprites[PLAYER1].dstrect.x += green->effect1*purple->effect1*sprites[PLAYER1].d.x;
        sprites[PLAYER1].dstrect.y += green->effect1*purple->effect1*sprites[PLAYER1].d.y;
        sprites[PLAYER2].dstrect.x += green->effect2*purple->effect2*sprites[PLAYER2].d.x;
        sprites[PLAYER2].dstrect.y += green->effect2*purple->effect2*sprites[PLAYER2].d.y;
  for (int i = BALL; i < PLAYER2+1; i++)
  {


    if (sprites[i].dstrect.x < 0)
    {
      sprites[i].dstrect.x = 0;
    }

    if (sprites[i].dstrect.x > WINDOW_WIDTH - ANIMATION_FRAME_WIDTH)
    {
      sprites[i].dstrect.x = WINDOW_WIDTH - ANIMATION_FRAME_WIDTH;
    }

    if (sprites[i].dstrect.y >= WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT)
    {
    sprites[i].dstrect.y = WINDOW_HEIGHT - ANIMATION_FRAME_HEIGHT;
    }

    if (sprites[i].dstrect.y < 0)
    {
      sprites[i].dstrect.y = 0;
    }
    if(i==PLAYER1)
    {
        if(sprites[PLAYER1].dstrect.x>=260)
            sprites[PLAYER1].dstrect.x=260;
    }
    else if(i==PLAYER2)
    {
        if(sprites[PLAYER2].dstrect.x<=320)
            sprites[PLAYER2].dstrect.x=320;
    }

  }
}



void render(SDL_Renderer *renderer, Sprite *sprites,Points points,Background bg,Tool green,ToolB purple)
{
  const SDL_Point *center = NULL;
  const SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Render the background
    SDL_RenderClear(renderer);

    bg.render(renderer);
    purple.settool(renderer);
        green.settool(renderer);
    
  for (int i = BALL; i < NET+1; i++)
  {

  SDL_RenderCopyEx(
    renderer, sprites[i].texture,
    &sprites[i].srcrect,
    &sprites[i].dstrect,
    sprites[i].angle,
    center,
    flip);

  }

    points.render(renderer);

  SDL_RenderPresent(renderer);
    
}
 

void show_score(SDL_Renderer *renderer,unsigned int *score,Sprite* sprites)
{
    if (TTF_Init() == -1) {
        SDL_Log("Unable to initialize TTF: %s\n", TTF_GetError());
        exit(1);
    }
    TTF_Font* font = TTF_OpenFont("ttf/show.ttf", 50);
    if (font == NULL) {
        SDL_Log("Unable to load font: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color color = {0, 0, 0, 255}; // White

    // Create a surface for the text
    char score_str[50];

    // Format the string
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    sprintf(score_str, "PLAYER%u scored!", score[BALL]);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font,score_str, color);
    if (text_surface == NULL) {
        SDL_Log("Unable to render text surface: %s\n", TTF_GetError());
        exit(1);
    }

    // Create a rectangle for the text
    SDL_Rect text_rect = {100, 100, text_surface->w, text_surface->h};

    // Create a texture for the text
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (text_texture == NULL) {
        SDL_Log("Unable to create texture from rendered text: %s\n", SDL_GetError());
        exit(1);
    }

    // Render the text
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    int i=score[BALL];
    SDL_RenderCopy(renderer, sprites[i].texture,&sprites[i].srcrect,&sprites[i].dstrect);
                   
    // Clean up
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    SDL_RenderPresent(renderer);
}




bool again(SDL_Renderer *renderer,bool running){
    //SDL_RenderClear(renderer);
    Mix_PlayChannel(-1, gButton, 0);
    SDL_Rect playAgainButton = { 120, 100, 400, 100 };
    SDL_Rect exitButton = { 220, 300, 200, 100 };
    
    // Main game loop
    
   
                int x, y;
                SDL_GetMouseState(&x, &y);
    
        // Check if the Play Again button was clicked
        if (x >= playAgainButton.x && x <= playAgainButton.x + playAgainButton.w &&
            y >= playAgainButton.y && y <= playAgainButton.y + playAgainButton.h) {
            // Restart the game
            return 1;
        }
        
        // Check if the Exit button was clicked
        else if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
                 y >= exitButton.y && y <= exitButton.y + exitButton.h) {
            return 0;
            
        }
    
                
    

    return 0;
}
void load_again(SDL_Renderer* renderer){
    
    SDL_Surface* buttonSurface = SDL_LoadBMP("image/again.bmp");
    if (buttonSurface == NULL) {
        SDL_Log("Unable to load image! SDL Error: %s\n", SDL_GetError());
        // handle error
    }
    
  // Load the font


  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

// Free the surface

  SDL_Rect playAgainButton = { 120, 100, 400, 100 };
  SDL_Rect exitButton = { 220, 300, 200, 100 };
 SDL_RenderCopy(renderer, textTexture, NULL, &playAgainButton);
    SDL_FreeSurface(buttonSurface);

// Create the text texture
    buttonSurface = SDL_LoadBMP("image/exit.bmp");
    if (buttonSurface == NULL) {
        SDL_Log("Unable to load image! SDL Error: %s\n", SDL_GetError());
        // handle error
    }
textTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

// Free the surface

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  //SDL_RenderDrawRect(renderer, &exitButton);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(renderer, textTexture, NULL, &exitButton);
    SDL_DestroyTexture(textTexture);

}
void win(SDL_Renderer* renderer,int player){
  SDL_Surface* buttonSurface = NULL;
    SDL_RenderClear(renderer);
  if(player==1){
    buttonSurface = SDL_LoadBMP("image/win1.bmp");
  }
  else{
    buttonSurface = SDL_LoadBMP("image/win2.bmp");
  }

  if (buttonSurface == NULL) {
    SDL_Log("Unable to load image! SDL Error: %s\n", SDL_GetError());
    // handle error
  } else {
    // Create a texture from the surface
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    if (buttonTexture == NULL) {
      SDL_Log("Unable to create texture! SDL Error: %s\n", SDL_GetError());
      // handle error
    } else {
      // Render the texture
      SDL_RenderCopy(renderer, buttonTexture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
      //SDL_Delay(5000);
    // Clean up
    SDL_FreeSurface(buttonSurface);
    SDL_DestroyTexture(buttonTexture);
  }
}

void player_gotG(Sprite *sprites,Tool* green){
    if(green->exist==1){
    
    if(abs(sprites[PLAYER1].dstrect.x-290)<40){
        green->exist=0;
        green->player=1;
        green->effect2=0.5;
    }
    else if(abs(sprites[PLAYER2].dstrect.x-290)<40){
        green->exist=0;
        green->player=2;
        green->effect1=0.5;
    }
    }
}
void player_gotP(Sprite *sprites,ToolB* purple){
    if(purple->exist==1){
    
    if(abs(sprites[PLAYER1].dstrect.x-purple->x)<20&&abs(sprites[PLAYER1].dstrect.y-purple->y)<20){
        purple->exist=0;
        purple->player=1;
        purple->effect1=2;
    }
    else if(abs(sprites[PLAYER2].dstrect.x-purple->x)<20&&abs(sprites[PLAYER2].dstrect.y-purple->y)<20){
        purple->exist=0;
        purple->player=2;
        purple->effect2=2;
    }
    }
}
 
int main(int argc, char **argv)
{
loadMedia();
    
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  Sprite sprites[4];
  unsigned int score[3];
  unsigned int is_npc = 0;
  //Point points[3];


    

  score[BALL] = 0; /**< last player who scored */
  score[PLAYER1] = 0;
  score[PLAYER2] = 0;


  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

  window = SDL_CreateWindow(
    "NTU Volleyball",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_OPENGL);


  renderer = SDL_CreateRenderer(
    window,
    -1,
   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);



    Background bg[4];
    bg[0]=Background(renderer,"image/volleyball.jpg");
    for(int i=1;i<5;i++){
        char a[50];sprintf(a, "image/%i.png",i);
        bg[i]=Background(renderer,a);}
    Points points(renderer);
    //points.construct(renderer);
    
    
    
    Tool green;
    
        ToolB purple;
        purple.exist=0;
        green.exist=0;
     

    // Main game loop
    bool running=1;
/* <-- GAME LOOP */
    //Mix_PlayMusic( gMusic, -1 );
   
    
    
    
    
    

    while(running){
        Open open;
        Mix_PlayMusic( gMain, -1 );
        open.start(renderer,bg);
        Mix_HaltMusic();
        load_sprites(renderer, sprites,open);
         
        place_sprites_on_start(sprites, PLAYER1);
        Mix_HaltMusic();
        Mix_PlayMusic( gMusic, -1 );
        runagain=0;
        for(int i=PLAYER1;i<PLAYER2+1;i++) score[i]=0;
        int point = 0;
        Uint32 timeout = 0;
        int times=0;
        int tool=1;
        
        while(!process_events(&is_npc))
        {
          
            if(score[1]==5||score[2]==5) {
                times++;
                if(times==1){
                    Mix_HaltMusic();
                Mix_PlayMusic( gWin, -1 );}
                if(times<=200){
                    win(renderer,score[BALL]);}
                
                else{
                    SDL_RenderClear(renderer);
                    bg[0].render(renderer);
                    
                    load_again(renderer);
                    
                    SDL_RenderPresent(renderer);
                    
                    if(runagain==1){ running=again(renderer,running); break;}
                }
            }
            else{
                if(!ENDTURN){
                    control_player(sprites);
                    control_oponent(sprites, is_npc);
                    point = bounce_ball(&sprites[BALL]);
                    
                }
                if (point && !ENDTURN)
                {
                    //Mix_HaltMusic();
                    Mix_PlayChannel( -1, gScore, 0 );
                    score[point] += 1;
                    score[BALL] = point;
                    points.add_point(point);
                    ENDTURN = 1;
                    show_score(renderer,score,sprites);
                    timeout = SDL_GetTicks() + 3000;
                    
                }
                
                if(!point){
                    
                    //Mix_PlayMusic( gMusic, -1 );
                    hit_net(sprites);
                    hit_ball(sprites);
                    apply_gravity(sprites);
                    apply_delta(sprites,&green,&purple);
                    animate_players(sprites);
                    animate_ball(&sprites[BALL]);
                    Uint32 currentTime = SDL_GetTicks();
                    if((currentTime%543==0)&&(purple.player==0)) purple.exist=1;
                    if((currentTime%987==0)&&(green.player==0)) green.exist=1;
                    player_gotG(sprites,&green);
                    player_gotP(sprites,&purple);
                    render(renderer, sprites,points,bg[open.arr[4]],green,purple);
                    
                    
                    
                  
                }
                if (ENDTURN && SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
                {
                    tool=1;
                    SDL_Log("Player %i: %i -- Player %i: %i\n", PLAYER1, score[PLAYER1], PLAYER2, score[PLAYER2]);
                    place_sprites_on_start(sprites, score[BALL]);
                    green.tool_reset();
                    purple.rand_xy();
                    purple.tool_reset();
                    ENDTURN = 0;
                }
            }
        }
        
    }
/* GAME LOOP -->*/

    for(int i=0;i<5;i++) bg[i].destruct();
  for (int i = BALL; i < NET+1; i++)
  {
    SDL_DestroyTexture(sprites[i].texture);
  }
    //SDL_FreeSurface(bg_surface);
    
    //Free the sound effects
    Mix_FreeChunk( gBall );
    Mix_FreeChunk( gButton );
    Mix_FreeChunk( gScore );
    Mix_FreeChunk( gJump );
    Mix_FreeMusic( gMusic );
    Mix_FreeMusic( gWin );
    Mix_FreeMusic( gMain );
    gMusic = NULL;
    gBall = NULL;
    gButton = NULL;
    gScore = NULL;
    gJump = NULL;
    gWin=NULL;
    gMain=NULL;
    Mix_Quit();
    
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
