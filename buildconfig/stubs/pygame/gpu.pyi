from typing import Any, Iterable
from collections.abc import Buffer as SupportsBuffer
from ctypes import Structure
from pygame.surface import Surface
from pygame.window import Window
from pygame.typing import ColorLike, RectLike, FileLike

GPU_SHADERSTAGE_VERTEX: int
GPU_SHADERSTAGE_FRAGMENT: int
GPU_LOADOP_LOAD: int
GPU_LOADOP_CLEAR: int
GPU_LOADOP_DONT_CARE: int
GPU_STOREOP_STORE: int
GPU_STOREOP_DONT_CARE: int
GPU_STOREOP_RESOLVE: int
GPU_STOREOP_RESOLVE_AND_STORE: int
GPU_PRIMITIVETYPE_TRIANGLELIST: int
GPU_PRIMITIVETYPE_TRIANGLESTRIP: int
GPU_PRIMITIVETYPE_LINELIST: int
GPU_PRIMITIVETYPE_LINESTRIP: int
GPU_PRIMITIVETYPE_POINTLIST: int
GPU_FILLMODE_FILL: int
GPU_FILLMODE_LINE: int
GPU_BUFFERUSAGE_VERTEX: int
GPU_BUFFERUSAGE_INDEX: int
GPU_BUFFERUSAGE_INDIRECT: int
GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ: int
GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ: int
GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE: int
GPU_INDEXELEMENTSIZE_16BIT: int
GPU_INDEXELEMENTSIZE_32BIT: int
GPU_CULLMODE_NONE: int
GPU_CULLMODE_FRONT: int
GPU_CULLMODE_BACK: int
GPU_FRONTFACE_COUNTER_CLOCKWISE: int
GPU_FRONTFACE_CLOCKWISE: int
GPU_FILTER_NEAREST: int
GPU_FILTER_LINEAR: int
GPU_SAMPLERMIPMAPMODE_NEAREST: int
GPU_SAMPLERMIPMAPMODE_LINEAR: int
GPU_SAMPLERADDRESSMODE_REPEAT: int
GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT: int
GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE: int
GPU_TEXTURETYPE_2D: int
GPU_TEXTURETYPE_2D_ARRAY: int
GPU_TEXTURETYPE_3D: int
GPU_TEXTURETYPE_CUBE: int
GPU_TEXTURETYPE_CUBE_ARRAY: int
GPU_TEXTUREUSAGE_SAMPLER: int
GPU_TEXTUREUSAGE_COLOR_TARGET: int
GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET: int
GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ: int
GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ: int
GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE: int
GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE: int
GPU_TEXTUREFORMAT_R8G8B8A8_UNORM: int
GPU_TEXTUREFORMAT_B8G8R8A8_UNORM: int
GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT: int
GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT: int
GPU_TEXTUREFORMAT_BC1_RGBA_UNORM: int
GPU_TEXTUREFORMAT_BC2_RGBA_UNORM: int
GPU_TEXTUREFORMAT_BC3_RGBA_UNORM: int
GPU_TEXTUREFORMAT_BC4_R_UNORM: int
GPU_TEXTUREFORMAT_BC5_RG_UNORM: int
GPU_TEXTUREFORMAT_BC6H_RGB_FLOAT: int
GPU_TEXTUREFORMAT_BC6H_RGB_UFLOAT: int
GPU_TEXTUREFORMAT_BC7_RGBA_UNORM: int
GPU_TEXTUREFORMAT_BC1_RGBA_UNORM_SRGB: int
GPU_TEXTUREFORMAT_BC2_RGBA_UNORM_SRGB: int
GPU_TEXTUREFORMAT_BC3_RGBA_UNORM_SRGB: int
GPU_TEXTUREFORMAT_BC7_RGBA_UNORM_SRGB: int
GPU_TEXTUREFORMAT_ASTC_4x4_UNORM: int
GPU_TEXTUREFORMAT_ASTC_5x4_UNORM: int
GPU_TEXTUREFORMAT_ASTC_5x5_UNORM: int
GPU_TEXTUREFORMAT_ASTC_6x5_UNORM: int
GPU_TEXTUREFORMAT_ASTC_6x6_UNORM: int
GPU_TEXTUREFORMAT_ASTC_8x5_UNORM: int
GPU_TEXTUREFORMAT_ASTC_8x6_UNORM: int
GPU_TEXTUREFORMAT_ASTC_8x8_UNORM: int
GPU_TEXTUREFORMAT_ASTC_10x5_UNORM: int
GPU_TEXTUREFORMAT_ASTC_10x6_UNORM: int
GPU_TEXTUREFORMAT_ASTC_10x8_UNORM: int
GPU_TEXTUREFORMAT_ASTC_10x10_UNORM: int
GPU_TEXTUREFORMAT_ASTC_12x10_UNORM: int
GPU_TEXTUREFORMAT_ASTC_12x12_UNORM: int
GPU_SWAPCHAINCOMPOSITION_SDR: int
GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR: int
GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR: int
GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084: int
GPU_PRESENTMODE_VSYNC: int
GPU_PRESENTMODE_MAILBOX: int
GPU_PRESENTMODE_IMMEDIATE: int
GPU_BLENDFACTOR_ZERO: int
GPU_BLENDFACTOR_ONE: int
GPU_BLENDFACTOR_SRC_COLOR: int
GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR: int
GPU_BLENDFACTOR_DST_COLOR: int
GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR: int
GPU_BLENDFACTOR_SRC_ALPHA: int
GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA: int
GPU_BLENDFACTOR_DST_ALPHA: int
GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA: int
GPU_BLENDFACTOR_CONSTANT_COLOR: int
GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR: int
GPU_BLENDFACTOR_SRC_ALPHA_SATURATE: int
GPU_TRANSFERBUFFERUSAGE_UPLOAD: int
GPU_TRANSFERBUFFERUSAGE_DOWNLOAD: int
GPU_SAMPLECOUNT_1: int
GPU_SAMPLECOUNT_2: int
GPU_SAMPLECOUNT_4: int
GPU_SAMPLECOUNT_8: int
GPU_TEXTUREFORMAT_D16_UNORM: int
GPU_TEXTUREFORMAT_D24_UNORM: int
GPU_TEXTUREFORMAT_D32_FLOAT: int
GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT: int
GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT: int
GPU_COMPAREOP_INVALID: int
GPU_COMPAREOP_NEVER: int
GPU_COMPAREOP_LESS: int
GPU_COMPAREOP_EQUAL: int
GPU_COMPAREOP_LESS_OR_EQUAL: int
GPU_COMPAREOP_GREATER: int
GPU_COMPAREOP_NOT_EQUAL: int
GPU_COMPAREOP_GREATER_OR_EQUAL: int
GPU_COMPAREOP_ALWAYS: int
GPU_STENCILOP_INVALID: int
GPU_STENCILOP_KEEP: int
GPU_STENCILOP_ZERO: int
GPU_STENCILOP_REPLACE: int
GPU_STENCILOP_INCREMENT_AND_CLAMP: int
GPU_STENCILOP_DECREMENT_AND_CLAMP: int
GPU_STENCILOP_INVERT: int
GPU_STENCILOP_INCREMENT_AND_WRAP: int
GPU_STENCILOP_DECREMENT_AND_WRAP: int

