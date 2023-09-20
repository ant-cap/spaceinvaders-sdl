#ifndef destroyed_hpp
#define destroyed_hpp

#include "SDL.h"
#include <unordered_set>

class DestroyedPixels
{
private:
	std::unordered_set<int> indexes;
public:
	DestroyedPixels() {}
	~DestroyedPixels() {}

	void addPixel(int p) { indexes.insert(p); }
	void draw(SDL_Surface* screen);

};

#endif
