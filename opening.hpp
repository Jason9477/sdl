//
//  opening.hpp
//  sdl2
//
//  Created by Jason Hsieh on 2023/12/23.
//



#ifndef opening_hpp
#define opening_hpp
#include <stdio.h>
#include "Background.hpp"

class Open:public Background{
public:
    void start(SDL_Renderer*,Background*);
    int arr[5];

private:

};



#endif
