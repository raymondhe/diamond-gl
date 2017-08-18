#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"

namespace dgl {
    class base_class {
    protected:
        GLuint globj = 0;
        void set_object(GLuint obj) { globj = obj; }
        operator GLuint&() { return globj; }
        operator GLuint*() { return &globj; }
    public:
        operator GLenum() const { return (GLenum)globj; }
        operator GLuint() const { return globj; }
    };
};
