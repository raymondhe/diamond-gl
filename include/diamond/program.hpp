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
    typedef program_pipeline_class* program_pipeline;

    class shader_class: public base_class {
    protected:
        shader_class(GLenum shaderType) {this->set_object(glCreateShader(shaderType));}

    public:

        static shader create(GLenum shaderType){
            return (new shader_class(shaderType));
        }

        template<class T>
        T get_val(GLenum pname, T * params = nullptr) {
            return *(this->get<T>(pname, params));
        }

        template<class T>
        T * get(GLenum pname, T * params = nullptr) {
            if (!params) params = {0};
            if (typeid(T) == typeid(int)) glGetShaderiv(*this, pname, params);
            return params;
        }

        std::string info_log(){
            GLsizei lsize = this->get_val<GLint>(GL_INFO_LOG_LENGTH);
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
            this->set_object(location);
            program = prog;
        }

    public:

        // base templates
        template<class T>
        void set(T value){
            if  (typeid(T) == typeid(int)) glProgramUniform1i(program, *this, value);
            if  (typeid(T) == typeid(GLuint)) glProgramUniform1ui(program, *this, value);
            if  (typeid(T) == typeid(float)) glProgramUniform1f(program, *this, value);
            if  (typeid(T) == typeid(double)) glProgramUniform1d(program, *this, value);
            if  (typeid(T) == typeid(int64_t)) glProgramUniform1i64ARB(program, *this, value);
            if  (typeid(T) == typeid(uint64_t)) glProgramUniform1ui64ARB(program, *this, value);
        }

        template<class T>
        void set(std::vector<T> values){
            if  (typeid(T) == typeid(int)) glProgramUniform1iv(program, *this, value.size(), value.data());
            if  (typeid(T) == typeid(GLuint)) glProgramUniform1uiv(program, *this, value.size(), value.data());
            if  (typeid(T) == typeid(float)) glProgramUniform1fv(program, *this, value.size(), value.data());
            if  (typeid(T) == typeid(double)) glProgramUniform1dv(program, *this, value.size(), value.data());
            if  (typeid(T) == typeid(int64_t)) glProgramUniform1iv64ARB(program, *this, value.size(), value.data());
            if  (typeid(T) == typeid(uint64_t)) glProgramUniform1uiv64ARB(program, *this, value.size(), value.data());
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

        static program create(){
            return (new program_class());
        }

        static program create(std::string source, GLenum shaderType){
            return (new program_class(source, shaderType));
        }

        static program create(std::vector<std::string> shaders, GLenum shaderType){
            return (new program_class(shaders, shaderType));
        }


        

        uniform get_uniform(GLuint location){
            return (new uniform_class(*this, location));
        }

        uniform get_uniform(std::string name) {
            return get_uniform(glGetUniformLocation(*this, name.c_str()));
        }



        void use(){
            glUseProgram(*this);
        }

        void attach(shader shad){
            glAttachShader(*this, *shad);
        }

        void link(){
            glLinkProgram(*this);
        }


        template<class T>
        T get_val(GLenum pname, T * params = nullptr) {
            return *this->get<T>(pname, params);
        }


        template<class T>
        T * get(GLenum pname, T * params = nullptr) {
            if (!params) params = {0};
            if (typeid(T) == typeid(int)) glGetProgramiv(*this, pname, params);
            return params;
        }

        std::string info_log(){
            GLsizei lsize = get_val<GLint>(GL_INFO_LOG_LENGTH);
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

        static program_pipeline create(){
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
