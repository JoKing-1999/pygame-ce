#define PYGAMEAPI_GPU_INTERNAL

#include "pygame.h"

#include "pgcompat.h"

/* Context */
static SDL_GPUDevice *device;

static SDL_GPUCommandBuffer *cmdbuf = NULL;

static int active_render_passes = 0;

static int swapchain_acquired = 0;

/* Types */
static PyTypeObject pgShader_Type;

static PyTypeObject pgRenderPass_Type;

static PyTypeObject pgPipeline_Type;

static PyTypeObject pgBuffer_Type;

static PyTypeObject pgGPUTexture_Type;

static PyTypeObject pgSampler_Type;

static PyTypeObject pgComputePipeline_Type;

static PyTypeObject pgComputePass_Type;

static PyTypeObject pgCopyPass_Type;

static PyTypeObject pgTransferBuffer_Type;

static PyTypeObject pgFence_Type;

#define pgShader_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgShader_Type))

#define pgRenderPass_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgRenderPass_Type))

#define pgPipeline_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgPipeline_Type))

#define pgBuffer_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgBuffer_Type))

#define pgGPUTexture_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgGPUTexture_Type))

#define pgSampler_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgSampler_Type))

#define pgComputePipeline_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgComputePipeline_Type))

#define pgComputePass_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgComputePass_Type))

#define pgCopyPass_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgCopyPass_Type))

#define pgTransferBuffer_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgTransferBuffer_Type))

#define pgFence_Check(x) \
    (PyObject_IsInstance((x), (PyObject *)&pgFence_Type))

#define DEC_CONSTS_(x, y)                           \
    if (PyModule_AddIntConstant(module, x, (int)y)) \
    {                                               \
        Py_DECREF(module);                          \
        return NULL;                                \
    }
#define DEC_CONST(x) DEC_CONSTS_(#x, SDL_##x)

static int
acquire_command_buffer()
{
    if (cmdbuf == NULL) {
        cmdbuf = SDL_AcquireGPUCommandBuffer(device);
        if (cmdbuf == NULL) {
            RAISE(pgExc_SDLError, SDL_GetError());
            return 0;
        }
    }
    return 1;
}

/* Shader implementation */
static int
shader_init(pgShaderObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject* file;
    SDL_RWops *rw = NULL;
    int stage, samplers = 0, uniform_buffers = 0, storage_buffers = 0, storage_textures = 0;
    char *keywords[] = {"file", "stage", "samplers", "uniform_buffers", "storage_buffers", "storage_textures", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oi|iiii", keywords,
                                     &file, &stage, &samplers,
                                     &uniform_buffers, &storage_buffers, &storage_textures)) {
        return -1;
    }
    if (device == NULL) {
        RAISERETURN(pgExc_SDLError, "gpu module hasn't been initialized!", -1)
    }
    rw = pgRWops_FromObject(file, NULL);
    if (rw == NULL) {
        RAISERETURN(pgExc_SDLError, "Unable to read file", -1);
    }
    size_t code_size = SDL_GetIOSize(rw);
    Uint8 *code = (Uint8 *)malloc(code_size);
    SDL_ReadIO(rw, code, code_size);
    SDL_CloseIO(rw);

    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    const char *entrypoint;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    } else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
        format = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    } else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	} else {
        free(code);
        RAISERETURN(pgExc_SDLError, "Unrecognized backend shader format!", -1);
	}
	SDL_GPUShaderCreateInfo shaderInfo = {
		.code = code,
		.code_size = code_size,
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = samplers,
		.num_uniform_buffers = uniform_buffers,
		.num_storage_buffers = storage_buffers,
		.num_storage_textures = storage_textures
	};
    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
    free(code);
    if (shader == NULL) {
        PyErr_SetString(pgExc_SDLError, SDL_GetError());
        return -1;
    }
    self->shader = shader;
    return 0;
}

static void
shader_dealloc(pgShaderObject *self, PyObject *_null)
{
    if (device != NULL && self->shader) {
        SDL_ReleaseGPUShader(device, self->shader);
    }
    Py_TYPE(self)->tp_free(self);
}

/* RenderPass implementation */
static PyObject *
render_pass_begin(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgWindowObject *window = NULL;
    pgGPUTextureObject *texture = NULL;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
    Uint32 layer = 0;                                                                                                                                                                                                                                                                                                  
    int cycle = 0;
    SDL_GPUTexture* swapchainTexture;                                                                                                                                                                                                                                                                                                  
    char *keywords[] = {"window", "texture", "layer", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|O!O!ip", keywords,
                                     &pgWindow_Type, &window, &pgGPUTexture_Type, &texture, &layer, &cycle)) {
        return NULL;
    }
    if (window == NULL && texture == NULL) {                                                                                                                                                                                                                                                                        
        return RAISE(pgExc_SDLError, "Must provide either window or texture");                                                                                                                                                                                                                                      
    }
    if (active_render_passes) {
        return RAISE(pgExc_SDLError, "You must end old render pass before starting new one");
    }
    if (!acquire_command_buffer()) {
        return NULL;
    }
    if (texture != NULL) {
        self->color_info.texture = texture->texture;
        self->color_info.layer_or_depth_plane = layer;
        self->color_info.cycle = cycle;
        if (self->render_pass == NULL) {
            self->render_pass = SDL_BeginGPURenderPass(cmdbuf, &self->color_info, 1, NULL);
            active_render_passes++;
        }
    }
    else {
        if (swapchain_acquired) {
            return RAISE(pgExc_SDLError, "Swapchain already acquired via acquire_swapchain_texture. Use texture= instead of window=");
        }
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, window->_win, &swapchainTexture, NULL, NULL)) {
            return RAISE(pgExc_SDLError, SDL_GetError());
        }
        if (swapchainTexture != NULL) {
            self->color_info.texture = swapchainTexture;
            if (self->render_pass == NULL) {
                self->render_pass = SDL_BeginGPURenderPass(cmdbuf, &self->color_info, 1, NULL);
                active_render_passes++;
            }
        }
    }
    Py_RETURN_NONE;
}

static PyObject *
render_pass_end(pgRenderPassObject *self, PyObject *_null)
{
    if (self->render_pass != NULL) {
        SDL_EndGPURenderPass(self->render_pass);
        active_render_passes--;
        self->render_pass = NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *
render_pass_draw_primitives(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    int vertices, instances, vertex_offset = 0, indexed = 0, index_offset;
    char *keywords[] = {"vertices", "instances", "vertex_offset", "indexed", "index_offset", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|ipi", keywords,
                                     &vertices, &instances, &vertex_offset, &indexed, &index_offset)) {
        return NULL;
    }
    if (indexed) {
        SDL_DrawGPUIndexedPrimitives(self->render_pass, vertices, instances, index_offset, vertex_offset, 0);
    }
    else {
        SDL_DrawGPUPrimitives(self->render_pass, vertices, instances, vertex_offset, 0);
    }
    Py_RETURN_NONE;
}

static PyObject *
render_pass_draw_primitives_indirect(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgBufferObject *buffer;
    Uint32 offset = 0, draw_count = 1;
    int indexed = 0;
    char *keywords[] = {"buffer", "offset", "draw_count", "indexed", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!|IIp", keywords,
                                     &pgBuffer_Type, &buffer, &offset, &draw_count, &indexed)) {
        return NULL;
    }
    if (indexed) {
        SDL_DrawGPUIndexedPrimitivesIndirect(self->render_pass, buffer->buffer, offset, draw_count);
    }
    else {
        SDL_DrawGPUPrimitivesIndirect(self->render_pass, buffer->buffer, offset, draw_count);
    }
    Py_RETURN_NONE;
}

static PyObject *
render_pass_set_viewport(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    float x, y, w, h, min_depth = 0, max_depth = 0;
    char *keywords[] = {"x", "y", "w", "h", "min_depth", "max_depth", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ffff|ff", keywords,
                                     &x, &y, &w, &h, &min_depth, &max_depth)) {
        return NULL;
    }
    SDL_SetGPUViewport(self->render_pass, (SDL_GPUViewport[]){x, y, w, h, min_depth, max_depth});
    Py_RETURN_NONE;
}

static PyObject *
render_pass_set_scissor(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *rectobj;
    SDL_Rect *rect = NULL, temp;
    char *keywords[] = {"rect", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", keywords,
                                     &rectobj)) {
        return NULL;
    }
    rect = pgRect_FromObject(rectobj, &temp);
    if (!rect) {
        return RAISE(PyExc_TypeError, "rect argument is invalid");
    }
    SDL_SetGPUScissor(self->render_pass, rect);
    Py_RETURN_NONE;
}

static int
render_pass_init(pgRenderPassObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *colorobj = NULL;
    SDL_GPULoadOp load_op = 0;
    SDL_GPUStoreOp store_op = 0;
    Uint8 rgba[4];
    SDL_FColor floatrgba = { 0 };
    char *keywords[] = {"clear_color", "load_op", "store_op", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|Oii", keywords,
                                     &colorobj, &load_op, &store_op)) {
        return -1;
    }
    if (colorobj != NULL) {
        if (!pg_RGBAFromObjEx(colorobj, rgba, PG_COLOR_HANDLE_ALL)) {
            return -1;
        }
        floatrgba.r = (float)rgba[0] / 255;
        floatrgba.g = (float)rgba[1] / 255;
        floatrgba.b = (float)rgba[2] / 255;
        floatrgba.a = (float)rgba[3] / 255;
    }
    self->color_info = (SDL_GPUColorTargetInfo){ 0 };
    self->color_info.clear_color = floatrgba;
    self->color_info.load_op = load_op;
    self->color_info.store_op = store_op;
    return 0;
}

