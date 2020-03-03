//
//  ExecutionEngine.hpp
//  binding
//
//  Created by 张志敏 on 12/9/13.
//
//

#pragma once

#include <iostream>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/Verifier.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/for_each.hpp>
#include <Dragon/IR/interface.hpp>


namespace Dragon{

    class ExecutionEngine{
    public:
        ExecutionEngine(shared_ptr<IR::imodule> spModule)
        : _spModule(spModule){

            std::string errStr;
            _p_llvm_engine =
                llvm::EngineBuilder(spModule->llvm_module())
                .setErrorStr(&errStr)
                .setEngineKind(llvm::EngineKind::JIT)
                .create();
            if (!_p_llvm_engine){
                std::cout << errStr << std::endl;
                std::terminate();
            }
        }

        void run_function(shared_ptr<IR::IFunction> sp_fun){
            auto sp_cmd_ptr = reinterpret_cast<void (*)()>(get_function_pointer(sp_fun));
            sp_cmd_ptr();
        }

        shared_ptr<IR::IFunction> mapCFunction(shared_ptr<IR::ifunction_type> sp_fun_type, void *fun_ptr, std::string name=""){
            auto decl_fun = make_shared<IR::Function>(sp_fun_type, _spModule, name);
            _p_llvm_engine->addGlobalMapping(decl_fun->llvm_function(), fun_ptr);
            return decl_fun;
        }

    private:
        void *get_function_pointer(shared_ptr<IR::IFunction> sp_fun){
            verifyModule(*_spModule->llvm_module());
            llvm::FunctionPassManager OurFPM(_spModule->llvm_module());
            // Set up the optimizer pipeline.  Start with registering info about how the
            // target lays out data structures.
            //OurFPM.add(new DataLayout(*_p_llvm_engine->getDataLayout()));
            // Provide basic AliasAnalysis support for GVN.
            OurFPM.add(llvm::createBasicAliasAnalysisPass());
            // Promote allocas to registers.
            OurFPM.add(llvm::createPromoteMemoryToRegisterPass());
            // Do simple "peephole" optimizations and bit-twiddling optzns.
            OurFPM.add(llvm::createInstructionCombiningPass());
            // Reassociate expressions.
            OurFPM.add(llvm::createReassociatePass());
            // Eliminate Common SubExpressions.
            OurFPM.add(llvm::createGVNPass());
            // Simplify the control flow graph (deleting unreachable blocks, etc).
            OurFPM.add(llvm::createCFGSimplificationPass());

            OurFPM.doInitialization();

            return _p_llvm_engine->getPointerToFunction(sp_fun->llvm_function());
        }

    private:
        shared_ptr<IR::imodule>  _spModule;
        llvm::ExecutionEngine *   _p_llvm_engine;
    };
}
