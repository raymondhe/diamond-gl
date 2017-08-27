#pragma once

#include "opengl.hpp"
#include "buffer.hpp"
#include "enums.hpp"
#include <memory>

namespace dgl {

    class _texture_context;


    // universal texture object
    class texture: public base {
    protected:
        friend _texture_context;
        _texture_context * gltarget;
        texture(_texture_context &gltarget, GLuint * allocation) {
            this->set_object(*allocation);
            this->gltarget = &gltarget;
        };

    public:
        texture() {
            base::allocate(1);
        }
        texture(_texture_context &gltarget);

        ~texture(){
            glDeleteTextures(1, thisref);
        }


        // new multi-bind creator
        static std::vector<texture> create(_texture_context &gltarget, size_t n = 1);



        _texture_context& target() const {
            return *gltarget;
        }


        template<class T>
        void parameter_val(GLenum pname, T param) const {
            this->parameter<T>(pname, &param);
        }

        template<class T>
        void parameter_int_val(GLenum pname, T param) const {
            this->parameter_int<T>(pname, &param);
        }

        template<class T>
        T get_parameter_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter(pname, params));
        }

        template<class T>
        T get_parameter_int_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter_int(pname, params));
        }



        template<class T>
        void parameter(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glTextureParameteriv(thisref, pname, (int*)params);
            if (typeid(T) == typeid(float)) glTextureParameterfv(thisref, pname, (float*)params);
        }

        template<class T>
        void parameter_int(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glTextureParameterIiv(thisref, pname, (int*)params);
            if (typeid(T) == typeid(GLuint)) glTextureParameterIuiv(thisref, pname, (GLuint*)params);
        }

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetTextureParameteriv(thisref, pname, (int*)params);
            if (typeid(T) == typeid(float)) glGetTextureParameterfv(thisref, pname, (float*)params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetTextureParameterIiv(thisref, pname, (int*)params);
            if (typeid(T) == typeid(GLuint)) glGetTextureParameterIuiv(thisref, pname, (GLuint*)params);
            return params;
        }



        // texture storage (accept GLM vector)
        void storage(GLsizei levels, const _internal_format& internalformat, GLsizei size) {
            glTextureStorage1D(thisref, levels, internalformat, size);
        }

        void storage(GLsizei levels, const _internal_format& internalformat, glm::uvec2 size) {
            glTextureStorage2D(thisref, levels, internalformat, size.x, size.y);
        }

        void storage(GLsizei levels, const _internal_format& internalformat, glm::uvec3 size) {
            glTextureStorage3D(thisref, levels, internalformat, size.x, size.y, size.z);
        }


        // subimage (accept GLM vector)
        void subimage(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage3D(thisref, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, pixels);
        }

        void subimage(GLint level, glm::ivec2 offset, glm::uvec2 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage2D(thisref, level, offset.x, offset.y, size.x, size.y, format, type, pixels);
        }

        void subimage(GLint level, GLint offset, GLsizei size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage1D(thisref, level, offset, size, format, type, pixels);
        }


        // simplified version of very hard function
        void copy_image_subdata(GLint srcLevel, glm::ivec3 srcOffset, texture& destination, GLint dstLevel, glm::ivec3 dstOffset, glm::uvec3 size) const;

        // texture of buffer 
        void buffer(const _internal_format& internalformat, buffer& buf){
            glTextureBuffer(thisref, internalformat, buf);
        }


        // generate mipmap
        void generate_mipmap() {
            glGenerateTextureMipmap(thisref);
        }



        void get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize, void *pixels) const {
            glGetTextureSubImage(thisref, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, buffersize, pixels);
        }


        // get subimage to vector
        template<class T>
        void get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, std::vector<T>& buffer) const {
            this->get_image_subdata(level, offset, size, format, type, buffer.size() * sizeof(T), buffer.data());
        }

        // get subimage as vector
        template<class T>
        std::vector<T>& get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize) const {
            std::vector<T> buffer(buffersize);
            this->get_image_subdata(level, offset, size, format, type, buffer.size() * sizeof(T), buffer.data());
            return buffer;
        }
    };









    // universal sampler object
    class sampler: public base {
    public:
        sampler(){
            base::allocate(1);
            glCreateSamplers(1, thisref);
        }

        ~sampler(){
            glDeleteSamplers(1, thisref);
        }



        template<class T>
        void parameter_val(GLenum pname, T param) {
            this->parameter<T>(pname, &param);
        }

        template<class T>
        void parameter_int_val(GLenum pname, T param) {
            this->parameter_int<T>(pname, &param);
        }

        template<class T>
        T get_parameter_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter(pname, params));
        }

        template<class T>
        T get_parameter_int_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter_int(pname, params));
        }



        template<class T>
        void parameter(GLenum pname, T * params) {
            if (typeid(T) == typeid(int)) glSamplerParameteriv(thisref, pname, params);
            if (typeid(T) == typeid(float)) glSamplerParameterfv(thisref, pname, params);
        }

        template<class T>
        void parameter_int(GLenum pname, T * params) {
            if (typeid(T) == typeid(int)) glSamplerParameterIiv(thisref, pname, params);
            if (typeid(T) == typeid(GLuint)) glSamplerParameterIuiv(thisref, pname, params);
        }

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetSamplerParameteriv(thisref, pname, params);
            if (typeid(T) == typeid(float)) glGetSamplerParameterfv(thisref, pname, params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetSamplerParameterIiv(thisref, pname, params);
            if (typeid(T) == typeid(GLuint)) glGetSamplerParameterIuiv(thisref, pname, params);
            return params;
        }

    };


    // texture binding
    class texture_binding: public base {
    public:
        friend _texture_context;
        texture_binding(GLuint binding = 0) {
            base::allocate(1);
            this->set_value(binding);
        }

        ~texture_binding(){}

        void bind_sampler(sampler& sam) {
            glBindSampler(thisref, sam);
        }

        void bind_texture(texture& tex) {
            glBindTextureUnit(thisref, tex);
        }


        void bind_sampler(std::vector<sampler>& sam) {
            const GLuint& handler = sam[0];
            glBindSamplers(thisref, sam.size(), &handler);
        }

        void bind_texture(std::vector<texture>& tex) {
            const GLuint& handler = tex[0];
            glBindTextures(thisref, tex.size(), &handler);
        }

    };


    // image binding
    class image: public base {
    public:
        image(GLuint binding = 0) {
            base::allocate(1);
            this->set_value(binding);
        }

        // bind image texture
        void bind_texture(texture& texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
            glBindImageTexture(thisref, texture, level, layered, layer, access, format);
        }
    };


    // contexted texture binding
    class _texture_context: public base {
    public:
        _texture_context(GLuint binding = 0) {
            base::allocate(1);
            this->set_value(binding);
        }

        // create multiply texture
        std::vector<texture>&& create(size_t n){
            return texture::create(*this, n);
        }

        // create single texture
        texture&& create() {
            return texture(*this);
        }

        // context named binding
        void bind(texture& tex){
            glBindTexture(thisref, tex);
        }
    };

    texture::texture(_texture_context &gltarget) {
        base::allocate(1);
        this->gltarget = &gltarget;//std::make_shared<_texture_context>(gltarget);
        glCreateTextures((GLenum)gltarget, 1, thisref);
    };

    void texture::copy_image_subdata(GLint srcLevel, glm::ivec3 srcOffset, texture& destination, GLint dstLevel, glm::ivec3 dstOffset, glm::uvec3 size) const {
        glCopyImageSubData(thisref, (GLenum)thisref.target(), srcLevel, srcOffset.x, srcOffset.y, srcOffset.z, destination, (GLenum)destination.target(), dstLevel, dstOffset.x, dstOffset.y, dstOffset.z, size.x, size.y, size.z);
    }


    std::vector<texture> texture::create(_texture_context &gltarget, size_t n) {
        GLuint * objects = new GLuint[n];
        std::vector<texture> textures;
        for (intptr_t pt = 0; pt < n; pt++) {
            textures.push_back(texture(gltarget, objects + pt));
        }
        glCreateTextures((GLenum)gltarget, n, objects);
        //return std::forward<std::vector<texture>>(textures);
        return textures;
    }



    // texture targets
    namespace texture_target {
        _texture_context sampler_cube(GL_TEXTURE_CUBE_MAP);
        _texture_context sampler_buffer(GL_TEXTURE_BUFFER);
        _texture_context sampler_1d(GL_TEXTURE_1D);
        _texture_context sampler_2d(GL_TEXTURE_2D);
        _texture_context sampler_3d(GL_TEXTURE_3D);
        _texture_context sampler_2d_msaa(GL_TEXTURE_2D_MULTISAMPLE);
        _texture_context sampler_1d_array(GL_TEXTURE_1D_ARRAY);
        _texture_context sampler_2d_array(GL_TEXTURE_2D_ARRAY);
    };



    // bindless pointers manipulations
    namespace bindless_texture { // TODO support

    };


};