#ifndef PG_EXPORT_H
#define PG_EXPORT_H

#if defined(BUILD_STATIC)
#define PG_CORE_EXPORT
#define PG_CORE_IMPORT
#elif defined(_WIN32)
#define PG_CORE_EXPORT __declspec(dllexport)
#define PG_CORE_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define PG_CORE_EXPORT __attribute__((visibility("default")))
#define PG_CORE_IMPORT __attribute__((visibility("default")))
#else
#define PG_CORE_EXPORT
#define PG_CORE_IMPORT
#endif

#ifdef PG_CORE_BUILD
#define PG_CORE_API PG_CORE_EXPORT
#else
#define PG_CORE_API PG_CORE_IMPORT
#endif

#endif /* ~PG_EXPORT_H */
