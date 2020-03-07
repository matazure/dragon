//
//  Attribute.hpp
//  Dragon
//
//  Created by 张志敏 on 2/13/15.
//
//

#pragma once

#include <Dragon/AST/Tag.hpp>

namespace Dragon{namespace AST{
    
    struct Attribute: Tag{
        
        
    };
    
    struct Attributes: vector<Attribute>, Tag{
        
    };
    
}}