POSITION_VERTEX: int
POSITION_COLOR_VERTEX: int
POSITION_TEXTURE_VERTEX: int
PUSH_VERTEX: int
PUSH_FRAGMENT: int
PUSH_UNIFORM: int
GPU_VERTEXELEMENTFORMAT_FLOAT: int
GPU_VERTEXELEMENTFORMAT_FLOAT2: int
GPU_VERTEXELEMENTFORMAT_FLOAT3: int
GPU_VERTEXELEMENTFORMAT_FLOAT4: int
GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM: int

def init() -> None: ...
def claim_window(window: Window) -> None: ...
def get_swapchain_format(window: Window) -> int: ...
def set_swapchain_parameters(window: Window, composition: int, present_mode: int) -> None: ...
def supports_swapchain_composition(window: Window, composition: int) -> bool: ...
def supports_present_mode(window: Window, present_mode: int) -> bool: ...
def format_supported(format: int, type: int, usage: int) -> bool: ...
def format_supports_sample_count(format: int, sample_count: int) -> bool: ...
def acquire_swapchain_texture(window: Window) -> Texture | None: ...
def blit_texture(source: Texture, source_w: int, source_h: int, dest: Texture, dest_w: int, dest_h: int, source_layer: int = 0, source_x: int = 0, source_y: int = 0, dest_layer: int = 0, dest_x: int = 0, dest_y: int = 0, load_op: int = ..., filter: int = ..., flip_mode: int = ..., source_mip_level: int = 0, dest_mip_level: int = 0, clear_color: ColorLike = None) -> None: ...
def push_data(structure: Structure, data_type: int) -> None: ...
def submit() -> None: ...
def submit_and_acquire_fence() -> Fence: ...
def wait_for_fences(fences: list[Fence], wait_all: bool = True) -> None: ...
def set_allowed_frames_in_flight(count: int) -> None: ...
def compile_shader(source: str, output: str, stage: int) -> None: ...
def quit() -> None: ...


