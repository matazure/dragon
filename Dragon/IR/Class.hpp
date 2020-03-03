#pragma once

#include <string>
#include <vector>

#include <Dragon/IR/PrimitiveType.hpp>
#include <Dragon/IR/Variable.hpp>
#include <Dragon/IR/Function.hpp>

namespace Dragon{namespace IR{
    
    enum  access_permission{
        private_ = 0,
        protected_  = 1,
        public_ = 2
    };
    
    
    class field{
    public:
        field(const std::string &name, shared_ptr<IType> sp_type, access_permission accessible)
        : _name(name), _sp_type(sp_type), _accessible(accessible){}
        
        std::string name() const                { return _name; }
        access_permission accessability() const        { return _accessible; }
        shared_ptr<IType> type() const          { return _sp_type; }
        
    private:
        std::string                     _name;
        shared_ptr<IType>               _sp_type;
        access_permission               _accessible;
    };
    
    
    class property{
    public:
        property(const std::string name, shared_ptr<IType> sp_type)
        : _name(name), _sp_type(sp_type){}
        
        void get_function(shared_ptr<Function> sp_get_fun){
            _get_fun = sp_get_fun;
        }
        
        void set_function(shared_ptr<Function> sp_set_fun){
            _set_fun = sp_set_fun;
        }
        
        shared_ptr<Function> get_function() const           { return *_get_fun; }
        shared_ptr<Function> set_function() const           { return *_set_fun; }
        
        shared_ptr<IType> valueType() const                      { return _sp_type; }
        std::string name() const                            { return _name; }
        
    private:
        std::string                 _name;
        shared_ptr<IType>           _sp_type;
        
        boost::optional<shared_ptr<Function>>           _get_fun;
        boost::optional<shared_ptr<Function>>           _set_fun;
    };
    
    
    class indexer{
    public:
        indexer(shared_ptr<IType> sp_type)
        :_sp_type(sp_type), _get_fun(nullptr), _set_fun(nullptr){}
        
        void getter(shared_ptr<Function> sp_get_fun){
            _get_fun = sp_get_fun;
        }
        
        void setter(shared_ptr<Function> sp_set_fun){
            _set_fun = sp_set_fun;
        }
        
        shared_ptr<Function> getter() const                         { return _get_fun; }
        shared_ptr<Function> setter() const                         { return _set_fun; }
        
        shared_ptr<IType> valueType() const                        { return _sp_type; }
        
    private:
        shared_ptr<IType>           _sp_type;
        
        shared_ptr<Function>           _get_fun;
        shared_ptr<Function>           _set_fun;
    };
    

    class Class: public TypeBase, public IClass{
    public:
        enum struct member_flag{
            none,
            field,
            member_function,
            property
        };
        
        Class(const std::string &name, shared_ptr<ISymbolTable> sp_parent)
        : TypeBase(sp_parent->uid()+"."+name, sp_parent->context()), _name(name), _domain(sp_parent, name){ }
        
        virtual size_t bitWidth() const{
            return _bitwidth;
        }
        
        void set_constructor(const std::string &name, shared_ptr<Function> sp_constructor){
            assert(!_constructors[name]);
            _constructors[name] = sp_constructor;
        }
        
        shared_ptr<Function> get_constructor(const std::string &name) const{
            auto it = _constructors.find(name);
            if (it == _constructors.end())
                return nullptr;
            return it->second;
        }
        
        shared_ptr<Function> get_copy_constructor() const{
            return get_constructor("::"+uid()+"/"+uid());
        }
        
        void set_member_function(const std::string &name, shared_ptr<Function> sp_mem_function){
            auto &mem_function = _member_functions[name];
            assert(!mem_function);
            
            _member_functions[name] = sp_mem_function;
        }
        
        shared_ptr<Function> get_member_function(const std::string &name) const {
            auto it = _member_functions.find(name);
            if (it == _member_functions.end())
                return shared_ptr<Function>(nullptr);
            return it->second;
        }
        
        void set_property(const std::string &name, shared_ptr<property> sp_property){
            if (_properties[name]){
                assert(false); ///TODO
            }
            
            _properties[name] = sp_property;
        }
        
        shared_ptr<property> get_property(const std::string &name) const{
            auto it = _properties.find(name);
            if (it == _properties.end()){
                return nullptr;
            }
            
            return it->second;
        }
        
        shared_ptr<field> get_field(const std::string &name) const{
            for (auto field: _fields){
                if (field->name() == name){
                    return field;
                }
            }
            
            return shared_ptr<field>(nullptr);
        }
        
