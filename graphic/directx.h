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
#  include <GL/gl.h>
typedef uint32_t ArgbColor;
typedef void* RenderingSysPtr; // OpenGLではたぶん未使用
typedef void* DevicePtr; // OpenGLでは未使用
typedef GLuint TexturePtr;
typedef GLfloat TransformMatrix;
typedef void* SpritePtr; // OpenGLではたぶん未使用
typedef void* LinePtr; // OpenGLではたぶん未使用
#endif /* defined(_WIN32) && defined(WITH_DIRECTX) */
