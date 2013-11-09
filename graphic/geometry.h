#pragma once

namespace mihajong_graphic {

class Geometry {
public:
	static const unsigned BaseSize = 1080;
	static unsigned WindowWidth;
	static unsigned WindowHeight;
	static float WindowScale() {return (float)WindowHeight / (float)BaseSize;}
	static unsigned SidebarWidth() {return (unsigned)((WindowWidth - WindowHeight) / WindowScale());}
public: /* Monostate class: cannot be instantiated */
	Geometry() = delete;
	Geometry(const Geometry&) = delete;
	Geometry& operator= (const Geometry&) = delete;
	~Geometry() = delete;
};

}