        member_flag is_member(const std::string &name) const{
            auto sp_field = get_field(name);
            auto sp_member_fun = get_member_function(name);
            auto sp_property = get_property(name);
            if (sp_field && sp_member_fun) assert(false);
            
            if (sp_field)   return member_flag::field;
            if (sp_member_fun)  return member_flag::member_function;
            if (sp_property) return member_flag::property;
            
            return member_flag::none;
        }
        
        virtual bool isClass() const                       { return true; }
        
        llvm::StructType *llvm_Class() const                 { return reinterpret_cast<llvm::StructType *>(_p_llvm_type); }
        
        std::vector<shared_ptr<field>> fields()  const      { return _fields; }
        
        void fields(std::vector<shared_ptr<field>> v){
            _fields = v;
            std::vector<llvm::Type *> llvm_types(_fields.size());
            _bitwidth = 0;
            for (size_t i = 0; i < _fields.size(); ++i){
                llvm_types[i] = _fields[i]->type()->llvmType();
                _bitwidth += _fields[i]->type()->bitWidth();
            }
            _p_llvm_type = llvm::StructType::get(_wp_context.lock()->llvmContext(), llvm_types);
        }
        
        virtual mapped_type get(const key_type &key) const{
            return _domain.get(key);
        }
        
        virtual void put(const key_type &key, mapped_type value){
            _domain.put(key, value);
        }
        
        virtual void import(shared_ptr<ISymbolTable> spScope){
            _domain.import(spScope);
        }
        
        virtual void export_(shared_ptr<ISymbolTable> spScope){
            _domain.export_(spScope);
        }
        
        virtual shared_ptr<ISymbolTable> parent() const  { return _domain.parent(); }
        virtual void parent(shared_ptr<ISymbolTable> sp_parent)  { _domain.parent(sp_parent); }
        
        virtual llvm::Module *llvm_module() const                         { return _domain.llvm_module(); }
        virtual shared_ptr<IContext> context() const                { return _domain.context(); }
        
        virtual std::string uid() const                             { return _domain.uid(); }
        virtual std::string name() const                            { return _name; }
        
    private:
        std::map<std::string, shared_ptr<Function>>     _member_functions;
        std::map<std::string, shared_ptr<property>>     _properties;
        std::map<std::string, shared_ptr<Function>>     _constructors;
        
        std::string                                     _name;
        std::vector<shared_ptr<field>>                  _fields;
        SymbolTable                                          _domain;
        size_t                                          _bitwidth;
        
    public:
        std::map<std::string, shared_ptr<indexer>>     indexers;
    };
    
    
    class FieldAccessor : public VariableBase, public IValue{
    public:
        FieldAccessor(shared_ptr<VariableBase> sp_variable, const std::string &field_name, shared_ptr<IBasicBlock> sp_block)
        : _sp_variable(sp_variable){
            auto spClass = dynamic_pointer_cast<Class>(sp_variable->valueType());
            assert(spClass);
            
            readable(true);
            writeable(true);
            
            auto fields = spClass->fields();
            size_t index = 0;
            for (; index < fields.size(); ++index){
                if (fields[index]->name() == field_name){
                    break;
                }
            }
            assert(index < fields.size());
            _sp_type = fields[index]->type();
            auto &Context = _sp_type->context()->llvmContext();
            auto zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(Context), 0);
            auto field_offset = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), index);
            std::vector<llvm::Value *> llvm_idxs;
            llvm_idxs.push_back(zero);
            llvm_idxs.push_back(field_offset);
            _p_llvm_element_ptr = llvm::GetElementPtrInst::Create(sp_variable->llvmValue(), llvm_idxs, "", sp_block->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> valueType() const            { return _sp_type; }
        virtual llvm::Value *llvmValue () const                      { return _p_llvm_element_ptr; }
        
        virtual shared_ptr<IType> type() const                  { return ReferenceType::get(_sp_type); }
        
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }
        
