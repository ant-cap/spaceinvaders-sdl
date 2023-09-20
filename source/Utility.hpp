#ifndef utility_hpp
#define utility_hpp

#include <stdio.h>
#include <stdlib.h>

struct XY {
	int x, y;
	XY() { x = 0; y = 0; }
	XY(int X, int Y) { x = X; y = Y; }
};

#endif
