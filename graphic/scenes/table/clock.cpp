#include "clock.h"

namespace mihajong_graphic {

TableProtoScene::Clock::Clock(TableProtoScene* caller) {
	parent = caller;
}

TableProtoScene::Clock::~Clock() {
}

void TableProtoScene::Clock::Render() {
}

}
