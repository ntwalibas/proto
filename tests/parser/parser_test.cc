#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/assignment.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/statements/continue.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/binary.h"
#include "ast/expressions/unary.h"
#include "ast/expressions/group.h"
#include "ast/declarations/type.h"
#include "ast/statements/return.h"
#include "ast/statements/break.h"
#include "ast/statements/while.h"
#include "ast/expressions/call.h"
#include "ast/statements/block.h"
#include "ast/statements/for.h"
#include "ast/statements/if.h"
#include "parser/parser.h"
#include "common/token.h"
#include "lexer/lexer.h"


class ParserTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};


// Program
TEST_F(ParserTest, parseEmptyProgramTest)
{
    std::string source = "";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);

    EXPECT_THROW({
        try {
            parser.parse();
        } catch (ParserError& e) {
            EXPECT_STREQ(e.getPrimaryMessage(), "a program must have at least one definition");
            throw;
        }
    }, ParserError);
}


// Definitions
TEST_F(ParserTest, parseDefinitionTest)
{
    // The definition doesn't start with an identifier that names it
    std::string noIdentifierSource = "function";
    Lexer noIdentifierLexer(std::make_shared<std::string>(noIdentifierSource), source_path);
    Parser noIdentifierparser(noIdentifierLexer);

    EXPECT_THROW({
        try {
            noIdentifierparser.parseDefinition();
        } catch (ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected an identifier");
            throw;
        }
    }, ParserError);

    // Missing colon after definition name
    std::string noColonSource = "name";
    Lexer noColonLexer(std::make_shared<std::string>(noColonSource), source_path);
    Parser noColonparser(noColonLexer);
    EXPECT_THROW({
        try {
            noColonparser.parseDefinition();
        } catch (ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected a colon after this definition name");
            throw;
        }
    }, ParserError);

    // Otherwise, we have a valid definition
    std::string Source = "name: string = \"John Doe\"";
    Lexer Lexer(std::make_shared<std::string>(Source), source_path);
    Parser parser(Lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def->getType(), DefinitionType::Variable);
}

TEST_F(ParserTest, parseVariableDefinitionTest)
{
    std::string source = "name: string = \"John Doe\"";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def->getType(), DefinitionType::Variable);
    VariableDefinition& var_def = static_cast<VariableDefinition&>(*def);

    // We have the correct variable name
    EXPECT_EQ(var_def.getToken().getLexeme(), "name");

    // We have the correct type
    std::unique_ptr<TypeDeclaration>& var_type = var_def.getTypeDeclaration();
    EXPECT_EQ(var_type -> getTypeCategory(), TypeCategory::Simple);
    SimpleTypeDeclaration& simple_type = static_cast<SimpleTypeDeclaration&>(*var_type);
    EXPECT_EQ(simple_type.getToken().getLexeme(), "string");

    // We have the correct initializer
    std::unique_ptr<Expression>& var_init = var_def.getInitializer();
    EXPECT_EQ(var_init->getType(), ExpressionType::Literal);
    LiteralExpression& lit_expr = static_cast<LiteralExpression&>(*var_init);
    EXPECT_EQ(lit_expr.getToken().getLexeme(), "John Doe");
    EXPECT_EQ(lit_expr.getLiteralType(), LiteralType::String);
}

TEST_F(ParserTest, parseFunctionDefinitionTest)
{
    std::string source = "sum: function(a: int, b: int) -> int{}";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def->getType(), DefinitionType::Function);
    FunctionDefinition& fun_def = static_cast<FunctionDefinition&>(*def);

    // We have the correct function name
    EXPECT_EQ(fun_def.getToken().getLexeme(), "sum");

    // We have the correct parameters
    std::vector<
        std::unique_ptr<VariableDeclaration>>& fun_params = fun_def.getParameters();
    EXPECT_EQ(fun_params.size(), 2);
    EXPECT_EQ(fun_params[0]->getToken().getLexeme(), "a");
    EXPECT_EQ(fun_params[1]->getToken().getLexeme(), "b");

    // We have the correct return type
    std::unique_ptr<TypeDeclaration>& ret_type = fun_def.getReturnType();
    SimpleTypeDeclaration& simple_type = static_cast<SimpleTypeDeclaration&>(*ret_type);
    EXPECT_EQ(simple_type.getToken().getLexeme(), "int");
}


