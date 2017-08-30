#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"
#include <memory>

#define thisref (*this)
#define NS_NAME dgl

namespace NS_NAME {

    class base {
    protected:
        std::shared_ptr<GLuint> globj;
        void set_object(std::shared_ptr<GLuint>& obj) { globj = obj; } // share pointer
        void set_object(std::shared_ptr<GLuint>&& obj) { globj = obj; } // share pointer
        void set_object(GLuint&& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLuint& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLuint* obj) { globj = std::shared_ptr<GLuint>(obj); }
        void set_object(GLint&& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLint& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLint* obj) { globj = std::shared_ptr<GLuint>((GLuint *)obj); }
        operator GLuint*() { return globj.get(); }

        std::shared_ptr<GLuint>& _get_shared() {
            return globj;
        }
    public:
        base() { }
        ~base() { }
        operator const GLuint&() const { return (*globj); }
        void make_ptr() {
            globj = std::shared_ptr<GLuint>(new GLuint[1]{0xFFFFFFFF});
        }
        bool ready_free() {
            return globj.use_count() <= 1; // count references before remove
        }
        bool is_active() {
            return !!globj;
        }
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