static void
render_pass_dealloc(pgRenderPassObject *self, PyObject *_null)
{
    Py_TYPE(self)->tp_free(self);
}

/* Pipeline implementation */
static void
pipeline_fill_vertex_input_state(pgPipelineObject *self, BufferType vertex_input_state)
{
    SDL_GPUVertexBufferDescription* buffer_description;
    SDL_GPUVertexAttribute* vertex_attribute;
    if (vertex_input_state == POSITION_COLOR_VERTEX) {
        buffer_description = (SDL_GPUVertexBufferDescription*)malloc(sizeof(SDL_GPUVertexBufferDescription));
        buffer_description->slot = 0;
        buffer_description->input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        buffer_description->instance_step_rate = 0;
        buffer_description->pitch = sizeof(PositionColorVertex);

        vertex_attribute = (SDL_GPUVertexAttribute*)malloc(2 * sizeof(SDL_GPUVertexAttribute));
        vertex_attribute[0].buffer_slot = 0;
        vertex_attribute[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        vertex_attribute[0].location = 0;
        vertex_attribute[0].offset = 0;
        vertex_attribute[1].buffer_slot = 0;
        vertex_attribute[1].format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM;
        vertex_attribute[1].location = 1;
        vertex_attribute[1].offset = sizeof(float) * 3;

        self->pipeline_info.vertex_input_state = (SDL_GPUVertexInputState){
            .num_vertex_buffers = 1,
            .vertex_buffer_descriptions = buffer_description,
            .num_vertex_attributes = 2,
            .vertex_attributes = vertex_attribute
        };
    }
    else if (vertex_input_state == POSITION_TEXTURE_VERTEX) {
        buffer_description = (SDL_GPUVertexBufferDescription*)malloc(sizeof(SDL_GPUVertexBufferDescription));
        buffer_description->slot = 0;
        buffer_description->input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        buffer_description->instance_step_rate = 0;
        buffer_description->pitch = sizeof(PositionTextureVertex);

        vertex_attribute = (SDL_GPUVertexAttribute*)malloc(2 * sizeof(SDL_GPUVertexAttribute));
        vertex_attribute[0].buffer_slot = 0;
        vertex_attribute[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        vertex_attribute[0].location = 0;
        vertex_attribute[0].offset = 0;
        vertex_attribute[1].buffer_slot = 0;
        vertex_attribute[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        vertex_attribute[1].location = 1;
        vertex_attribute[1].offset = sizeof(float) * 3;

        self->pipeline_info.vertex_input_state = (SDL_GPUVertexInputState){
            .num_vertex_buffers = 1,
            .vertex_buffer_descriptions = buffer_description,
            .num_vertex_attributes = 2,
            .vertex_attributes = vertex_attribute
        };
    }
}

static void
pipeline_fill_target_info(pgPipelineObject *self, pgWindowObject *window, SDL_GPUBlendFactor src_color_blendfactor, SDL_GPUBlendFactor src_alpha_blendfactor, SDL_GPUBlendFactor dst_color_blendfactor, SDL_GPUBlendFactor dst_alpha_blendfactor)
{
    SDL_GPUColorTargetDescription *color_target_descriptions = NULL;
    if (src_color_blendfactor || src_alpha_blendfactor || dst_color_blendfactor || dst_alpha_blendfactor) {
        color_target_descriptions = (SDL_GPUColorTargetDescription *)calloc(1, sizeof(SDL_GPUColorTargetDescription));
        color_target_descriptions->format = SDL_GetGPUSwapchainTextureFormat(device, window->_win);
        color_target_descriptions->blend_state.enable_blend = true;
        color_target_descriptions->blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
        color_target_descriptions->blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        color_target_descriptions->blend_state.src_color_blendfactor = src_color_blendfactor;
        color_target_descriptions->blend_state.src_alpha_blendfactor = src_alpha_blendfactor;
        color_target_descriptions->blend_state.dst_color_blendfactor = dst_color_blendfactor;
        color_target_descriptions->blend_state.dst_alpha_blendfactor = dst_alpha_blendfactor;
        self->pipeline_info.target_info = (SDL_GPUGraphicsPipelineTargetInfo){
            .num_color_targets = 1,
            .color_target_descriptions = color_target_descriptions
        };
    }
    else {
        self->pipeline_info.target_info = (SDL_GPUGraphicsPipelineTargetInfo){
            .num_color_targets = 1,
            .color_target_descriptions = (SDL_GPUColorTargetDescription[]){{
                .format = SDL_GetGPUSwapchainTextureFormat(device, window->_win)
            }},
        };
    }
}

static PyObject *
pipeline_bind(pgPipelineObject *self, PyObject *args, PyObject *kwargs)
{
    pgRenderPassObject *render_pass;
    PyObject *storage_textures = NULL;
    char *keywords[] = {"render_pass", "storage_textures", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!|O", keywords,
                                     &pgRenderPass_Type, &render_pass, &storage_textures)) {
        return NULL;
    }
    SDL_BindGPUGraphicsPipeline(render_pass->render_pass, self->pipeline);
    if (storage_textures != NULL && storage_textures != Py_None) {
        Uint32 count = (Uint32)PySequence_Length(storage_textures);
        SDL_GPUTexture **tex_array = (SDL_GPUTexture **)malloc(count * sizeof(SDL_GPUTexture *));
        for (Uint32 i = 0; i < count; i++) {
            PyObject *item = PySequence_GetItem(storage_textures, i);
            if (!pgGPUTexture_Check(item)) {
                Py_DECREF(item);
                free(tex_array);
                return RAISE(PyExc_TypeError, "storage_textures must contain Texture objects");
            }
            tex_array[i] = ((pgGPUTextureObject *)item)->texture;
            Py_DECREF(item);
        }
        SDL_BindGPUFragmentStorageTextures(render_pass->render_pass, 0, tex_array, count);
        free(tex_array);
    }
    Py_RETURN_NONE;
}

static int
pipeline_init(pgPipelineObject *self, PyObject *args, PyObject *kwargs)
{
    pgWindowObject *window;
    pgShaderObject *vertex_shader, *fragment_shader;
    SDL_GPUPrimitiveType primitive_type;
    SDL_GPUFillMode fill_mode = SDL_GPU_FILLMODE_FILL;
    BufferType vertex_input_state = -1;
    SDL_GPUCullMode cull_mode = SDL_GPU_CULLMODE_NONE;
    SDL_GPUFrontFace front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;
    SDL_GPUBlendFactor src_color_blendfactor = SDL_GPU_BLENDFACTOR_INVALID, src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_INVALID, dst_color_blendfactor = SDL_GPU_BLENDFACTOR_INVALID, dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_INVALID;
    char *keywords[] = {"window", "vertex_shader", "fragment_shader", "primitive_type", "fill_mode", "vertex_input_state", "cull_mode", "front_face", "src_color_blendfactor", "src_alpha_blendfactor", "dst_color_blendfactor", "dst_alpha_blendfactor", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!O!i|iiiiiiii", keywords,
                                     &pgWindow_Type, &window, &pgShader_Type, &vertex_shader, &pgShader_Type, &fragment_shader, &primitive_type, &fill_mode, &vertex_input_state, &cull_mode, &front_face, &src_color_blendfactor, &src_alpha_blendfactor, &dst_color_blendfactor, &dst_alpha_blendfactor)) {
        return -1;
    }
    pipeline_fill_target_info(self, window, src_color_blendfactor, src_alpha_blendfactor, dst_color_blendfactor, dst_alpha_blendfactor);
    self->pipeline_info.vertex_shader = vertex_shader->shader;
    self->pipeline_info.fragment_shader = fragment_shader->shader;
    self->pipeline_info.primitive_type = primitive_type;
    self->pipeline_info.rasterizer_state.fill_mode = fill_mode;
    self->pipeline_info.rasterizer_state.cull_mode = cull_mode;
    self->pipeline_info.rasterizer_state.front_face = front_face;
    if (vertex_input_state > -1) {
        pipeline_fill_vertex_input_state(self, vertex_input_state);
    }
    self->pipeline = SDL_CreateGPUGraphicsPipeline(device, &self->pipeline_info);

    free((void*)self->pipeline_info.vertex_input_state.vertex_buffer_descriptions);
    free((void*)self->pipeline_info.vertex_input_state.vertex_attributes);
    if (src_color_blendfactor || src_alpha_blendfactor || dst_color_blendfactor || dst_alpha_blendfactor) {
        free((void*)self->pipeline_info.target_info.color_target_descriptions);
    }
    if (self->pipeline == NULL) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    return 0;
}

static void
pipeline_dealloc(pgPipelineObject *self, PyObject *_null)
{
    if (device != NULL && self->pipeline) {
        SDL_ReleaseGPUGraphicsPipeline(device, self->pipeline);
        self->pipeline = NULL;
    }
    Py_TYPE(self)->tp_free(self);
}

/* Buffer implementation */
static int
buffer_get_element_size(SDL_GPUBufferUsageFlags usage, BufferType buffer_type) {
    if (usage == SDL_GPU_BUFFERUSAGE_VERTEX) {
        switch (buffer_type) {
            case POSITION_VERTEX:
                return sizeof(float) * 3;
            case POSITION_COLOR_VERTEX:
                return sizeof(float) * 3 + 4 * sizeof(Uint8);
            case POSITION_TEXTURE_VERTEX:
                return sizeof(float) * 5;
            default:
                return 0;
        }
    }
    else if (usage == SDL_GPU_BUFFERUSAGE_INDEX) {
        return sizeof(Uint16);
    }
    else if (usage == SDL_GPU_BUFFERUSAGE_INDIRECT ||
             usage == SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ ||
             usage == SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ ||
             usage == SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE) {
        return 1;  /* raw bytes — size param is byte count */
    }
    return 0;
}

static inline SDL_GPUTransferBuffer*
buffer_upload_position_color_vertex(pgBufferObject *self, PyObject* data, int size)
{
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &(SDL_GPUTransferBufferCreateInfo) {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size
    });
    PositionColorVertex* transfer_data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    for (int i = 0; i < self->no_of_elements; i++) {
        PyObject* inner_data_obj = PySequence_GetItem(data, i);
        PyObject *v0 = PySequence_GetItem(inner_data_obj, 0);
        PyObject *v1 = PySequence_GetItem(inner_data_obj, 1);
        PyObject *v2 = PySequence_GetItem(inner_data_obj, 2);
        PyObject *v3 = PySequence_GetItem(inner_data_obj, 3);
        PyObject *v4 = PySequence_GetItem(inner_data_obj, 4);
        PyObject *v5 = PySequence_GetItem(inner_data_obj, 5);
        PyObject *v6 = PySequence_GetItem(inner_data_obj, 6);
        transfer_data[i].x = (float)PyFloat_AsDouble(v0);
        transfer_data[i].y = (float)PyFloat_AsDouble(v1);
        transfer_data[i].z = (float)PyFloat_AsDouble(v2);
        transfer_data[i].r = (Uint8)PyLong_AsInt(v3);
        transfer_data[i].g = (Uint8)PyLong_AsInt(v4);
        transfer_data[i].b = (Uint8)PyLong_AsInt(v5);
        transfer_data[i].a = (Uint8)PyLong_AsInt(v6);
        Py_DECREF(v0); Py_DECREF(v1); Py_DECREF(v2);
        Py_DECREF(v3); Py_DECREF(v4); Py_DECREF(v5); Py_DECREF(v6);
        Py_DECREF(inner_data_obj);
    }
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
    return transfer_buffer;
}

