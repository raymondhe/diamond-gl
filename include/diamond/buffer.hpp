#pragma once

#include "opengl.hpp"
#include <vector>
#include <tuple>
#include <utility>

namespace dgl {


    template<class T>
    class structured_buffer;

    template<class T>
    class void_buffer: public base {
    protected:
        //void_buffer<T>(GLuint * allocationPointer) { this->set_object(*allocationPointer); } // can be used with allocators

    public:
        
        void_buffer<T>(GLuint * allocationPointer) { this->set_object(*allocationPointer); } // can be used with allocators
        void_buffer<T>() { base::allocate(1); glCreateBuffers(1, thisref); }
        ~void_buffer<T>() {glDeleteBuffers(1, thisref);}
        
        // new multi-bind creator
        static std::vector<void_buffer<T>> create(GLint n) {
            GLuint * objects = new GLuint[n];
            std::vector<void_buffer<T>> buffers;
            for (intptr_t pt = 0; pt < n; pt++) {
                buffers.push_back(void_buffer<T>(objects + pt));
            }
            glCreateBuffers(n, objects);
            return buffers;
            //return std::forward<std::vector<buffer>>(buffers);
        }



        // create tuple of buffers (only voids support)
        template<typename... T, size_t... Is>
        static std::tuple<void_buffer<T>...>&& _make_tuple(GLuint * a, std::index_sequence<Is...>)
        {
            return std::make_tuple(void_buffer<T>(&a[Is])...);
        }

        template<typename... T>
        static std::tuple<void_buffer<T>...>&& _make_tuple(GLuint * a)
        {
            return _make_tuple<T...>(a, std::index_sequence_for<T...>{});
        }

        template<typename... T>
        static std::tuple<void_buffer<T>...>&& create_tuple() {
            constexpr size_t n = sizeof...(T);
            GLuint * objects = new GLuint[n];
            glCreateBuffers(n, objects);
            return _make_tuple<T...>(objects);
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
    class structured_buffer: public void_buffer<T> {
    protected:
        using buffer = void_buffer<T>;

    public:
        structured_buffer<T>(GLuint * allocationPointer) : void_buffer<T>(allocationPointer) {};
        structured_buffer<T>() : void_buffer<T>() {};

        // new multi-bind creator
        
        static std::vector<structured_buffer<T>> create(GLint n) {
            GLuint * objects = new GLuint[n];
            std::vector<structured_buffer<T>> buffers;
            for (intptr_t pt = 0; pt < n; pt++) {
                buffers.push_back(structured_buffer<T>(objects + pt));
            }
            glCreateBuffers(n, objects);
            return buffers;
        }

        // create tuple of buffers (only voids support)
        template<typename... T, size_t... Is>
        static std::tuple<structured_buffer<T>...>&& _make_tuple(GLuint * a, std::index_sequence<Is...>)
        {
            return std::make_tuple(structured_buffer<T>(&a[Is])...);
        }

        template<typename... T>
        static std::tuple<structured_buffer<T>...>&& _make_tuple(GLuint * a)
        {
            return _make_tuple<T...>(a, std::index_sequence_for<T...>{});
        }

        template<typename... T>
        static std::tuple<structured_buffer<T>...>&& create_tuple() {
            constexpr size_t n = sizeof...(T);
            GLuint * objects = new GLuint[n];
            glCreateBuffers(n, objects);
            return _make_tuple<T...>(objects);
        }




        void get_subdata(GLintptr offset, GLsizei size, void *data) const {
            buffer::get_subdata(offset, size * sizeof(T), data);
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            buffer::data(size * sizeof(T), data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            buffer::subdata(offset, size * sizeof(T), data);
        }

        void storage(GLsizei size, const void *data = nullptr, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT){
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


    // buffer is structured_buffer<void>;
    using buffer = structured_buffer<void>;



    class _buffer_context;

    // register buffer binding
    class buffer_binding: public base {
    protected:
        friend _buffer_context;
        _buffer_context * gltarget;

    public:
        buffer_binding(_buffer_context& btarget, GLuint binding = 0) {
            base::allocate(1);
            gltarget = &btarget; 
            this->set_value(binding); 
        }

        ~buffer_binding();

        template<typename T>
        void bind(structured_buffer<T>& buf);

        template<typename T>
        void bind_range(structured_buffer<T>& buf, GLintptr offset = 0, GLsizei size = 1);

        template<typename T>
        void bind(std::vector<structured_buffer<T>>& buf);

        template<typename T>
        void bind_range(std::vector<structured_buffer<T>>& buf, GLintptr * offsets = nullptr, GLsizeiptr * sizes = nullptr);
    };


    // contextual targeted bindings
    class _buffer_context: public base {
    public:
        _buffer_context(GLuint binding = 0) {
            base::allocate(1);
            this->set_value(binding);
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


    // basic bind support
    template<typename T>
    void buffer_binding::bind(structured_buffer<T>& buf) {
        glBindBufferBase(*gltarget, thisref, buf);
    }

    template<typename T>
    void buffer_binding::bind_range(structured_buffer<T>& buf, GLintptr offset, GLsizei size) {
        glBindBufferRange(*gltarget, thisref, buf, offset, size);
    }

    // multi-bind support
    template<typename T>
    void buffer_binding::bind(std::vector<structured_buffer<T>>& buf) {
        GLuint handler = buf[0];
        glBindBuffersBase(*gltarget, thisref, buf.size(), /*buf.data()*/ &handler);
    }

    template<typename T>
    void buffer_binding::bind_range(std::vector<structured_buffer<T>>& buf, GLintptr * offsets, GLsizeiptr * sizes) {
        GLuint handler = buf[0];
        glBindBuffersRange(*gltarget, thisref, buf.size(), &handler, offsets, sizes);
    }


    // bindables
    namespace buffer_target {
        _buffer_context element_array(GL_ELEMENT_ARRAY_BUFFER);
        _buffer_context array(GL_ARRAY_BUFFER);
        _buffer_context shader_storage(GL_SHADER_STORAGE_BUFFER);
        _buffer_context uniform(GL_UNIFORM_BUFFER);
    }

};