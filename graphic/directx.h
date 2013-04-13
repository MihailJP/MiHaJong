#pragma once

#if defined(_WIN32) && defined(WITH_DIRECTX)
#ifdef _MSC_VER
#  include <d3dx9.h>
#  include <dxerr.h>
#else
#  include <windows.h>
#  ifndef DECLARE_INTERFACE_IID_
#    define DECLARE_INTERFACE_IID_(i,b,d) DECLARE_INTERFACE_(i,b)
#  endif
#  include <d3dx9.h>
#  include <dxerr9.h>
#endif /* _MSC_VER */
#else
#include <GL/gl.h>
#endif /* defined(_WIN32) && defined(WITH_DIRECTX) */
