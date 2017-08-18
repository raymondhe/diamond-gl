#pragma once

#include "opengl.hpp"
#include "buffer.hpp"

namespace dgl {

    class texture_class;
    class sampler_class;
    class texture_binding_class;
    class image_binding_class;

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
        texture_class(_texture_context * gltarget){
            glCreateTextures(*gltarget, 1, *this);
        }

    public:
        ~texture_class(){
            glDeleteTextures(1, *this);
        }

        _texture_context * target() const {
            return gltarget;
        }


        template<typename T>
        void parameter(GLenum pname, T param) const {
            this->parameter<T *>(pname, &param);
        }

        template<typename T>
        void parameter_int(GLenum pname, T param) const {
            this->parameter_int<T *>(pname, &param);
        }

        template<typename T>
        T get_parameter(GLenum pname, T * params = GLint[1]) {
            return *(this->get_parameter<T *>(pname, params));
        }

        template<typename T>
        T get_parameter_int(GLenum pname, T * params = GLint[1]) {
            return *(this->get_parameter_int<T *>(pname, params));
        }


        // parameter float vector by type cast
        template<GLint *>
        void parameter(GLenum pname, GLint * params) const {
            glTextureParameteriv(*this, pname, params);
        }

        template<GLfloat *>
        void parameter(GLenum pname, GLfloat * params) const {
            glTextureParameterfv(*this, pname, params);
        }


        // parameter integer array
        template<GLint *>
        void parameter_int(GLenum pname, GLint * params) const {
            glTextureParameterIiv(*this, pname, params);
        }

        template<GLuint *>
        void parameter_int(GLenum pname, GLuint * params) const {
            glTextureParameterIuiv(*this, pname, params);
        }


        // parameter float vector by type cast
        template<GLint *>
        GLint * get_parameter(GLenum pname, GLint * params = GLint[8]){
            glGetTextureParameteriv(*this, pname, params);
            return params;
        }

        template<GLfloat *>
        GLfloat * get_parameter(GLenum pname, GLfloat * params = GLfloat[8]){
            glGetTextureParameterfv(*this, pname, params);
            return params;
        }


        // parameter integer array
        template<GLint *>
        GLint * get_parameter_int(GLenum pname, GLint * params = GLint[8]){
            glGetTextureParameterIiv(*this, pname, params);
            return params;
        }

        template<GLuint *>
        GLuint * get_parameter_int(GLenum pname, GLuint * params = GLuint[8]){
            glGetTextureParameterIuiv(*this, pname, params);
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


        template<typename T>
        void parameter(GLenum pname, T param) const {
            this->parameter<T *>(pname, &param);
        }

        template<typename T>
        void parameter_int(GLenum pname, T param) const {
            this->parameter_int<T *>(pname, &param);
        }

        template<typename T>
        T get_parameter(GLenum pname, T * params = GLint[1]) {
            return *(this->get_parameter<T *>(pname, params));
        }

        template<typename T>
        T get_parameter_int(GLenum pname, T * params = GLint[1]) {
            return *(this->get_parameter_int<T *>(pname, params));
        }


        // parameter float vector by type cast
        template<GLint *>
        GLint * get_parameter(GLenum pname, GLint * params = GLint[8]) {
            glGetSamplerParameteriv(*this, pname, params);
            return params;
        }

        template<GLfloat *>
        GLfloat * get_parameter(GLenum pname, GLfloat * params = GLfloat[8]) {
            glGetSamplerParameterfv(*this, pname, params);
            return params;
        }


        // parameter integer array
        template<GLint *>
        GLint * get_parameter_int(GLenum pname, GLint * params = GLint[8]) {
            glGetSamplerParameterIiv(*this, pname, params);
            return params;
        }

        template<GLuint *>
        GLuint * get_parameter_int(GLenum pname, GLuint * params = GLuint[8]) {
            glGetSamplerParameterIuiv(*this, pname, params);
            return params;
        }



        // parameter float vector by type cast
        template<GLint *>
        void parameter(GLenum pname, GLint * params) const {
            glSamplerParameteriv(*this, pname, params);
        }

        template<GLfloat *>
        void parameter(GLenum pname, GLfloat * params) const {
            glSamplerParameterfv(*this, pname, params);
        }


        // parameter integer array
        template<GLint *>
        void parameter_int(GLenum pname, GLint * params) const {
            glSamplerParameterIiv(*this, pname, params);
        }

        template<GLuint *>
        void parameter_int(GLenum pname, GLuint * params) const {
            glSamplerParameterIuiv(*this, pname, params);
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