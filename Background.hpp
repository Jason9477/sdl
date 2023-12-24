
#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL.h>

//#include <SDL2/SDL_image.h>
class Background {
public:
    Background();
    Background(SDL_Renderer* renderer,char *);
    ~Background();
    void destruct() ;
    void render(SDL_Renderer* renderer) ;
protected:
   // SDL_Renderer* renderer;
    
    SDL_Surface* surface;
    SDL_Texture* texture;
};
#endif
