#pragma once

#include "opengl.hpp"
#include "buffer.hpp"
#include "enums.hpp"
#include <memory>

namespace NS_NAME {

    class _texture_context;



    class texture;

    class texture_level : public base {
        friend texture;
        texture * gltex;

    public:
        ~texture_level() { 
        };
        texture_level(texture& tex, GLint level = 0);

        void subimage(GLint offset, GLuint size, GLenum format, GLenum type, const GLvoid * pixels);
        void subimage(glm::ivec2 offset, glm::uvec2 size, GLenum format, GLenum type, const GLvoid * pixels);
        void subimage(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, const GLvoid * pixels);


        void get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize, void *pixels) const;

        template<class T>
        std::vector<T>& get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, std::vector<T>& buffer) const;

        template<class T>
        std::vector<T> get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize) const;

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const;

        template<class T>
        T get_parameter_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter(pname, params));
        }
    };



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
        texture() {}
        texture(_texture_context &gltarget);
        texture(texture& another) { this->set_object(another._get_shared()); } // when assign, share pointer
        texture(texture&& another) { this->set_object(another._get_shared()); } // when assign, share pointer
        texture(GLuint * allocationPointer) { this->set_object(*allocationPointer); } // re-assign gl pointer value to class

        static std::vector<texture> create(_texture_context &gltarget, size_t n = 1);

        ~texture(){
            if (base::ready_free()) glDeleteTextures(1, thisref);
        }

        texture_level get_level(GLint level = 0) {
            return texture_level(thisref, level);
        }

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
            if constexpr (std::is_same<T, int>::value) glTextureParameteriv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, float>::value) glTextureParameterfv(thisref, pname, (float*)params);
        }

        template<class T>
        void parameter_int(GLenum pname, T * params) const {
            if constexpr (std::is_same<T, int>::value) glTextureParameterIiv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, GLuint>::value) glTextureParameterIuiv(thisref, pname, (GLuint*)params);
        }

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetTextureParameteriv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, float>::value) glGetTextureParameterfv(thisref, pname, (float*)params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetTextureParameterIiv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, GLuint>::value) glGetTextureParameterIuiv(thisref, pname, (GLuint*)params);
            return params;
        }



        // texture storage (accept GLM vector)
        void storage(GLsizei levels, const _internal_format& internalformat, GLsizei size) {
            glTextureStorage1D(thisref, levels, internalformat.internal(), size);
        }

        void storage(GLsizei levels, const _internal_format& internalformat, glm::uvec2 size) {
            glTextureStorage2D(thisref, levels, internalformat.internal(), size.x, size.y);
        }

        void storage(GLsizei levels, const _internal_format& internalformat, glm::uvec3 size) {
            glTextureStorage3D(thisref, levels, internalformat.internal(), size.x, size.y, size.z);
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
            glTextureBuffer(thisref, internalformat.internal(), buf);
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
        std::vector<T> get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize) const {
            std::vector<T> buffer(buffersize);
            this->get_image_subdata(level, offset, size, format, type, buffer.size() * sizeof(T), buffer.data());
            return buffer;
        }
    };


    texture_level::texture_level(texture& tex, GLint level) {
        gltex = &tex;
        this->set_object(level);
    }

    void texture_level::subimage(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, const GLvoid * pixels) {
        gltex->subimage(thisref, offset, size, format, type, pixels);
    };

    void texture_level::subimage(glm::ivec2 offset, glm::uvec2 size, GLenum format, GLenum type, const GLvoid * pixels) {
        gltex->subimage(thisref, offset, size, format, type, pixels);
    };

    void texture_level::subimage(GLint offset, GLuint size, GLenum format, GLenum type, const GLvoid * pixels) {
        gltex->subimage(thisref, offset, size, format, type, pixels);
    };

    void texture_level::get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize, void *pixels) const {
        gltex->get_image_subdata(thisref, offset, size, format, type, buffersize, pixels);
    }

    // get subimage to vector
    template<class T>
    std::vector<T>& texture_level::get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, std::vector<T>& buffer) const {
        return gltex->get_image_subdata<T>(thisref, offset, size, format, type, buffer);
    }

    // get subimage as vector
    template<class T>
    std::vector<T> texture_level::get_image_subdata(glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize) const {
        return gltex->get_image_subdata<T>(thisref, offset, size, format, type, buffersize);
    }

    template<class T>
    T * texture_level::get_parameter(GLenum pname, T * params) const {
        if (!params) params = { 0 };
        if constexpr (std::is_same<T, int>::value) glGetTextureLevelParameteriv(*gltex, thisref, pname, (int*)params);
        if constexpr (std::is_same<T, float>::value) glGetTextureLevelParameterfv(*gltex, thisref, pname, (float*)params);
        return params;
    }




    // universal sampler object
    class sampler: public base {
    public:
        sampler(){
            base::make_ptr();
            glCreateSamplers(1, thisref);
        }

        ~sampler(){
            if (base::ready_free()) glDeleteSamplers(1, thisref);
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
            if constexpr (std::is_same<T, int>::value) glSamplerParameteriv(thisref, pname, params);
            if constexpr (std::is_same<T, float>::value) glSamplerParameterfv(thisref, pname, params);
        }

        template<class T>
        void parameter_int(GLenum pname, T * params) {
            if constexpr (std::is_same<T, int>::value) glSamplerParameterIiv(thisref, pname, params);
            if constexpr (std::is_same<T, GLuint>::value) glSamplerParameterIuiv(thisref, pname, params);
        }

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetSamplerParameteriv(thisref, pname, params);
            if constexpr (std::is_same<T, float>::value) glGetSamplerParameterfv(thisref, pname, params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetSamplerParameterIiv(thisref, pname, params);
            if constexpr (std::is_same<T, GLuint>::value) glGetSamplerParameterIuiv(thisref, pname, params);
            return params;
        }

    };


    // texture binding
    class texture_binding: public base {
    public:
        friend _texture_context;
        texture_binding(GLuint binding = 0) {
            this->set_object(binding);
        }

        ~texture_binding(){
        }

        void bind_sampler(sampler& sam) {
            glBindSampler(thisref, sam);
        }

        void bind_texture(texture& tex) {
            glBindTextureUnit(thisref, tex);
        }

    };


    // image binding
    class image: public base {
    public:
        image(GLuint binding = 0) {
            this->set_object(binding);
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
            this->set_object(binding);
        }

        // create single texture
        texture create() {
            return texture(thisref);
        }

        // context named binding
        void bind(texture& tex){
            glBindTexture(thisref, tex);
        }
    };

    texture::texture(_texture_context &gltarget) {
        this->gltarget = &gltarget;
        base::make_ptr();
        glCreateTextures(gltarget, 1, (GLuint *)thisref);
    };

    void texture::copy_image_subdata(GLint srcLevel, glm::ivec3 srcOffset, texture& destination, GLint dstLevel, glm::ivec3 dstOffset, glm::uvec3 size) const {
        glCopyImageSubData(thisref, (GLenum)thisref.target(), srcLevel, srcOffset.x, srcOffset.y, srcOffset.z, destination, (GLenum)destination.target(), dstLevel, dstOffset.x, dstOffset.y, dstOffset.z, size.x, size.y, size.z);
    }

    std::vector<texture> texture::create(_texture_context &gltarget, size_t n) {
        GLuint * objects = new GLuint[n];
        glCreateTextures(gltarget, n, objects);
        std::vector<texture> textures;
        for (intptr_t pt = 0; pt < n; pt++) {
            textures.push_back(texture(gltarget, objects + pt));
        }
        return std::move(textures);
    }



    // texture targets
    namespace texture_target {
        _texture_context texture_cube(GL_TEXTURE_CUBE_MAP);
        _texture_context texture_buffer(GL_TEXTURE_BUFFER);
        _texture_context texture1d(GL_TEXTURE_1D);
        _texture_context texture2d(GL_TEXTURE_2D);
        _texture_context texture3d(GL_TEXTURE_3D);
        _texture_context texture2d_msaa(GL_TEXTURE_2D_MULTISAMPLE);
        _texture_context texture1d_array(GL_TEXTURE_1D_ARRAY);
        _texture_context texture2d_array(GL_TEXTURE_2D_ARRAY);
    };



    // bindless pointers manipulations
    namespace bindless_texture { // TODO support

    };


};