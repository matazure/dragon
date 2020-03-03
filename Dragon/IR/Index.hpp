#pragma once

#include <Dragon/IR/class.hpp>
#include <Dragon/IR/utility.hpp>

namespace Dragon{namespace IR{
    
    
    class pointer_indexer : public VariableBase{
    public:
        
        pointer_indexer(shared_ptr<IValue> sp_variable, shared_ptr<IValue> sp_index, shared_ptr<IBasicBlock> sp_block) throw(invalid_pointer_indexer){
            readable(true);
            writeable(true);
            
            if (sp_index->type() != sp_index->type()->context()->getType("int")){
                throw invalid_pointer_indexer();
            }

            auto sp_type = sp_variable->type();
            auto sp_point_type = dynamic_pointer_cast<PointerType>(sp_type);
            assert(sp_point_type);
            _sp_type = sp_point_type->valueType();
            
            _p_llvm_element_ptr = llvm::GetElementPtrInst::Create(sp_variable->llvmValue(), sp_index->llvmValue(), "",sp_block->llvmBasicBlock());
        }
        
        virtual llvm::Value *llvmValue() const       { return _p_llvm_element_ptr; }
        virtual shared_ptr<IType> valueType() const  { return _sp_type; }
        
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Value *             _p_llvm_element_ptr;
    };
    
    class array_indexer : public VariableBase, public IValue{
    public:
        
        array_indexer(shared_ptr<VariableBase> sp_variable, shared_ptr<IValue> sp_index, shared_ptr<IBasicBlock> sp_block) throw(invalid_array_indexer){
            readable(true);
            writeable(true);
            
            if (sp_index->type() != sp_index->type()->context()->getType("int")){
                throw invalid_array_indexer();
            }

            auto sp_type = sp_variable->valueType();
            auto sp_array_type = dynamic_pointer_cast<ArrayType>(sp_type);
            assert(sp_array_type);
            _sp_type = sp_array_type->valueType();
            auto &Context = _sp_type->context()->llvmContext();
            auto zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(Context), 0);
            std::vector<llvm::Value *> llvm_idxs;
            llvm_idxs.push_back(zero);
            llvm_idxs.push_back(sp_index->llvmValue());
            _p_llvm_element_ptr = llvm::GetElementPtrInst::Create(sp_variable->llvmValue(), llvm_idxs, "",sp_block->llvmBasicBlock());
        }
        
        virtual llvm::Value *llvmValue() const       { return _p_llvm_element_ptr; }
        virtual shared_ptr<IType> valueType() const  { return _sp_type; }
        
        virtual shared_ptr<IType> type() const  { return PointerType::get(_sp_type); }
        
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_llvm_element_ptr, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Value *             _p_llvm_element_ptr;
    };
    
    class index{
    public:
        typedef Address             result_type;
        
        index(shared_ptr<IBasicBlock> sp_cur_block): _spCurrentBlock(sp_cur_block){}
        
        result_type operator()(const Address &object, Address arguments){
            return boost::apply_visitor(*this, object, arguments);
        }
        
        template <typename T, typename M>
        result_type operator()(T t, M m){
            assert(false); //unreachable.
        }
        
        result_type operator()(shared_ptr<VariableBase> sp_variable, AddressList arguments) throw(index_error){
            auto sp_type = sp_variable->valueType();
            if (sp_type->isClass()){
                 auto spClass = dynamic_pointer_cast<Class>(sp_type);
                return make_shared<indexer_accessor>(sp_variable, arguments);
            }else{//如果是值类型， 那么应当只有array有索引操作
                auto sp_get = sp_variable->get(_spCurrentBlock);
                return (*this)(sp_get, arguments);
            }
        }
        
        
        ///<Todo    pointer indexer?
        result_type operator()(shared_ptr<IValue> spValue, AddressList arguments) throw(index_error) {
            auto sp_type = spValue->type();
            auto sp_array_type = dynamic_pointer_cast<PointerType>(sp_type);
            if (sp_array_type){
                if (arguments.size() != 1){
                    assert(false);
                }
                
                get_value getter_(_spCurrentBlock);
                auto sp_index = boost::apply_visitor(getter_, arguments.front());
                return make_shared<pointer_indexer>(spValue, sp_index, _spCurrentBlock);
            }
            if (sp_type->isClass()){
                auto sp_var = make_shared<Variable>(spValue->type(), _spCurrentBlock->parent());
                sp_var->set(spValue, _spCurrentBlock);
                sp_var->writeable(false);
                sp_var->readable(true);
                return (*this)(static_pointer_cast<VariableBase>(sp_var), arguments);
            }
           
            assert(false);
        }
        
        
    private:
        shared_ptr<IBasicBlock> _spCurrentBlock;
    };
    
    
}}















