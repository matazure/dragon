{
    'target_defaults': {
        'default_configuration': 'Debug',
        'configurations': {
            'Debug': {
                'defines': [ 'DEBUG', '_DEBUG' ],
                'cflags': ['-std=c++11', '-Wall', '-O0', '-g'],
                'xcode_settings':{
                    'OTHER_CPLUSPLUSFLAGS':['-O0', '-Wall'],
                    'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
                    'CLANG_CXX_LIBRARY': 'libc++',
                    # 'CONFIGURATION_BUILD_DIR': './',
                },
            },
            'Release': {
                'defines': [ 'NDEBUG', 'RELEASE' ],
                'cflags': ['-std=c++11', '-Wall', '-O0', '-s'],
                'xcode_settings':{
                    'OTHER_CPLUSPLUSFLAGS':['-Os', '-Wall'],
                    'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
                    'CLANG_CXX_LIBRARY': 'libc++',
                    # 'CONFIGURATION_BUILD_DIR': './',
                },
            },
        },
    },

    'variables':{
        'INCLUDE_DIR%': '/usr/local/include',
        'LIBRARY_DIR%': '/usr/local/lib',
    },

    'targets':[
        {
            'target_name' : 'Dragon',
            'type' : 'static_library',
            'include_dirs':['.', '<(INCLUDE_DIR)'],
            # 'dependencies': ['drive/drive.gyp:drive'],
            'sources' : ['Dragon/Parser/parse.cpp',
                         'Dragon/Compiler/Compiler.cpp',
                         'Dragon/Issue/handler.cpp',],
            'defines' : ['__STDC_LIMIT_MACROS', '__STDC_CONSTANT_MACROS'],

            'xcode_settings':{
                'LIBRARY_SEARCH_PATHS':['<(LIBRARY_DIR)'],
            },
            'link_settings':{
                'library_dirs':['<(LIBRARY_DIR)'],
            },

            'conditions':[
                ['OS=="mac"',{
                    'xcode_settings':{
                        'OTHER_CPLUSPLUSFLAGS':['-ftemplate-depth=512'],
                    },

                    'all_dependent_settings':
                    {
                        'include_dirs': [ '.', '<(INCLUDE_DIR)' ],
                        'xcode_settings':{
                            'LIBRARY_SEARCH_PATHS':['<(LIBRARY_DIR)',],
                        },
                        'link_settings':{
                            'libraries':[
                                'libboost_filesystem.a',
                                'libboost_system.a',
                                'libboost_program_options.a',
                                'libLLVMJIT.a',
                                'libLLVMInterpreter.a',
                                'libLLVMExecutionEngine.a',
                                'libLLVMX86CodeGen.a',
                                'libLLVMX86Desc.a',
                                'libLLVMX86Info.a',
                                'libLLVMX86AsmPrinter.a',
                                'libLLVMSelectionDAG.a',
                                'libLLVMMCParser.a',
                                'libLLVMCodeGen.a',
                                'libLLVMScalarOpts.a',
                                'libLLVMInstCombine.a',
                                'libLLVMTransformUtils.a',
                                'libLLVMipa.a',
                                'libLLVMAnalysis.a',
                                'libLLVMTarget.a',
                                'libLLVMMC.a',
                                'libLLVMObject.a',
                                'libLLVMBitReader.a',
                                'libLLVMBitWriter.a',
                                'libLLVMCore.a',
                                'libLLVMX86Utils.a',
                                'libLLVMTableGen.a',
                                'libLLVMSupport.a',
                                'libpthread.a',
                                'libcurses.a',
                                'libdl.a',
                                'libz.a',
                            ]
                        },
                    }
                }],
                ['OS=="linux"', {
                    'all_dependent_settings':
                    {
                        'include_dirs': [ '.', '<(INCLUDE_DIR)' ],
                        'link_settings':{
                            'library_dirs':['<(LIBRARY_DIR)'],
                            'libraries':[
                                '-lboost_filesystem',
                                '-lboost_system',
                                '-lboost_program_options',
                                '-lLLVMJIT',
                                '-lLLVMInterpreter',
                                '-lLLVMExecutionEngine',
                                '-lLLVMMCJIT',
                                '-lLLVMX86CodeGen',
                                '-lLLVMX86Desc',
                                '-lLLVMX86Info',
                                '-lLLVMX86AsmPrinter',
                                '-lLLVMSelectionDAG',
                                '-lLLVMCodeGen', #
                                '-lLLVMScalarOpts',
                                '-lLLVMInstCombine',
                                '-lLLVMTransformUtils',
                                '-lLLVMipa',
                                '-lLLVMAnalysis',
                                '-lLLVMTarget',
                                '-lLLVMMC',
                                '-lLLVMObject',#
                                '-lLLVMMCParser',
                                '-lLLVMBitReader',
                                '-lLLVMCore',
                                '-lLLVMX86Utils', #
                                '-lLLVMTableGen',
                                '-lLLVMSupport',
                                '-lpthread',
                                '-lcurses',
                                '-ldl',
                                '-lz',
                            ],
                        }
                    },
                }],
            ],
        },

        {
            'target_name' : 'Shell',
            'type' : 'executable',
            'product_extension' : 'out',
            'defines' : ['__STDC_LIMIT_MACROS', '__STDC_CONSTANT_MACROS'],
            'dependencies': ['Dragon.gyp:Dragon'],
            'sources' : ['Shell/main.cpp',],
            #'product_dir': '.',
            #
            # 'copies': [
            #     {
            #         'destination': '.',
            #         'files': ['<(PRODUCT_DIR)/shell.out',],
            #     },
            # ]
        },

        # {
        #     'target_name' : 'unit_test',
        #     'type' : 'executable',
        #     'product_extension' : 'out',
        #     'defines' : ['__STDC_LIMIT_MACROS', '__STDC_CONSTANT_MACROS'],
        #     'dependencies': ['dragon'],
        #     'sources' : [   'unit_test/main.cpp',
        #                     'unit_test/buildin_lib_ut.cpp',
        #                     'unit_test/expression_ut.cpp',
        #                     'unit_test/issue/function_issue_ut.cpp',
        #                     'unit_test/issue/identifier_issue_ut.cpp',
        #                     'unit_test/issue/out_range_issue_ut.cpp',
        #                     'unit_test/issue/sytax_issue_ut.cpp',
        #                     'unit_test/issue/expression_issue_ut.cpp',
        #                     'unit_test/issue/type_issue_ut.cpp',
        #                     'unit_test/grammar/type_grammar_ut.cpp',
        #                     'unit_test/grammar/declaration_grammar_ut.cpp',
        #                     'unit_test/grammar/expression_grammar_ut.cpp',
        #                     'unit_test/grammar/pattern_grammar_ut.cpp',
        #                     'unit_test/grammar/statement_grammar_ut.cpp'],
        #
        #     'product_dir': '.',
        #     'conditions':[
        #         ['OS=="mac"',{
        #             'link_settings':{
        #                 'libraries':[
        #                     'libgtest.a',
        #                 ]
        #             },
        #         }],
        #         ['OS=="linux"', {
        #             'link_settings':{
        #                 'libraries':[
        #                     '-lgtest',
        #                 ],
        #             }
        #         }],
        #     ],
        #
        #     #
        #     # 'copies': [
        #     #     {
        #     #         'destination': '.',
        #     #         'files': ['<(PRODUCT_DIR)/unit_test.out',],
        #     #     },
        #     # ]
        # }
    ]
}
