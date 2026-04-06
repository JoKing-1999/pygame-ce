gpu = py.extension_module(
    'gpu',
    [
        'gpu.c',
        'gpu_constants.c',
    ],
    c_args: warnings_error,
    dependencies: pg_base_deps + [sdl_shadercross_dep],
    include_directories: '..',
    install: true,
    subdir: pg,
)
