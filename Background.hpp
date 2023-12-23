#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Background {
public:
    Background();
    Background(SDL_Renderer* renderer,char *);
    //~Background();
    void destruct() const;
    void render(SDL_Renderer* renderer) const;
private:
   // SDL_Renderer* renderer;
    
    SDL_Surface* bg_surface;
    SDL_Texture* bg_texture;
};
