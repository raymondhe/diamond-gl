#pragma once

#include "buffer.hpp"
#include <memory>

namespace dgl {

    class vertex_array;

    class vertex_array_binding: public base {
        friend vertex_array;
        //std::shared_ptr<vertex_array> glvao;
        vertex_array * glvao;

    public:
        vertex_array_binding(vertex_array& vao, GLuint binding = 0) {
            //glvao = std::make_shared<vertex_array>(vao);
            glvao = &vao;
            this->set_object(binding);
        }
    
        void vertex_buffer(buffer& buf, GLintptr offset = 0, GLintptr stride = 0);

        template<class T>
        void vertex_buffer(structured_buffer<T>& buf, GLintptr offset = 0);

        template<class T>
        void vertex_buffer(buffer& buf, GLintptr offset = 0);
    };




    class vertex_array_attribute: public base {
    protected:
        friend vertex_array;
        //std::shared_ptr<vertex_array> glvao;
        vertex_array * glvao;

    public:
        vertex_array_attribute(vertex_array& vao, GLuint binding = 0);
        //vertex_array_attribute(vertex_array& vao, GLuint binding = 0) {
        //    glvao = std::make_shared<vertex_array>(vao);
        //    this->set_object(binding);
        //    glEnableVertexArrayAttrib(*glvao, thisref);
        //}
        ~vertex_array_attribute();
        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0);
        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0);
        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0);
        void binding(GLuint binding);
        void binding(vertex_array_binding& bnd) {this->binding((GLuint)bnd);}
    };




    class vertex_array: public base {
    public:
        vertex_array() {glCreateVertexArrays(1, thisref);}
        ~vertex_array() {glDeleteVertexArrays(1, thisref);}

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
        glDisableVertexArrayAttrib(*glvao, thisref);
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

    vertex_array_attribute::vertex_array_attribute(vertex_array& vao, GLuint binding) {
        //glvao = std::make_shared<vertex_array>(vao);
        glvao = &vao;
        this->set_object(binding);
        glEnableVertexArrayAttrib(*glvao, thisref);
    }

    
};