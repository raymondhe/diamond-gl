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
        bool allocated = false;
        GLuint * globj = nullptr;
        void set_object(GLuint &&obj) { allocate(1); *globj = obj; }
        void set_object(GLuint &obj) { deallocate(); globj = &obj; }
        void set_object(GLuint *obj) { deallocate(); globj = obj; }
        void set_value(const GLuint& obj) { if (globj) *globj = obj; };
        operator GLuint*() { return globj; }
    public:
        base() { allocate(1); }
        ~base() { }
        void deallocate() { if (allocated && globj) { *globj = -1;/*delete globj;*/ } this->globj = nullptr; allocated = false; };
        void allocate(const size_t cnt) { globj = new GLuint[cnt]; allocated = true; }
        operator const GLuint&() const { return (*globj); }
    };



    // crop tuple element
    template <class Tm, class... T, size_t... Is>
    constexpr decltype(auto) get_globj_reduce_impl(GLuint * rest, std::tuple<Tm&, T&...> &t, std::index_sequence<Is...>) {
        *rest = (GLuint)std::get<0>(t); return std::make_tuple(std::ref(std::get<Is + 1>(t))...);
    }

    template <class... T>
    constexpr decltype(auto) get_globj_reduce(GLuint * rest, std::tuple<T&...> &t) {
        return get_globj_reduce_impl(rest, t, std::make_index_sequence<(sizeof...(T)) - 1>{});
    }

    // getting gl buffers
    template <class T>
    constexpr decltype(auto) get_globj_impl(GLuint * ptr, std::tuple<T&> &t) {
        *ptr = (GLuint)std::get<0>(t);
    }

    template <class... T>
    constexpr decltype(auto) get_globj_impl(GLuint * ptr, std::tuple<T&...> &t) {
        decltype(auto) rest_tuple = get_globj_reduce<T...>(ptr, t);
        get_globj_impl(ptr + 1, rest_tuple);
    }

    template <class... T>
    constexpr decltype(auto) get_globj(GLuint * ptr, std::tuple<T&...> &t) {
        return get_globj_impl(ptr, t);
    }

    template <class... T>
    constexpr GLuint *  get_globj_wrap(std::tuple<T&...> &t) {
        GLuint * ptr = new GLuint[sizeof...(T)];
        get_globj_impl(ptr, t);
        return ptr;
    }



    // create tuple of buffers
    template<typename... T, size_t... Is>
    constexpr decltype(auto) _ref_tuple_impl(std::tuple<T...> &t, std::index_sequence<Is...>) {
        return std::make_tuple(std::ref(std::get<Is>(t))...);
    }

    template<typename... T>
    constexpr decltype(auto) _ref_tuple(std::tuple<T...> &t) {
        return _ref_tuple_impl(t, std::index_sequence_for<T...>{});
    }




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
