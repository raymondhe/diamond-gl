#pragma once

#include "opengl.hpp"
#include <vector>

namespace dgl {

    template<class T>
    class structured_buffer;

    class buffer: public base {
    public:
        buffer() {glCreateBuffers(1, thisref);}
        ~buffer() {glDeleteBuffers(1, thisref);}

        void get_subdata(GLintptr offset, GLsizei size, void *data) const {
            glGetNamedBufferSubData(thisref, offset, size, data);
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            glNamedBufferData(thisref, size, data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            glNamedBufferSubData(thisref, offset, size, data);
        }

        void storage(GLsizei size, const void *data, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT){
            glNamedBufferStorage(thisref, size, data, flags);
        }

        void copydata(buffer& dest, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
            glCopyNamedBufferSubData(thisref, *dest, readOffset, writeOffset, size);
        }

        
        // data from vector
        template<class T>
        void data(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW){
            this->data(data.size() * sizeof(T), data.data(), usage);
        }

        // fill subdata by vector
        template<class T>
        void subdata(GLintptr offset, const std::vector<T>& data){
            this->subdata(offset, data.size() * sizeof(T), data.data());
        }

        // get subdata by range
        template<class T>
        std::vector<T>& get_subdata(GLintptr offset, GLsizei size) const {
            std::vector<T> vctr(size);
            this->get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }

        // get subdata by full vector
        template<class T>
        std::vector<T>& get_subdata(GLintptr offset, std::vector<T>&vctr) const {
            this->get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }
    };

    template<class T>
    class structured_buffer: public buffer {
    public:
        void get_subdata(GLintptr offset, GLsizei size, void *data) const {
            buffer::get_subdata(offset, size * sizeof(T), data);
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            buffer::data(size * sizeof(T), data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            buffer::subdata(offset, size * sizeof(T), data);
        }

        void storage(GLsizei size, const void *data, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT){
            buffer::storage(size * sizeof(T), data, flags);
        }

        void copydata(buffer& dest, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
            buffer::copydata(dest, readOffset, writeOffset, size * sizeof(T));
        }


        void data(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW){
            return buffer::data<T>(data, usage);
        }

        void subdata(GLintptr offset, const std::vector<T>& data){
            return buffer::subdata<T>(offset, data);
        }

        std::vector<T>& get_subdata(GLintptr offset, GLsizei size) const {
            return buffer::get_subdata<T>(offset, size);
        }

        std::vector<T>& get_subdata(GLintptr offset, std::vector<T>&vctr) const {
            return buffer::get_subdata<T>(offset, vctr);
        }
    };


    class _buffer_context;

    // register buffer binding
    class buffer_binding: public base {
    protected:
        friend _buffer_context;
        _buffer_context * gltarget;

    public:
        buffer_binding(_buffer_context& btarget, GLuint binding = 0) { gltarget = &btarget; this->set_object(binding); }

        ~buffer_binding();
        void bind(buffer& buf);
        void bind_range(buffer& buf, GLintptr offset = 0, GLsizei size = 1);
    };


    // contextual targeted bindings
    class _buffer_context: public base {
    public:
        _buffer_context(GLuint binding = 0) {
            this->set_object(binding);
        }

        buffer_binding&& create_binding(GLuint binding = 0){
            return buffer_binding(thisref, binding);
        }

        // context named binding
        void bind(buffer& buf){
            glBindBuffer(thisref, buf);
        }


        // TODO support of non-DSA (binded) operations
    };


    
    buffer_binding:: ~buffer_binding() { // unbind
        glBindBufferBase(*gltarget, thisref, 0); 
    }

    void buffer_binding::bind(buffer& buf) {
        glBindBufferBase(*gltarget, thisref, buf);
    }

    void buffer_binding::bind_range(buffer& buf, GLintptr offset, GLsizei size) {
        glBindBufferRange(*gltarget, thisref, buf, offset, size);
    }




    // bindables
    namespace buffer_target {
        _buffer_context element_array(GL_ELEMENT_ARRAY_BUFFER);
        _buffer_context array(GL_ARRAY_BUFFER);
        _buffer_context shader_storage(GL_SHADER_STORAGE_BUFFER);
        _buffer_context uniform(GL_UNIFORM_BUFFER);
    }

};