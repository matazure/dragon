cmake_minimum_required(VERSION 3.4)

# add_executable(DragonExe)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

add_library(dragon_front INTERFACE)

add_subdirectory(third_party/llvm-project/llvm)

target_include_directories(dragon_front
    INTERFACE ${PROJECT_SOURCE_DIR}
    INTERFACE third_party/llvm-project/llvm/include
    INTERFACE ${PROJECT_BINARY_DIR}/third_party/llvm-project/llvm/include
    # INTERFACE $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}>
)


add_executable(dragon Shell/main.cpp Dragon/Compiler/Compiler.cpp Dragon/Parser/parse.cpp Dragon/Issue/handler.cpp)
target_link_libraries(dragon PRIVATE dragon_front
    LLVMJITLink
    LLVMInterpreter
    LLVMExecutionEngine
    LLVMMCJIT
    LLVMX86CodeGen
    LLVMX86Desc
    LLVMX86Info 
    # LLVMX86AsmPrinter
    LLVMSelectionDAG
    LLVMCodeGen
    LLVMScalarOpts
    LLVMInstCombine
    LLVMTransformUtils
    # LLVMipa
    LLVMAnalysis
    LLVMTarget
    LLVMMC
    LLVMObject
    LLVMMCParser
    LLVMBitReader
    LLVMCore
    LLVMX86Utils
    LLVMTableGen
    LLVMSupport
    LLVMInterpreter
)