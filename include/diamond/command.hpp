#pragma once

#include "opengl.hpp"

namespace dgl {

    namespace commands {
        void clear(GLbitfield cbits) {
            glClear(cbits);
        }

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
        }

    }
}