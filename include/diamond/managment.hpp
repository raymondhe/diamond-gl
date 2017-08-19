#pragma once

#include "opengl.hpp"
#include "program.hpp"
#include "vertex_array.hpp"

namespace dgl {
    namespace managment {

        namespace program {
            void use(dgl::program prog){
                glUseProgram(*prog);
            }

            namespace pipeline {
                void bind(dgl::program_pipeline ppl){
                    glBindProgramPipeline(*ppl);
                }
            }
        }

        namespace vertex_array {
            void bind(dgl::vertex_array vao){
                glBindVertexArray(*vao);
            }
        }
    };
};