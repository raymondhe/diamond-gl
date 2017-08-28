#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"
#include <tuple>

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



    // crop tuple element
    template <class Tm, class... T, size_t... Is>
    constexpr decltype(auto) get_globj_reduce_impl(GLuint * rest, std::tuple<Tm, T...> &t, std::index_sequence<Is...>) {
        *rest = (GLuint)std::get<0>(t); return std::make_tuple(std::get<Is + 1>(t)...);
    }

    template <class... T>
    constexpr decltype(auto) get_globj_reduce(GLuint * rest, std::tuple<T...> &t) {
        return get_globj_reduce_impl(rest, t, std::make_index_sequence<(sizeof...(T)) - 1>{});
    }

    // getting gl buffers
    template <class T>
    constexpr decltype(auto) get_globj_impl(GLuint * ptr, std::tuple<T> &t) {
        *ptr = (GLuint)std::get<0>(t);
    }

    template <class... T>
    constexpr decltype(auto) get_globj_impl(GLuint * ptr, std::tuple<T...> &t) {
        decltype(auto) rest_tuple = get_globj_reduce<T...>(ptr, t);
        get_globj_impl(ptr + 1, rest_tuple);
    }

    template <class... T>
    constexpr decltype(auto) get_globj(GLuint * ptr, std::tuple<T...> &t) {
        return get_globj_impl(ptr, t);
    }



};
