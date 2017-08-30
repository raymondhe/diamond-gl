#pragma once

#include "buffer.hpp"
#include <memory>
#include <vector>
#include <tuple>
#include <utility>

namespace NS_NAME {

    class vertex_array;

    template<class... T>
    class vertex_array_binding: public base {
        friend vertex_array;
        vertex_array * glvao;

    public:
        vertex_array_binding(vertex_array& vao, GLuint binding = 0) {
            glvao = &vao;
            this->set_object(std::move(binding));
        }
        ~vertex_array_binding(){
            this->set_object(-1);
        }

        void vertex_buffer(buffer& buf, GLintptr offset = 0);
        void vertex_buffer(std::vector<buffer>& buf, const GLintptr * offsets = 0);
        void vertex_buffer(buffer*bufs, const GLintptr * offsets = 0);
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

        template<class... T>
        void binding(vertex_array_binding<T...>& bnd) {this->binding((GLuint)bnd);}
    };




    class vertex_array: public base {
    public:
        vertex_array() {
            glCreateVertexArrays(1, thisref);
        }
        ~vertex_array() {
            glDeleteVertexArrays(1, thisref);
            this->set_object(-1);
        }

        template<class... T>
        vertex_array_binding<T...> create_binding(GLuint binding = 0) {
            return std::move(vertex_array_binding<T...>(thisref, std::move(binding)));
        }

        vertex_array_attribute create_attribute(GLuint attribute = 0){
            return std::move(vertex_array_attribute(thisref, std::move(attribute)));
        }

        void element_buffer(buffer& buf){
            glVertexArrayElementBuffer(thisref, buf);
        }
    };


    vertex_array_attribute::vertex_array_attribute(vertex_array& vao, GLuint binding) {
        glvao = &vao;
        this->set_object(std::move(binding));
        glEnableVertexArrayAttrib(*glvao, thisref);
    }

    // attribute removing when not using anymore
    vertex_array_attribute::~vertex_array_attribute() {
        //glDisableVertexArrayAttrib(*glvao, thisref);
        //this->set_object(-1);
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



    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(buffer& buf, GLintptr offset) {
        GLsizei * strides = get_stride_wrap<T...>();
        glVertexArrayVertexBuffer(*glvao, thisref, buf, offset, strides[0]);
    }

    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(std::vector<buffer>& bufs, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLsizei * strides = get_stride_wrap<T...>();
        if (!offsets) offsets = new GLintptr[N]{0};
        glVertexArrayVertexBuffers(*glvao, thisref, std::min(N, bufs.size()), bufs.data(), offsets, strides);
    }

    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(buffer * bufs, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLsizei * strides = get_stride_wrap<T...>();
        if (!offsets) offsets = new GLintptr[N]{0};
        glVertexArrayVertexBuffers(*glvao, thisref, N, bufs, offsets, strides);
    }

    


};