class Shader:
    def __init__(self, file: FileLike, stage: int, samplers: int = 0, uniform_buffers: int = 0, storage_buffers: int = 0, storage_textures: int = 0): ...


class RenderPass:
    def __init__(self, clear_color: ColorLike, load_op: int, store_op: int): ...
    def begin(self, window: Window | None = None, texture: Texture | None = None, layer: int = 0, cycle: bool = False, resolve_texture: Texture | None = None, depth_stencil_texture: Texture | None = None, depth_clear: float = 0, depth_store_op: int = GPU_STOREOP_DONT_CARE, stencil_store_op: int = GPU_STOREOP_DONT_CARE) -> None: ...
    def end(self) -> None: ...
    def draw_primitives(self, vertices: int, instances: int, vertex_offset: int = 0, indexed: bool = False, index_offset: int = 0) -> None: ...
    def draw_primitives_indirect(self, buffer: Buffer, offset: int = 0, draw_count: int = 1, indexed: bool = False) -> None: ...
    def set_viewport(self, x: float, y: float, w: float, h: float, min_depth: float = 0, max_depth: float = 0) -> None: ...
    def set_scissor(self, rect: RectLike) -> None: ...
    def set_stencil_reference(self, reference: int) -> None: ...


class DepthStencilState:
    def __init__(self, enable_depth_test: bool = False, enable_depth_write: bool = False, enable_stencil_test: bool = False, compare_op: int = GPU_COMPAREOP_INVALID, compare_mask: int = 0xFF, write_mask: int = 0xFF, front_compare_op: int = GPU_COMPAREOP_INVALID, front_fail_op: int = GPU_STENCILOP_KEEP, front_pass_op: int = GPU_STENCILOP_KEEP, front_depth_fail_op: int = GPU_STENCILOP_KEEP, back_compare_op: int = GPU_COMPAREOP_INVALID, back_fail_op: int = GPU_STENCILOP_KEEP, back_pass_op: int = GPU_STENCILOP_KEEP, back_depth_fail_op: int = GPU_STENCILOP_KEEP): ...


class VertexInputState:
    def __init__(self, pitch: int, attributes: list[tuple[int, int]]): ...


class TextureRegion:
    def __init__(self, texture: Texture, mip_level: int = 0, layer: int = 0, x: int = 0, y: int = 0, z: int = 0, w: int = 0, h: int = 0, d: int = 1): ...


