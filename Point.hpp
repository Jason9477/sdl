//
//  Point.hpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/23.
//

#ifndef POINT_HPP
#define POINT_HPP
#include <SDL2/SDL_ttf.h>
#include "Background.hpp"
//#include <SDL.h>

//#include <SDL_image.h>
class Point: public Background{
public:
    
    
    Point(SDL_Renderer* renderer,const char* path);
    
    Point(const Point& old_point); // copy constructor
    ~Point();
    
    Point& operator++() ;
    void render(SDL_Renderer* renderer);
    
    void set_xpos(int );
private:
    TTF_Font *font;
    
    SDL_Rect text_rect;
    int score=0;
    char score_char[10];
    const SDL_Color color = {255, 255, 255,255};

    
};
#endif
