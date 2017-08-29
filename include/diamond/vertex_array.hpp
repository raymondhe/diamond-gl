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
        vertex_array_binding(vertex_array& vao, GLuint&& binding = 0) {
            glvao = &vao;
            this->set_object(std::forward<GLuint>(binding));
        }

        vertex_array_binding(vertex_array& vao, GLuint& binding) {
            glvao = &vao;
            this->set_object(binding);
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
    
    template<class... T>
    class vertex_array_binding_multi : public vertex_array_binding {
    public:
        vertex_array_binding_multi<T...>(vertex_array& vao, GLuint&& binding = 0) : vertex_array_binding(vao, std::forward<GLuint>(binding)) {};
        vertex_array_binding_multi<T...>(vertex_array& vao, GLuint& binding) : vertex_array_binding(vao, binding) {};

        void vertex_buffer(std::tuple<structured_buffer<T>...>& buf, const GLintptr * offsets = nullptr) {
            vertex_array_binding::vertex_buffer<T...>(buf, offsets);
        };
    };

    template<class T>
    class vertex_array_binding_single : public vertex_array_binding {
    public:
        vertex_array_binding_single<T>(vertex_array& vao, GLuint&& binding = 0) : vertex_array_binding(vao, std::forward<GLuint>(binding)) {};
        vertex_array_binding_single<T>(vertex_array& vao, GLuint& binding) : vertex_array_binding(vao, binding) {};

        void vertex_buffer(buffer& buf, GLintptr offset = 0) {
            vertex_array_binding::vertex_buffer<T>(buf, offset);
        };
        void vertex_buffer(structured_buffer<T>& buf, GLintptr offset = 0) {
            vertex_array_binding::vertex_buffer<T>(buf, offset);
        };
        void vertex_buffer(std::tuple<structured_buffer<T>>& buf, const GLintptr * offsets = nullptr) {
            vertex_array_binding::vertex_buffer<T>(buf, offsets);
        };
    };

    class vertex_array_attribute: public base {
    protected:
        friend vertex_array;
        vertex_array * glvao;

    public:
        ~vertex_array_attribute();
        vertex_array_attribute(vertex_array& vao, GLuint&& binding = 0);
        vertex_array_attribute(vertex_array& vao, GLuint& binding);
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
            base::deallocate();
        }

        vertex_array_binding&& create_binding(GLuint &&binding = 0){
            return vertex_array_binding(thisref, std::forward<GLuint>(binding));
        }

        vertex_array_binding&& create_binding(GLuint &binding) {
            return vertex_array_binding(thisref, binding);
        }

        template<class T>
        vertex_array_binding_single<T>&& create_binding(GLuint &&binding = 0) {
            return vertex_array_binding_single<T>(thisref, std::forward<GLuint>(binding));
        }

        template<class... T>
        vertex_array_binding_multi<T...>&& create_binding(GLuint &&binding = 0) {
            return vertex_array_binding_multi<T...>(thisref, std::forward<GLuint>(binding));
        }

        template<class T>
        vertex_array_binding_single<T>&& create_binding(GLuint &binding) {
            return vertex_array_binding_single<T>(thisref, binding);
        }

        template<class... T>
        vertex_array_binding_multi<T...>&& create_binding(GLuint &binding) {
            return vertex_array_binding_multi<T...>(thisref, binding);
        }

        vertex_array_attribute&& create_attribute(GLuint &&attribute = 0){
            return vertex_array_attribute(thisref, std::forward<GLuint>(attribute));
        }

        vertex_array_attribute&& create_attribute(GLuint &attribute) {
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
        GLuint * buffers = get_globj_wrap<structured_buffer<T>...>(_ref_tuple(buf));
        GLsizei * strides = get_stride_wrap<T...>();
        if (!offsets) offsets = new GLintptr[N]{0};
        glVertexArrayVertexBuffers(*glvao, thisref, N, buffers, offsets, strides);
    }

    vertex_array_attribute::vertex_array_attribute(vertex_array& vao, GLuint& binding) {
        glvao = &vao;
        this->set_object(binding);
        glEnableVertexArrayAttrib(*glvao, thisref);
    }

    vertex_array_attribute::vertex_array_attribute(vertex_array& vao, GLuint&& binding) {
        glvao = &vao;
        this->set_object(std::forward<GLuint>(binding));
        glEnableVertexArrayAttrib(*glvao, thisref);
    }

};