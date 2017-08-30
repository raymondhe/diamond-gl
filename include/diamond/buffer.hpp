#pragma once

#include "opengl.hpp"
#include <vector>
#include <tuple>
#include <utility>

namespace NS_NAME {

    template<class T>
    class void_buffer;

    template<class T>
    class void_buffer: public base {
    protected:
        using buffer = void_buffer<T>;
        using buffer_ptr = void_buffer<T> *;

    public:

        void_buffer<T>(void_buffer<T>& another) { this->set_object(another.globj); } // when assign, share pointer
        void_buffer<T>(GLuint * allocationPointer) { this->set_object(allocationPointer); } // can be used with allocators
        void_buffer<T>() { base::make_ptr(); glCreateBuffers(1, thisref); }
        ~void_buffer<T>() { if (base::ready_free()) glDeleteBuffers(1, thisref); }

        void get_subdata(GLintptr offset, GLsizei size, void *data) const {
            glGetNamedBufferSubData(thisref, offset, size, data);
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            glNamedBufferData(thisref, size, data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            glNamedBufferSubData(thisref, offset, size, data);
        }

        void storage(GLsizei size, const void *data = nullptr, buffer_storage_bits flags = GL_DYNAMIC_STORAGE_BIT) {
            glNamedBufferStorage(thisref, size, data, flags.bitfield);
        }

        void copydata(void_buffer<T>& dest, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
            glCopyNamedBufferSubData(thisref, *dest, readOffset, writeOffset, size);
        }




        // data from vector
        template<class T>
        void data(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
            buffer::data(data.size() * sizeof(T), data.data(), usage);
        }

        // fill subdata by vector
        template<class T>
        void subdata(GLintptr offset, const std::vector<T>& data) {
            buffer::subdata(offset, data.size() * sizeof(T), data.data());
        }

        // get subdata by range
        template<class T>
        std::vector<T> get_subdata(GLintptr offset, GLsizei size) const {
            std::vector<T> vctr(size);
            buffer::get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }

        // get subdata by full vector
        template<class T>
        std::vector<T>& get_subdata(GLintptr offset, std::vector<T>&vctr) const {
            buffer::get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }

    };

    using buffer = void_buffer<void>;



    class _buffer_context;

    // register buffer binding
    class buffer_binding: public base {
    protected:
        friend _buffer_context;
        _buffer_context * gltarget;

    public:
        buffer_binding(_buffer_context& btarget, GLuint binding = 0) {
            gltarget = &btarget; 
            this->set_object(binding);
        }
        ~buffer_binding();

        void bind(buffer& buf);
        void bind_range(buffer& buf, GLintptr offset = 0, GLsizei size = 1);
    };


    // contextual targeted bindings
    class _buffer_context: public base {
    public:
        _buffer_context(GLuint binding) {
            this->set_object(binding);
        }

        buffer_binding create_binding(GLuint binding = 0){
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


    // basic bind support
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