static inline SDL_GPUTransferBuffer*
buffer_upload_position_texture_vertex(pgBufferObject *self, PyObject* data, int size)
{
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &(SDL_GPUTransferBufferCreateInfo) {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size
    });
    PositionTextureVertex* transfer_data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    for (int i = 0; i < self->no_of_elements; i++) {
        PyObject* inner_data_obj = PySequence_GetItem(data, i);
        PyObject *v0 = PySequence_GetItem(inner_data_obj, 0);
        PyObject *v1 = PySequence_GetItem(inner_data_obj, 1);
        PyObject *v2 = PySequence_GetItem(inner_data_obj, 2);
        PyObject *v3 = PySequence_GetItem(inner_data_obj, 3);
        PyObject *v4 = PySequence_GetItem(inner_data_obj, 4);
        transfer_data[i].x = (float)PyFloat_AsDouble(v0);
        transfer_data[i].y = (float)PyFloat_AsDouble(v1);
        transfer_data[i].z = (float)PyFloat_AsDouble(v2);
        transfer_data[i].u = (float)PyFloat_AsDouble(v3);
        transfer_data[i].v = (float)PyFloat_AsDouble(v4);
        Py_DECREF(v0); Py_DECREF(v1); Py_DECREF(v2);
        Py_DECREF(v3); Py_DECREF(v4);
        Py_DECREF(inner_data_obj);
    }
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
    return transfer_buffer;
}

static inline SDL_GPUTransferBuffer*
buffer_upload_index(pgBufferObject *self, PyObject* data, int size)
{
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &(SDL_GPUTransferBufferCreateInfo) {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size
    });
    Uint16* transfer_data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    for (int i = 0; i < self->no_of_elements; i++) {
        PyObject *item = PySequence_GetItem(data, i);
        transfer_data[i] = (Uint16)PyLong_AsInt(item);
        Py_DECREF(item);
    }
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
    return transfer_buffer;
}

static inline SDL_GPUTransferBuffer*
buffer_upload_indirect(pgBufferObject *self, PyObject* data, int size)
{
    Py_buffer view;
    if (PyObject_GetBuffer(data, &view, PyBUF_SIMPLE) < 0) {
        return NULL;
    }
    if (view.len != size) {
        PyBuffer_Release(&view);
        PyErr_SetString(PyExc_ValueError, "Buffer data size does not match expected size");
        return NULL;
    }
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &(SDL_GPUTransferBufferCreateInfo) {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size
    });
    void* transfer_data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(transfer_data, view.buf, size);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
    PyBuffer_Release(&view);
    return transfer_buffer;
}

static PyObject *
buffer_upload(pgBufferObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject* data;
    int size = buffer_get_element_size(self->usage, self->buffer_type) * self->no_of_elements;
    char *keywords[] = {"data", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", keywords, &data)) {
        return NULL;
    }
    SDL_GPUTransferBuffer* transfer_buffer;
    if (self->usage == SDL_GPU_BUFFERUSAGE_VERTEX) {
        switch (self->buffer_type) {
            case POSITION_COLOR_VERTEX:
                transfer_buffer = buffer_upload_position_color_vertex(self, data, size);
                break;
            case POSITION_TEXTURE_VERTEX:
                transfer_buffer = buffer_upload_position_texture_vertex(self, data, size);
                break;
            default:
                transfer_buffer = buffer_upload_position_color_vertex(self, data, size);
        }
    }
    else if (self->usage == SDL_GPU_BUFFERUSAGE_INDEX) {
        transfer_buffer = buffer_upload_index(self, data, size);
    }
    else if (self->usage == SDL_GPU_BUFFERUSAGE_INDIRECT ||
             self->usage == SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ ||
             self->usage == SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ ||
             self->usage == SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE) {
        transfer_buffer = buffer_upload_indirect(self, data, size);
        if (transfer_buffer == NULL) {
            return NULL;
        }
    }
    SDL_GPUCommandBuffer* upload_cmd_buf = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buf);
    SDL_UploadToGPUBuffer(
		copy_pass,
		&(SDL_GPUTransferBufferLocation) {
			.transfer_buffer = transfer_buffer,
			.offset = 0
		},
		&(SDL_GPUBufferRegion) {
			.buffer = self->buffer,
			.offset = 0,
			.size = size
		},
		false
	);
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
    SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd_buf);
    Py_RETURN_NONE;
}

static PyObject *
buffer_bind(pgBufferObject *self, PyObject *args, PyObject *kwargs)
{
    pgRenderPassObject *render_pass;
    char *keywords[] = {"render_pass", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords,
                                     &pgRenderPass_Type, &render_pass)) {
        return NULL;
    }
    if (self->usage == SDL_GPU_BUFFERUSAGE_VERTEX) {
        SDL_BindGPUVertexBuffers(render_pass->render_pass, 0,
            &(SDL_GPUBufferBinding){
                .buffer = self->buffer,
                .offset = 0
            },
            1
        );
    }
    else if (self->usage == SDL_GPU_BUFFERUSAGE_INDEX) {
        SDL_BindGPUIndexBuffer(render_pass->render_pass, 
            &(SDL_GPUBufferBinding){
                .buffer = self->buffer,
                .offset = 0
            },
            SDL_GPU_INDEXELEMENTSIZE_16BIT);
    }
    Py_RETURN_NONE;
}

static int
buffer_init(pgBufferObject *self, PyObject *args, PyObject *kwargs)
{
    SDL_GPUBuffer *buffer = NULL;
    SDL_GPUBufferUsageFlags usage;
    BufferType buffer_type = -1;
    Uint32 size;
    int element_size;
    char *keywords[] = {"usage", "size", "buffer_type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", keywords,
                                     &usage, &size, &buffer_type)) {
        return -1;
    }
    element_size = buffer_get_element_size(usage, buffer_type);
    if (!element_size) {
        RAISERETURN(pgExc_SDLError, "Unknown buffer type!", -1)
    }
    buffer = SDL_CreateGPUBuffer(device, &(SDL_GPUBufferCreateInfo) {
        .usage = usage,
        .size = element_size * size
    });
    if (buffer == NULL) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    self->buffer = buffer;
    self->buffer_type = buffer_type;
    self->no_of_elements = size;
    self->usage = usage;
    return 0;
}

static void
buffer_dealloc(pgBufferObject *self, PyObject *_null)
{
    if (device != NULL && self->buffer) {
        SDL_ReleaseGPUBuffer(device, self->buffer);
    }
    Py_TYPE(self)->tp_free(self);
}

