#pragma once

namespace mihajong_graphic {

class Geometry {
public:
	static const unsigned BaseSize = 1080;
	static unsigned WindowWidth;
	static unsigned WindowHeight;
	static float WindowScale() {return static_cast<float>(WindowHeight) / static_cast<float>(BaseSize);}
	static unsigned SidebarWidth() {return static_cast<unsigned>(static_cast<float>(WindowWidth - WindowHeight) / WindowScale());}
public: /* Monostate class: cannot be instantiated */
	Geometry() = delete;
	Geometry(const Geometry&) = delete;
	Geometry& operator= (const Geometry&) = delete;
	~Geometry() = delete;
};

}
