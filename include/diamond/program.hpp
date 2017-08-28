#pragma once

#include "opengl.hpp"
#include <string>
#include <vector>

namespace NS_NAME {

    class program;


    class shader: public base {
    public:
        shader(GLenum shaderType) {
            base::allocate(1);
            GLuint shader = glCreateShader(shaderType);
            this->set_value(shader);
        }

        template<class T>
        T get_val(GLenum pname, T * params = nullptr) const {
            return *(this->get<T>(pname, params));
        }

        template<class T>
        T * get(GLenum pname, T * params = nullptr) const {
            if (!params) params = new T[1];
            if (typeid(T) == typeid(int)) glGetShaderiv(thisref, pname, params);
            return params;
        }

        std::string info_log(){
            GLsizei lsize = this->get_val<GLint>(GL_INFO_LOG_LENGTH);
            GLsizei size = lsize;
            GLchar * info = new GLchar[lsize];
            glGetShaderInfoLog(thisref, lsize, &size, info);
            return std::string(info, size);
        }

        void source(const std::vector<std::string>& shaders) {
            const GLchar ** parts = new const GLchar *[shaders.size()];
            GLsizei * sizes = new GLsizei[shaders.size()];
            for (int i = 0; i < shaders.size(); i++) {
                parts[i] = shaders[i].c_str();
                sizes[i] = shaders[i].size();
            }
            glShaderSource(thisref, shaders.size(), parts, sizes);
        }

        void source(std::string source){
            const GLsizei size = source.size();
            const GLchar * cstr = source.c_str();
            glShaderSource(thisref, 1, &cstr, &size);
        }

        void binary(const std::vector<GLchar>& binary, GLenum binType = GL_SPIR_V_BINARY){
            glShaderBinary(1, thisref, binType, binary.data(), binary.size());
        }

        void specialize(std::string entry_point = "main", const std::vector<GLuint> &constantIndex = std::vector<GLuint>(0), const GLuint * constantValue = nullptr){
            glSpecializeShader(thisref, entry_point.c_str(), constantIndex.size(), constantIndex.data(), constantValue);
        }

        void compile(){
            glCompileShader(thisref);
        }

    };



    class uniform: public base {
    protected:
        friend program;
        GLuint program = 0;

    public:

        uniform(GLuint prog, GLuint location = 0) {
            base::allocate(1);
            this->set_value(location);
            program = prog;
        }

        // base templates
        template<class T>
        void set(T value){
            if  (typeid(T) == typeid(int)) glProgramUniform1i(program, thisref, value);
            if  (typeid(T) == typeid(GLuint)) glProgramUniform1ui(program, thisref, value);
            if  (typeid(T) == typeid(float)) glProgramUniform1f(program, thisref, value);
            if  (typeid(T) == typeid(double)) glProgramUniform1d(program, thisref, value);
            if  (typeid(T) == typeid(int64_t)) glProgramUniform1i64ARB(program, thisref, value);
            if  (typeid(T) == typeid(uint64_t)) glProgramUniform1ui64ARB(program, thisref, value);
        }

        template<class T>
        void set(const std::vector<T>& values){
            if  (typeid(T) == typeid(int)) glProgramUniform1iv(program, thisref, value.size(), value.data());
            if  (typeid(T) == typeid(GLuint)) glProgramUniform1uiv(program, thisref, value.size(), value.data());
            if  (typeid(T) == typeid(float)) glProgramUniform1fv(program, thisref, value.size(), value.data());
            if  (typeid(T) == typeid(double)) glProgramUniform1dv(program, thisref, value.size(), value.data());
            if  (typeid(T) == typeid(int64_t)) glProgramUniform1iv64ARB(program, thisref, value.size(), value.data());
            if  (typeid(T) == typeid(uint64_t)) glProgramUniform1uiv64ARB(program, thisref, value.size(), value.data());
        }
    };

    template<class T>
    class uniform_typed : public uniform {
    public:
        uniform_typed(GLuint prog, GLuint location = 0): uniform(prog, location) {}

        void set(T value) {
            uniform::set<T>(value);
        }

        void set(const std::vector<T>& values) {
            uniform::set<T>(values);
        }

        uniform_typed<T>& operator=(T value) {
            this->set(value);
            return thisref;
        }
        
        uniform_typed<T>& operator=(const std::vector<T>& values) {
            this->set(values);
            return thisref;
        }
    };



    class program: public base {
    public:
        program() {
            base::allocate(1);
            GLuint program = glCreateProgram();
            this->set_value(program);
        }

        program(const std::vector<std::string>& shaders, GLenum shaderType){
            const GLchar ** parts = new const GLchar*[shaders.size()];
            for (int i = 0; i < shaders.size(); i++) {
                parts[i] = shaders[i].c_str();
            }
            GLuint program = glCreateShaderProgramv(shaderType, shaders.size(), parts);
            this->set_value(program);
        }

        program(std::string source, GLenum shaderType){
            const GLchar * src = source.c_str();
            GLuint program = glCreateShaderProgramv(shaderType, 1, &src);
            this->set_value(program);
        }

        ~program() { glDeleteProgram(thisref); }

        

        uniform&& get_uniform(GLuint location) const {
            return uniform((GLuint)thisref, location);
        }

        uniform&& get_uniform(std::string name) const {
            return get_uniform(glGetUniformLocation(thisref, name.c_str()));
        }

        template<class T>
        uniform_typed<T>&& get_uniform(GLuint location) const {
            return uniform_typed<T>((GLuint)thisref, location);
        }

        template<class T>
        uniform_typed<T>&& get_uniform(std::string name) const {
            return this->get_uniform<T>(glGetUniformLocation(thisref, name.c_str()));
        }



        void attach(shader& shad){
            glAttachShader(thisref, shad);
        }

        void link(){
            glLinkProgram(thisref);
        }


        template<class T>
        T get_val(GLenum pname, T * params = nullptr) const {
            return *thisref.get<T>(pname, params);
        }


        template<class T>
        T * get(GLenum pname, T * params = nullptr) const {
            if (!params) params = new T[1];
            if (typeid(T) == typeid(int)) glGetProgramiv(thisref, pname, params);
            return params;
        }

        std::string info_log() const {
            GLsizei lsize = get_val<GLint>(GL_INFO_LOG_LENGTH);
            GLsizei size = lsize;
            GLchar * info = new GLchar[lsize];
            glGetProgramInfoLog(thisref, lsize, &size, info);
            return std::string(info, size);
        }
    };


    // program pipeline
    class program_pipeline: public base {
    public:
         program_pipeline() {
             base::allocate(1);
             glCreateProgramPipelines(1, thisref);
         }
        ~program_pipeline() {glDeleteProgramPipelines(1, thisref);}

        void use_stages(program_stage_bits stages, program& prog){
            glUseProgramStages(thisref, stages.bitfield, prog);
        }

        void active_program(program& prog){
            glActiveShaderProgram(thisref, prog);
        }
    };


};
