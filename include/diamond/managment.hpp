#pragma once

#include "opengl.hpp"
#include "program.hpp"
#include "vertex_array.hpp"

namespace dgl {

    class _managment {
    public:
        void use_program(dgl::program& prog){
            glUseProgram((GLuint)prog);
        }

        void bind_program_pipeline(dgl::program_pipeline& ppl){
            glBindProgramPipeline((GLuint)ppl);
        }
       
        void bind_vertex_array(dgl::vertex_array& vao) {
            glBindVertexArray((GLuint)vao);
        }
    };

    _managment managment;
};