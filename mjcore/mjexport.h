#ifndef MJEXPORT_H
#define MJEXPORT_H

#ifdef IMPORT_MJCORE
#define MJCORE __declspec(dllimport)
#define MJCEXTERN extern
#else
#define MJCORE __declspec(dllexport)
#define MJCEXTERN
#endif /* IMPORT_MJCORE */

#define EXPORT_CLASS MJCEXTERN class MJCORE
#define EXPORT_STRUCT MJCEXTERN struct MJCORE
#define INSTANTIATE MJCEXTERN template class MJCORE

#endif /* MJEXPORT_H */
