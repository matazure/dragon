#pragma once

#include <memory>

namespace Dragon{
    typedef int64_t     int_t;
    typedef double  float_t;
    typedef bool    bool_t;
    typedef unsigned char   byte;

    using std::shared_ptr;
    using std::make_shared;
    using std::weak_ptr;
    using std::enable_shared_from_this;
    using std::static_pointer_cast;
    using std::dynamic_pointer_cast;

}
