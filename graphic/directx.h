#pragma once

#ifdef _MSC_VER
#  include <d3dx9.h>
#  include <dxerr.h>
#else
#  include <Windows.h>
#  ifndef DECLARE_INTERFACE_IID_
#    define DECLARE_INTERFACE_IID_(i,b,d) DECLARE_INTERFACE_(i,b)
#  endif
#  include <d3dx9.h>
#  include <dxerr9.h>
#endif
