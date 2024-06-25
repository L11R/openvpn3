//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2022 OpenVPN Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

/**
 * @file ostream_containers.hpp
 * @brief Coercion safe method to insert a container into an ostream
 *
 * The contained type T must be ostream'able
 */

#pragma once

#include <ostream>

namespace openvpn {

namespace C2os {

/**
 * @brief New typename for holding the underlying container type
 *
 * Note also that C2os::Container could be extended by passing in optional arguments to
 * the Container ctor that could adjust the formatting (e.g., different delimiter,
 * curlies vs square brackets, etc.)
 *
 * @tparam C underlying container type
 */
template <typename C>
struct Container
{
    explicit Container(const C &container)
        : ref(container)
    {
    }
    const C &ref;
};

/**
 * @brief Provide an instance of C2os::Container<C> from the underlying container
 *
 * This is the only interface the consumer sees (but see cast_deref, below)
 *
 * @tparam C underlying container type (deduced from the container param)
 * @param container instance of the underlying container
 * @return const C2os::Container<C> instance
 */
template <typename C>
inline const auto cast(const C &container)
{
    return Container<C>(container);
}

/**
 * @brief overload ostream operator<< for the C2os::Container<C> type
 *
 * @tparam C underlying container type
 * @param os ostream instance
 * @param container instance of the underlying container
 * @return std::ostream& param os
 */
template <typename C>
inline std::ostream &operator<<(std::ostream &os, const Container<C> &container)
{
    constexpr char separator[] = ", ";
    const char *delimiter = "";
    os << "[";
    for (const auto &e : container.ref)
    {
        os << delimiter << e;
        delimiter = separator;
    }
    os << "]";
    return os;
}

/**
 * @brief same idea as C2os::Container<C>, above; importantly, with a different typename
 *
 * @tparam C underlying container type
 */
template <typename C>
struct PtrContainer : Container<C>
{
    using Container<C>::Container;
};

/**
 * @brief Provide an instance of C2os::PtrContainer<C> from the underlying container
 *
 * This is the only interface the consumer sees if he wants to dereference the contained
 * type T \a before ostream'ing it
 *
 * @tparam C underlying container type (deduced from the container param)
 * @param container instance of the underlying container
 * @return const C2os::PtrContainer<C> instance
 */
template <typename C>
inline const auto cast_deref(const C &container)
{
    return PtrContainer<C>(container);
}

/**
 * @brief overload ostream operator<< for the C2os::PtrContainer<C> type
 *
 * @tparam C underlying container type
 * @param os ostream instance
 * @param container instance of the underlying container
 * @return std::ostream& param os
 */
template <typename C>
inline std::ostream &operator<<(std::ostream &os, const PtrContainer<C> &container)
{
    constexpr char separator[] = ", ";
    const char *delimiter = "";
    os << "[";
    for (const auto &e : container.ref)
    {
        os << delimiter << *e;
        delimiter = separator;
    }
    os << "]";
    return os;
}

} // namespace C2os
} // namespace openvpn