/* Texture implementation */
static PyObject *
texture_upload(pgGPUTextureObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *data;
    char *keywords[] = {"data", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", keywords, &data)) {
        return NULL;
    }

    Uint8 *src_pixels;
    Uint32 size;

    Py_buffer view;
    SDL_Surface *surf = NULL;

    if (pgSurface_Check(data)) {
        surf = pgSurface_AsSurface((pgSurfaceObject *)data);
        SURF_INIT_CHECK(surf)
        src_pixels = (Uint8 *)surf->pixels;
        size = self->width * self->height * 4;
    } else if (PyObject_GetBuffer(data, &view, PyBUF_SIMPLE) == 0) {
        src_pixels = (Uint8 *)view.buf;
        size = (Uint32)view.len;
    } else {
        return RAISE(PyExc_TypeError, "expected a Surface or buffer object");
    }

    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(
		device,
		&(SDL_GPUTransferBufferCreateInfo) {
			.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
			.size = size
		}
	);
    Uint8* transfer_data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    SDL_memcpy(transfer_data, src_pixels, size);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    if (surf == NULL) {
        PyBuffer_Release(&view);
    }

    SDL_GPUCommandBuffer* upload_cmd_buf = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buf);
    SDL_UploadToGPUTexture(
        copy_pass,
        &(SDL_GPUTextureTransferInfo){
            .transfer_buffer = transfer_buffer,
            .offset = 0
        },
        &(SDL_GPUTextureRegion) {
            .texture = self->texture,
            .w = self->width,
            .h = self->height,
            .d = 1
        },
        false
    );
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
    SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd_buf);
    Py_RETURN_NONE;
}

static int
texture_init(pgGPUTextureObject *self, PyObject *args, PyObject *kwargs)
{
    SDL_GPUTexture *texture = NULL;
    SDL_GPUTextureType texture_type;
    SDL_GPUTextureUsageFlags usage;
    int width, height;
    SDL_GPUTextureFormat format = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;                                                                                                                                                                                                                                                              
    Uint32 depth = 1;
    PyObject *sizeobj = NULL;
    char *keywords[] = {"size", "texture_type", "usage", "format", "depth", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oii|ii", keywords,                                                                                                                                                                                                                                              
                                     &sizeobj, &texture_type, &usage, &format, &depth)) {
        return -1;
    }
    if (!pg_TwoIntsFromObj(sizeobj, &width, &height)) {
        RAISERETURN(PyExc_TypeError, "invalid size argument", -1)
    }
    self->texture_info.type = texture_type;
    self->texture_info.format = format;
    self->texture_info.width = width;
    self->texture_info.height = height;
    self->texture_info.layer_count_or_depth = depth;
    self->texture_info.num_levels = 1;
    self->texture_info.usage = usage;
    texture = SDL_CreateGPUTexture(device, &self->texture_info);
    if (texture == NULL) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    self->texture = texture;
    self->width = width;
    self->height = height;
    self->is_swapchain = 0;
    return 0;
}

static void
texture_dealloc(pgGPUTextureObject *self, PyObject *_null)
{
    if (device != NULL && self->texture && !self->is_swapchain) {
        SDL_ReleaseGPUTexture(device, self->texture);
    }
    Py_TYPE(self)->tp_free(self);
}

/* Sampler implementation */
static PyObject *
sampler_bind(pgSamplerObject *self, PyObject *args, PyObject *kwargs)
{
    pgRenderPassObject *render_pass;
    pgGPUTextureObject *texture;
    char *keywords[] = {"render_pass", "texture", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!", keywords,
                                     &pgRenderPass_Type, &render_pass, &pgGPUTexture_Type, &texture)) {
        return NULL;
    }
    SDL_BindGPUFragmentSamplers(render_pass->render_pass, 0, &(SDL_GPUTextureSamplerBinding){ .texture = texture->texture, .sampler = self->sampler }, 1);
    Py_RETURN_NONE;
}

static int
sampler_init(pgSamplerObject *self, PyObject *args, PyObject *kwargs)
{
    SDL_GPUSampler *sampler = NULL;
    SDL_GPUFilter filter;
    SDL_GPUSamplerMipmapMode mipmap_mode;
    SDL_GPUSamplerAddressMode address_mode;
    float anisotropy = 0;
    char *keywords[] = {"filter", "mipmap_mode", "address_mode", "anisotropy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii|f", keywords,
                                     &filter, &mipmap_mode, &address_mode, &anisotropy)) {
        return -1;
    }
    
    self->sampler_info.min_filter = filter;
    self->sampler_info.mag_filter = filter;
    self->sampler_info.mipmap_mode = mipmap_mode;
    self->sampler_info.address_mode_u = address_mode;
    self->sampler_info.address_mode_v = address_mode;
    self->sampler_info.address_mode_w = address_mode;
    if (anisotropy) {
        self->sampler_info.enable_anisotropy = true;
        self->sampler_info.max_anisotropy = anisotropy;
    }
    sampler = SDL_CreateGPUSampler(device, &self->sampler_info);
    if (sampler == NULL) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    self->sampler = sampler;
    return 0;
}

static void
sampler_dealloc(pgSamplerObject *self, PyObject *_null)
{
    if (device != NULL && self->sampler) {
        SDL_ReleaseGPUSampler(device, self->sampler);
    }
    Py_TYPE(self)->tp_free(self);
}

/* ComputePipeline implementation */
static int
compute_pipeline_init(pgComputePipelineObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *file;
    SDL_RWops *rw = NULL;
    int readwrite_storage_textures = 0, readwrite_storage_buffers = 0;
    int readonly_storage_textures = 0, readonly_storage_buffers = 0;
    int uniform_buffers = 0, samplers = 0;
    int threadcount_x, threadcount_y, threadcount_z;
    char *keywords[] = {"file", "threadcount_x", "threadcount_y", "threadcount_z",
                        "readwrite_storage_textures", "readwrite_storage_buffers",
                        "readonly_storage_textures", "readonly_storage_buffers",
                        "uniform_buffers", "samplers", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oiii|iiiiii", keywords,
                                     &file, &threadcount_x, &threadcount_y, &threadcount_z,
                                     &readwrite_storage_textures, &readwrite_storage_buffers,
                                     &readonly_storage_textures, &readonly_storage_buffers,
                                     &uniform_buffers, &samplers)) {
        return -1;
    }
    if (device == NULL) {
        RAISERETURN(pgExc_SDLError, "gpu module hasn't been initialized!", -1)
    }
    rw = pgRWops_FromObject(file, NULL);
    if (rw == NULL) {
        RAISERETURN(pgExc_SDLError, "Unable to read file", -1);
    }
    size_t code_size = SDL_GetIOSize(rw);
    Uint8 *code = (Uint8 *)malloc(code_size);
    SDL_ReadIO(rw, code, code_size);
    SDL_CloseIO(rw);

    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    const char *entrypoint;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    } else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
        format = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    } else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
    } else {
        free(code);
        RAISERETURN(pgExc_SDLError, "Unrecognized backend shader format!", -1);
    }
    SDL_GPUComputePipelineCreateInfo info = {
        .code = code,
        .code_size = code_size,
        .entrypoint = entrypoint,
        .format = format,
        .num_readwrite_storage_textures = readwrite_storage_textures,
        .num_readwrite_storage_buffers = readwrite_storage_buffers,
        .num_readonly_storage_textures = readonly_storage_textures,
        .num_readonly_storage_buffers = readonly_storage_buffers,
        .num_uniform_buffers = uniform_buffers,
        .num_samplers = samplers,
        .threadcount_x = threadcount_x,
        .threadcount_y = threadcount_y,
        .threadcount_z = threadcount_z,
    };
    self->pipeline = SDL_CreateGPUComputePipeline(device, &info);
    free(code);
    if (self->pipeline == NULL) {
        PyErr_SetString(pgExc_SDLError, SDL_GetError());
        return -1;
    }
    return 0;
}

static void
compute_pipeline_dealloc(pgComputePipelineObject *self, PyObject *_null)
{
    if (device != NULL && self->pipeline) {
        SDL_ReleaseGPUComputePipeline(device, self->pipeline);
    }
    Py_TYPE(self)->tp_free(self);
}

