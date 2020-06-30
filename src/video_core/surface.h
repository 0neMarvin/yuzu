// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <climits>
#include <utility>
#include "common/assert.h"
#include "common/common_types.h"
#include "common/logging/log.h"
#include "video_core/gpu.h"
#include "video_core/textures/texture.h"

namespace VideoCore::Surface {

enum class PixelFormat {
    ABGR8U,
    ABGR8S,
    ABGR8UI,
    B5G6R5U,
    A2B10G10R10U,
    A1B5G5R5U,
    R8U,
    R8S,
    R8I,
    R8UI,
    RGBA16F,
    RGBA16U,
    RGBA16S,
    RGBA16UI,
    R11FG11FB10F,
    RGBA32UI,
    DXT1,
    DXT23,
    DXT45,
    DXN1, // This is also known as BC4
    DXN2UNORM,
    DXN2SNORM,
    BC7U,
    BC6H_UF16,
    BC6H_SF16,
    ASTC_2D_4X4,
    BGRA8,
    RGBA32F,
    RG32F,
    RG32I,
    R32F,
    R16F,
    R16U,
    R16S,
    R16UI,
    R16I,
    RG16,
    RG16F,
    RG16UI,
    RG16I,
    RG16S,
    RGB32F,
    RGBA8_SRGB,
    RG8U,
    RG8S,
    RG8I,
    RG8UI,
    RG32UI,
    RGBX16F,
    R32UI,
    R32I,
    ASTC_2D_8X8,
    ASTC_2D_8X5,
    ASTC_2D_5X4,
    BGRA8_SRGB,
    DXT1_SRGB,
    DXT23_SRGB,
    DXT45_SRGB,
    BC7U_SRGB,
    R4G4B4A4U,
    ASTC_2D_4X4_SRGB,
    ASTC_2D_8X8_SRGB,
    ASTC_2D_8X5_SRGB,
    ASTC_2D_5X4_SRGB,
    ASTC_2D_5X5,
    ASTC_2D_5X5_SRGB,
    ASTC_2D_10X8,
    ASTC_2D_10X8_SRGB,
    ASTC_2D_6X6,
    ASTC_2D_6X6_SRGB,
    ASTC_2D_10X10,
    ASTC_2D_10X10_SRGB,
    ASTC_2D_12X12,
    ASTC_2D_12X12_SRGB,
    ASTC_2D_8X6,
    ASTC_2D_8X6_SRGB,
    ASTC_2D_6X5,
    ASTC_2D_6X5_SRGB,
    E5B9G9R9F,

    MaxColorFormat,

    // Depth formats
    Z32F = MaxColorFormat,
    Z16,

    MaxDepthFormat,

    // DepthStencil formats
    Z24S8 = MaxDepthFormat,
    S8Z24,
    Z32FS8,

    MaxDepthStencilFormat,

