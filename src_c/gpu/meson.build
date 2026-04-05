gpu = py.extension_module(
    'gpu',
    'gpu.c',
    c_args: warnings_error,
    dependencies: pg_base_deps,
    include_directories: '..',
    install: true,
    subdir: pg,
)
