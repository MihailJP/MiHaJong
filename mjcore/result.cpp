#include "result.h"
#include "../graphic/graphic.h"

void gameResult(const GameTable* gameStat, int origTurn, int origHonba) {
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	/* TODO: �I�����̏��� */
	/* TODO: ���͑҂�
	buttonPressed = 0
	repeat
		redrscreen
		await 1000
		if (buttonPressed) {break}
	loop
	*/
	Sleep(INFINITE);
}
