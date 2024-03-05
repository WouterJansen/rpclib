//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2020 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_TYPE_COMPLEX_HPP
#define MSGPACK_V1_TYPE_COMPLEX_HPP

#include <complex>
#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/meta.hpp"

namespace clmdep_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

#if !defined(MSGPACK_USE_CPP03)

template <typename T>
struct as<std::complex<T>, typename std::enable_if<clmdep_msgpack::has_as<T>::value>::type> {
    std::complex<T> operator()(clmdep_msgpack::object const& o) const {
        if (o.type != clmdep_msgpack::type::ARRAY)
            throw clmdep_msgpack::type_error();
        if (o.via.array.size != 2)
            throw clmdep_msgpack::type_error();
        return std::complex<T>(o.via.array.ptr[0].as<T>(), o.via.array.ptr[1].as<T>());
    }
};

#endif // !defined(MSGPACK_USE_CPP03)

template <typename T>
struct convert<std::complex<T> > {
    clmdep_msgpack::object const& operator()(clmdep_msgpack::object const& o, std::complex<T>& v) const {
        if(o.type != clmdep_msgpack::type::ARRAY)
            throw clmdep_msgpack::type_error();
        if(o.via.array.size != 2)
            throw clmdep_msgpack::type_error();
        T real;
        T imag;
        o.via.array.ptr[0].convert(real);
        o.via.array.ptr[1].convert(imag);
        v.real(real);
        v.imag(imag);
        return o;
    }
};

template <typename T>
struct pack<std::complex<T> > {
    template <typename Stream>
    clmdep_msgpack::packer<Stream>& operator()(clmdep_msgpack::packer<Stream>& o, std::complex<T> const& v) const {
        o.pack_array(2);
        o.pack(v.real());
        o.pack(v.imag());
        return o;
    }
};

template <typename T>
struct object_with_zone<std::complex<T> > {
    void operator()(clmdep_msgpack::object::with_zone& o, std::complex<T> const& v) const {
        o.type = clmdep_msgpack::type::ARRAY;
        clmdep_msgpack::object* p = static_cast<clmdep_msgpack::object*>(o.zone.allocate_align(sizeof(clmdep_msgpack::object)*2, MSGPACK_ZONE_ALIGNOF(clmdep_msgpack::object)));
        o.via.array.ptr = p;
        o.via.array.size = 2;
        p[0] = clmdep_msgpack::object(v.real(), o.zone);
        p[1] = clmdep_msgpack::object(v.imag(), o.zone);
    }
};

} // namespace adaptor

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace clmdep_msgpack


#endif // MSGPACK_V1_TYPE_COMPLEX_HPP