    Max = MaxDepthStencilFormat,
    Invalid = 255,
};
static constexpr std::size_t MaxPixelFormat = static_cast<std::size_t>(PixelFormat::Max);

enum class SurfaceType {
    ColorTexture = 0,
    Depth = 1,
    DepthStencil = 2,
    Invalid = 3,
};

enum class SurfaceTarget {
    Texture1D,
    TextureBuffer,
    Texture2D,
    Texture3D,
    Texture1DArray,
    Texture2DArray,
    TextureCubemap,
    TextureCubeArray,
};

constexpr std::array<u32, MaxPixelFormat> compression_factor_shift_table = {{
    0, // ABGR8U
    0, // ABGR8S
    0, // ABGR8UI
    0, // B5G6R5U
    0, // A2B10G10R10U
    0, // A1B5G5R5U
    0, // R8U
    0, // R8S
    0, // R8I
    0, // R8UI
    0, // RGBA16F
    0, // RGBA16U
    0, // RGBA16S
    0, // RGBA16UI
    0, // R11FG11FB10F
    0, // RGBA32UI
    2, // DXT1
    2, // DXT23
    2, // DXT45
    2, // DXN1
    2, // DXN2UNORM
    2, // DXN2SNORM
    2, // BC7U
    2, // BC6H_UF16
    2, // BC6H_SF16
    2, // ASTC_2D_4X4
    0, // BGRA8
    0, // RGBA32F
    0, // RG32F
    0, // RG32I
    0, // R32F
    0, // R16F
    0, // R16U
    0, // R16S
    0, // R16UI
    0, // R16I
    0, // RG16
    0, // RG16F
    0, // RG16UI
    0, // RG16I
    0, // RG16S
    0, // RGB32F
    0, // RGBA8_SRGB
    0, // RG8U
    0, // RG8S
    0, // RG8I
    0, // RG8UI
    0, // RG32UI
    0, // RGBX16F
    0, // R32UI
    0, // R32I
    2, // ASTC_2D_8X8
    2, // ASTC_2D_8X5
    2, // ASTC_2D_5X4
    0, // BGRA8_SRGB
    2, // DXT1_SRGB
    2, // DXT23_SRGB
    2, // DXT45_SRGB
    2, // BC7U_SRGB
    0, // R4G4B4A4U
    2, // ASTC_2D_4X4_SRGB
    2, // ASTC_2D_8X8_SRGB
    2, // ASTC_2D_8X5_SRGB
    2, // ASTC_2D_5X4_SRGB
    2, // ASTC_2D_5X5
    2, // ASTC_2D_5X5_SRGB
    2, // ASTC_2D_10X8
    2, // ASTC_2D_10X8_SRGB
    2, // ASTC_2D_6X6
    2, // ASTC_2D_6X6_SRGB
    2, // ASTC_2D_10X10
    2, // ASTC_2D_10X10_SRGB
    2, // ASTC_2D_12X12
    2, // ASTC_2D_12X12_SRGB
    2, // ASTC_2D_8X6
    2, // ASTC_2D_8X6_SRGB
    2, // ASTC_2D_6X5
    2, // ASTC_2D_6X5_SRGB
    0, // E5B9G9R9F
    0, // Z32F
    0, // Z16
    0, // Z24S8
    0, // S8Z24
    0, // Z32FS8
}};

/**
 * Gets the compression factor for the specified PixelFormat. This applies to just the
 * "compressed width" and "compressed height", not the overall compression factor of a
 * compressed image. This is used for maintaining proper surface sizes for compressed
 * texture formats.
 */
inline constexpr u32 GetCompressionFactorShift(PixelFormat format) {
    DEBUG_ASSERT(format != PixelFormat::Invalid);
    DEBUG_ASSERT(static_cast<std::size_t>(format) < compression_factor_shift_table.size());
    return compression_factor_shift_table[static_cast<std::size_t>(format)];
}

inline constexpr u32 GetCompressionFactor(PixelFormat format) {
    return 1U << GetCompressionFactorShift(format);
}

constexpr std::array<u32, MaxPixelFormat> block_width_table = {{
    1,  // ABGR8U
    1,  // ABGR8S
    1,  // ABGR8UI
    1,  // B5G6R5U
    1,  // A2B10G10R10U
    1,  // A1B5G5R5U
    1,  // R8U
    1,  // R8S
    1,  // R8I
    1,  // R8UI
    1,  // RGBA16F
    1,  // RGBA16U
    1,  // RGBA16S
    1,  // RGBA16UI
    1,  // R11FG11FB10F
    1,  // RGBA32UI
    4,  // DXT1
    4,  // DXT23
    4,  // DXT45
    4,  // DXN1
    4,  // DXN2UNORM
    4,  // DXN2SNORM
    4,  // BC7U
    4,  // BC6H_UF16
    4,  // BC6H_SF16
    4,  // ASTC_2D_4X4
    1,  // BGRA8
    1,  // RGBA32F
    1,  // RG32F
    1,  // RG32I
    1,  // R32F
    1,  // R16F
    1,  // R16U
    1,  // R16S
    1,  // R16UI
    1,  // R16I
    1,  // RG16
    1,  // RG16F
    1,  // RG16UI
    1,  // RG16I
    1,  // RG16S
    1,  // RGB32F
    1,  // RGBA8_SRGB
    1,  // RG8U
    1,  // RG8S
    1,  // RG8I
    1,  // RG8UI
    1,  // RG32UI
    1,  // RGBX16F
    1,  // R32UI
    1,  // R32I
    8,  // ASTC_2D_8X8
    8,  // ASTC_2D_8X5
    5,  // ASTC_2D_5X4
    1,  // BGRA8_SRGB
    4,  // DXT1_SRGB
    4,  // DXT23_SRGB
    4,  // DXT45_SRGB
    4,  // BC7U_SRGB
    1,  // R4G4B4A4U
    4,  // ASTC_2D_4X4_SRGB
    8,  // ASTC_2D_8X8_SRGB
    8,  // ASTC_2D_8X5_SRGB
    5,  // ASTC_2D_5X4_SRGB
    5,  // ASTC_2D_5X5
    5,  // ASTC_2D_5X5_SRGB
    10, // ASTC_2D_10X8
    10, // ASTC_2D_10X8_SRGB
    6,  // ASTC_2D_6X6
    6,  // ASTC_2D_6X6_SRGB
    10, // ASTC_2D_10X10
    10, // ASTC_2D_10X10_SRGB
    12, // ASTC_2D_12X12
    12, // ASTC_2D_12X12_SRGB
    8,  // ASTC_2D_8X6
    8,  // ASTC_2D_8X6_SRGB
    6,  // ASTC_2D_6X5
    6,  // ASTC_2D_6X5_SRGB
    1,  // E5B9G9R9F
    1,  // Z32F
    1,  // Z16
    1,  // Z24S8
    1,  // S8Z24
    1,  // Z32FS8
}};

static constexpr u32 GetDefaultBlockWidth(PixelFormat format) {
    if (format == PixelFormat::Invalid)
        return 0;

    ASSERT(static_cast<std::size_t>(format) < block_width_table.size());
    return block_width_table[static_cast<std::size_t>(format)];
}

constexpr std::array<u32, MaxPixelFormat> block_height_table = {{
    1,  // ABGR8U
    1,  // ABGR8S
    1,  // ABGR8UI
    1,  // B5G6R5U
    1,  // A2B10G10R10U
    1,  // A1B5G5R5U
    1,  // R8U
    1,  // R8S
    1,  // R8I
    1,  // R8UI
    1,  // RGBA16F
    1,  // RGBA16U
    1,  // RGBA16S
    1,  // RGBA16UI
    1,  // R11FG11FB10F
    1,  // RGBA32UI
    4,  // DXT1
    4,  // DXT23
    4,  // DXT45
    4,  // DXN1
    4,  // DXN2UNORM
    4,  // DXN2SNORM
    4,  // BC7U
    4,  // BC6H_UF16
    4,  // BC6H_SF16
    4,  // ASTC_2D_4X4
    1,  // BGRA8
    1,  // RGBA32F
    1,  // RG32F
    1,  // RG32I
    1,  // R32F
    1,  // R16F
    1,  // R16U
    1,  // R16S
    1,  // R16UI
    1,  // R16I
    1,  // RG16
    1,  // RG16F
    1,  // RG16UI
    1,  // RG16I
    1,  // RG16S
    1,  // RGB32F
    1,  // RGBA8_SRGB
    1,  // RG8U
    1,  // RG8S
    1,  // RG8I
    1,  // RG8UI
    1,  // RG32UI
    1,  // RGBX16F
    1,  // R32UI
    1,  // R32I
    8,  // ASTC_2D_8X8
    5,  // ASTC_2D_8X5
    4,  // ASTC_2D_5X4
    1,  // BGRA8_SRGB
    4,  // DXT1_SRGB
    4,  // DXT23_SRGB
    4,  // DXT45_SRGB
    4,  // BC7U_SRGB
    1,  // R4G4B4A4U
    4,  // ASTC_2D_4X4_SRGB
    8,  // ASTC_2D_8X8_SRGB
    5,  // ASTC_2D_8X5_SRGB
    4,  // ASTC_2D_5X4_SRGB
    5,  // ASTC_2D_5X5
    5,  // ASTC_2D_5X5_SRGB
    8,  // ASTC_2D_10X8
    8,  // ASTC_2D_10X8_SRGB
    6,  // ASTC_2D_6X6
    6,  // ASTC_2D_6X6_SRGB
    10, // ASTC_2D_10X10
    10, // ASTC_2D_10X10_SRGB
    12, // ASTC_2D_12X12
    12, // ASTC_2D_12X12_SRGB
    6,  // ASTC_2D_8X6
    6,  // ASTC_2D_8X6_SRGB
    5,  // ASTC_2D_6X5
    5,  // ASTC_2D_6X5_SRGB
    1,  // E5B9G9R9F
    1,  // Z32F
    1,  // Z16
    1,  // Z24S8
    1,  // S8Z24
    1,  // Z32FS8
}};

static constexpr u32 GetDefaultBlockHeight(PixelFormat format) {
    if (format == PixelFormat::Invalid)
        return 0;

    ASSERT(static_cast<std::size_t>(format) < block_height_table.size());
    return block_height_table[static_cast<std::size_t>(format)];
}

constexpr std::array<u32, MaxPixelFormat> bpp_table = {{
    32,  // ABGR8U
    32,  // ABGR8S
    32,  // ABGR8UI
    16,  // B5G6R5U
    32,  // A2B10G10R10U
    16,  // A1B5G5R5U
    8,   // R8U
    8,   // R8S
    8,   // R8I
    8,   // R8UI
    64,  // RGBA16F
    64,  // RGBA16U
    64,  // RGBA16S
    64,  // RGBA16UI
    32,  // R11FG11FB10F
    128, // RGBA32UI
    64,  // DXT1
    128, // DXT23
    128, // DXT45
    64,  // DXN1
    128, // DXN2UNORM
    128, // DXN2SNORM
    128, // BC7U
    128, // BC6H_UF16
    128, // BC6H_SF16
    128, // ASTC_2D_4X4
    32,  // BGRA8
    128, // RGBA32F
    64,  // RG32F
    64,  // RG32I
    32,  // R32F
    16,  // R16F
    16,  // R16U
    16,  // R16S
    16,  // R16UI
    16,  // R16I
    32,  // RG16
    32,  // RG16F
    32,  // RG16UI
    32,  // RG16I
    32,  // RG16S
    96,  // RGB32F
    32,  // RGBA8_SRGB
    16,  // RG8U
    16,  // RG8S
    16,  // RG8I
    16,  // RG8UI
    64,  // RG32UI
    64,  // RGBX16F
    32,  // R32UI
    32,  // R32I
    128, // ASTC_2D_8X8
    128, // ASTC_2D_8X5
    128, // ASTC_2D_5X4
    32,  // BGRA8_SRGB
    64,  // DXT1_SRGB
    128, // DXT23_SRGB
    128, // DXT45_SRGB
    128, // BC7U
    16,  // R4G4B4A4U
    128, // ASTC_2D_4X4_SRGB
    128, // ASTC_2D_8X8_SRGB
    128, // ASTC_2D_8X5_SRGB
    128, // ASTC_2D_5X4_SRGB
    128, // ASTC_2D_5X5
    128, // ASTC_2D_5X5_SRGB
    128, // ASTC_2D_10X8
    128, // ASTC_2D_10X8_SRGB
    128, // ASTC_2D_6X6
    128, // ASTC_2D_6X6_SRGB
    128, // ASTC_2D_10X10
    128, // ASTC_2D_10X10_SRGB
    128, // ASTC_2D_12X12
    128, // ASTC_2D_12X12_SRGB
    128, // ASTC_2D_8X6
    128, // ASTC_2D_8X6_SRGB
    128, // ASTC_2D_6X5
    128, // ASTC_2D_6X5_SRGB
    32,  // E5B9G9R9F
    32,  // Z32F
    16,  // Z16
    32,  // Z24S8
    32,  // S8Z24
    64,  // Z32FS8
}};

static constexpr u32 GetFormatBpp(PixelFormat format) {
    if (format == PixelFormat::Invalid)
        return 0;

    ASSERT(static_cast<std::size_t>(format) < bpp_table.size());
    return bpp_table[static_cast<std::size_t>(format)];
}

/// Returns the sizer in bytes of the specified pixel format
static constexpr u32 GetBytesPerPixel(PixelFormat pixel_format) {
    if (pixel_format == PixelFormat::Invalid) {
        return 0;
    }
    return GetFormatBpp(pixel_format) / CHAR_BIT;
}

SurfaceTarget SurfaceTargetFromTextureType(Tegra::Texture::TextureType texture_type);

bool SurfaceTargetIsLayered(SurfaceTarget target);

bool SurfaceTargetIsArray(SurfaceTarget target);

PixelFormat PixelFormatFromDepthFormat(Tegra::DepthFormat format);

PixelFormat PixelFormatFromRenderTargetFormat(Tegra::RenderTargetFormat format);

PixelFormat PixelFormatFromGPUPixelFormat(Tegra::FramebufferConfig::PixelFormat format);

SurfaceType GetFormatType(PixelFormat pixel_format);

bool IsPixelFormatASTC(PixelFormat format);

bool IsPixelFormatSRGB(PixelFormat format);

std::pair<u32, u32> GetASTCBlockSize(PixelFormat format);

/// Returns true if the specified PixelFormat is a BCn format, e.g. DXT or DXN
bool IsFormatBCn(PixelFormat format);

} // namespace VideoCore::Surface
