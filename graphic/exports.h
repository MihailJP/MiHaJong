#ifndef GRAPHIC_EXPORTS_H
#define GRAPHIC_EXPORTS_H

#ifdef GRAPHIC_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#endif
