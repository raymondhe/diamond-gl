#pragma once

#include "buffer.hpp"
#include <memory>
#include <vector>
#include <tuple>
#include <utility>

namespace NS_NAME {

    class vertex_array;

    class vertex_array_binding: public base {
        friend vertex_array;
        vertex_array * glvao;

    public:
        vertex_array_binding(vertex_array& vao, GLuint binding = 0) {
            base::allocate(1);
            glvao = &vao;
            this->set_value(binding);
        }

        void vertex_buffer(buffer& buf, GLintptr offset = 0, GLintptr stride = 0);

        template<class T>
        void vertex_buffer(structured_buffer<T>& buf, GLintptr offset = 0);

        template<class T>
        void vertex_buffer(buffer& buf, GLintptr offset = 0);

        // multi-bind not support, because C++ does not support natively different type of vectors



        template<class... T>
        void vertex_buffer(std::tuple<structured_buffer<T>...>& buf, const GLintptr * offsets = nullptr);
    };




    class vertex_array_attribute: public base {
    protected:
        friend vertex_array;
        vertex_array * glvao;

    public:
        ~vertex_array_attribute();
        vertex_array_attribute(vertex_array& vao, GLuint binding = 0);
        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0);
        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0);
        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0);
        void binding(GLuint binding);
        void binding(vertex_array_binding& bnd) {this->binding((GLuint)bnd);}
    };




    class vertex_array: public base {
    public:
        vertex_array() {
            base::allocate(1);
            glCreateVertexArrays(1, thisref);
        }
        ~vertex_array() {
            glDeleteVertexArrays(1, thisref);
        }

        vertex_array_binding&& create_binding(GLuint binding = 0){
            return vertex_array_binding(thisref, binding);
        }

        vertex_array_attribute&& create_attribute(GLuint attribute = 0){
            return vertex_array_attribute(thisref, attribute);
        }

        void element_buffer(buffer& buf){
            glVertexArrayElementBuffer(thisref, buf);
        }
    };


    vertex_array_attribute::~vertex_array_attribute() {
        //glDisableVertexArrayAttrib(*glvao, thisref);
    }

    void vertex_array_attribute::attrib_format(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
        glVertexArrayAttribFormat(*glvao, thisref, size, type, normalized, relativeoffset);
    }

    void vertex_array_attribute::attrib_format_int(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribIFormat(*glvao, thisref, size, type, relativeoffset);
    }

    void vertex_array_attribute::attrib_format_long(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribLFormat(*glvao, thisref, size, type, relativeoffset);
    }

    void vertex_array_attribute::binding(GLuint binding) { // low level function
        glVertexArrayAttribBinding(*glvao, thisref, binding);
    }

    void vertex_array_binding::vertex_buffer(buffer& buf, GLintptr offset, GLintptr stride) {
        glVertexArrayVertexBuffer(*glvao, thisref, (GLuint)buf, offset, stride);
    }

    template<class T>
    void vertex_array_binding::vertex_buffer(structured_buffer<T>& buf, GLintptr offset) {
        glVertexArrayVertexBuffer(*glvao, thisref, (GLuint)buf, offset, sizeof(T));
    }

    template<class T>
    void vertex_array_binding::vertex_buffer(buffer& buf, GLintptr offset) {
        glVertexArrayVertexBuffer(*glvao, thisref, (GLuint)buf, offset, sizeof(T));
    }




    template<class... T>
    void vertex_array_binding::vertex_buffer(std::tuple<structured_buffer<T>...>& buf, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLuint * buffers = get_globj_wrap(buf);;
        GLsizei * strides = get_stride_wrap<T...>();
        if (!offsets) offsets = new GLintptr[N]{0};
        glVertexArrayVertexBuffers(*glvao, thisref, N, buffers, offsets, strides);
    }

    vertex_array_attribute::vertex_array_attribute(vertex_array& vao, GLuint binding) {
        //glvao = std::make_shared<vertex_array>(vao);
        base::allocate(1);
        glvao = &vao;
        this->set_value(binding);
        glEnableVertexArrayAttrib(*glvao, thisref);
    }

};