#pragma once

#include <string>
#include <sstream>

namespace util {
    const std::string DELIM = ", ";
    const std::string LCAP = "[";
    const std::string RCAP = "]";

    //! Join C-array
    // Similar to the Python join function.
    // @param arr Input C-array
    // @param len Length of the array
    // @param delim Delimeter string
    // @param lcap Left surrounding character
    // @param rcap Right surrounding character
    // @return String of joined values
    template <class T>
    const std::string join(
            const T arr[],
            size_t len,
            const std::string &delim=DELIM,
            const std::string &lcap=LCAP,
            const std::string &rcap=RCAP)
    {
        std::ostringstream oss;
        oss << lcap;
        if (len > 0) {
            for (size_t i = 0; i < len - 1; ++i)
                oss << arr[i] << delim;
            oss << arr[len - 1];
        }
        oss << rcap;
        return oss.str();
    }

    //! Join generic STL container
    // Similar to the Python join function.
    // @param c Input container
    // @param delim Delimeter string
    // @param lcap Left surrounding character
    // @param rcap Right surrounding character
    // @return String of joined values
    template <template<typename ...> class Tc, typename T>
    const std::string join(
            const Tc<T> &c,
            const std::string &delim=DELIM,
            const std::string &lcap=LCAP,
            const std::string &rcap=RCAP)
    {
        std::stringstream oss;
        oss << lcap;
        if (c.size() > 0) {
            auto it = c.cbegin();
            for (size_t i = 0; i < c.size() - 1; ++i, ++it)
                oss << *it << delim;
            oss << *it;
        }
        oss << rcap;
        return oss.str();
    };
}
