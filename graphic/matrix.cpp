#include "matrix.h"
#include "geometry.h"
#include "pi.h"

namespace mihajong_graphic {

TransformMatrix getMatrix(float baseX, float baseY, float scaleX, float scaleY, float rotateAngle, float rotateOffsetX, float rotateOffsetY, float scaleAfterX, float scaleAfterY) {
	TransformMatrix matrix;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix matrix1;
	D3DXMatrixIdentity(&matrix);
	D3DXMatrixTranslation(&matrix1, -baseX, -baseY, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scaleX, scaleY, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, -rotateOffsetX * scaleX, -rotateOffsetY * scaleY, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixRotationZ(&matrix1, rotateAngle); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, rotateOffsetX * scaleX, rotateOffsetY * scaleY, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, baseX, baseY, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scaleAfterX * Geometry::WindowScale(), scaleAfterY * Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
#else
	/* DirectXとOpenGLだと座標原点が違う */
	/* しかも行列を指定する順番が逆 */
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, static_cast<float>(Geometry::WindowHeight), 0.0f);
	glScalef(scaleAfterX * Geometry::WindowScale(), scaleAfterY * Geometry::WindowScale(), 1.0f);
	glTranslatef(baseX, -baseY, 0.0f);
	glTranslatef(rotateOffsetX * scaleX, -rotateOffsetY * scaleY, 0.0f);
	glRotatef(-rotateAngle * 180.0f / static_cast<float>(Pi), 0.0f, 0.0f, 1.0f);
	glTranslatef(-rotateOffsetX * scaleX, rotateOffsetY * scaleY, 0.0f);
	glScalef(scaleX, scaleY, 1.0f);
	glTranslatef(-baseX, baseY, 0.0f);
	glTranslatef(0.0f, -static_cast<float>(Geometry::WindowHeight), 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
	glPopMatrix();
#endif
	return matrix;
}

}
