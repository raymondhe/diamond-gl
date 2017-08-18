#pragma once

#include "opengl.hpp"

namespace dgl {

    class buffer_class;
    class buffer_binding_class;

    typedef buffer_class* buffer;
    typedef buffer_binding_class* buffer_binding;


    class buffer_class: public base_class {
    protected:
        buffer_class() {glCreateBuffers(1, *this);}

    public:
        ~buffer_class() {glDeleteBuffers(1, *this);}

        static buffer create(){
            return (new buffer_class());
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            glNamedBufferData(*this, size, data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            glNamedBufferSubData(*this, offset, size, data);
        }

        void storage(GLsizei size, const void *data, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT){
            glNamedBufferStorage(*this, size, data, flags);
        }

        void copydata(buffer dest, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
            glCopyNamedBufferSubData(*this, *dest, readOffset, writeOffset, size);
        }
    };


    class _buffer_context;

    // register buffer binding
    class buffer_binding_class: public base_class {
        friend _buffer_context;
        _buffer_context * gltarget = nullptr;
        buffer_binding_class(_buffer_context * btarget, GLuint binding = 0);

    public:
        ~buffer_binding_class();
        void bind(buffer buf);
        void bind_range(buffer buf, GLintptr offset = 0, GLsizei size = 1);
    };


    // contextual targeted bindings
    class _buffer_context: public base_class {
    public:
        _buffer_context(GLuint binding = 0) {this->set_object(binding);}

        buffer_binding create_binding(GLuint binding = 0){
            return (new buffer_binding_class(this, binding));
        }

        // context named binding
        void bind(buffer buf){
            glBindBuffer(*this, *buf);
        }


        // TODO support of non-DSA (binded) operations
    };






    buffer_binding_class:: buffer_binding_class(_buffer_context * btarget, GLuint binding) { this->set_object(binding); gltarget = btarget; }
    buffer_binding_class:: ~buffer_binding_class() { glBindBufferBase((GLenum)*gltarget, *this, 0); } // unbind

    void buffer_binding_class::bind(buffer buf) {
        glBindBufferBase((GLenum)*gltarget, *this, *buf);
    }

    void buffer_binding_class::bind_range(buffer buf, GLintptr offset, GLsizei size) {
        glBindBufferRange((GLenum)*gltarget, *this, *buf, offset, size);
    }




    // bindables
    namespace buffer_target {
        _buffer_context element_array(GL_ELEMENT_ARRAY_BUFFER);
        _buffer_context array(GL_ARRAY_BUFFER);
        _buffer_context shader_storage(GL_SHADER_STORAGE_BUFFER);
        _buffer_context uniform(GL_UNIFORM_BUFFER);
    }

};