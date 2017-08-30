#pragma once

#include "opengl.hpp"
#include <string>
#include <vector>

namespace NS_NAME {

    class program;


    class shader: public base {
    public:
        shader(GLenum shaderType) {
            this->set_object(glCreateShader(shaderType));
        }

        ~shader() {
            glDeleteShader(thisref);
            this->set_object(-1);
        }

        template<class T>
        T get_val(GLenum pname, T * params = nullptr) const {
            return *(this->get<T>(pname, params));
        }

        template<class T>
        T * get(GLenum pname, T * params = nullptr) const {
            if (!params) params = new T[1];
            if constexpr (std::is_same<T, int>::value) glGetShaderiv(thisref, pname, params);
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

        ~uniform() {
        }

        uniform(GLuint prog, GLuint location = 0) {
            this->set_object(location);
            program = prog;
        }

        // base templates
        template<class T>
        void set(T value){
            if constexpr (std::is_same<T, int>::value) glProgramUniform1i(program, thisref, value);
            if constexpr (std::is_same<T, GLuint>::value) glProgramUniform1ui(program, thisref, value);
            if constexpr (std::is_same<T, float>::value) glProgramUniform1f(program, thisref, value);
            if constexpr (std::is_same<T, double>::value) glProgramUniform1d(program, thisref, value);
            if constexpr (std::is_same<T, int64_t>::value) glProgramUniform1i64ARB(program, thisref, value);
            if constexpr (std::is_same<T, uint64_t>::value) glProgramUniform1ui64ARB(program, thisref, value);
        }

        template<class T>
        void set(const std::vector<T>& values){
            if constexpr (std::is_same<T, int>::value) glProgramUniform1iv(program, thisref, value.size(), value.data());
            if constexpr (std::is_same<T, GLuint>::value) glProgramUniform1uiv(program, thisref, value.size(), value.data());
            if constexpr (std::is_same<T, float>::value) glProgramUniform1fv(program, thisref, value.size(), value.data());
            if constexpr (std::is_same<T, double>::value) glProgramUniform1dv(program, thisref, value.size(), value.data());
            if constexpr (std::is_same<T, int64_t>::value) glProgramUniform1iv64ARB(program, thisref, value.size(), value.data());
            if constexpr (std::is_same<T, uint64_t>::value) glProgramUniform1uiv64ARB(program, thisref, value.size(), value.data());
        }
    };

    template<class T>
    class uniform_typed : public uniform {
    public:
        uniform_typed(GLuint prog, GLuint location = 0): uniform(prog, location) {}
        ~uniform_typed() {}

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
            this->set_object(glCreateProgram());
        }

        program(const std::vector<std::string>& shaders, GLenum shaderType){
            const GLchar ** parts = new const GLchar*[shaders.size()];
            for (int i = 0; i < shaders.size(); i++) {
                parts[i] = shaders[i].c_str();
            }
            this->set_object(glCreateShaderProgramv(shaderType, shaders.size(), parts));
        }

        program(std::string source, GLenum shaderType){
            const GLchar * src = source.c_str();
            this->set_object(glCreateShaderProgramv(shaderType, 1, &src));
        }

        ~program() { 
            glDeleteProgram(thisref);
            this->set_object(-1);
        }

        uniform get_uniform(GLuint location) const {
            return std::move(uniform(thisref, location));
        }

        uniform get_uniform(std::string name) const {
            return std::move(get_uniform(glGetUniformLocation(thisref, name.c_str())));
        }

        template<class T>
        uniform_typed<T> get_uniform(GLuint location) const {
            return std::move(uniform_typed<T>((GLuint)thisref, location));
        }

        template<class T>
        uniform_typed<T> get_uniform(std::string name) const {
            return std::move(this->get_uniform<T>(glGetUniformLocation(thisref, name.c_str())));
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
            if constexpr (std::is_same<T, int>::value) glGetProgramiv(thisref, pname, params);
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
             glCreateProgramPipelines(1, thisref);
         }
        ~program_pipeline() {
            glDeleteProgramPipelines(1, thisref);
            this->set_object(-1);
        }

        void use_stages(program_stage_bits stages, program& prog){
            glUseProgramStages(thisref, stages.bitfield, prog);
        }

        void active_program(program& prog){
            glActiveShaderProgram(thisref, prog);
        }
    };


};
