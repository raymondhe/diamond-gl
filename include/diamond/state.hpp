#pragma once

#include "opengl.hpp"

namespace NS_NAME {

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
    };

    class _feature: public base {
    public:
        _feature(GLuint &feature) {
            this->set_object(feature);
        }
        _feature(GLuint &&feature) {
            this->set_object(std::forward<GLuint>(feature));
        }

        void enable() {
            glEnable(thisref);
        }

        void disable() {
            glDisable(thisref);
        }
    };

    class _feature_control {
    public:
        void enable(_feature& feature) {
            feature.enable();
        }

        void disable(_feature& feature) {
            feature.disable();
        }
    };


    namespace options {
        _feature conservative_rasterization_nv(GL_CONSERVATIVE_RASTERIZATION_NV);
        _feature blend(GL_BLEND);
        _feature depth_test(GL_DEPTH_TEST);
        _feature texture_2d(GL_TEXTURE_2D);
    };



    // initialize these classes
    _blend blend;
    _clear clear;
    _feature_control option;

    
}