/* ComputePass implementation */
static PyObject *
compute_pass_begin(pgComputePassObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *storage_textures = NULL;
    PyObject *storage_buffers = NULL;
    int cycle = 0;
    char *keywords[] = {"storage_textures", "storage_buffers", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|OOp", keywords,
                                     &storage_textures, &storage_buffers, &cycle)) {
        return NULL;
    }
    if (!acquire_command_buffer()) {
        return NULL;
    }
    Uint32 num_textures = 0;
    SDL_GPUStorageTextureReadWriteBinding *tex_bindings = NULL;
    if (storage_textures != NULL && storage_textures != Py_None) {
        num_textures = (Uint32)PySequence_Length(storage_textures);
        tex_bindings = (SDL_GPUStorageTextureReadWriteBinding *)calloc(
            num_textures, sizeof(SDL_GPUStorageTextureReadWriteBinding));
        for (Uint32 i = 0; i < num_textures; i++) {
            PyObject *item = PySequence_GetItem(storage_textures, i);
            if (!pgGPUTexture_Check(item)) {
                Py_DECREF(item);
                free(tex_bindings);
                return RAISE(PyExc_TypeError, "storage_textures must contain Texture objects");
            }
            tex_bindings[i].texture = ((pgGPUTextureObject *)item)->texture;
            tex_bindings[i].cycle = cycle;
            Py_DECREF(item);
        }
    }
    Uint32 num_buffers = 0;
    SDL_GPUStorageBufferReadWriteBinding *buf_bindings = NULL;
    if (storage_buffers != NULL && storage_buffers != Py_None) {
        num_buffers = (Uint32)PySequence_Length(storage_buffers);
        buf_bindings = (SDL_GPUStorageBufferReadWriteBinding *)calloc(
            num_buffers, sizeof(SDL_GPUStorageBufferReadWriteBinding));
        for (Uint32 i = 0; i < num_buffers; i++) {
            PyObject *item = PySequence_GetItem(storage_buffers, i);
            if (!pgBuffer_Check(item)) {
                Py_DECREF(item);
                free(tex_bindings);
                free(buf_bindings);
                return RAISE(PyExc_TypeError, "storage_buffers must contain Buffer objects");
            }
            buf_bindings[i].buffer = ((pgBufferObject *)item)->buffer;
            Py_DECREF(item);
        }
    }
    self->compute_pass = SDL_BeginGPUComputePass(
        cmdbuf, tex_bindings, num_textures, buf_bindings, num_buffers);
    free(tex_bindings);
    free(buf_bindings);
    if (self->compute_pass == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    Py_RETURN_NONE;
}

static PyObject *
compute_pass_bind(pgComputePassObject *self, PyObject *args, PyObject *kwargs)
{
    pgComputePipelineObject *pipeline;
    PyObject *storage_textures = NULL;
    PyObject *samplers = NULL;
    char *keywords[] = {"compute_pipeline", "storage_textures", "samplers", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!|OO", keywords,
                                     &pgComputePipeline_Type, &pipeline, &storage_textures, &samplers)) {
        return NULL;
    }
    SDL_BindGPUComputePipeline(self->compute_pass, pipeline->pipeline);
    if (storage_textures != NULL && storage_textures != Py_None) {
        Uint32 count = (Uint32)PySequence_Length(storage_textures);
        SDL_GPUTexture **tex_array = (SDL_GPUTexture **)malloc(count * sizeof(SDL_GPUTexture *));
        for (Uint32 i = 0; i < count; i++) {
            PyObject *item = PySequence_GetItem(storage_textures, i);
            if (!pgGPUTexture_Check(item)) {
                Py_DECREF(item);
                free(tex_array);
                return RAISE(PyExc_TypeError, "storage_textures must contain Texture objects");
            }
            tex_array[i] = ((pgGPUTextureObject *)item)->texture;
            Py_DECREF(item);
        }
        SDL_BindGPUComputeStorageTextures(self->compute_pass, 0, tex_array, count);
        free(tex_array);
    }
    if (samplers != NULL && samplers != Py_None) {
        Uint32 count = (Uint32)PySequence_Length(samplers);
        SDL_GPUTextureSamplerBinding *bindings = (SDL_GPUTextureSamplerBinding *)calloc(
            count, sizeof(SDL_GPUTextureSamplerBinding));
        for (Uint32 i = 0; i < count; i++) {
            PyObject *tuple = PySequence_GetItem(samplers, i);
            if (!PyTuple_Check(tuple) || PyTuple_GET_SIZE(tuple) != 2) {
                Py_DECREF(tuple);
                free(bindings);
                return RAISE(PyExc_TypeError, "samplers must contain (Sampler, Texture) tuples");
            }
            PyObject *sampler_obj = PyTuple_GET_ITEM(tuple, 0);
            PyObject *texture_obj = PyTuple_GET_ITEM(tuple, 1);
            if (!pgSampler_Check(sampler_obj) || !pgGPUTexture_Check(texture_obj)) {
                Py_DECREF(tuple);
                free(bindings);
                return RAISE(PyExc_TypeError, "samplers must contain (Sampler, Texture) tuples");
            }
            bindings[i].sampler = ((pgSamplerObject *)sampler_obj)->sampler;
            bindings[i].texture = ((pgGPUTextureObject *)texture_obj)->texture;
            Py_DECREF(tuple);
        }
        SDL_BindGPUComputeSamplers(self->compute_pass, 0, bindings, count);
        free(bindings);
    }
    Py_RETURN_NONE;
}

static PyObject *
compute_pass_dispatch(pgComputePassObject *self, PyObject *args, PyObject *kwargs)
{
    Uint32 x, y, z;
    char *keywords[] = {"x", "y", "z", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "III", keywords, &x, &y, &z)) {
        return NULL;
    }
    SDL_DispatchGPUCompute(self->compute_pass, x, y, z);
    Py_RETURN_NONE;
}

static PyObject *
compute_pass_end(pgComputePassObject *self, PyObject *_null)
{
    if (self->compute_pass != NULL) {
        SDL_EndGPUComputePass(self->compute_pass);
        self->compute_pass = NULL;
    }
    Py_RETURN_NONE;
}

static int
compute_pass_init(pgComputePassObject *self, PyObject *args, PyObject *kwargs)
{
    self->compute_pass = NULL;
    return 0;
}

static void
compute_pass_dealloc(pgComputePassObject *self, PyObject *_null)
{
    Py_TYPE(self)->tp_free(self);
}