// Declarations
TEST_F(ParserTest, parseSimpleTypeDeclarationTest)
{
    std::string source = "string";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<SimpleTypeDeclaration> type_decl = parser.parseSimpleTypeDeclaration(true);

    EXPECT_EQ(type_decl->isConst(), true);
    EXPECT_EQ(type_decl->getToken().getLexeme(), "string");
}

TEST_F(ParserTest, parseVariableDeclarationTest)
{
    std::string source = "count: int";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<VariableDeclaration> var_decl = parser.parseVariableDeclaration();

    // We have the correct variable name
    EXPECT_EQ(var_decl->getToken().getLexeme(), "count");

    // We have the correct type
    std::unique_ptr<TypeDeclaration>& var_type = var_decl->getTypeDeclaration();
    EXPECT_EQ(var_type->getTypeCategory(), TypeCategory::Simple);
    SimpleTypeDeclaration& simple_type = static_cast<SimpleTypeDeclaration&>(*var_type);
    EXPECT_EQ(simple_type.getToken().getLexeme(), "int");
}


// Statements
TEST_F(ParserTest, parseBlockStatementTest)
{
    std::string source = "{\n\n\nstart: int = 0\n\n step: int = 2}";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<BlockStatement> block_stmt = parser.parseBlockStatement();
    EXPECT_EQ(block_stmt->getToken().getLexeme(), "{");
    EXPECT_EQ(block_stmt->getDefinitions().size(), 2);
}

TEST_F(ParserTest, parseIfStatementTest)
{
    // We only have an if branch
    std::string ifSource = "if(name == \"John Doe\"){IdentifyHim()}";
    Lexer ifLexer(std::make_shared<std::string>(ifSource), source_path);
    Parser ifParser(ifLexer);
    std::unique_ptr<IfStatement> if_stmt = ifParser.parseIfStatement();
    EXPECT_EQ(if_stmt->getToken().getLexeme(), "if");
    EXPECT_EQ(if_stmt->getCondition()->getType(), ExpressionType::Binary);
    EXPECT_EQ(if_stmt->getBody()->getDefinitions().size(), 1);

    // We add an elif branch
    std::string elifSource = "if(True){} elif(False){}";
    Lexer elifLexer(std::make_shared<std::string>(elifSource), source_path);
    Parser elifParser(elifLexer);
    std::unique_ptr<IfStatement> elif_stmt = elifParser.parseIfStatement();
    EXPECT_EQ(elif_stmt->getElifBranches().size(), 1);
    std::unique_ptr<ElifBranch>& elif_branch = elif_stmt->getElifBranches()[0];
    EXPECT_EQ(elif_branch->getToken().getLexeme(), "elif");
    EXPECT_EQ(elif_branch->getCondition()->getType(), ExpressionType::Literal);
    EXPECT_EQ(elif_branch->getBody()->getDefinitions().size(), 0);

    // We add the else branch
    std::string elseSource = "if(True){} elif(False){} else{print()}";
    Lexer elseLexer(std::make_shared<std::string>(elseSource), source_path);
    Parser elseParser(elseLexer);
    std::unique_ptr<IfStatement> else_stmt = elseParser.parseIfStatement();
    EXPECT_NE(else_stmt->getElseBranch(), nullptr);
    std::unique_ptr<ElseBranch>& else_branch = else_stmt->getElseBranch();
    EXPECT_EQ(else_branch->getToken().getLexeme(), "else");
    EXPECT_EQ(else_branch->getBody()->getDefinitions().size(), 1);
}

