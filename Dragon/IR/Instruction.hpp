#pragma once

#include <Dragon/IR/Interface.hpp>
#include <Dragon/AST/Statement.hpp>
#include <Dragon/IR/Constant.hpp>

namespace Dragon{namespace IR{
    
    class ArithmeticalOperator : public IInstruction{
    public:
        ArithmeticalOperator(AST::BinaryOperator opt, shared_ptr<IR::IValue> sp_v1, shared_ptr<IR::IValue> sp_v2, shared_ptr<IBasicBlock> sp_block)
        :_sp_type(sp_v1->type()){
            assert(sp_v1->type() == sp_v2->type());
            bool isIntergral = sp_v1->type()->isIntergral();
            llvm::Instruction::BinaryOps ops;
            
            if (opt ==  "+"){
                    if (isIntergral)
                        ops = llvm::Instruction::BinaryOps::Add;
                    else
                        ops = llvm::Instruction::BinaryOps::FAdd;
            }
            if (opt ==  "-"){
                    if (isIntergral)
                        ops = llvm::Instruction::BinaryOps::Sub;
                    else
                        ops = llvm::Instruction::BinaryOps::FSub;
            }
            if (opt == "*"){
                    if (isIntergral)
                        ops = llvm::Instruction::BinaryOps::Mul;
                    else
                        ops = llvm::Instruction::BinaryOps::FMul;
            }
            if (opt == "/"){
                    if (isIntergral)
                        ops = llvm::Instruction::BinaryOps::SDiv;
                    else
                        ops = llvm::Instruction::BinaryOps::FDiv;
            }
            if (opt == "%"){
                    if (isIntergral)
                        ops = llvm::Instruction::BinaryOps::SRem;
                    else
                        ops = llvm::Instruction::BinaryOps::FRem;
            }
            
            _p_arithemtical_operator = llvm::BinaryOperator::Create(ops, sp_v1->llvmValue(), sp_v2->llvmValue(), "",sp_block->llvmBasicBlock());
        }
        
        virtual llvm::Instruction *llvm_instruction() const       { assert(_p_arithemtical_operator); return _p_arithemtical_operator; }
        virtual llvm::Value *llvmValue() const                   { return llvm_instruction(); }
        virtual shared_ptr<IType> type() const              { return _sp_type; }
        
        
    private:
        shared_ptr<IType>           _sp_type;
        llvm::BinaryOperator   *_p_arithemtical_operator;
    };    
    
    class logical_operator : public IInstruction{
    public:
        logical_operator(AST::BinaryOperator opt, shared_ptr<IR::IValue> sp_v1, shared_ptr<IR::IValue> sp_v2, shared_ptr<IBasicBlock> sp_block)
        :_sp_type(sp_v1->type()->context()->getType("bool")){
            assert(sp_v1->type() == sp_v2->type());
            assert(sp_v1->type()->isIntergral());
            llvm::Instruction::BinaryOps ops;
            
            if (opt == "&&"){
                ops = llvm::Instruction::BinaryOps::And;
            }
            if (opt == "||"){
                ops = llvm::Instruction::BinaryOps::Or;
            }
            if (opt == "!"){
                assert(false);
                //ops = llvm::Instruction::BinaryOps::And;
            }
      
            _p_logical_inst = llvm::BinaryOperator::Create(ops, sp_v1->llvmValue(), sp_v2->llvmValue(), "",sp_block->llvmBasicBlock());
        }
        
        virtual llvm::Instruction *llvm_instruction() const       { assert(_p_logical_inst); return _p_logical_inst; }
        virtual llvm::Value *llvmValue() const                   { return llvm_instruction(); }
        
        virtual shared_ptr<IType> type() const              { return _sp_type; }
        
    private:
        shared_ptr<IType>           _sp_type;
        llvm::BinaryOperator *            _p_logical_inst;
    };
    
    
    class unary_operator : public IInstruction{
    public:
        unary_operator(AST::BinaryOperator opt, shared_ptr<IR::IValue> spValue, shared_ptr<IBasicBlock> sp_block)
        :_sp_type(spValue->type()){
            auto Context = spValue->type()->context();
            
            if (opt == "+"){
                _sp_value = spValue;
            }
            if (opt == "-"){
                if (spValue->type()  == Context->getType("int")){
                    auto neg_one = make_shared<Constant<IntType>>(-1, Context->getType("int"));
                    _sp_value = make_shared<ArithmeticalOperator>("*", spValue, neg_one, sp_block);
                    return;
                }
                if (spValue->type() == Context->getType("double")){
                    auto neg_one = make_shared<Constant<FloatType>>(-1.0, Context->getType("double"));
                    _sp_value = make_shared<ArithmeticalOperator>("*", spValue, neg_one, sp_block);
                    return;
                }
                
                assert(false);
            }
        }
        
        virtual llvm::Instruction *llvm_instruction() const       { assert(false); }
        virtual llvm::Value *llvmValue() const                   { return _sp_value->llvmValue(); }
        
        virtual shared_ptr<IType> type() const              { return _sp_type; }
        
