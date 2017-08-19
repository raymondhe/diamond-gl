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
        void vertex_buffer(buffer buf, GLintptr offset = 0, GLintptr stride = 0);
    };




    class vertex_array_attribute_class: public base_class {
    protected:
        friend vertex_array_class;
        vertex_array glvao = nullptr;
        vertex_array_attribute_class(vertex_array vao, GLuint binding = 0);

    public:
        ~vertex_array_attribute_class();
        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0);
        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0);
        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0);
        void binding(GLuint binding);
        void binding(vertex_array_binding bnd) {this->binding((GLuint)*bnd);}
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

        //void bind(){
        //    glBindVertexArray(*this);
        //}

    };




    vertex_array_attribute_class::vertex_array_attribute_class(vertex_array vao, GLuint binding) {
        glvao = vao;
        this->set_object(binding);
        glEnableVertexArrayAttrib((GLuint)*glvao, *this);
    }

    vertex_array_attribute_class::~vertex_array_attribute_class() {
        glDisableVertexArrayAttrib((GLuint)*glvao, *this);
    }

    void vertex_array_attribute_class::attrib_format(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
        glVertexArrayAttribFormat((GLuint)*glvao, *this, size, type, normalized, relativeoffset);
    }

    void vertex_array_attribute_class::attrib_format_int(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribIFormat((GLuint)*glvao, *this, size, type, relativeoffset);
    }

    void vertex_array_attribute_class::attrib_format_long(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribLFormat((GLuint)*glvao, *this, size, type, relativeoffset);
    }

    void vertex_array_attribute_class::binding(GLuint binding) { // low level function
        glVertexArrayAttribBinding((GLuint)*glvao, *this, binding);
    }

    void vertex_array_binding_class::vertex_buffer(buffer buf, GLintptr offset, GLintptr stride) {
        glVertexArrayVertexBuffer((GLuint)*glvao, *this, *buf, offset, stride);
    }




    
};