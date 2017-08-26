#pragma once

#include "opengl.hpp"

namespace dgl {
    namespace state {

        namespace blend {

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
        }

        // logic
        namespace logic {
            void op(GLenum opcode) {
                glLogicOp(opcode);
            }
        }

        // clear
        namespace clear {
            void color(glm::vec4 color) {
                glClearColor(color.x, color.y, color.z, color.w);
            }

            void depth(float depth) {
                glClearDepth(depth);
            }
        };
    };
    
}