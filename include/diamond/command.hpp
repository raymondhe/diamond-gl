#pragma once

#include "opengl.hpp"

namespace dgl {

    namespace commands {
        void clear(GLbitfield cbits) {
            glClear(cbits);
        }

        namespace dispatch {
            void compute(GLuint enq){
                glDispatchCompute(enq, 1, 1);
            }

            void compute(glm::uvec2 enq){
                glDispatchCompute(enq.x, enq.y, 1);
            }

            void compute(glm::uvec3 enq){
                glDispatchCompute(enq.x, enq.y, enq.z);
            }

            void compute_indirect(GLintptr indirect = 0){
                glDispatchComputeIndirect(indirect);
            }
        };


        namespace multi_draw {

        };


        namespace draw {

            void arrays(GLenum mode, GLint first, GLsizei count = 1, GLsizei primcount = 1) {
                glDrawArraysInstanced(mode, first, count, primcount);
            }

            void elements(GLenum mode, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1) {
                glDrawElementsInstanced(mode, count, type, indices, primcount);
            }

            void elements_base_vertex(GLenum mode, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, GLvoid *indices = nullptr, GLint basevertex = 0) {
                glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
            }

            void elements_range(GLenum mode, glm::ivec2 range, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1) {
                glDrawRangeElements(mode, range.x, range.y, count, type, indices);
            }



            void arrays_indirect(GLenum mode, const void *indirect = 0){
                glDrawArraysIndirect(mode, indirect);
            }

            void elements_indirect(GLenum mode, GLenum type = GL_UNSIGNED_INT, const void *indirect = 0){
                glDrawElementsIndirect(mode, type, indirect);
            }

        }

    }
}