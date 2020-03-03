#pragma once

#include <Dragon/Lexer/detail.hpp>


namespace Dragon{ namespace Lexer{
    
    enum token_id{
        newline = 1000,
        space = 1001,
        c_comment = 1002,
        line_comment = 1003
    };
    
    template <typename Iterator, typename Context>
    void print(Iterator const &b, const Iterator &e, BOOST_SCOPED_ENUM(boost::spirit::lex::pass_flags)&, std::size_t&, Context&){
        std::cout << "new line" << std::endl;
    }
    
    namespace lex = boost::spirit::lex;
    
    ///////////////////////////////////////////////////////////////////////////////
    //  Token definition base, defines all tokens for the base grammar below
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Lexer>
    struct CodeTokens : lex::lexer<Lexer>{
        typedef typename Lexer::token_type::iterator_type  base_iterator_type;
        // this lexer is supposed to be used as a base type only
        CodeTokens() {
            init_token_definitions();
        }
        
        lex::token_def<AST::Operator> increment, decrement;
        lex::token_def<AST::Operator> plus, minus, times, divide, remain;
        lex::token_def<AST::Operator> or_, and_, not_, xor_;
        lex::token_def<AST::Operator> equal, not_equal, less, less_or_equal, greater, greater_or_equal;
        lex::token_def<AST::Operator> superscript;
        lex::token_def<AST::Operator> typeof_, sizeof_;
        
        lex::token_def<AST::Operator> assign;
        
        lex::token_def<lex::omit> pound;
        lex::token_def<lex::omit> at;
        
        lex::token_def<lex::omit> arrow;
        
        //some key words
        lex::token_def<lex::omit> if_, else_, else_if, while_, for_;
        
        lex::token_def<lex::omit> null;
        lex::token_def<std::string> this_;
        lex::token_def<lex::omit> new_;
        lex::token_def<lex::omit> return_;
        lex::token_def<lex::omit> function, struct_, class_, property, indexer;
        //        lex::token_def<AST::Identifier> IntType, FloatType, type_type;
        // lex::token_def<ast::access_permission> accessability;
        lex::token_def<std::string> attribute;
        lex::token_def<AST::Operator> ref;
        lex::token_def<lex::omit> break_;
        lex::token_def<lex::omit> continue_;
        lex::token_def<lex::omit>   get;
        lex::token_def<lex::omit>   set;
        lex::token_def<lex::omit>   namespace_;
        lex::token_def<lex::omit>   require;
        lex::token_def<lex::omit>   using_;
//        lex::token_def<ast::char_pattern>      literal_char;
        lex::token_def<AST::StringLiteral>     string_;
        lex::token_def<lex::omit>   operator_;
        lex::token_def<lex::omit>   cast;
        
        //declaration specifiers
        lex::token_def<AST::DeclarationSpecifier>      declarationSpecifier;
        lex::token_def<lex::omit>                       var, const_, let;
        
        
        //defined
        lex::token_def<AST::Identifier>                 identifier;
        lex::token_def<AST::IntegerLiteral>                int_;
        lex::token_def<AST::FloatingPointLiteral>              float_;
//        lex::token_def<ast::bool_pattern>               true_, false_;
        
        //signs
        //   lex::token_def<ast::optoken> assign;
        lex::token_def<AST::Operator> l_bracket;                //left bracket
        lex::token_def<lex::omit> r_bracket;                //right bracket
        lex::token_def<AST::Operator>      l_s_bracket;              //left square bracket
        lex::token_def<lex::omit> r_s_bracket;              //right square bracket
        lex::token_def<lex::omit> l_braces;                 //left braces;
        lex::token_def<lex::omit> r_braces;                 //right braces;
        lex::token_def<lex::omit> l_a_bracket;              //left angle bracket <;
        lex::token_def<lex::omit> r_a_bracket;              //right angle bracket >;
        lex::token_def<AST::Operator> period;
        lex::token_def<lex::omit> comma;                    //,
        lex::token_def<lex::omit> colon;                    //:
        lex::token_def<lex::omit> semicolon;                //;
        
        //predefine
        lex::token_def<lex::omit> define;
        lex::token_def<lex::omit> undefine;
        lex::token_def<lex::omit> define_if;
        lex::token_def<lex::omit> define_else;
        lex::token_def<lex::omit> define_end;
        
        lex::token_def<lex::omit> newline;
        lex::token_def<lex::omit> space;
        
        lex::token_def<std::string> c_comment;
        lex::token_def<std::string> line_comment;
        
