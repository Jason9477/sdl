//
//  Points.hpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/24.
//

#ifndef Points_hpp
#define Points_hpp

#include <stdio.h>
#include "Point.hpp"
#include "Points.hpp"
class Points{
    public:
    Points(SDL_Renderer* renderer);
    
    void add_point(int player);
    void render(SDL_Renderer* renderer);
    
private:
    Point apoint;
    Point bpoint;
};
#endif /* Points_hpp */
