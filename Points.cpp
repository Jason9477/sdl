//
//  Points.cpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/24.
//

#include "Points.hpp"

    Points::Points(SDL_Renderer* renderer):apoint(renderer,"ttf/point.ttf"),bpoint(apoint){
        apoint.set_xpos(10);
        bpoint.set_xpos(580);
    }
void Points::reset(){
    apoint.reset();
    bpoint.reset();
}
    void Points::add_point(int player){
        if(player==1) ++apoint;
        else ++bpoint;
    }
void Points::render(SDL_Renderer* renderer){
    apoint.render(renderer);
    bpoint.render(renderer);
}
