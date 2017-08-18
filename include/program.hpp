#pragma once

#include "opengl.hpp"
#include <string>
#include <vector>

namespace dgl {

    class shader_class;
    class program_class;
    class program_pipeline_class;
    class uniform_class;

    typedef uniform_class* uniform;
    typedef shader_class* shader;
    typedef program_class* program;
    typedef program_pipeline* program_pipeline;

    class shader_class: public base_class {
    protected:
        shader_class(GLenum shaderType) {this->set_object(glCreateShader(shaderType));}

    public:

        shader create(GLenum shaderType){
            return (new shader_class(shaderType));
        }

        template<typename T>
        T get(GLenum pname, T * params = GLint[1]) {
            return *(this->get<T *>(pname, params));
        }

        // parameter float vector by type cast
        template<GLint *>
        GLint * get(GLenum pname, GLint * params = GLint[8]) {
            glGetShaderiv(*this, pname, params);
            return params;
        }

        std::string info_log(){
            GLsizei lsize = this->get<GLint>(GL_INFO_LOG_LENGTH);
            GLsizei size = lsize;
            GLchar * info = new GLchar[lsize];
            glGetShaderInfoLog(*this, lsize, &size, info);
            return std::string(info, size);
        }

        void source(std::vector<std::string> shaders) {
            const GLchar ** parts = new const GLchar *[shaders.size()];
            GLsizei * sizes = new GLsizei[shaders.size()];
            for (int i = 0; i < shaders.size(); i++) {
                parts[i] = shaders[i].c_str();
                sizes[i] = shaders[i].size();
            }
            glShaderSource(*this, shaders.size(), parts, sizes);
        }

        void source(std::string source){
            const GLsizei size = source.size();
            const GLchar * cstr = source.c_str();
            glShaderSource(*this, 1, &cstr, &size);
        }

        void binary(std::vector<GLchar> binary, GLenum binType = GL_SPIR_V_BINARY){
            glShaderBinary(1, *this, binType, binary.data(), binary.size());
        }

        void specialize(std::string entry_point = "main", std::vector<GLuint> constantIndex = std::vector<GLuint>(0), GLuint * constantValue = nullptr){
            glSpecializeShader(*this, entry_point.c_str(), constantIndex.size(), constantIndex.data(), constantValue);
        }

        void compile(){
            glCompileShader(*this);
        }

    };



    class uniform_class: public base_class {
    protected:
        friend program_class;
        GLuint program = 0;
        uniform_class(GLuint prog, GLuint location = 0){
            program = prog;
            *this = location;
        }

    public:

        // base templates

        template<class T>
        set(T value){}

        template<class T>
        set(std::vector<T> values){}


        // basic vector values

        template<int> set(int value){
            glProgramUniform1i(program, value);
        }

        template<GLuint> set(GLuint value){
            glProgramUniform1ui(program, value);
        }

        template<float> set(float value){
            glProgramUniform1f(program, value);
        }

        template<double> set(double value){
            glProgramUniform1d(program, value);
        }

        template<int64_t> set(int64_t value){
            glProgramUniform1i64ARB(program, value);
        }

        template<uint64_t> set(uint64_t value){
            glProgramUniform1ui64ARB(program, value);
        }


        // basic type vectors

        template<int> set(std::vector<int> value){
            glProgramUniform1iv(program, value.size(), value.data());
        }

        template<GLuint> set(std::vector<GLuint> value){
            glProgramUniform1uiv(program, value.size(), value.data());
        }

        template<float> set(std::vector<float> value){
            glProgramUniform1fv(program, value.size(), value.data());
        }

        template<double> set(std::vector<double> value){
            glProgramUniform1dv(program, value.size(), value.data());
        }

        template<int64_t> set(std::vector<int64_t> value){
            glProgramUniform1iv64ARB(program, value.size(), value.data());
        }

        template<uint64_t> set(std::vector<uint64_t> value){
            glProgramUniform1uiv64ARB(program, value.size(), value.data());
        }
    };



    class program_class: public base_class {
    protected:
        program_class() {this->set_object(glCreateProgram());}

        program_class(std::vector<std::string> shaders, GLenum shaderType){
            const GLchar ** parts = new const GLchar*[shaders.size()];
            for (int i = 0; i < shaders.size(); i++) {
                parts[i] = shaders[i].c_str();
            }
            (GLuint&)*this = glCreateShaderProgramv(shaderType, shaders.size(), parts);
        }

        program_class(std::string source, GLenum shaderType){
            const GLchar * src = source.c_str();
            (GLuint&)*this = glCreateShaderProgramv(shaderType, 1, &src);
        }

    public:

        ~program_class() { glDeleteProgram(*this); }

        program create(){
            return (new program_class());
        }

        program create(std::string source, GLenum shaderType){
            return (new program_class(source, shaderType));
        }

        program create(std::vector<std::string> shaders, GLenum shaderType){
            return (new program_class(shaders, shaderType));
        }


        uniform get_uniform(std::string name){
            return get_uniform(glGetUniformLocation(*this, name.c_str()));
        }

        uniform get_uniform(GLuint location){
            return (new uniform_class(*this, location));
        }



        void attach(shader shad){
            glAttachShader(*this, *shad);
        }

        void link(){
            glLinkProgram(*this);
        }


        template<typename T>
        T get(GLenum pname, T * params = GLint[1]) {
            return *(this->get<T *>(pname, params));
        }

        // parameter float vector by type cast
        template<GLint *>
        GLint * get(GLenum pname, GLint * params = GLint[8]) {
            glGetProgramiv(*this, pname, params);
            return params;
        }

        std::string info_log(){
            GLsizei lsize = get<GLint>(GL_INFO_LOG_LENGTH);
            GLsizei size = lsize;
            GLchar * info = new GLchar[lsize];
            glGetProgramInfoLog(*this, lsize, &size, info);
            return std::string(info, size);
        }
    };


    // program pipeline
    class program_pipeline_class: public base_class {
    protected:
         program_pipeline_class() {glCreateProgramPipelines(1, *this);}

    public:
        ~program_pipeline_class() {glDeleteProgramPipelines(1, *this);}

        program_pipeline create(){
            return (new program_pipeline_class());
        }

        void use_stages(GLbitfield stages, program prog){
            glUseProgramStages(*this, stages, *prog);
        }

        void active_program(program prog){
            glActiveShaderProgram(*this, *prog);
        }

        void bind(){
            glBindProgramPipeline(*this);
        }
    };


};
