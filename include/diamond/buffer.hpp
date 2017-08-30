#pragma once

#include "opengl.hpp"
#include <vector>
#include <tuple>
#include <utility>

namespace NS_NAME {

    class buffer_builder {
    public:
        static void create(GLuint * heap){
            glCreateBuffers(1, heap);
        }
        static void release(GLuint * heap){
            glDeleteBuffers(1, heap);
        }
    };

    template<class T>
    class void_buffer: public gl_object<buffer_builder> {
    protected:
        using buffer = void_buffer<T>;
        using buffer_ptr = void_buffer<T> *;
        using base = gl_object<buffer_builder>;
        
        template<class T>
        friend class void_buffer;

    public:

        // constructor (variadic)
        buffer() { base::create_alloc(); }
        buffer(buffer& another) { base::move(another); } // copy (it refs)
        buffer(buffer&& another) { base::move(std::forward<buffer>(another)); } // move
        buffer(GLuint * another) { base::move(another); } // heap by ptr

        static std::vector<buffer> create(GLint n) {
            GLuint * objects = new GLuint[n];
            glCreateBuffers(n, objects);
            std::vector<buffer> buffers;
            for (intptr_t pt = 0; pt < n; pt++) {
                buffers.push_back(buffer(objects + pt));
            }
            delete objects; // free, because pointer values was copied
            return std::move(buffers);
        }

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
    class buffer_binding {
    protected:
        friend _buffer_context;
        GLuint binding = 0;
        _buffer_context * gltarget;
        
    public:
        buffer_binding(_buffer_context& btarget, GLuint binding = 0) : binding(binding) {
            gltarget = &btarget;
        }
        ~buffer_binding() {};

        void bind(buffer& buf);
        void bind_range(buffer& buf, GLintptr offset = 0, GLsizei size = 1);

        operator GLuint(){ return binding; }
    };


    // contextual targeted bindings
    class _buffer_context {
    protected:
        GLenum target = 0;

    public:
        _buffer_context(GLenum target) : target(target) {}

        buffer_binding create_binding(GLuint binding = 0){
            return buffer_binding(thisref, binding);
        }

        // context named binding
        void bind(buffer& buf){
            glBindBuffer(thisref, buf);
        }

        operator GLenum(){ return target; }

        // TODO support of non-DSA (binded) operations
    };


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