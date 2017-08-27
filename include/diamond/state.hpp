#pragma once

#include "opengl.hpp"

namespace dgl {

    class _blend {
    public:
        // context based
        void func(GLenum sfactor, GLenum dfactor) {
            glBlendFunc(sfactor, dfactor);
        }

        void func(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
            glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
        }

        void equation(GLenum mode) {
            glBlendEquation(mode);
        }

        void color(glm::vec4 color) {
            glBlendColor(color.x, color.y, color.z, color.w);
        }


        // with draw buffers support
        void func(GLuint draw_buffer, GLenum sfactor, GLenum dfactor) {
            glBlendFunci(draw_buffer, sfactor, dfactor);
        }

        void func(GLuint draw_buffer, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
            glBlendFuncSeparatei(draw_buffer, srcRGB, dstRGB, srcAlpha, dstAlpha);
        }

        void equation(GLuint draw_buffer, GLenum mode) {
            glBlendEquationi(draw_buffer, mode);
        }
    };

    class _clear {
    public:
        void color(glm::vec4 color) {
            glClearColor(color.x, color.y, color.z, color.w);
        }

        void depth(float depth) {
            glClearDepth(depth);
        }

        void enqueue(GLbitfield cbits) {
            glClear(cbits);
        }
    };

    // initialize these classes
    _blend blend;
    _clear clear;


    class _feature: public base {
    public:
        _feature(GLuint feature = 0) {
            base::allocate(1);
            this->set_value(feature);
        }

        void enable() {
            glEnable(thisref);
        }

        void disable() {
            glDisable(thisref);
        }
    };


    namespace options {
        _feature conservative_rasterization_nv(GL_CONSERVATIVE_RASTERIZATION_NV);
        _feature blend(GL_BLEND);
        _feature depth_test(GL_DEPTH_TEST);
        _feature texture_2d(GL_TEXTURE_2D);
    };

    
}