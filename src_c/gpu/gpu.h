/*
 * GPU module
 */

#ifndef GPU_H
#define GPU_H

#include "pygame.h"

#include "pgcompat.h"

#include <SDL3_shadercross/SDL_shadercross.h>

typedef struct {
    float x, y, z;
} PositionVertex;

typedef struct {
    float x, y, z;
    Uint8 r, g, b, a;
} PositionColorVertex;

typedef struct {
    float x, y, z;
    float u, v;
} PositionTextureVertex;

typedef enum BufferType {
    POSITION_VERTEX,
    POSITION_COLOR_VERTEX,
    POSITION_TEXTURE_VERTEX
} BufferType;

typedef enum UniformDataType {
    PUSH_VERTEX,
    PUSH_FRAGMENT,
    PUSH_UNIFORM
} UniformDataType;

typedef struct {
    PyObject_HEAD SDL_GPUBuffer *buffer;
    SDL_GPUBufferUsageFlags usage;
    BufferType buffer_type;
    int no_of_elements;
    SDL_GPUIndexElementSize index_element_size;
} pgBufferObject;

typedef struct {
    PyObject_HEAD SDL_GPUTexture *texture;
    int width;
    int height;
    int is_swapchain;  // 0 = user-created (released on dealloc), 1 = swapchain (not released)
    SDL_GPUTextureCreateInfo texture_info;
} pgGPUTextureObject;

typedef struct {
    PyObject_HEAD SDL_GPUShader *shader;
} pgShaderObject;

typedef struct {
    PyObject_HEAD SDL_GPURenderPass *render_pass;
    SDL_GPUColorTargetInfo color_info;
} pgRenderPassObject;

typedef struct {
    PyObject_HEAD SDL_GPUGraphicsPipeline *pipeline;
    SDL_GPUGraphicsPipelineCreateInfo pipeline_info;
} pgPipelineObject;

typedef struct {
    PyObject_HEAD SDL_GPUSampler *sampler;
    SDL_GPUSamplerCreateInfo sampler_info;
} pgSamplerObject;

typedef struct {
    PyObject_HEAD SDL_GPUComputePipeline *pipeline;
} pgComputePipelineObject;

typedef struct {
    PyObject_HEAD SDL_GPUComputePass *compute_pass;
} pgComputePassObject;

typedef struct {
    PyObject_HEAD SDL_GPUCopyPass *copy_pass;
} pgCopyPassObject;

typedef struct {
    PyObject_HEAD SDL_GPUTransferBuffer *transfer_buffer;
    Uint32 size;
} pgTransferBufferObject;

typedef struct {
    PyObject_HEAD SDL_GPUFence *fence;
} pgFenceObject;

typedef struct {
    PyObject_HEAD
    SDL_GPUDepthStencilState state;
} pgDepthStencilStateObject;

typedef struct {
    PyObject_HEAD
    Uint32 pitch;
    Uint32 num_attributes;
    SDL_GPUVertexAttribute *attributes;
} pgVertexInputStateObject;

typedef struct {
    PyObject_HEAD
    pgGPUTextureObject *texture;
    Uint32 mip_level;
    Uint32 layer;
    Uint32 x, y, z;
    Uint32 w, h, d;
} pgTextureRegionObject;

int gpu_register_constants(PyObject *module);

#endif /* GPU_H */