    private:
        shared_ptr<VariableBase>               _sp_variable;
        shared_ptr<IType>                   _sp_type;
        llvm::Value *                             _p_llvm_element_ptr;
    };
    
    class MemberFunctionAccessor{
    public:
        class call : public IInstruction{
        public:
            call(shared_ptr<MemberFunctionAccessor> sp_mem_fun_accessor,  std::vector<Address> arguments, shared_ptr<IBasicBlock> sp_cur_block){
                //add object at first
                arguments.insert(arguments.begin(), dynamic_pointer_cast<IValue>(sp_mem_fun_accessor->object()));
                //call as Function
                _sp_fun_caller = make_shared<Function::call>(sp_mem_fun_accessor->function_prototype(), arguments, sp_cur_block);
            }

            virtual shared_ptr<IType>   type() const        { return _sp_fun_caller->type(); }
            virtual llvm::Instruction *llvm_instruction() const   { return _sp_fun_caller->llvm_instruction(); }
            virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }

        private:
            shared_ptr<Function::call> _sp_fun_caller;
        };
        
        MemberFunctionAccessor(shared_ptr<VariableBase> sp_object, const std::string &name): _sp_object(sp_object){
            auto sp_type = sp_object->valueType();
            auto spClass = dynamic_pointer_cast<Class>(sp_type);
            assert(spClass);
            _sp_fun = spClass->get_member_function(name);
            assert(_sp_fun);
        }
        
        shared_ptr<Function>   function_prototype() const      { return _sp_fun; }
        shared_ptr<VariableBase>   object() const                  { return _sp_object; }
        
    private:
        shared_ptr<VariableBase>   _sp_object;
        shared_ptr<Function>    _sp_fun;
    };
    
    
    class property_accessor : public VariableBase{
    public:
        property_accessor(shared_ptr<VariableBase> sp_object, const std::string &name): _sp_object(sp_object){
            readable(false);
            writeable(false);
            
            auto sp_type = sp_object->valueType();
            auto spClass = dynamic_pointer_cast<Class>(sp_type);
            if (!spClass){
                assert(false); ///TODO
            }
            
            _sp_property = spClass->get_property(name);
        }
        
        virtual llvm::Value *llvmValue() const       { assert(false); }
        virtual shared_ptr<IType> valueType() const    { return _sp_property->valueType(); }
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_cur_block){
            std::vector<Address> args;
            auto sp_this_ref = dynamic_pointer_cast<IValue>(_sp_object);
            assert(sp_this_ref);
            args.push_back(sp_this_ref);
            auto sp_fun = _sp_property->get_function();
            if (!sp_fun){
                assert(false); ///TODO
            }
            
            return make_shared<Function::call>(sp_fun, args, sp_cur_block);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_cur_block){
            std::vector<Address> args;
            auto sp_this_ref = dynamic_pointer_cast<IValue>(_sp_object);
            assert(sp_this_ref);
            args.push_back(sp_this_ref);
            args.push_back(spValue);
            auto sp_fun = _sp_property->set_function();
            if (!sp_fun){
                assert(false); ///TODO
            }
            
            return make_shared<Function::call>(sp_fun, args, sp_cur_block);
        }
        
    private:
        shared_ptr<VariableBase>   _sp_object;
        shared_ptr<property>    _sp_property;
    };
    
    class indexer_accessor : public VariableBase{
    public:
        indexer_accessor(shared_ptr<VariableBase> sp_object, std::list<Address> arguments) throw (no_matched_indexer)
        : _sp_this(sp_object), _arguments(arguments){
            readable(false);
            writeable(false);
            
            auto sp_type = sp_object->valueType();
            auto spClass = dynamic_pointer_cast<Class>(sp_type);
            if (!spClass){
                assert(false); ///TODO
            }
            
            auto suf = suffix(arguments);
            _sp_index = spClass->indexers[suf];
            if (!_sp_index){
                throw no_matched_indexer();
            }
            if (_sp_index->getter()){
                readable(true);
            }
            if (_sp_index->setter()){
                writeable(true);
            }
            
            //add this object
            auto sp_this_ref = dynamic_pointer_cast<IValue>(_sp_this);
            assert(sp_this_ref);
            _arguments.push_front(sp_this_ref);
        }
        
        virtual llvm::Value *llvmValue() const       { assert(false); }
        virtual shared_ptr<IType> valueType() const    { return _sp_index->valueType(); }
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_cur_block){
            if (!readable()){
                assert(false);
            }
            
            std::vector<Address> arguments(_arguments.begin(), _arguments.end());
            auto sp_fun = _sp_index->getter();
            return make_shared<Function::call>(sp_fun,  arguments, sp_cur_block);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_cur_block){
            if (!writeable()){
                assert(false);
            }
            
            std::vector<Address> arguments(_arguments.begin(), _arguments.end());
            arguments.push_back(spValue);
            auto sp_fun = _sp_index->setter();
            return make_shared<Function::call>(sp_fun, arguments, sp_cur_block);
        }
        
    private:
        shared_ptr<VariableBase>   _sp_this;
        shared_ptr<indexer>     _sp_index;
        std::list<Address>      _arguments;
    };
    
}}

















