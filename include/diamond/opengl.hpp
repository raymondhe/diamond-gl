#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"
#include <tuple>

#define thisref (*this)
#define NS_NAME dgl

namespace NS_NAME {

    class base {
    protected:
        GLuint globj = 0;
        void set_object(GLint obj) { globj = obj; }
        void set_object(GLint * obj) { globj = *obj; }
        operator GLuint*() { return &globj; }
    public:
        base() { }
        ~base() { }
        operator const GLuint&() const { return globj; }
    };


    // some hack
    template <class... T>
    struct restor {};

    // getting strides of buffer types
    template <class Tm>
    constexpr decltype(auto) get_stride_impl(GLsizei * ptr, restor<Tm>) {
        *ptr = (GLsizei)sizeof(Tm);
    }

    template <class Tm, class... T>
    constexpr decltype(auto) get_stride_impl(GLsizei * ptr, restor<Tm, T...> rst) {
        *ptr = (GLsizei)sizeof(Tm);
        get_stride_impl<T...>(ptr + 1, restor<T...>{});
    }

    template <class... T>
    constexpr decltype(auto) get_stride(GLsizei * ptr) {
        return get_stride_impl<T...>(ptr, restor<T...>{});
    }

    template <class... T>
    constexpr GLsizei * get_stride_wrap() {
        GLsizei * ptr = new GLsizei[sizeof...(T)];
        get_stride<T...>(ptr);
        return ptr;
    }

};
