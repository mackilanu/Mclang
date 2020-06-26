#include "include/parser.h"
#include <stdio.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer) 
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);

    return parser;
}

void parser_eat(parser_T* parser, int token_type)
{
    if(parser->current_token->type == token_type) 
    {
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else 
    {
        printf(
            "Unexpected token '%s' with type '%d'",
            parser->current_token->value, parser->current_token->type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser) 
{
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser) 
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser) ; 
    }
}

AST_T* parser_parse_statements(parser_T* parser) 
{
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT));

    AST_T* ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;


    while(parser->current_token->type == TOKEN_SEMI) 
    {
        parser_eat(parser, TOKEN_SEMI);

        AST_T* ast_statement = parser_parse_statement(parser);
        compound->compound_size += 1;
        compound->compound_value = realloc(compound->compound_value,
            compound->compound_size * sizeof(struct AST_STRUCT*));
        compound->compound_value[compound->compound_size-1] = ast_statement;
    }

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser) 
{
    switch(parser->current_token->type) 
    {
        case TOKEN_STRING: return parser_parse_string(parser);
    }
}

AST_T* parser_parse_factor(parser_T* parser) 
{

}
AST_T* parser_parse_term(parser_T* parser) 
{

}

AST_T* parser_parse_function_call(parser_T* parser) 
{
  printf("Trying..\n");
}

AST_T* parser_parse_variable_definition(parser_T* parser) 
{
    parser_eat(parser, TOKEN_ID); /* var */

    char* var_def_var_name= parser->current_token->value;

    parser_eat(parser, TOKEN_ID); /* var name */
    parser_eat(parser, TOKEN_EQUALS); /* equal sign. I.e: var name = ... */

    AST_T* var_def_var_val = parser_parse_expr(parser);
    AST_T* var_def = init_ast(AST_VARIABLE_DEFINITION);

    var_def->variable_definition_variable_name = var_def_var_name;
    var_def->variable_definition_value = var_def_var_val;

    return var_def;
}

AST_T* parser_parse_variable(parser_T* parser) 
{
    char* token_val = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    if(parser->current_token->type = TOKEN_LPAREN)
        return parser_parse_function_call(parser);

    AST_T* ast_var = init_ast(AST_VARIABLE);
    ast_var->variable_name = token_val;

    return ast_var;
}

AST_T* parser_parse_string(parser_T* parser) 
{
    AST_T* ast_str = init_ast(AST_STRING);
    ast_str->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    return ast_str;
}

AST_T* parser_parse_id(parser_T* parser) 
{
    if(strcmp(parser->current_token->value, "var") == 0) {
        return parser_parse_variable_definition(parser);
    } else {
        return parser_parse_variable(parser);
    }
}