/* CopyPass implementation */
static PyObject *
copy_pass_begin(pgCopyPassObject *self, PyObject *_null)
{
    if (!acquire_command_buffer()) {
        return NULL;
    }
    self->copy_pass = SDL_BeginGPUCopyPass(cmdbuf);
    if (self->copy_pass == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_end(pgCopyPassObject *self, PyObject *_null)
{
    if (self->copy_pass != NULL) {
        SDL_EndGPUCopyPass(self->copy_pass);
        self->copy_pass = NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_upload_to_texture(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgTransferBufferObject *tb;
    pgGPUTextureObject *texture;
    int cycle = 0;
    char *keywords[] = {"transfer_buffer", "texture", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!|p", keywords,
                                     &pgTransferBuffer_Type, &tb,
                                     &pgGPUTexture_Type, &texture, &cycle)) {
        return NULL;
    }
    SDL_UploadToGPUTexture(
        self->copy_pass,
        &(SDL_GPUTextureTransferInfo){
            .transfer_buffer = tb->transfer_buffer,
            .offset = 0
        },
        &(SDL_GPUTextureRegion){
            .texture = texture->texture,
            .w = texture->width,
            .h = texture->height,
            .d = 1
        },
        cycle
    );
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_upload_to_buffer(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgTransferBufferObject *tb;
    pgBufferObject *buffer;
    Uint32 size;
    int cycle = 0;
    char *keywords[] = {"transfer_buffer", "buffer", "size", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!I|p", keywords,
                                     &pgTransferBuffer_Type, &tb,
                                     &pgBuffer_Type, &buffer,
                                     &size, &cycle)) {
        return NULL;
    }
    SDL_UploadToGPUBuffer(
        self->copy_pass,
        &(SDL_GPUTransferBufferLocation){
            .transfer_buffer = tb->transfer_buffer,
            .offset = 0
        },
        &(SDL_GPUBufferRegion){
            .buffer = buffer->buffer,
            .offset = 0,
            .size = size
        },
        cycle
    );
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_download_from_texture(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgGPUTextureObject *texture;
    pgTransferBufferObject *tb;
    char *keywords[] = {"texture", "transfer_buffer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!", keywords,
                                     &pgGPUTexture_Type, &texture,
                                     &pgTransferBuffer_Type, &tb)) {
        return NULL;
    }
    SDL_DownloadFromGPUTexture(
        self->copy_pass,
        &(SDL_GPUTextureRegion){
            .texture = texture->texture,
            .w = texture->width,
            .h = texture->height,
            .d = 1
        },
        &(SDL_GPUTextureTransferInfo){
            .transfer_buffer = tb->transfer_buffer,
            .offset = 0
        }
    );
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_download_from_buffer(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgBufferObject *buffer;
    pgTransferBufferObject *tb;
    Uint32 size;
    char *keywords[] = {"buffer", "transfer_buffer", "size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!I", keywords,
                                     &pgBuffer_Type, &buffer,
                                     &pgTransferBuffer_Type, &tb,
                                     &size)) {
        return NULL;
    }
    SDL_DownloadFromGPUBuffer(
        self->copy_pass,
        &(SDL_GPUBufferRegion){
            .buffer = buffer->buffer,
            .offset = 0,
            .size = size
        },
        &(SDL_GPUTransferBufferLocation){
            .transfer_buffer = tb->transfer_buffer,
            .offset = 0
        }
    );
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_copy_texture_to_texture(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgGPUTextureObject *source, *dest;
    Uint32 w, h;
    int cycle = 0;
    char *keywords[] = {"source", "dest", "w", "h", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!II|p", keywords,
                                     &pgGPUTexture_Type, &source,
                                     &pgGPUTexture_Type, &dest,
                                     &w, &h, &cycle)) {
        return NULL;
    }
    SDL_CopyGPUTextureToTexture(
        self->copy_pass,
        &(SDL_GPUTextureLocation){.texture = source->texture},
        &(SDL_GPUTextureLocation){.texture = dest->texture},
        w, h, 1, cycle
    );
    Py_RETURN_NONE;
}

static PyObject *
copy_pass_copy_buffer_to_buffer(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    pgBufferObject *source, *dest;
    Uint32 size;
    int cycle = 0;
    char *keywords[] = {"source", "dest", "size", "cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!I|p", keywords,
                                     &pgBuffer_Type, &source,
                                     &pgBuffer_Type, &dest,
                                     &size, &cycle)) {
        return NULL;
    }
    SDL_CopyGPUBufferToBuffer(
        self->copy_pass,
        &(SDL_GPUBufferLocation){.buffer = source->buffer},
        &(SDL_GPUBufferLocation){.buffer = dest->buffer},
        size, cycle
    );
    Py_RETURN_NONE;
}

static int
copy_pass_init(pgCopyPassObject *self, PyObject *args, PyObject *kwargs)
{
    self->copy_pass = NULL;
    return 0;
}

static void
copy_pass_dealloc(pgCopyPassObject *self, PyObject *_null)
{
    Py_TYPE(self)->tp_free(self);
}

/* TransferBuffer implementation */
static int
transfer_buffer_init(pgTransferBufferObject *self, PyObject *args, PyObject *kwargs)
{
    SDL_GPUTransferBufferUsage usage;
    Uint32 size;
    char *keywords[] = {"usage", "size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iI", keywords, &usage, &size)) {
        return -1;
    }
    self->transfer_buffer = SDL_CreateGPUTransferBuffer(
        device, &(SDL_GPUTransferBufferCreateInfo){
            .usage = usage,
            .size = size
        });
    if (self->transfer_buffer == NULL) {
        RAISERETURN(pgExc_SDLError, SDL_GetError(), -1);
    }
    self->size = size;
    return 0;
}

static void
transfer_buffer_dealloc(pgTransferBufferObject *self, PyObject *_null)
{
    if (device != NULL && self->transfer_buffer) {
        SDL_ReleaseGPUTransferBuffer(device, self->transfer_buffer);
    }
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
transfer_buffer_map(pgTransferBufferObject *self, PyObject *args, PyObject *kwargs)
{
    int cycle = 0;
    char *keywords[] = {"cycle", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|p", keywords, &cycle)) {
        return NULL;
    }
    void *data = SDL_MapGPUTransferBuffer(device, self->transfer_buffer, cycle);
    if (data == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    return PyMemoryView_FromMemory((char *)data, self->size, PyBUF_WRITE);
}

static PyObject *
transfer_buffer_unmap(pgTransferBufferObject *self, PyObject *_null)
{
    SDL_UnmapGPUTransferBuffer(device, self->transfer_buffer);
    Py_RETURN_NONE;
}

/* Fence implementation */
static void
fence_dealloc(pgFenceObject *self, PyObject *_null)
{
    if (device != NULL && self->fence) {
        SDL_ReleaseGPUFence(device, self->fence);
    }
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
fence_query(pgFenceObject *self, PyObject *_null)
{
    if (SDL_QueryGPUFence(device, self->fence)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

/* GPU Functions */
static PyObject *
init(PyObject *self, PyObject *args, PyObject *kwargs)
{
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
    }
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);  // TODO: Reintroduce other backend formats
    if (device == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    Py_RETURN_NONE;
}

static PyObject *
claim_window(PyObject *self, PyObject *args, PyObject *kwargs)
{
    pgWindowObject *window;
    char *keywords[] = {"window", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords,
                                     &pgWindow_Type, &window)) {
        return NULL;
    }
    if (!SDL_ClaimWindowForGPUDevice(device, window->_win)) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    Py_RETURN_NONE;
}

static PyObject *
get_swapchain_format(PyObject *self, PyObject *args, PyObject *kwargs)                                                                                                                                                                                                                                              
{                                                                                                                                                                                                                                                                                                                   
    pgWindowObject *window;                                                                                                                                                                                                                                                                                         
    char *keywords[] = {"window", NULL};                                                                                                                                                                                                                                                                            
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords,                                                                                                                                                                                                                                                  
                                     &pgWindow_Type, &window)) {                                                                                                                                                                                                                                                    
        return NULL;                                                                                                                                                                                                                                                                                                
    }                                                                                                                                                                                                                                                                                                               
    return PyLong_FromLong((long)SDL_GetGPUSwapchainTextureFormat(device, window->_win));                                                                                                                                                                                                                           
}

static PyObject *
set_swapchain_parameters(PyObject *self, PyObject *args, PyObject *kwargs)
{
    pgWindowObject *window;
    int composition, present_mode;
    char *keywords[] = {"window", "composition", "present_mode", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!ii", keywords,
                                     &pgWindow_Type, &window, &composition, &present_mode)) {
        return NULL;
    }
    if (!SDL_SetGPUSwapchainParameters(device, window->_win, composition, present_mode)) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    Py_RETURN_NONE;
}

static PyObject *
supports_swapchain_composition(PyObject *self, PyObject *args, PyObject *kwargs)
{
    pgWindowObject *window;
    int composition;
    char *keywords[] = {"window", "composition", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!i", keywords,
                                     &pgWindow_Type, &window, &composition)) {
        return NULL;
    }
    if (SDL_WindowSupportsGPUSwapchainComposition(device, window->_win, composition)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject *
acquire_swapchain_texture(PyObject *self, PyObject *args, PyObject *kwargs)                                                                                                                                                                                                                                         
{                                                                                                                                                                                                                                                                                                                   
    pgWindowObject *window;                                                                                                                                                                                                                                                                                         
    Uint32 w, h;                                                                                                                                                                                                                                                                                                    
    char *keywords[] = {"window", NULL};                                                                                                                                                                                                                                                                            
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords,                                                                                                                                                                                                                                                  
                                     &pgWindow_Type, &window)) {                                                                                                                                                                                                                                                    
        return NULL;                                                                                                                                                                                                                                                                                                
    }                                                                                                                                                                                                                                                                                                               
    if (!acquire_command_buffer()) {                                                                                                                                                                                                                                                                                
        return NULL;                                                                                                                                                                                                                                                                                                
    }                                                                                                                                                                                                                                                                                                               
    SDL_GPUTexture *tex;                                                                                                                                                                                                                                                                                            
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, window->_win, &tex, &w, &h)) {                                                                                                                                                                                                                               
        return RAISE(pgExc_SDLError, SDL_GetError());                                                                                                                                                                                                                                                               
    }                                                                                                                                                                                                                                                                                                               
    if (tex == NULL) {                                                                                                                                                                                                                                                                                              
         Py_RETURN_NONE;                                                                                                                                                                                                                                                                                             
    }                                                                                                                                                                                                                                                                                                               
    pgGPUTextureObject *texture = PyObject_New(pgGPUTextureObject, &pgGPUTexture_Type);                                                                                                                                                                                                                                 
    texture->texture = tex;                                                                                                                                                                                                                                                                                             
    texture->width = w;                                                                                                                                                                                                                                                                                                 
    texture->height = h;                                                                                                                                                                                                                                                                                                
    texture->is_swapchain = 1;
    swapchain_acquired = 1;
    return (PyObject *)texture;
}

static PyObject *
blit_texture(PyObject *self, PyObject *args, PyObject *kwargs)                                                                                                                                                                                                                                                      
{                                                                                                                                                                                                                                                                                                                   
    pgGPUTextureObject *source, *dest;                                                                                                                                                                                                                                                                              
    int source_w, source_h, dest_w, dest_h;                                                                                                                                                                                                                                                                         
    int source_layer = 0, source_x = 0, source_y = 0;                                                                                                                                                                                                                                                               
    int dest_x = 0, dest_y = 0;                                                                                                                                                                                                                                                                                     
    int load_op = SDL_GPU_LOADOP_LOAD;                                                                                                                                                                                                                                                                              
    int filter = SDL_GPU_FILTER_NEAREST;                                                                                                                                                                                                                                                                            
    char *keywords[] = {                                                                                                                                                                                                                                                                                            
        "source", "source_w", "source_h",                                                                                                                                                                                                                                                                           
        "dest", "dest_w", "dest_h",                                                                                                                                                                                                                                                                                 
        "source_layer", "source_x", "source_y",                                                                                                                                                                                                                                                                     
        "dest_x", "dest_y",                                                                                                                                                                                                                                                                                         
        "load_op", "filter", NULL                                                                                                                                                                                                                                                                                   
    };                                                                                                                                                                                                                                                                                                              
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!iiO!ii|iiiiiii", keywords,                                                                                                                                                                                                                                    
                                     &pgGPUTexture_Type, &source,                                                                                                                                                                                                                                                   
                                     &source_w, &source_h,                                                                                                                                                                                                                                                          
                                     &pgGPUTexture_Type, &dest,                                                                                                                                                                                                                                                     
                                     &dest_w, &dest_h,                                                                                                                                                                                                                                                              
                                     &source_layer, &source_x, &source_y,                                                                                                                                                                                                                                           
                                     &dest_x, &dest_y,                                                                                                                                                                                                                                                              
                                     &load_op, &filter)) {                                                                                                                                                                                                                                                          
        return NULL;                                                                                                                                                                                                                                                                                                
    }                                                                                                                                                                                                                                                                                                               
    SDL_BlitGPUTexture(cmdbuf, &(SDL_GPUBlitInfo){                                                                                                                                                                                                                                                                  
        .source.texture = source->texture,                                                                                                                                                                                                                                                                          
        .source.layer_or_depth_plane = source_layer,                                                                                                                                                                                                                                                                
        .source.x = source_x,                                                                                                                                                                                                                                                                                       
        .source.y = source_y,                                                                                                                                                                                                                                                                                       
        .source.w = source_w,                                                                                                                                                                                                                                                                                       
        .source.h = source_h,                                                                                                                                                                                                                                                                                       
        .destination.texture = dest->texture,                                                                                                                                                                                                                                                                       
        .destination.x = dest_x,                                                                                                                                                                                                                                                                                    
        .destination.y = dest_y,                                                                                                                                                                                                                                                                                    
        .destination.w = dest_w,                                                                                                                                                                                                                                                                                    
        .destination.h = dest_h,                                                                                                                                                                                                                                                                                    
        .load_op = load_op,                                                                                                                                                                                                                                                                                         
        .filter = filter                                                                                                                                                                                                                                                                                            
    });                                                                                                                                                                                                                                                                                                             
    Py_RETURN_NONE;                                                                                                                                                                                                                                                                                                 
}

