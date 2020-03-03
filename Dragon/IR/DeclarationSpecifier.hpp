#pragma once

#include <set>

namespace Dragon{namespace IR{
    
    enum struct declarationSpecifier{
        none_,
        static_,
        readonly,
        private_,
        protected_,
        public_
    };
    
    class specifiers{
    public:
        void add(declarationSpecifier specifier){
            if (_data.count(specifier)){
                assert(false);
            }else{
                _data.insert(specifier);
            }
        }
        
        bool has(declarationSpecifier specifier){
            return _data.count(specifier);
        }
        
    private:
        std::set<declarationSpecifier>  _data;
    };
    
}}