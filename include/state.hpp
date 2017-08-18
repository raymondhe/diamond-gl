#pragma once

#include "opengl.hpp"

namespace dgl {
    namespace state {

        // context based
        void blend_func(GLenum sfactor, GLenum dfactor){
            glBlendFunc(sfactor, dfactor);
        }

        void blend_func(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
            glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
        }

        void blend_equation(GLenum mode){
            glBlendEquation(mode);
        }

        void blend_color(glm::vec4 color){
            glBlendColor(color.x, color.y, color.z, color.w);
        }

        void logic_op(GLenum opcode){
            glLogicOp(opcode);
        }



        // with draw buffers support
        void blend_func(GLuint draw_buffer, GLenum sfactor, GLenum dfactor){
            glBlendFunci(draw_buffer, sfactor, dfactor);
        }

        void blend_func(GLuint draw_buffer, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
            glBlendFuncSeparatei(draw_buffer, srcRGB, dstRGB, srcAlpha, dstAlpha);
        }

        void blend_equation(GLuint draw_buffer, GLenum mode){
            glBlendEquationi(draw_buffer, mode);
        }



        // draw

        void draw_arrays(GLenum mode, GLint first, GLsizei count = 1, GLsizei primcount = 1){
            glDrawArraysInstanced(mode, first, count, primcount);
        }

        void draw_elements(GLenum mode, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1){
            glDrawElementsInstanced(mode, count, type, indices, primcount);
        }

        void draw_elements_base_vertex(GLenum mode, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, GLvoid *indices = nullptr, GLint basevertex = 0){
            glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
        }

        void draw_elements_range(GLenum mode, glm::ivec2 range, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1){
            glDrawRangeElements(mode, range.x, range.y, count, type, indices);
        }


    };
}