    private:
        shared_ptr<IType>           _sp_type;
        shared_ptr<IValue>          _sp_value;
    };
    
    
    class comparison_operator : public IInstruction{
    public:
        comparison_operator(AST::BinaryOperator opt, shared_ptr<IR::IValue> sp_v1, shared_ptr<IR::IValue> sp_v2, shared_ptr<IBasicBlock> sp_block){
            assert(sp_v1->type() == sp_v2->type());
            bool isIntergral = sp_v1->type()->isIntergral();
            
            llvm::CmpInst::Predicate pre;
            if (opt == "=="){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_EQ;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_OEQ;
            }
            if (opt == "!="){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_NE;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_ONE;
            }
            if (opt == "<"){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_SLT;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_OLT;
            }
            if (opt == "<="){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_SLE;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_OLE;
            }
            if (opt == ">"){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_SGT;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_OGT;
            }
            if (opt == ">="){
                if (isIntergral)
                    pre = llvm::CmpInst::Predicate::ICMP_SGE;
                else
                    pre = llvm::CmpInst::Predicate::FCMP_OGE;
            }
            
            if (isIntergral){
                _p_cmp_inst = new llvm::ICmpInst(*sp_block->llvmBasicBlock(), pre, sp_v1->llvmValue(), sp_v2->llvmValue());
            }else{
                 _p_cmp_inst = new llvm::FCmpInst(*sp_block->llvmBasicBlock(), pre, sp_v1->llvmValue(), sp_v2->llvmValue());
            }
            _sp_type = sp_v1->type()->context()->getType("bool");
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_cmp_inst; }
        virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }

    private:
        shared_ptr<IType>           _sp_type;
        llvm::CmpInst *                  _p_cmp_inst;
    };
    
    class return_instruction : public IInstruction{
    public:
        return_instruction(shared_ptr<IValue> spValue, shared_ptr<IContext> sp_context, shared_ptr<IBasicBlock> sp_insert_at_end)
        : _sp_type(spValue->type()){
            _p_return_instruction = llvm::ReturnInst::Create(sp_context->llvmContext(), spValue->llvmValue(), sp_insert_at_end->llvmBasicBlock());
        }
        
        return_instruction(shared_ptr<IContext> sp_context, shared_ptr<IBasicBlock> sp_insert_at_end){
            _sp_type = sp_context->getType("void");
            _p_return_instruction = llvm::ReturnInst::Create(sp_context->llvmContext(), sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual llvm::Instruction *llvm_instruction() const{
            assert(_p_return_instruction);
            return _p_return_instruction;
        }
        
        virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        
    private:
        llvm::ReturnInst  *_p_return_instruction;
        shared_ptr<IType>   _sp_type;
    };
    
    class branch_instruction : public IInstruction{
    public:
        branch_instruction(shared_ptr<IBasicBlock> sp_then_block, shared_ptr<IBasicBlock> sp_else_block, shared_ptr<IValue> sp_condition, shared_ptr<IBasicBlock> sp_insert_at_end){
            auto sp_type = sp_condition->type();
            assert(sp_type = sp_type->context()->getType("bool")); //condition should be bool
            
            _p_branch_instruction = llvm::BranchInst::Create(sp_then_block->llvmBasicBlock(), sp_else_block->llvmBasicBlock(), sp_condition->llvmValue(), sp_insert_at_end->llvmBasicBlock());
        }
        
        branch_instruction(shared_ptr<IBasicBlock> sp_block, shared_ptr<IBasicBlock> sp_insert_at_end){
            _p_branch_instruction = llvm::BranchInst::Create(sp_block->llvmBasicBlock(), sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual llvm::Instruction *llvm_instruction() const       { return _p_branch_instruction; }
        virtual llvm::Value *llvmValue() const                   { return llvm_instruction(); }
        
        virtual shared_ptr<IType> type() const              { assert(false); }
        
    private:
        llvm::BranchInst          *_p_branch_instruction;
    };
    
    class store : public IInstruction{
    public:
        store(shared_ptr<IValue> spValue, shared_ptr<IValue> sp_ptr, shared_ptr<IBasicBlock> sp_insert_at_end){
            auto sp_ptr_type = PointerType::get(spValue->type());
            if (sp_ptr->type() != sp_ptr_type){
                assert(false);
            }
            
            _p_llvm_value = new llvm::StoreInst(spValue->llvmValue(), sp_ptr->llvmValue(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { assert(false); }
        virtual llvm::Instruction *llvm_instruction() const   { assert(_p_llvm_value); return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        
    private:
        llvm::Instruction *                           _p_llvm_value;
        shared_ptr<IType>                       _sp_type;
    };
    
    class load : public IInstruction{
    public:
        load(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(nullptr){
            auto sp_pointer_type = dynamic_pointer_cast<PointerType>(spValue->type());
            if (!sp_pointer_type){
                assert(false);
            }
            _sp_type = sp_pointer_type->valueType();
            _p_llvm_value = new llvm::LoadInst(spValue->llvmValue(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { assert(_p_llvm_value); return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        
    private:
        llvm::Instruction *                           _p_llvm_value;
        shared_ptr<IType>                       _sp_type;
    };
    
//    class assign_instruction : public IInstruction{
//        
//    };
}}



















