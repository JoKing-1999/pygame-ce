#ifndef SURFACE_CORE_H
#define SURFACE_CORE_H

#include "pg_export.h"

struct SDL_Surface;
struct pgSubSurface_Data;

typedef struct pgSurfaceObject {
    PyObject_HEAD struct SDL_Surface *surf;
    int owner;
    struct pgSubSurface_Data *subsurface;
    PyObject *weakreflist;
    PyObject *locklist;
    PyObject *dependency;
} pgSurfaceObject;

#define pgSurface_AsSurface(x) (((pgSurfaceObject *)x)->surf)

PG_CORE_API extern PyTypeObject pgSurface_Type;

PG_CORE_API pgSurfaceObject *
pgSurface_New2(SDL_Surface *info, int owner);

PG_CORE_API int
pgSurface_SetSurface(pgSurfaceObject *self, SDL_Surface *s, int owner);

PG_CORE_API int
pgSurface_Blit(pgSurfaceObject *dstobj, pgSurfaceObject *srcobj,
               SDL_Rect *dstrect, SDL_Rect *srcrect, int blend_flags);

static inline int
pgSurface_Check(PyObject *obj)
{
    return PyObject_IsInstance(obj, (PyObject *)&pgSurface_Type);
}

#define pgSurface_New(s) pgSurface_New2((s), 1)
#define pgSurface_NewNoOwn(s) pgSurface_New2((s), 0)

PG_CORE_API void
pgSurface_Prep(pgSurfaceObject *surfobj);

PG_CORE_API void
pgSurface_Unprep(pgSurfaceObject *surfobj);

PG_CORE_API int
pgSurface_Lock(pgSurfaceObject *surfobj);

PG_CORE_API int
pgSurface_Unlock(pgSurfaceObject *surfobj);

PG_CORE_API int
pgSurface_LockBy(pgSurfaceObject *surfobj, PyObject *lockobj);

PG_CORE_API int
pgSurface_UnlockBy(pgSurfaceObject *surfobj, PyObject *lockobj);

#endif /* ~SURFACE_CORE_H */
