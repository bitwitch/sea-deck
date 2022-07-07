/*
 * This program parses a C declaration and translates it to an English sentence.
 * It is an programming challenge issued in the book "Expert C Programming:
 * Deep C Secrets" by Peter Van Der Linden 
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 256
#define MAX_TOKENS 64

typedef struct {
    char type;
    char string[MAX_TOKEN_LENGTH];
} Token;

typedef enum {
    TOKEN_TYPE_TYPE,
    TOKEN_TYPE_STORAGE_CLASS,
    TOKEN_TYPE_QUALIFIER,
    TOKEN_TYPE_IDENTIFIER
} Token_Type;

static Token stack[MAX_TOKENS];
static int sp;
static Token current;

/* utility routines */
Token_Type classify_string(void);
void get_token(void);
void read_to_first_identifier(void);

/* parsing routines */
void parse_function_args(void);
void parse_arrays(void);
void parse_pointers(void);
void parse_declarator(void);


void read_to_first_identifier(void) {
    while (get_token(), current.type != TOKEN_TYPE_IDENTIFIER)
        stack[sp++] = current;
    printf("%s is a", current.string);
    get_token();
}

void get_token(void) {
    int i = 0;
    char c;

    /* eat leading spaces */
    while ((c = getchar()) == ' ');

    /* handle single character tokens */
    if (c == '*' || c == '[' || c == ']' || c == '(' || c == ')' || c == '[' || c == ';') {
        current.string[i++] = c;
        current.string[i] = '\0';
        current.type = c;
        
    /* handle multi character tokens */
    } else {

        while (c != ' ' && c != '\n' && c != EOF) {
            /* if char is one of single char tokens, ungetc it and handle it in next call to get_token */
            if (c == '*' || c == '[' || c == ']' || c == '(' || c == ')' || c == '[' || c == ';') {
                ungetc(c, stdin);
                break;
            }

            assert(i < MAX_TOKEN_LENGTH);
            current.string[i++] = c;
            c = getchar();
        } 

        assert(i < MAX_TOKEN_LENGTH);
        current.string[i] = '\0';

        /* TODO(shaw): how to handle this, should there be a special token type for EOF? */
        if (c == '\n' || c == EOF) return;

        current.type = classify_string();
    }

    /*printf("token is %s\n", current.string);*/
}

Token_Type classify_string(void) {
    if (0 == strcmp(current.string, "void"))     return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "char"))     return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "short"))    return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "int"))      return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "long"))     return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "signed"))   return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "unsigned")) return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "float"))    return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "double"))   return TOKEN_TYPE_TYPE;

    /* TODO(shaw): handle struct, union, enum specifiers */
    /* does this need to combine with struct tag?? */
    if (0 == strcmp(current.string, "struct"))   return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "enum"))     return TOKEN_TYPE_TYPE;
    if (0 == strcmp(current.string, "union"))    return TOKEN_TYPE_TYPE;

    if (0 == strcmp(current.string, "extern"))   return TOKEN_TYPE_STORAGE_CLASS;
    if (0 == strcmp(current.string, "static"))   return TOKEN_TYPE_STORAGE_CLASS;
    if (0 == strcmp(current.string, "register")) return TOKEN_TYPE_STORAGE_CLASS;
    if (0 == strcmp(current.string, "auto"))     return TOKEN_TYPE_STORAGE_CLASS;
    if (0 == strcmp(current.string, "typedef"))  return TOKEN_TYPE_STORAGE_CLASS;

    if (0 == strcmp(current.string, "const"))    return TOKEN_TYPE_QUALIFIER;
    if (0 == strcmp(current.string, "volatile")) return TOKEN_TYPE_QUALIFIER;

    return TOKEN_TYPE_IDENTIFIER;
}

void parse_declarator(void) {
    if (current.type == '[') 
        parse_arrays();
    else if (current.type == '(') 
        parse_function_args();

    parse_pointers();

    Token left;
    while (sp > 0) {
        left = stack[--sp];
        if (left.type == '(') {
            if (current.type != ')') {
                get_token();
                if (current.type != ')') {
                    fprintf(stderr, "expected closing ')' to match '(' on the stack, but found %c instead\n", current.type);
                    exit(1);
                }
            }
            get_token();
            parse_declarator();
        } else if (left.type == '*') {
            printf(" pointer to");
        } else {
            printf(" %s", left.string);
        }
    }
}

void parse_arrays(void) {
    while (current.type == '[') {
        printf(" array of");
        while (get_token(), current.type != ']')
            printf(" %s", current.string);
        get_token();
    }
}

void parse_function_args(void) {
    while (get_token(), current.type != ')') {
        /* TODO(shaw): handle function arguments */
    }
    printf(" function returning");
}

void parse_pointers(void) {
    while (sp > 0 && stack[sp-1].type == '*') {
        printf(" pointer to");
        --sp;
    }
}

int main(void) {
    read_to_first_identifier();
    parse_declarator();
    printf("\n");
    return 0;
}
