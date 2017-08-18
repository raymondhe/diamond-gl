#pragma once

#include "opengl.hpp"
#include "buffer.hpp"

namespace dgl {

    class texture_class;
    class sampler_class;
    class texture_binding_class;
    class image_class;

    typedef texture_class* texture;
    typedef sampler_class* sampler;
    typedef texture_binding_class* texture_binding;
    typedef image_class* image;


    class _texture_context;


    // universal texture object
    class texture_class: public base_class {
    protected:
        friend _texture_context;
        _texture_context * gltarget = nullptr;
        texture_class(_texture_context * gltarget);

    public:
        ~texture_class(){
            glDeleteTextures(1, *this);
        }

        _texture_context * target() const {
            return gltarget;
        }


        template<class T>
        void parameter(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glTextureParameteriv(*this, pname, params);
            if (typeid(T) == typeid(float)) glTextureParameterfv(*this, pname, params);
        }


        template<class T>
        void parameter_int(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glTextureParameterIiv(*this, pname, params);
            if (typeid(T) == typeid(GLuint)) glTextureParameterIuiv(*this, pname, params);
        }



        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetTextureParameteriv(*this, pname, params);
            if (typeid(T) == typeid(float)) glGetTextureParameterfv(*this, pname, params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetTextureParameterIiv(*this, pname, params);
            if (typeid(T) == typeid(GLuint)) glGetTextureParameterIuiv(*this, pname, params);
            return params;
        }



        // texture storage (accept GLM vector)
        void storage(GLsizei levels, GLenum internalformat, GLsizei size) {
            glTextureStorage1D(*this, levels, internalformat, size);
        }

        void storage(GLsizei levels, GLenum internalformat, glm::uvec2 size) {
            glTextureStorage2D(*this, levels, internalformat, size.x, size.y);
        }

        void storage(GLsizei levels, GLenum internalformat, glm::uvec3 size) {
            glTextureStorage3D(*this, levels, internalformat, size.x, size.y, size.z);
        }


        // subimage (accept GLM vector)
        void subimage(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage3D(*this, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, pixels);
        }

        void subimage(GLint level, glm::ivec2 offset, glm::uvec2 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage2D(*this, level, offset.x, offset.y, size.x, size.y, format, type, pixels);
        }

        void subimage(GLint level, GLint offset, GLsizei size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage1D(*this, level, offset, size, format, type, pixels);
        }


        // simplified version of very hard function
        void copy_mage_subdata(GLint srcLevel, glm::ivec3 srcOffset, texture destination, GLint dstLevel, glm::ivec3 dstOffset, glm::uvec3 size) {
            glCopyImageSubData(*this, (GLenum)this->target(), srcLevel, srcOffset.x, srcOffset.y, srcOffset.z, *destination, (GLenum)destination->target(), dstLevel, dstOffset.x, dstOffset.y, dstOffset.z, size.x, size.y, size.z);
        }


        // texture of buffer 
        void buffer(GLenum internalformat, buffer buf){
            glTextureBuffer(*this, internalformat, *buf);
        }

    };


    // universal sampler object
    class sampler_class: public base_class {
    protected:
        sampler_class(){
            glCreateSamplers(1, *this);
        }

    public:
        static sampler create(){
            return (new sampler_class());
        }

        ~sampler_class(){
            glDeleteSamplers(1, *this);
        }


        template<class T>
        void parameter(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glSamplerParameteriv(*this, pname, params);
            if (typeid(T) == typeid(float)) glSamplerParameterfv(*this, pname, params);
        }


        template<class T>
        void parameter_int(GLenum pname, T * params) const {
            if (typeid(T) == typeid(int)) glSamplerParameterIiv(*this, pname, params);
            if (typeid(T) == typeid(GLuint)) glSamplerParameterIuiv(*this, pname, params);
        }



        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetSamplerParameteriv(*this, pname, params);
            if (typeid(T) == typeid(float)) glGetSamplerParameterfv(*this, pname, params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if (typeid(T) == typeid(int)) glGetSamplerParameterIiv(*this, pname, params);
            if (typeid(T) == typeid(GLuint)) glGetSamplerParameterIuiv(*this, pname, params);
            return params;
        }

    };


    // texture binding
    class texture_binding_class: public base_class {
    protected:
        texture_binding_class(GLuint binding = 0) {this->set_object(binding);}

    public:
        static texture_binding create(GLuint binding = 0){
            return (new texture_binding_class(binding));
        }

        ~texture_binding_class(){}

        void bind_sampler(sampler sam) {
            glBindSampler(*this, *sam);
        }

        void bind_texture(texture tex) {
            glBindTextureUnit(*this, *tex);
        }
    };


    // image binding
    class image_class: public base_class {
    protected:
        image_class(GLuint binding = 0) {this->set_object(binding);}

    public:
        static image create(GLuint binding = 0){
            return (new image_class(binding));
        }

        // bind image texture
        void bind_texture(texture texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
            glBindImageTexture(*this, *texture, level, layered, layer, access, format);
        }
    };


    // contexted texture binding
    class _texture_context: public base_class {
    public:
        _texture_context(GLuint binding = 0) {this->set_object(binding);}        

        // create texture
        texture create(){
            return (new texture_class(this));
        }

        // context named binding
        void bind(texture tex){
            glBindTexture(*this, *tex);
        }
    };



    texture_class::texture_class(_texture_context * gltarget) {
        glCreateTextures((GLenum)*gltarget, 1, *this);
    }



    // texture targets
    namespace texture_target {
        _texture_context t_cube(GL_TEXTURE_CUBE_MAP);
        _texture_context t_buffer(GL_TEXTURE_BUFFER);
        _texture_context t_1d(GL_TEXTURE_1D);
        _texture_context t_2d(GL_TEXTURE_2D);
        _texture_context t_3d(GL_TEXTURE_3D);
        _texture_context t_2d_msaa(GL_TEXTURE_2D_MULTISAMPLE);
        _texture_context t_1d_array(GL_TEXTURE_1D_ARRAY);
        _texture_context t_2d_array(GL_TEXTURE_2D_ARRAY);
    };



    // bindless pointers manipulations
    namespace bindless_texture { // TODO support

    };


};