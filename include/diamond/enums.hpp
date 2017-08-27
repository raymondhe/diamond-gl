#pragma once

#include "opengl.hpp"

namespace dgl {

    class _internal_format: public base {
        GLuint _format;
        GLuint _type;

    public:
        _internal_format(GLenum internalFormat, GLenum generalFormat, GLenum generalType): _format(generalFormat), _type(generalType)
        {
            this->set_object(internalFormat);
        }

        GLuint format() const {
            return _format;
        }

        GLuint type() const {
            return _type;
        }
    };


    namespace internal_format {
        // RGBA
        _internal_format rgba32f(GL_RGBA32F, GL_RGBA, GL_FLOAT);
        _internal_format rgba16f(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);
        _internal_format rgba8_unorm(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        _internal_format rgba8_snorm(GL_RGBA8_SNORM, GL_RGBA_SNORM, GL_BYTE);
        _internal_format rgba8ui(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rgba8i(GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE);
        _internal_format rgba16_unorm(GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT);
        _internal_format rgba16_snorm(GL_RGBA16_SNORM, GL_RGBA_SNORM, GL_SHORT);
        _internal_format rgba16ui(GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rgba16i(GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT);
        _internal_format rgba32ui(GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT);
        _internal_format rgba32i(GL_RGBA16I, GL_RGBA_INTEGER, GL_INT);

        // RGB
        _internal_format rgb32f(GL_RGB32F, GL_RGB, GL_FLOAT);
        _internal_format rgb16f(GL_RGB16F, GL_RGB, GL_HALF_FLOAT);
        _internal_format rgb8_unorm(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
        _internal_format rgb8_snorm(GL_RGB8_SNORM, GL_RGB_SNORM, GL_BYTE);
        _internal_format rgb8ui(GL_RGB8UI, GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rgb8i(GL_RGB8I, GL_RGB_INTEGER, GL_BYTE);
        _internal_format rgb16_unorm(GL_RGB16, GL_RGB, GL_UNSIGNED_SHORT);
        _internal_format rgb16_snorm(GL_RGB16_SNORM, GL_RGB_SNORM, GL_SHORT);
        _internal_format rgb16ui(GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rgb16i(GL_RGB16I, GL_RGB_INTEGER, GL_SHORT);
        _internal_format rgb32ui(GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_INT);
        _internal_format rgb32i(GL_RGB16I, GL_RGB_INTEGER, GL_INT);

        // RG
        _internal_format rg32f(GL_RG32F, GL_RG, GL_FLOAT);
        _internal_format rg16f(GL_RG16F, GL_RG, GL_HALF_FLOAT);
        _internal_format rg8_unorm(GL_RG8, GL_RG, GL_UNSIGNED_BYTE);
        _internal_format rg8_snorm(GL_RG8_SNORM, GL_RG_SNORM, GL_BYTE);
        _internal_format rg8ui(GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rg8i(GL_RG8I, GL_RG_INTEGER, GL_BYTE);
        _internal_format rg16_unorm(GL_RG16, GL_RG, GL_UNSIGNED_SHORT);
        _internal_format rg16_snorm(GL_RG16_SNORM, GL_RG_SNORM, GL_SHORT);
        _internal_format rg16ui(GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rg16i(GL_RG16I, GL_RG_INTEGER, GL_SHORT);
        _internal_format rg32ui(GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_INT);
        _internal_format rg32i(GL_RG16I, GL_RG_INTEGER, GL_INT);

        // Red
        _internal_format r32f(GL_R32F, GL_RED, GL_FLOAT);
        _internal_format r16f(GL_R16F, GL_RED, GL_HALF_FLOAT);
        _internal_format r8_unorm(GL_R8, GL_RED, GL_UNSIGNED_BYTE);
        _internal_format r8_snorm(GL_R8_SNORM, GL_RED_SNORM, GL_BYTE);
        _internal_format r8ui(GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format r8i(GL_R8I, GL_RED_INTEGER, GL_BYTE);
        _internal_format r16_unorm(GL_R16, GL_RED, GL_UNSIGNED_SHORT);
        _internal_format r16_snorm(GL_R16_SNORM, GL_RED_SNORM, GL_SHORT);
        _internal_format r16ui(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format r16i(GL_R16I, GL_RED_INTEGER, GL_SHORT);
        _internal_format r32ui(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
        _internal_format r32i(GL_R16I, GL_RED_INTEGER, GL_INT);
    };

};