#pragma once

#include "buffer.hpp"
#include <memory>
#include <vector>
#include <tuple>
#include <utility>

namespace dgl {

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
        void vertex_buffer(std::tuple<structured_buffer<T>...>& buf, const GLintptr * offsets);
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





    // crop tuple element
    template <class Tm, class... T, size_t... Is>
    constexpr decltype(auto) get_gl_buf_reduce_impl(GLuint * rest, std::tuple<Tm, T...> &t, std::index_sequence<Is...>) {
        *rest = (GLuint)std::get<0>(t); return std::make_tuple(std::get<Is + 1>(t)...);
    }

    template <class... T>
    constexpr decltype(auto) get_gl_buf_reduce(GLuint * rest, std::tuple<T...> &t) {
        return get_gl_buf_reduce_impl(rest, t, std::make_index_sequence<(sizeof...(T)) - 1>{});
    }

    // getting gl buffers
    template <class T>
    constexpr decltype(auto) get_gl_buf_impl(GLuint * ptr, std::tuple<T> &t) {
        *ptr = (GLuint)std::get<0>(t);
    }

    template <class... T>
    constexpr decltype(auto) get_gl_buf_impl(GLuint * ptr, std::tuple<T...> &t) {
        decltype(auto) rest_tuple = get_gl_buf_reduce<T...>(ptr, t);
        get_gl_buf_impl(ptr + 1, rest_tuple);
    }

    template <class... T>
    constexpr decltype(auto) get_gl_buf(GLuint * ptr, std::tuple<T...> &t) {
        return get_gl_buf_impl(ptr, t);
    }




    // crop tuple element
    template <class Tm, class... T, size_t... Is>
    constexpr decltype(auto) get_gl_buf_reduce_impl(GLsizei * rest, std::tuple<structured_buffer<Tm>, structured_buffer<T>...> &t, std::index_sequence<Is...>) {
        *rest = (GLsizei)sizeof(typename std::tuple_element<0, std::tuple<T...>>::type); return std::make_tuple(std::get<Is + 1>(t)...);
    }

    template <class... T>
    constexpr decltype(auto) get_gl_buf_reduce(GLsizei * rest, std::tuple<structured_buffer<T>...> &t) {
        return get_gl_buf_reduce_impl(rest, t, std::make_index_sequence<(sizeof...(T)) - 1>{});
    }

    // getting gl buffers
    template <class T>
    constexpr decltype(auto) get_stride_impl(GLsizei * ptr, std::tuple<structured_buffer<T>> &t) {
        *ptr = sizeof(typename std::tuple_element<0, std::tuple<T>>::type);
    }

    template <class... T>
    constexpr decltype(auto) get_stride_impl(GLsizei * ptr, std::tuple<structured_buffer<T>...> &t) {
        decltype(auto) rest_tuple = get_gl_buf_reduce<T...>(ptr, t);
        get_stride_impl(ptr + 1, rest_tuple);
    }

    template <class... T>
    constexpr decltype(auto) get_stride(GLsizei * ptr, std::tuple<structured_buffer<T>...> &t) {
        return get_stride_impl(ptr, t);
    }



    template<class... T>
    void vertex_array_binding::vertex_buffer(std::tuple<structured_buffer<T>...>& buf, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLuint * buffers = new GLuint[N];
        GLsizei * strides = new GLsizei[N];
        get_gl_buf(buffers, buf);
        get_stride<T...>(strides, buf);
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