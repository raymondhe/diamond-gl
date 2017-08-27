#pragma once

#include "opengl.hpp"

namespace dgl {

    class _dispatch {
    public:
        void compute(GLuint enq) {
            glDispatchCompute(enq, 1, 1);
        }

        void compute(glm::uvec2 enq) {
            glDispatchCompute(enq.x, enq.y, 1);
        }

        void compute(glm::uvec3 enq) {
            glDispatchCompute(enq.x, enq.y, enq.z);
        }

        void compute_indirect(GLintptr indirect = 0) {
            glDispatchComputeIndirect(indirect);
        }
    };

    class _mode: public base {
    public:
        _mode(GLuint mode) {
            base::allocate(1);
            this->set_value(mode);
        }

        void arrays(GLint first, GLsizei count = 1, GLsizei primcount = 1) {
            glDrawArraysInstanced(thisref, first, count, primcount);
        }

        void elements(GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1) {
            glDrawElementsInstanced(thisref, count, type, indices, primcount);
        }

        void elements_base_vertex(GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, GLvoid *indices = nullptr, GLint basevertex = 0) {
            glDrawElementsBaseVertex(thisref, count, type, indices, basevertex);
        }

        void elements_range(glm::ivec2 range, GLsizei count = 1, GLenum type = GL_UNSIGNED_INT, const GLvoid * indices = nullptr, GLsizei primcount = 1) {
            glDrawRangeElements(thisref, range.x, range.y, count, type, indices);
        }
            
        void arrays_indirect(const void *indirect = 0) {
            glDrawArraysIndirect(thisref, indirect);
        }

        void elements_indirect(GLenum type = GL_UNSIGNED_INT, const void *indirect = 0) {
            glDrawElementsIndirect(thisref, type, indirect);
        }
    };


    namespace draw_mode {
        _mode triangles(GL_TRIANGLES);
        _mode points(GL_POINTS);
        _mode triangle_fan(GL_TRIANGLE_FAN);
        _mode triangle_strip(GL_TRIANGLE_STRIP);
        _mode line_strip(GL_LINE_STRIP);
        _mode lines(GL_LINES);
        _mode lines_adjacency(GL_LINES_ADJACENCY);
    };

    _dispatch dispatch;

}