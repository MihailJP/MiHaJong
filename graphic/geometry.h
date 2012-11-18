#ifndef GRAPHIC_GEOMETRY_H
#define GRAPHIC_GEOMETRY_H

class Geometry {
public:
	static const unsigned BaseSize = 1200;
	static unsigned WindowWidth;
	static unsigned WindowHeight;
	static float WindowScale() {return (float)WindowHeight / (float)BaseSize;}
};

#endif
