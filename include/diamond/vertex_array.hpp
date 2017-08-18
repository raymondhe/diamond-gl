#pragma once

#include "buffer.hpp"

namespace dgl {

    class vertex_array_class;
    class vertex_array_binding_class;
    class vertex_array_attribute_class;

    typedef vertex_array_class* vertex_array;
    typedef vertex_array_binding_class* vertex_array_binding;
    typedef vertex_array_attribute_class* vertex_array_attribute;



    class vertex_array_binding_class: public base_class {
    protected:
        friend vertex_array_class;
        vertex_array glvao = nullptr;
        vertex_array_binding_class(vertex_array vao, GLuint binding = 0) {
            glvao = vao;
            this->set_object(binding);
        }

    public:
        void buffer(buffer buf, GLintptr offset = 0, GLintptr stride = 0){
            glVertexArrayVertexBuffer(*glvao, *this, *buf, offset, stride);
        }
    };




    class vertex_array_attribute_class: public base_class {
    protected:
        friend vertex_array_class;
        vertex_array glvao = nullptr;
        vertex_array_attribute_class(vertex_array vao, GLuint binding = 0) {
            glvao = vao;
            this->set_object(binding);
        }

    public:
        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0){
            glVertexArrayAttribFormat(*glvao, *this, size, type, normalized, relativeoffset);
        }

        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0){
            glVertexArrayAttribIFormat(*glvao, *this, size, type, relativeoffset);
        }

        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0){
            glVertexArrayAttribLFormat(*glvao, *this, size, type, relativeoffset);
        }

        void binding(vertex_array_binding binding) {
            glVertexArrayAttribBinding(*glvao, *this, *binding);
        }

        void binding(GLuint binding) { // low level function
            glVertexArrayAttribBinding(*glvao, *this, binding);
        }
    };




    class vertex_array_class: public base_class {
    protected:
        vertex_array_class() {glCreateVertexArrays(1, *this);}

    public:
        static vertex_array create(){
            return (new vertex_array_class());
        }

        ~vertex_array_class() {glDeleteVertexArrays(1, *this);}

        vertex_array_binding create_binding(GLuint binding = 0){
            return (new vertex_array_binding_class(this, binding));
        }

        vertex_array_attribute create_attribute(GLuint attribute = 0){
            return (new vertex_array_attribute_class(this, attribute));
        }

        void element_buffer(buffer buf){
            glVertexArrayElementBuffer(*this, *buf);
        }

        void bind(){
            glBindVertexArray(*this);
        }

    };
    
};