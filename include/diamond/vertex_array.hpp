#pragma once

#include "buffer.hpp"
#include <memory>
#include <vector>
#include <tuple>
#include <utility>
#include <algorithm>

namespace NS_NAME {

    class vertex_array;

    template<class... T>
    class vertex_array_binding {
        friend vertex_array;
        GLuint binding = 0;
        vertex_array * glvao;

    public:
        vertex_array_binding(vertex_array& vao, GLuint binding = 0) {
            this->glvao = &vao;
            this->binding = binding;
        }
        ~vertex_array_binding(){
        }

        void vertex_buffer(buffer& buf, GLintptr offset = 0);
        void vertex_buffer(std::vector<buffer>& buf, const GLintptr * offsets = 0);
        void vertex_buffer(buffer*bufs, const GLintptr * offsets = 0);

        operator GLuint(){
            return binding;
        }
    };



    class vattrib_builder {
    public:
        static GLuint create(vertex_array&vao, GLuint attribute);
        static void release(GLuint * heap);
    };



    class vertex_array_attribute: public gl_object<vattrib_builder> {
    protected:
        using base = gl_object<vattrib_builder>;
        friend vertex_array;
        vertex_array * glvao;

    public:

        // constructor
        vertex_array_attribute(vertex_array& vao, GLuint attribute) { base::create_heap(vao, attribute); glvao = &vao; }
        vertex_array_attribute(vertex_array_attribute& another) { base::move(another); } // copy (it refs)
        vertex_array_attribute(vertex_array_attribute&& another) { base::move(std::forward<vertex_array_attribute>(another)); } // move
        vertex_array_attribute(GLuint * another) { base::move(another); } // heap by ptr


        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0);
        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0);
        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0);
        void binding(GLuint binding);

        template<class... T>
        void binding(vertex_array_binding<T...>& bnd) {this->binding((GLuint)bnd);}
    };


    class vertex_array_builder {
    public:
        static void create(GLuint * heap) {
            glCreateVertexArrays(1, heap);
        };
        static void release(GLuint * heap) {
            glDeleteVertexArrays(1, heap);
        };
    };


    class vertex_array: public gl_object<vertex_array_builder> {
    protected:
        using base = gl_object<vertex_array_builder>;
        
    public:

        // constructor (variadic)
        template<class ...ARG>
        vertex_array(ARG&&... args) { base::create_alloc(std::forward<ARG>(args)...); }
        vertex_array(vertex_array& another) { base::move(another); } // copy (it refs)
        vertex_array(vertex_array&& another) { base::move(std::forward<vertex_array>(another)); } // move
        vertex_array(GLuint * another) { base::move(another); } // heap by ptr

        template<class... T>
        vertex_array_binding<T...> create_binding(GLuint binding = 0) {
            return vertex_array_binding<T...>(thisref, binding);
        }

        vertex_array_attribute create_attribute(GLuint attribute = 0){
            return vertex_array_attribute(thisref, attribute);
        }

        void element_buffer(buffer& buf){
            glVertexArrayElementBuffer(thisref, buf);
        }
    };


    GLuint vattrib_builder::create(vertex_array& vao, GLuint attrib){
        glEnableVertexArrayAttrib(vao, attrib);
        return attrib;
    }

    // at now unsupported
    void vattrib_builder::release(GLuint * heap){
        //glDisableVertexArrayAttrib(vao, *heap);
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
        bool hadOffsets = !!offsets;
        if (!hadOffsets) offsets = new GLintptr[N]{ 0 };
        GLuint * bufsp = new GLuint[N];
        const size_t Nv = std::min(N, bufs.size());
        for (int i = 0; i < Nv; i++) bufsp[i] = bufs[i];
        glVertexArrayVertexBuffers(*glvao, thisref, Nv, bufsp, offsets, strides);
        if (!hadOffsets) delete offsets;
        delete bufsp;
    }

    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(buffer * bufs, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLsizei * strides = get_stride_wrap<T...>();
        bool hadOffsets = !!offsets;
        if (!hadOffsets) offsets = new GLintptr[N]{ 0 };
        glVertexArrayVertexBuffers(*glvao, thisref, N, bufs, offsets, strides);
        if (!hadOffsets) delete offsets;
    }

    


};