class Pipeline:
    def __init__(self, window: Window, vertex_shader: Shader, fragment_shader: Shader, primitive_type: int, fill_mode: int = GPU_FILLMODE_FILL, vertex_input_state: int | VertexInputState | None = None, cull_mode: int = GPU_CULLMODE_NONE, front_face: int = GPU_FRONTFACE_COUNTER_CLOCKWISE, src_color_blendfactor: int = 0, src_alpha_blendfactor: int = 0, dst_color_blendfactor: int = 0, dst_alpha_blendfactor: int = 0, sample_count: int = 0, depth_stencil_state: DepthStencilState | None = None, depth_stencil_format: int = 0, target_format: int = 0): ...
    def bind(self, render_pass: RenderPass, storage_textures: list[Texture] | None = None, vertex_storage_buffers: list[Buffer] | None = None) -> None: ...


class Buffer:
    def __init__(self, usage: int, size: int, buffer_type: int = -1, index_element_size: int = GPU_INDEXELEMENTSIZE_16BIT): ...
    def upload(self, data: list[Any] | SupportsBuffer) -> None: ...
    def bind(self, render_pass: RenderPass) -> None: ...


class Texture:
    def __init__(self, size: Iterable[int], texture_type: int, usage: int, format: int = 12, depth: int = 1, sample_count: int = 0, num_levels: int = 1): ...
    def upload(self, data: Surface | bytes | bytearray, layer: int = 0) -> None: ...
    def generate_mipmaps(self) -> None: ...
    def get_region(self, mip_level: int = 0, layer: int = 0, x: int = 0, y: int = 0, z: int = 0, w: int = 0, h: int = 0, d: int = 1) -> TextureRegion: ...


class Sampler:
    def __init__(self, filter: int, mipmap_mode: int, address_mode: int, anisotropy: float = 0): ...
    def bind(self, render_pass: RenderPass, textures: Texture | list[Texture]) -> None: ...


class ComputePipeline:
    def __init__(self, file: FileLike, threadcount_x: int, threadcount_y: int, threadcount_z: int, readwrite_storage_textures: int = 0, readwrite_storage_buffers: int = 0, readonly_storage_textures: int = 0, readonly_storage_buffers: int = 0, uniform_buffers: int = 0, samplers: int = 0): ...


class ComputePass:
    def __init__(self) -> None: ...
    def begin(self, storage_textures: list[Texture] | None = None, storage_buffers: list[Buffer] | None = None, cycle: bool = False) -> None: ...
    def bind(self, compute_pipeline: ComputePipeline, storage_textures: list[Texture] | None = None, storage_buffers: list[Buffer] | None = None, samplers: list[tuple[Sampler, Texture]] | None = None) -> None: ...
    def dispatch(self, x: int, y: int, z: int) -> None: ...
    def end(self) -> None: ...


class CopyPass:
    def __init__(self) -> None: ...
    def begin(self) -> None: ...
    def end(self) -> None: ...
    def upload_to_texture(self, transfer_buffer: TransferBuffer, region: Texture | TextureRegion, cycle: bool = False, offset: int = 0) -> None: ...
    def upload_to_buffer(self, transfer_buffer: TransferBuffer, buffer: Buffer, size: int, cycle: bool = False) -> None: ...
    def download_from_texture(self, region: Texture | TextureRegion, transfer_buffer: TransferBuffer, offset: int = 0) -> None: ...
    def download_from_buffer(self, buffer: Buffer, transfer_buffer: TransferBuffer, size: int) -> None: ...
    def copy_texture_to_texture(self, source: Texture | TextureRegion, dest: Texture | TextureRegion, w: int, h: int, d: int = 1, cycle: bool = False) -> None: ...
    def copy_buffer_to_buffer(self, source: Buffer, dest: Buffer, size: int, cycle: bool = False) -> None: ...


class TransferBuffer:
    def __init__(self, usage: int, size: int): ...
    def map(self, cycle: bool = False) -> memoryview: ...
    def unmap(self) -> None: ...


class Fence:
    def query(self) -> bool: ...
