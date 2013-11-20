#pragma once

#if defined(_WIN32) && defined(WITH_DIRECTX)
#  ifdef _MSC_VER
#    include <d3dx9.h>
#    include <dxerr.h>
#  else
#    include <windows.h>
#    ifndef DECLARE_INTERFACE_IID_
#      define DECLARE_INTERFACE_IID_(i,b,d) DECLARE_INTERFACE_(i,b)
#    endif
#    include <d3dx9.h>
#    include <dxerr9.h>
#  endif /* _MSC_VER */
typedef D3DCOLOR ArgbColor;
typedef LPDIRECT3D9 RenderingSysPtr;
typedef LPDIRECT3DDEVICE9 DevicePtr;
typedef LPDIRECT3DTEXTURE9 TexturePtr;
typedef D3DXMATRIX TransformMatrix;
typedef LPD3DXSPRITE SpritePtr;
typedef LPD3DXLINE LinePtr;
#else
#  include <cstdint>
#  include <array>
#  include <GL/gl.h>
#  ifndef _WIN32
#    include <GL/glx.h>
#  endif /* _WIN32 */
typedef uint32_t ArgbColor;
typedef void* RenderingSysPtr; // OpenGLではたぶん未使用
#  ifdef _WIN32
typedef HDC DevicePtr;
#  else /* _WIN32 */
typedef GLXContext DevicePtr;
#  endif /* _WIN32 */
typedef GLuint TexturePtr;
typedef std::array<GLfloat, 16> TransformMatrix;
typedef void* SpritePtr; // OpenGLではたぶん未使用
typedef void* LinePtr; // OpenGLではたぶん未使用
#  ifndef _WIN32
#    define MAKEINTRESOURCE(r) ((LPTSTR)r)
struct RECT {int left, top, right, bottom;};
#  endif /* _WIN32 */
#endif /* defined(_WIN32) && defined(WITH_DIRECTX) */