TEST_F(ParserTest, parseForStatementTest)
{
    // We have a variable definition as init clause
    std::string defSource = "for(i: int = 10; i > 0; i = i - 1){}";
    Lexer defLexer(std::make_shared<std::string>(defSource), source_path);
    Parser defParser(defLexer);
    std::unique_ptr<ForStatement> def_for_stmt = defParser.parseForStatement();
    EXPECT_EQ(def_for_stmt->getToken().getLexeme(), "for");
    EXPECT_EQ(def_for_stmt->getBody()->getDefinitions().size(), 0);
    // Check init clause
    std::unique_ptr<Definition>& def_init_clause = def_for_stmt->getInitClause();
    EXPECT_EQ(def_init_clause->getType(), DefinitionType::Variable);
    // Check term clause
    std::unique_ptr<Expression>& def_term_clause = def_for_stmt->getTermClause();
    EXPECT_EQ(def_term_clause->getType(), ExpressionType::Binary);
    // Check incr clause
    std::unique_ptr<Expression>& def_incr_clause = def_for_stmt->getIncrClause();
    EXPECT_EQ(def_incr_clause->getType(), ExpressionType::Assignment);

   // We have an expression as init clause
    std::string exprSource = "for(i = 10; i > 0; i = i - 1){}";
    Lexer exprLexer(std::make_shared<std::string>(exprSource), source_path);
    Parser exprParser(exprLexer);
    std::unique_ptr<ForStatement> expr_for_stmt = exprParser.parseForStatement();
    std::unique_ptr<Definition>& expr_init_clause = expr_for_stmt->getInitClause();
    EXPECT_EQ(expr_init_clause->getType(), DefinitionType::Statement);
    AssignmentExpression& init_clause = static_cast<AssignmentExpression&>(*expr_init_clause);
    EXPECT_EQ(init_clause.getLvalue()->getType(), ExpressionType::Variable);
    EXPECT_EQ(init_clause.getRvalue()->getType(), ExpressionType::Literal);

   // We have no init clause
    std::string noInitSource = "for(; i > 0; i = i - 1){}";
    Lexer noInitLexer(std::make_shared<std::string>(noInitSource), source_path);
    Parser noInitParser(noInitLexer);
    std::unique_ptr<ForStatement> noInit_for_stmt = noInitParser.parseForStatement();
    EXPECT_EQ(noInit_for_stmt->getInitClause(), nullptr);

   // We have no term clause
    std::string noTermSource = "for(i = 10;; i = i - 1){}";
    Lexer noTermLexer(std::make_shared<std::string>(noTermSource), source_path);
    Parser noTermParser(noTermLexer);
    std::unique_ptr<ForStatement> noTerm_for_stmt = noTermParser.parseForStatement();
    EXPECT_EQ(noTerm_for_stmt->getTermClause(), nullptr);

   // We have no incr clause
    std::string noIncrSource = "for(i = 10; i > 0;){}";
    Lexer noIncrLexer(std::make_shared<std::string>(noIncrSource), source_path);
    Parser noIncrParser(noIncrLexer);
    std::unique_ptr<ForStatement> noIncr_for_stmt = noIncrParser.parseForStatement();
    EXPECT_EQ(noIncr_for_stmt->getIncrClause(), nullptr);

   // We have no clauses
    std::string noClausesSource = "for(;;){}";
    Lexer noClausesLexer(std::make_shared<std::string>(noClausesSource), source_path);
    Parser noClausesParser(noClausesLexer);
    std::unique_ptr<ForStatement> noClauses_for_stmt = noClausesParser.parseForStatement();
    EXPECT_EQ(noClauses_for_stmt->getInitClause(), nullptr);
    EXPECT_EQ(noClauses_for_stmt->getTermClause(), nullptr);
    EXPECT_EQ(noClauses_for_stmt->getIncrClause(), nullptr);
}

TEST_F(ParserTest, parseWhileStatementTest)
{
    std::string source = "while(True){makeItHappen()}";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<WhileStatement> wh_stmt = parser.parseWhileStatement();
    EXPECT_EQ(wh_stmt->getToken().getLexeme(), "while");
    EXPECT_EQ(wh_stmt->getCondition()->getType(), ExpressionType::Literal);
    EXPECT_EQ(wh_stmt->getBody()->getDefinitions().size(), 1);

    // We miss the opening parenthesis
    std::string noLeftParenSource = "while True){makeItHappen()}";
    Lexer noLeftParenLexer(std::make_shared<std::string>(noLeftParenSource), source_path);
    Parser noLeftParenParser(noLeftParenLexer);
    EXPECT_THROW({
        try {
            noLeftParenParser.parseWhileStatement();
        } catch (ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected a left opening parenthesis before loop condition");
            throw;
        }
    }, ParserError);

    // We miss the closing parenthesis
    std::string noRightParenSource = "while(True{makeItHappen()}";
    Lexer noRightParenLexer(std::make_shared<std::string>(noRightParenSource), source_path);
    Parser noRightParenParser(noRightParenLexer);
    EXPECT_THROW({
        try {
            noRightParenParser.parseWhileStatement();
        } catch (ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected right closing parenthesis after loop condition");
            throw;
        }
    }, ParserError);
}