static PyObject *
push_data(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject* structure_obj;
    Py_buffer view;
    UniformDataType data_type;
    char *keywords[] = {"structure", "data_type", NULL};
    if (!PyArg_ParseTuple(args, "Oi", &structure_obj, &data_type)) {
        return NULL;
    }
    if (PyObject_GetBuffer(structure_obj, &view, PyBUF_SIMPLE) != 0) {
        return RAISE(pgExc_SDLError, "Expected a ctypes Structure object");
    }
    switch (data_type) {
        case PUSH_VERTEX:
            SDL_PushGPUVertexUniformData(cmdbuf, 0, view.buf, (Uint32)view.len);
            break;
        case PUSH_FRAGMENT:
            SDL_PushGPUFragmentUniformData(cmdbuf, 0, view.buf, (Uint32)view.len);
            break;
        case PUSH_UNIFORM:
            SDL_PushGPUComputeUniformData(cmdbuf, 0, view.buf, (Uint32)view.len);
            break;
    }
    PyBuffer_Release(&view);
    Py_RETURN_NONE;
}

static PyObject *
submit(PyObject *self, PyObject *_null)
{
    if (cmdbuf != NULL) {
        SDL_SubmitGPUCommandBuffer(cmdbuf);
        cmdbuf = NULL;
        swapchain_acquired = 0;
        Py_RETURN_NONE;
    }
    else {
        RAISERETURN(pgExc_SDLError, "command buffer is not acquired", NULL);
    }
}

static PyObject *
submit_and_acquire_fence(PyObject *self, PyObject *_null)
{
    if (cmdbuf == NULL) {
        RAISERETURN(pgExc_SDLError, "command buffer is not acquired", NULL);
    }
    SDL_GPUFence *sdl_fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf);
    cmdbuf = NULL;
    swapchain_acquired = 0;
    if (sdl_fence == NULL) {
        return RAISE(pgExc_SDLError, SDL_GetError());
    }
    pgFenceObject *fence_obj = PyObject_New(pgFenceObject, &pgFence_Type);
    if (fence_obj == NULL) {
        SDL_ReleaseGPUFence(device, sdl_fence);
        return NULL;
    }
    fence_obj->fence = sdl_fence;
    return (PyObject *)fence_obj;
}

static PyObject *
wait_for_fences(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *fence_list;
    int wait_all = 1;
    char *keywords[] = {"fences", "wait_all", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|p", keywords,
                                     &fence_list, &wait_all)) {
        return NULL;
    }
    Uint32 count = (Uint32)PySequence_Length(fence_list);
    SDL_GPUFence **fences = (SDL_GPUFence **)malloc(count * sizeof(SDL_GPUFence *));
    for (Uint32 i = 0; i < count; i++) {
        PyObject *item = PySequence_GetItem(fence_list, i);
        if (!pgFence_Check(item)) {
            Py_DECREF(item);
            free(fences);
            return RAISE(PyExc_TypeError, "fences must contain Fence objects");
        }
        fences[i] = ((pgFenceObject *)item)->fence;
        Py_DECREF(item);
    }
    SDL_WaitForGPUFences(device, wait_all, fences, count);
    free(fences);
    Py_RETURN_NONE;
}

