#include <SDL.h>
#include <SDL_image.h>


class Tool {
public:
	bool exist;
	int player;
	double effect1,effect2;
    Tool();
   	void settool(SDL_Renderer*);
    void playergot(int);
    void tool_reset();
	SDL_Surface* imageSurface;
    SDL_Texture* texture;
    ~Tool(); 
	int x;
	int y;
protected:

    
private:

	int got_times;
	
};

class ToolB : public Tool{
	public:
		ToolB();
		void rand_xy();
	private:
		
	
};
