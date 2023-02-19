/*  This file is part of the Proto programming language
 * 
 *  Copyright (c) 2023- Ntwali Bashige Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#ifndef PROTO_PARSER_H
#define PROTO_PARSER_H

#include <exception>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/declarations/variable.h"
#include "ast/definitions/function.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/statements/statement.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/group.h"
#include "ast/expressions/array.h"
#include "ast/declarations/type.h"
#include "ast/expressions/call.h"
#include "ast/statements/block.h"
#include "common/token.h"
#include "lexer/lexer.h"
#include "ast/program.h"


class Parser
{
    public:
        Parser(Lexer& lexer);

        /**
         * Parses the token stream and returns an AST representing the program.
         */
        Program parse();
        Program parseProgram();

        // Definitions
        std::unique_ptr<Definition> parseDefinition();
        std::unique_ptr<VariableDefinition> parseVariableDefinition(Token& var_token);
        std::unique_ptr<FunctionDefinition> parseFunctionDefinition(Token& fun_token);

        // Declarations
        std::unique_ptr<TypeDeclaration> parseTypeDeclaration();
        std::unique_ptr<SimpleTypeDeclaration> parseSimpleTypeDeclaration(bool is_const);
        std::unique_ptr<ArrayTypeDeclaration> parseArrayTypeDeclaration(bool is_const);
        std::unique_ptr<VariableDeclaration> parseVariableDeclaration();

        // Statements
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<BlockStatement> parseBlockStatement();

        // Expressions
        std::unique_ptr<Expression> parseExpression();
        std::unique_ptr<Expression> parseSignExpression();
        std::unique_ptr<Expression> parseBitwiseNotExpression();
        std::unique_ptr<Expression> parseSubscriptExpression();
        std::unique_ptr<Expression> parsePrimaryExpression();
        std::unique_ptr<CallExpression> parseCallExpression();
        std::unique_ptr<GroupExpression> parseGroupExpression();
        std::unique_ptr<ArrayExpression> parseArrayExpression();
        std::unique_ptr<VariableExpression> parseVariableExpression();
        std::unique_ptr<LiteralExpression> parseLiteralExpression();

        std::vector<class ParserError>  errors;     /* List of non-fatal errors to be displayed all at once. */

    private:
        Lexer&                          lexer;      /* Lexer to generate tokens */
        Token                           previous;   /* Last token to be consumed. */
        Token                           current;    /* Token to be consumed. */
        Token                           next;       /* Next token to be consumed. */

        // Returns the token that comes before the one currently being parsed.
        Token& peekBack();

        // Returns the current token.
        Token& peek();

        // Returns the token that comes after the one currently being parsed.
        Token& peekFront();

        // Returns true if we are at the end of the token stream.
        bool atEnd();

        // Returns true if we are one token away from the end of the token stream.
        bool pastEnd();

        // Returns true if the previous token type matches the given type.
        bool checkPrevious(enum TokenType type);

        // Returns true if the current token type matches the given type.
        bool check(enum TokenType type);

        // Returns true if the next token type matches the given type.
        bool checkNext(enum TokenType type);

        // Advances in the token stream and returns the current token.
        Token& advance();

        // Returns true if the current token type matches the given token type.
        // Advances to the next token only if the match was true.
        bool match(enum TokenType type);

        // If the current token is of the given type, return it and advance.
        // Throws an invalid argument exception otherwise.
        Token& consume(enum TokenType type);

        // Synchronize the parser so we can consume more tokens even under failure.
        void synchronize();
};


class ParserError : public std::runtime_error
{
    public:
        ParserError(
            Token& token,
            std::string const& primary_message,
            std::string const& secondary_message,
            bool fatal
        )
        : std::runtime_error(primary_message),
          token(token),
          secondary_message(secondary_message),
          fatal(fatal) {}
        
        /**
         * Returns the token associated with this error.
         */
        Token& getToken()
        {
            return token;
        }
        
        /**
         * Returns the primary error message.
         */
        char const * getPrimaryMessage() const
        {
            return what();
        }
        
        /**
         * Returns the secondary error message.
         */
        char const * getSecondaryMessage() const
        {
            return secondary_message.c_str();
        }

        /**
         * Returns true if we can't recover from this error.
         */
        bool isFatal() const
        {
            return fatal;
        }

    private:
        Token token;                    /* Where the error happened. */
        std::string secondary_message;  /* Error secondary message. */
        bool fatal;                     /* Is this an error we can recover from. */
};

#endif