static PyObject *
quit(PyObject *self, PyObject *args, PyObject *kwargs)
{
    if (device != NULL) {
        SDL_DestroyGPUDevice(device);
        device = NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef shader_methods[] = {{NULL, NULL, 0, NULL}};

static PyGetSetDef shader_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef render_pass_methods[] = {
    {"begin", (PyCFunction)render_pass_begin,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"draw_primitives", (PyCFunction)render_pass_draw_primitives,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"draw_primitives_indirect", (PyCFunction)render_pass_draw_primitives_indirect,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"set_viewport", (PyCFunction)render_pass_set_viewport,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"set_scissor", (PyCFunction)render_pass_set_scissor,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"end", (PyCFunction)render_pass_end,
    METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef render_pass_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef pipeline_methods[] = {
    {"bind", (PyCFunction)pipeline_bind,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef pipeline_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef buffer_methods[] = {
    {"upload", (PyCFunction)buffer_upload,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"bind", (PyCFunction)buffer_bind,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef buffer_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef texture_methods[] = {
    {"upload", (PyCFunction)texture_upload,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef texture_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef sampler_methods[] = {
    {"bind", (PyCFunction)sampler_bind,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef sampler_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef compute_pipeline_methods[] = {{NULL, NULL, 0, NULL}};

static PyGetSetDef compute_pipeline_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef compute_pass_methods[] = {
    {"begin", (PyCFunction)compute_pass_begin,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"bind", (PyCFunction)compute_pass_bind,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"dispatch", (PyCFunction)compute_pass_dispatch,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"end", (PyCFunction)compute_pass_end,
     METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef compute_pass_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef copy_pass_methods[] = {
    {"begin", (PyCFunction)copy_pass_begin,
     METH_NOARGS, NULL},
    {"end", (PyCFunction)copy_pass_end,
     METH_NOARGS, NULL},
    {"upload_to_texture", (PyCFunction)copy_pass_upload_to_texture,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"upload_to_buffer", (PyCFunction)copy_pass_upload_to_buffer,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"download_from_texture", (PyCFunction)copy_pass_download_from_texture,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"download_from_buffer", (PyCFunction)copy_pass_download_from_buffer,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"copy_texture_to_texture", (PyCFunction)copy_pass_copy_texture_to_texture,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"copy_buffer_to_buffer", (PyCFunction)copy_pass_copy_buffer_to_buffer,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef copy_pass_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef transfer_buffer_methods[] = {
    {"map", (PyCFunction)transfer_buffer_map,
     METH_VARARGS | METH_KEYWORDS, NULL},
    {"unmap", (PyCFunction)transfer_buffer_unmap,
     METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef transfer_buffer_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyMethodDef fence_methods[] = {
    {"query", (PyCFunction)fence_query,
     METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef fence_getset[] = {{NULL, 0, NULL, NULL, NULL}};

static PyTypeObject pgShader_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Shader",
    .tp_basicsize = sizeof(pgShaderObject),
    .tp_dealloc = (destructor)shader_dealloc,
    //.tp_doc = DOC_GPU_SHADER,
    .tp_methods = shader_methods,
    .tp_init = (initproc)shader_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = shader_getset
};

static PyTypeObject pgRenderPass_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.RenderPass",
    .tp_basicsize = sizeof(pgRenderPassObject),
    .tp_dealloc = (destructor)render_pass_dealloc,
    //.tp_doc = DOC_GPU_RENDER_PASS,
    .tp_methods = render_pass_methods,
    .tp_init = (initproc)render_pass_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = render_pass_getset
};

static PyTypeObject pgPipeline_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Pipeline",
    .tp_basicsize = sizeof(pgPipelineObject),
    .tp_dealloc = (destructor)pipeline_dealloc,
    //.tp_doc = DOC_GPU_PIPELINE,
    .tp_methods = pipeline_methods,
    .tp_init = (initproc)pipeline_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = pipeline_getset
};

static PyTypeObject pgBuffer_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Buffer",
    .tp_basicsize = sizeof(pgBufferObject),
    .tp_dealloc = (destructor)buffer_dealloc,
    //.tp_doc = DOC_GPU_BUFFER,
    .tp_methods = buffer_methods,
    .tp_init = (initproc)buffer_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = buffer_getset
};

static PyTypeObject pgGPUTexture_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Texture",
    .tp_basicsize = sizeof(pgGPUTextureObject),
    .tp_dealloc = (destructor)texture_dealloc,
    //.tp_doc = DOC_GPU_TEXTURE,
    .tp_methods = texture_methods,
    .tp_init = (initproc)texture_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = texture_getset
};

static PyTypeObject pgSampler_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Sampler",
    .tp_basicsize = sizeof(pgSamplerObject),
    .tp_dealloc = (destructor)sampler_dealloc,
    //.tp_doc = DOC_GPU_SAMPLER,
    .tp_methods = sampler_methods,
    .tp_init = (initproc)sampler_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = sampler_getset
};

static PyTypeObject pgComputePipeline_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.ComputePipeline",
    .tp_basicsize = sizeof(pgComputePipelineObject),
    .tp_dealloc = (destructor)compute_pipeline_dealloc,
    .tp_methods = compute_pipeline_methods,
    .tp_init = (initproc)compute_pipeline_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = compute_pipeline_getset
};

static PyTypeObject pgComputePass_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.ComputePass",
    .tp_basicsize = sizeof(pgComputePassObject),
    .tp_dealloc = (destructor)compute_pass_dealloc,
    .tp_methods = compute_pass_methods,
    .tp_init = (initproc)compute_pass_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = compute_pass_getset
};

static PyTypeObject pgCopyPass_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.CopyPass",
    .tp_basicsize = sizeof(pgCopyPassObject),
    .tp_dealloc = (destructor)copy_pass_dealloc,
    .tp_methods = copy_pass_methods,
    .tp_init = (initproc)copy_pass_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = copy_pass_getset
};

static PyTypeObject pgTransferBuffer_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.TransferBuffer",
    .tp_basicsize = sizeof(pgTransferBufferObject),
    .tp_dealloc = (destructor)transfer_buffer_dealloc,
    .tp_methods = transfer_buffer_methods,
    .tp_init = (initproc)transfer_buffer_init,
    .tp_new = PyType_GenericNew,
    .tp_getset = transfer_buffer_getset
};

static PyTypeObject pgFence_Type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "pygame.gpu.Fence",
    .tp_basicsize = sizeof(pgFenceObject),
    .tp_dealloc = (destructor)fence_dealloc,
    .tp_methods = fence_methods,
    .tp_new = PyType_GenericNew,
    .tp_getset = fence_getset
};

static PyMethodDef gpu_methods[] = {
    {"init", (PyCFunction)init, METH_NOARGS, NULL},
    {"claim_window", (PyCFunction)claim_window, METH_VARARGS | METH_KEYWORDS, NULL},
    {"get_swapchain_format", (PyCFunction)get_swapchain_format, METH_VARARGS | METH_KEYWORDS, NULL},
    {"set_swapchain_parameters", (PyCFunction)set_swapchain_parameters, METH_VARARGS | METH_KEYWORDS, NULL},
    {"supports_swapchain_composition", (PyCFunction)supports_swapchain_composition, METH_VARARGS | METH_KEYWORDS, NULL},
    {"acquire_swapchain_texture", (PyCFunction)acquire_swapchain_texture, METH_VARARGS | METH_KEYWORDS, NULL},
    {"blit_texture", (PyCFunction)blit_texture, METH_VARARGS | METH_KEYWORDS, NULL},
    {"push_data", (PyCFunction)push_data, METH_VARARGS | METH_KEYWORDS, NULL},
    {"submit", (PyCFunction)submit, METH_NOARGS, NULL},
    {"submit_and_acquire_fence", (PyCFunction)submit_and_acquire_fence, METH_NOARGS, NULL},
    {"wait_for_fences", (PyCFunction)wait_for_fences, METH_VARARGS | METH_KEYWORDS, NULL},
    {"quit", (PyCFunction)quit, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

MODINIT_DEFINE(gpu)
{
    PyObject *module, *apiobj;
    static void *c_api[PYGAMEAPI_GPU_NUMSLOTS];

    static struct PyModuleDef _module = {PyModuleDef_HEAD_INIT,
                                         "gpu",
                                         "docs_needed",
                                         -1,
                                         gpu_methods,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL};

    /* imported needed apis; Do this first so if there is an error
       the module is not loaded.
    */
    import_pygame_base();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_pygame_surface();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_pygame_window();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_pygame_rwobject();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_pygame_color();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_pygame_rect();
    if (PyErr_Occurred()) {
        return NULL;
    }

    /* create the module */
    module = PyModule_Create(&_module);
    if (module == 0) {
        return NULL;
    }

    if (PyModule_AddType(module, &pgShader_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgRenderPass_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgPipeline_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgBuffer_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgGPUTexture_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgSampler_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgComputePipeline_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgComputePass_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgCopyPass_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgTransferBuffer_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    if (PyModule_AddType(module, &pgFence_Type)) {
        Py_XDECREF(module);
        return NULL;
    }

    DEC_CONST(GPU_SHADERSTAGE_VERTEX);
    DEC_CONST(GPU_SHADERSTAGE_FRAGMENT);
    DEC_CONST(GPU_LOADOP_LOAD);
    DEC_CONST(GPU_LOADOP_CLEAR);
    DEC_CONST(GPU_LOADOP_DONT_CARE);
    DEC_CONST(GPU_STOREOP_STORE);
    DEC_CONST(GPU_STOREOP_DONT_CARE);
    DEC_CONST(GPU_STOREOP_RESOLVE);
    DEC_CONST(GPU_STOREOP_RESOLVE_AND_STORE);
    DEC_CONST(GPU_PRIMITIVETYPE_TRIANGLELIST);
    DEC_CONST(GPU_PRIMITIVETYPE_TRIANGLESTRIP);
    DEC_CONST(GPU_PRIMITIVETYPE_LINELIST);
    DEC_CONST(GPU_PRIMITIVETYPE_LINESTRIP);
    DEC_CONST(GPU_PRIMITIVETYPE_POINTLIST);
    DEC_CONST(GPU_FILLMODE_FILL);
    DEC_CONST(GPU_FILLMODE_LINE);
    DEC_CONST(GPU_BUFFERUSAGE_VERTEX);
    DEC_CONST(GPU_BUFFERUSAGE_INDEX);
    DEC_CONST(GPU_BUFFERUSAGE_INDIRECT);
    DEC_CONST(GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ);
    DEC_CONST(GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
    DEC_CONST(GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
    DEC_CONST(GPU_CULLMODE_NONE);
    DEC_CONST(GPU_CULLMODE_FRONT);
    DEC_CONST(GPU_CULLMODE_BACK);
    DEC_CONST(GPU_FRONTFACE_COUNTER_CLOCKWISE);
    DEC_CONST(GPU_FRONTFACE_CLOCKWISE);
    DEC_CONST(GPU_FILTER_NEAREST);
    DEC_CONST(GPU_FILTER_LINEAR);
    DEC_CONST(GPU_SAMPLERMIPMAPMODE_NEAREST);
    DEC_CONST(GPU_SAMPLERMIPMAPMODE_LINEAR);
    DEC_CONST(GPU_SAMPLERADDRESSMODE_REPEAT);
    DEC_CONST(GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT);
    DEC_CONST(GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE);
    DEC_CONST(GPU_TEXTURETYPE_2D);
    DEC_CONST(GPU_TEXTURETYPE_2D_ARRAY);
    DEC_CONST(GPU_TEXTURETYPE_3D);
    DEC_CONST(GPU_TEXTURETYPE_CUBE);
    DEC_CONST(GPU_TEXTURETYPE_CUBE_ARRAY);
    DEC_CONST(GPU_TEXTUREUSAGE_SAMPLER);
    DEC_CONST(GPU_TEXTUREUSAGE_COLOR_TARGET);
    DEC_CONST(GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET);
    DEC_CONST(GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ);
    DEC_CONST(GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ);
    DEC_CONST(GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE);
    DEC_CONST(GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE);
    DEC_CONST(GPU_TEXTUREFORMAT_R8G8B8A8_UNORM);
    DEC_CONST(GPU_TEXTUREFORMAT_B8G8R8A8_UNORM);
    DEC_CONST(GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT);
    DEC_CONST(GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT);
    DEC_CONST(GPU_SWAPCHAINCOMPOSITION_SDR);
    DEC_CONST(GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR);
    DEC_CONST(GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR);
    DEC_CONST(GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084);
    DEC_CONST(GPU_PRESENTMODE_VSYNC);
    DEC_CONST(GPU_PRESENTMODE_MAILBOX);
    DEC_CONST(GPU_PRESENTMODE_IMMEDIATE);
    DEC_CONST(GPU_BLENDFACTOR_ZERO);
    DEC_CONST(GPU_BLENDFACTOR_ONE);
    DEC_CONST(GPU_BLENDFACTOR_SRC_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_DST_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_SRC_ALPHA);
    DEC_CONST(GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA);
    DEC_CONST(GPU_BLENDFACTOR_DST_ALPHA);
    DEC_CONST(GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA);
    DEC_CONST(GPU_BLENDFACTOR_CONSTANT_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR);
    DEC_CONST(GPU_BLENDFACTOR_SRC_ALPHA_SATURATE);
    DEC_CONST(GPU_TRANSFERBUFFERUSAGE_UPLOAD);
    DEC_CONST(GPU_TRANSFERBUFFERUSAGE_DOWNLOAD);
    DEC_CONSTS_("POSITION_VERTEX", POSITION_VERTEX);
    DEC_CONSTS_("POSITION_COLOR_VERTEX", POSITION_COLOR_VERTEX);
    DEC_CONSTS_("POSITION_TEXTURE_VERTEX", POSITION_TEXTURE_VERTEX);
    DEC_CONSTS_("PUSH_VERTEX", PUSH_VERTEX);
    DEC_CONSTS_("PUSH_FRAGMENT", PUSH_FRAGMENT);
    DEC_CONSTS_("PUSH_UNIFORM", PUSH_UNIFORM);

    apiobj = encapsulate_api(c_api, "gpu");
    if (PyModule_AddObject(module, PYGAMEAPI_LOCAL_ENTRY, apiobj)) {
        Py_XDECREF(apiobj);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}
