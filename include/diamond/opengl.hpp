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
        GLuint * globj = new GLuint[1];
        void set_object(GLuint &obj) { globj = &obj; }
        void set_value(const GLuint& obj) { *globj = obj; };
        operator GLuint*() { return globj; }
    public:
        base() { allocate(1); }
        void allocate(const size_t cnt) { globj = new GLuint[cnt]; }
        operator const GLuint&() const { return (*globj); }
    };
};
