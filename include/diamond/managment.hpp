#pragma once

#include "opengl.hpp"
#include "program.hpp"
#include "vertex_array.hpp"

namespace NS_NAME {

    class _managment {
    public:
        void use_program(program& prog){
            glUseProgram((GLuint)prog);
        }

        void bind_program_pipeline(program_pipeline& ppl){
            glBindProgramPipeline((GLuint)ppl);
        }
       
        void bind_vertex_array(vertex_array& vao) {
            glBindVertexArray((GLuint)vao);
        }
    };

    _managment managment;
};