    private:
        void init_token_definitions(){
            
            increment = "\\+\\+"; decrement = "--";
            
            //binary operator
            plus = '+'; minus = '-'; times = '*'; divide = '/'; remain = '%';
            //logical
            or_ = "(\\|\\|)|or"; and_ = "&&|and"; not_ = "!|not"; xor_ = "xor";
            superscript = '^';
            //comapre
            equal = "=="; not_equal = "!="; less = "<"; less_or_equal = "<="; greater = ">"; greater_or_equal = ">=";
            //reflection
            typeof_ = "typeof"; sizeof_ = "sizeof";
            
            
            pound = '#';
            at = '@';
            arrow= "->";
            
            //some keywords
            if_ = "if"; else_ = "else";
            else_if = "else if";
            while_ = "while";
            for_ = "for";
//            true_ = "true"; false_ = "false";
            null = "null";
            this_ = "this";
            new_ = "new";
            return_ = "return";
            function = "function", struct_ = "struct"; class_ = "class", property="property", indexer="indexer";
            // accessability = "private|protected|public";
            attribute = "private|public|const|static";
            ref = "ref";
            break_ = "break";
            continue_ = "continue";
            get = "get"; set="set";
            namespace_ = "namespace";
            require = "require";
            using_ = "using";
//            literal_char = "('[^'\\\"\\\\]')|('\\\\a')|('\\\\b')|('\\\\e')|('\\\\f')|('\\\\n')|('\\\\r')|('\\\\t')|('\\\\v')|('\\\\\\\\')|('\\\\'')|('\\\\\\\"')|('\\\\0')";
            string_ = "\\\"([^\"]|\\\\\\\")*\\\"";
            operator_ = "operator";
            cast = "cast";
            
            declarationSpecifier = "static|readonly|private|protected|public";
            var = "var", const_ = "const", let=  "let";
            
            //            //buildin type
            //            IntType = "int"; FloatType = "double";
            
            //defined
            identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
            int_ = "(([1-9][0-9]*)|0)";
            float_ = "(([1-9][0-9]*)|0)\\.[0-9]*";
            
            //signs
            assign = '=';
            l_bracket = '('; r_bracket = ')';
            l_s_bracket = '['; r_s_bracket = ']';
            l_braces = '{'; r_braces = '}';
            l_a_bracket = '<'; r_a_bracket = '>';
            period = '.'; comma = ','; colon = ':'; semicolon = ';';
            
            //predefine
            define = "#define";
            undefine = "#undefine";
            define_if = "#if";
            define_else = "#else";
            define_end = "#end";
            
            newline = "\\n|\\r|\\r\\n";
            newline.id(token_id::newline);
            space = " ";
            space.id(token_id::space);
            
            this->self += arrow;
            this->self += increment | decrement;
            //binary
            this->self += plus | minus | times | divide | remain;
            //compare
            this->self += equal | not_equal;
            this->self += less_or_equal | greater_or_equal | less | greater;   //waring the sorting
            this->self += superscript;
            //logical
            this->self += and_ | or_ | xor_ | not_;
            //reflection
            this->self += typeof_ | sizeof_;
            
            this->self += pound;
            this->self += at;
         
            // keywords
            this->self += else_if;
            this->self += if_ | else_ | for_ | while_;
//            this->self += true_ | false_;
            this->self += null;
            this->self += this_;
            this->self += new_;
            this->self += return_;
            this->self += function | struct_ | class_ | property | indexer;
            
            this->self += attribute;
            this->self += ref;
            this->self += break_;
            this->self += continue_;
            this->self += get | set;
            this->self += namespace_;
            this->self += require;
            this->self += using_;
//            this->self += literal_char;
            this->self += string_;
            this->self += operator_;
            this->self += cast;
            
            this->self += declarationSpecifier;
            this->self += var | const_ | let;
            
            //defined
            this->self += identifier | int_ | float_;   //identifier must be  afer keywords
            
            //signs
            this->self += assign;
            this->self += l_bracket | r_bracket | l_s_bracket | r_s_bracket | l_braces | r_braces | l_a_bracket | r_a_bracket;
            this->self += period | comma | colon | semicolon;
            
            //predefine
            this->self += define | undefine | define_if | define_else | define_end;
            
            c_comment = "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/";
            c_comment.id(token_id::c_comment);
            line_comment = "\\/\\/[^\\n]*\\n";
            line_comment.id(token_id::line_comment);
            
            this->self("WS") =
                space
            |   c_comment
            |   line_comment
            |   newline
            ;
        }
    };
    
}}