#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"

#define thisref (*this)

namespace dgl {
    class base {
    protected:
        GLuint globj = 0;
        void set_object(GLuint obj) { globj = obj; }
        operator GLuint*() { return &globj; }
    public:
        operator GLuint() const { return globj; }
        operator GLuint&() { return globj; }
    };
};