TEST_F(ParserTest, parseContinueStatementTest)
{
    std::string source = "continue";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<ContinueStatement> cont_stmt = parser.parseContinueStatement();
    EXPECT_EQ(cont_stmt->getToken().getLexeme(), "continue");
}

TEST_F(ParserTest, parseBreakStatementTest)
{
    std::string source = "break";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<BreakStatement> br_stmt = parser.parseBreakStatement();
    EXPECT_EQ(br_stmt->getToken().getLexeme(), "break");
}

TEST_F(ParserTest, parseReturnStatementTest)
{
    std::string source = "return a + b";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<ReturnStatement> ret_stmt = parser.parseReturnStatement();
    EXPECT_EQ(ret_stmt->getToken().getLexeme(), "return");
    EXPECT_EQ(ret_stmt->getExpression()->getType(), ExpressionType::Binary);
}


// Expressions
TEST_F(ParserTest, parseExpressionTest)
{
    std::string source = "2 * 5 + 1";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& add_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the binary expression
    EXPECT_EQ(add_expr.getToken().getLexeme(), "+");
    EXPECT_EQ(add_expr.getLeft()->getType(), ExpressionType::Binary);
    EXPECT_EQ(add_expr.getRight()->getType(), ExpressionType::Literal);
    LiteralExpression& one_expr = static_cast<LiteralExpression&>(*(add_expr.getRight()));
    EXPECT_EQ(one_expr.getToken().getLexeme(), "1");

    // We have the right data on the lhs of the binary expression
    std::unique_ptr<Expression>& left_expr = add_expr.getLeft();
    BinaryExpression& mul_expr = static_cast<BinaryExpression&>(*left_expr);
    EXPECT_EQ(mul_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(mul_expr.getRight()->getType(), ExpressionType::Literal);
    LiteralExpression& two_expr = static_cast<LiteralExpression&>(*(mul_expr.getLeft()));
    LiteralExpression& five_expr = static_cast<LiteralExpression&>(*(mul_expr.getRight()));
    EXPECT_EQ(two_expr.getToken().getLexeme(), "2");
    EXPECT_EQ(five_expr.getToken().getLexeme(), "5");
}

TEST_F(ParserTest, parseAssignmentExpressionTest)
{
    std::string source = "a = b = 0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseAssignmentExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Assignment);
    AssignmentExpression& assign_expr = static_cast<AssignmentExpression&>(*expr);

    // We have the right data on the assignment expression
    EXPECT_EQ(assign_expr.getToken().getLexeme(), "=");
    EXPECT_EQ(assign_expr.getLvalue()->getType(), ExpressionType::Variable);
    EXPECT_EQ(assign_expr.getRvalue()->getType(), ExpressionType::Assignment);
}

TEST_F(ParserTest, parseLogicalOrExpressionTest)
{
    std::string source = "True || False";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseLogicalOrExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& logicor_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the logical or expression
    EXPECT_EQ(logicor_expr.getToken().getLexeme(), "||");
    EXPECT_EQ(logicor_expr.getBinaryType(), BinaryType::LogicalOr);
    EXPECT_EQ(logicor_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(logicor_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseLogicalAndExpressionTest)
{
    std::string source = "True && False";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseLogicalAndExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& logicand_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the logical and expression
    EXPECT_EQ(logicand_expr.getToken().getLexeme(), "&&");
    EXPECT_EQ(logicand_expr.getBinaryType(), BinaryType::LogicalAnd);
    EXPECT_EQ(logicand_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(logicand_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseComparisonExpressionTest)
{
    std::string source = "counter <> 0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseComparisonExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& comp_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the comparison expression
    EXPECT_EQ(comp_expr.getToken().getLexeme(), "<>");
    EXPECT_EQ(comp_expr.getBinaryType(), BinaryType::NotEqual);
    EXPECT_EQ(comp_expr.getLeft()->getType(), ExpressionType::Variable);
    EXPECT_EQ(comp_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseBitwiseOrExpressionTest)
{
    std::string source = "1 | 0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseBitwiseOrExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& bitor_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the bitwise or expression
    EXPECT_EQ(bitor_expr.getToken().getLexeme(), "|");
    EXPECT_EQ(bitor_expr.getBinaryType(), BinaryType::BitwiseOr);
    EXPECT_EQ(bitor_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(bitor_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseBitwiseXorExpressionTest)
{
    std::string source = "1 ^ 0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseBitwiseXorExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& bitxor_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the bitwise xor expression
    EXPECT_EQ(bitxor_expr.getToken().getLexeme(), "^");
    EXPECT_EQ(bitxor_expr.getBinaryType(), BinaryType::BitwiseXor);
    EXPECT_EQ(bitxor_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(bitxor_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseBitwiseAndExpressionTest)
{
    std::string source = "1 & 0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseBitwiseAndExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& bitand_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the bitwise and expression
    EXPECT_EQ(bitand_expr.getToken().getLexeme(), "&");
    EXPECT_EQ(bitand_expr.getBinaryType(), BinaryType::BitwiseAnd);
    EXPECT_EQ(bitand_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(bitand_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseBitshiftExpressionTest)
{
    std::string source = "uint << 5";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseBitshiftExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& shift_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the bit shift expression
    EXPECT_EQ(shift_expr.getToken().getLexeme(), "<<");
    EXPECT_EQ(shift_expr.getBinaryType(), BinaryType::LeftShift);
    EXPECT_EQ(shift_expr.getLeft()->getType(), ExpressionType::Variable);
    EXPECT_EQ(shift_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseTermExpressionTest)
{
    std::string source = "(2 * counter) + 1";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseTermExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& term_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the term expression
    EXPECT_EQ(term_expr.getToken().getLexeme(), "+");
    EXPECT_EQ(term_expr.getBinaryType(), BinaryType::Plus);
    EXPECT_EQ(term_expr.getLeft()->getType(), ExpressionType::Group);
    EXPECT_EQ(term_expr.getRight()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseFactorExpressionTest)
{
    std::string source = "2 * counter";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseFactorExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Binary);
    BinaryExpression& fact_expr = static_cast<BinaryExpression&>(*expr);

    // We have the right data on the factor expression
    EXPECT_EQ(fact_expr.getToken().getLexeme(), "*");
    EXPECT_EQ(fact_expr.getBinaryType(), BinaryType::Mul);
    EXPECT_EQ(fact_expr.getLeft()->getType(), ExpressionType::Literal);
    EXPECT_EQ(fact_expr.getRight()->getType(), ExpressionType::Variable);
}

TEST_F(ParserTest, parseSignExpressionTest)
{
    std::string source = "-10";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseSignExpression();

    // We have the correct type of expression
    EXPECT_EQ(expr->getType(), ExpressionType::Unary);
    UnaryExpression& sign_expr = static_cast<UnaryExpression&>(*expr);

    // We have the right data on the signed expression
    EXPECT_EQ(sign_expr.getToken().getLexeme(), "-");
    EXPECT_EQ(sign_expr.getUnaryType(), UnaryType::Minus);
    EXPECT_EQ(sign_expr.getExpression()->getType(), ExpressionType::Literal);
}

TEST_F(ParserTest, parseNotExpressionTest)
{
    // Bitwise not
    {
        std::string source = "~~1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseNotExpression();

        // We have the correct type of expression
        EXPECT_EQ(expr->getType(), ExpressionType::Unary);
        UnaryExpression& bitnot_expr = static_cast<UnaryExpression&>(*expr);

        // We have the right data on the bitwise not expression
        EXPECT_EQ(bitnot_expr.getToken().getLexeme(), "~");
        EXPECT_EQ(bitnot_expr.getUnaryType(), UnaryType::BitwiseNot);
        EXPECT_EQ(bitnot_expr.getExpression()->getType(), ExpressionType::Unary);
    }

    // Logical not
    {
        std::string source = "!False";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseNotExpression();

        // We have the correct type of expression
        EXPECT_EQ(expr->getType(), ExpressionType::Unary);
        UnaryExpression& logicnot_expr = static_cast<UnaryExpression&>(*expr);

        // We have the right data on the logical not expression
        EXPECT_EQ(logicnot_expr.getToken().getLexeme(), "!");
        EXPECT_EQ(logicnot_expr.getUnaryType(), UnaryType::LogicalNot);
        EXPECT_EQ(logicnot_expr.getExpression()->getType(), ExpressionType::Literal);
    }
}

TEST_F(ParserTest, parsePrimaryExpressionTest)
{
    std::string source = "name";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parsePrimaryExpression();

    // We have the correct expression type
    EXPECT_EQ(expr->getType(), ExpressionType::Variable);
    VariableExpression& var_expr = static_cast<VariableExpression&>(*expr);

    // We have the correct variable expression name
    EXPECT_EQ(var_expr.getToken().getLexeme(), "name");
}

TEST_F(ParserTest, parseCallExpressionTest)
{
    std::string source = "sum(1, 2)";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<CallExpression> call_expr = parser.parseCallExpression();
    EXPECT_EQ(call_expr->getToken().getLexeme(), "sum");
    EXPECT_EQ(call_expr->getArguments().size(), 2);
}

TEST_F(ParserTest, parseGroupExpressionTest)
{
    std::string source = "(name)";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<GroupExpression> group_expr = parser.parseGroupExpression();
    EXPECT_EQ(group_expr->getToken().getLexeme(), "(");
    EXPECT_EQ(group_expr->getExpression()->getType(), ExpressionType::Variable);
}

TEST_F(ParserTest, parseVariableExpressionTest)
{
    std::string source = "name";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<VariableExpression> var_expr = parser.parseVariableExpression();
    EXPECT_EQ(var_expr->getToken().getLexeme(), "name");
}

TEST_F(ParserTest, parseLiteralExpressionTest)
{
    // Boolean literal
    std::string booleanSource = "False";
    Lexer booleanLexer(std::make_shared<std::string>(booleanSource), source_path);
    Parser booleanParser(booleanLexer);
    std::unique_ptr<LiteralExpression> boolean_lit = booleanParser.parseLiteralExpression();
    EXPECT_EQ(boolean_lit->getToken().getLexeme(), "False");
    EXPECT_EQ(boolean_lit->getLiteralType(), LiteralType::Boolean);

    // Integer literal
    std::string intSource = "0";
    Lexer intLexer(std::make_shared<std::string>(intSource), source_path);
    Parser intParser(intLexer);
    std::unique_ptr<LiteralExpression> int_lit = intParser.parseLiteralExpression();
    EXPECT_EQ(int_lit->getToken().getLexeme(), "0");
    EXPECT_EQ(int_lit->getLiteralType(), LiteralType::Integer);

    // Float literal
    std::string floatSource = "0.5";
    Lexer floatLexer(std::make_shared<std::string>(floatSource), source_path);
    Parser floatParser(floatLexer);
    std::unique_ptr<LiteralExpression> float_lit = floatParser.parseLiteralExpression();
    EXPECT_EQ(float_lit->getToken().getLexeme(), "0.5");
    EXPECT_EQ(float_lit->getLiteralType(), LiteralType::Float);

    // String literal
    std::string stringSource = "\"string\"";
    Lexer stringLexer(std::make_shared<std::string>(stringSource), source_path);
    Parser stringParser(stringLexer);
    std::unique_ptr<LiteralExpression> string_lit = stringParser.parseLiteralExpression();
    EXPECT_EQ(string_lit->getToken().getLexeme(), "string");
    EXPECT_EQ(string_lit->getLiteralType(), LiteralType::String);
}
