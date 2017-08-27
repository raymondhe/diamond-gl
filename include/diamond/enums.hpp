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
        _internal_format rgba32f(GL_RGBA32F, GL_RGBA, GL_FLOAT);
        _internal_format rgba16f(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);

        _internal_format rgba8(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        _internal_format rgba8_snorm(GL_RGBA8_SNORM, GL_RGBA, GL_BYTE);
        _internal_format rgba8ui(GL_RGBA8UI, GL_RGBA, GL_UNSIGNED_BYTE);
        _internal_format rgba8i(GL_RGBA8I, GL_RGBA, GL_BYTE);

        _internal_format rgba16(GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT);
        _internal_format rgba16_snorm(GL_RGBA16_SNORM, GL_RGBA, GL_SHORT);
        _internal_format rgba16ui(GL_RGBA16UI, GL_RGBA, GL_UNSIGNED_SHORT);
        _internal_format rgba16i(GL_RGBA16I, GL_RGBA, GL_SHORT);

        _internal_format rgba32ui(GL_RGBA16UI, GL_RGBA, GL_UNSIGNED_INT);
        _internal_format rgba32i(GL_RGBA16I, GL_RGBA, GL_INT);
    };

};