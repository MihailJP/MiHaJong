#ifndef GRAPHIC_GEOMETRY_H
#define GRAPHIC_GEOMETRY_H

namespace mihajong_graphic {

class Geometry {
public:
	static const unsigned BaseSize = 1080;
	static unsigned WindowWidth;
	static unsigned WindowHeight;
	static float WindowScale() {return (float)WindowHeight / (float)BaseSize;}
	static unsigned SidebarWidth() {return (WindowWidth - WindowHeight) / WindowScale() ;}
};

}
#endif
