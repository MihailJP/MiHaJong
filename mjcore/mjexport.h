#ifndef MJEXPORT_H
#define MJEXPORT_H

#ifdef MJCORE_EXPORTS
#define MJCORE __declspec(dllexport)
#define MJCEXTERN
#else
#define MJCORE __declspec(dllimport)
#define MJCEXTERN extern
#endif /* MJCORE_EXPORTS */

#define EXPORT_STRUCT MJCEXTERN struct MJCORE

#endif /* MJEXPORT_H */
