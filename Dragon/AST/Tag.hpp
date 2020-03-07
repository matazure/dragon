#pragma once

#include <vector>
#include <map>
#include <vector>
#include <tuple>
#include <boost/config/warning_disable.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <Dragon/Config.hpp>



namespace Dragon{namespace AST{
    
    using std::vector;
    using std::string;
    using std::map;
    using std::pair;
    using std::vector;
    using std::tuple;
    using boost::variant;
    using boost::blank;
    using boost::optional;
    using boost::recursive_wrapper;

    struct Tag{
        size_t positionId;
        size_t position_id;
    };

}}