//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2015 FURUHASHI Sadayuki
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_TYPE_DEQUE_HPP
#define MSGPACK_V1_TYPE_DEQUE_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/object.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <deque>

namespace clmdep_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

#if !defined(MSGPACK_USE_CPP03)

template <typename T, typename Alloc>
struct as<std::deque<T, Alloc>, typename std::enable_if<clmdep_msgpack::has_as<T>::value>::type> {
    std::deque<T, Alloc> operator()(const clmdep_msgpack::object& o) const {
        if (o.type != clmdep_msgpack::type::ARRAY) { throw clmdep_msgpack::type_error(); }
        std::deque<T, Alloc> v;
        if (o.via.array.size > 0) {
            clmdep_msgpack::object* p = o.via.array.ptr;
            clmdep_msgpack::object* const pend = o.via.array.ptr + o.via.array.size;
            do {
                v.push_back(p->as<T>());
                ++p;
            } while (p < pend);
        }
        return v;
    }
};

#endif // !defined(MSGPACK_USE_CPP03)

template <typename T, typename Alloc>
struct convert<std::deque<T, Alloc> > {
    clmdep_msgpack::object const& operator()(clmdep_msgpack::object const& o, std::deque<T, Alloc>& v) const {
        if(o.type != clmdep_msgpack::type::ARRAY) { throw clmdep_msgpack::type_error(); }
        v.resize(o.via.array.size);
        clmdep_msgpack::object* p = o.via.array.ptr;
        clmdep_msgpack::object* const pend = o.via.array.ptr + o.via.array.size;
        typename std::deque<T, Alloc>::iterator it = v.begin();
        for(; p < pend; ++p, ++it) {
            p->convert(*it);
        }
        return o;
    }
};

template <typename T, typename Alloc>
struct pack<std::deque<T, Alloc> > {
    template <typename Stream>
    clmdep_msgpack::packer<Stream>& operator()(clmdep_msgpack::packer<Stream>& o, const std::deque<T, Alloc>& v) const {
        uint32_t size = checked_get_container_size(v.size());
        o.pack_array(size);
        for(typename std::deque<T, Alloc>::const_iterator it(v.begin()), it_end(v.end());
            it != it_end; ++it) {
            o.pack(*it);
        }
        return o;
    }
};

template <typename T, typename Alloc>
struct object_with_zone<std::deque<T, Alloc> > {
    void operator()(clmdep_msgpack::object::with_zone& o, const std::deque<T, Alloc>& v) const {
        o.type = clmdep_msgpack::type::ARRAY;
        if(v.empty()) {
            o.via.array.ptr = MSGPACK_NULLPTR;
            o.via.array.size = 0;
        } else {
            uint32_t size = checked_get_container_size(v.size());
            clmdep_msgpack::object* p = static_cast<clmdep_msgpack::object*>(o.zone.allocate_align(sizeof(clmdep_msgpack::object)*size, MSGPACK_ZONE_ALIGNOF(clmdep_msgpack::object)));
            clmdep_msgpack::object* const pend = p + size;
            o.via.array.ptr = p;
            o.via.array.size = size;
            typename std::deque<T, Alloc>::const_iterator it(v.begin());
            do {
                *p = clmdep_msgpack::object(*it, o.zone);
                ++p;
                ++it;
            } while(p < pend);
        }
    }
};

} // namespace adaptor

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace clmdep_msgpack

#endif // MSGPACK_V1_TYPE_DEQUE_HPP
