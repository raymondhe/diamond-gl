#pragma once

#include "opengl.hpp"
#include "buffer.hpp"
#include "enums.hpp"
#include <memory>

namespace NS_NAME {

    class _texture_context;



    class texture;

    class texture_level {
        friend texture;
        GLuint level = 0;
        texture * gltex;

    public:
        ~texture_level() {};
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

        operator GLuint() const {
            return level;
        }
    };





    class texture_builder {
    public:
        static void create(GLuint * heap, _texture_context& target);
        static void release(GLuint * heap){
            glDeleteTextures(1, heap);
        }
    };


    // universal texture object
    class texture: public gl_object<texture_builder> {

    protected:
        friend _texture_context;
        using base = gl_object<texture_builder>;
        _texture_context * gltarget;

    public:

        // constructor (variadic)
        texture(_texture_context& target) { base::create_alloc(target); gltarget = &target; } // variadic unsupported
        texture(texture& another) { base::move(another); gltarget = &another.target(); } // copy (it refs)
        texture(texture&& another) { base::move(std::forward<texture>(another)); gltarget = &another.target(); } // move
        texture(_texture_context& target, GLuint * another) { base::move(another); gltarget = &target; } // heap by ptr

        static std::vector<texture> create(_texture_context &gltarget, size_t n = 1);

        texture_level level(GLint level = 0) {
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
        this->gltex = &tex;
        this->level = level;
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
        gltex->get_image_subdata((GLuint)thisref, offset, size, format, type, buffersize, pixels);
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




    class sampler_builder {
    public:
        static void create(GLuint * heap){
            glCreateSamplers(1, heap);
        }
        static void release(GLuint * heap){
            glDeleteSamplers(1, heap);
        }
    };


    // universal texture object
    class sampler: public gl_object<sampler_builder> {

    protected:
        using base = gl_object<sampler_builder>;

    public:

        // constructor (variadic)
        sampler() { base::create_alloc(); }
        sampler(sampler& another) { base::move(another); } // copy (it refs)
        sampler(sampler&& another) { base::move(std::forward<sampler>(another)); } // move
        sampler(GLuint * another) { base::move(another); } // heap by ptr

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
    class texture_binding {
    protected:
        GLuint binding = 0;

    public:
        texture_binding(GLuint binding = 0) : binding(binding) {}
        ~texture_binding(){}

        void bind_sampler(sampler& sam) {
            glBindSampler(thisref, sam);
        }

        void bind_texture(texture& tex) {
            glBindTextureUnit(thisref, tex);
        }

        operator GLuint(){
            return binding;
        }

    };


    // image binding
    class image {
    protected:
        GLuint binding = 0;

    public:
        image(GLuint binding = 0): binding(binding) { }

        // bind image texture
        void bind_texture(texture& texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
            glBindImageTexture(thisref, texture, level, layered, layer, access, format);
        }

        operator GLuint() const {
            return binding;
        }
    };


    // contexted texture binding
    class _texture_context {
    protected:
        GLenum target;
    public:
        _texture_context(GLenum target) : target(target) {}

        // create single texture
        texture create() {
            return texture(thisref);
        }

        // context named binding
        void bind(texture& tex){
            glBindTexture(thisref, tex);
        }

        operator GLenum(){
            return target;
        }
    };
    
    
    void texture_builder::create(GLuint * heap, _texture_context& target){
        glCreateTextures(target, 1, heap);
    }

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