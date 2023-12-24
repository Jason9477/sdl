//#include <SDL.h>
//#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Tool {
public:
	bool exist;
	int player;
	double effect1,effect2;
    Tool();
   	void settool(SDL_Renderer*);
    //void playergot(int);
    void tool_reset();
	
    ~Tool();
    int x;
	int y;
protected:

    SDL_Surface* imageSurface;
    SDL_Texture* texture;
private:

	int got_times;
	
};

 
 class ToolB : public Tool{
 public:
 ToolB();
 void rand_xy();
 private:
 
 
 };
 
