#include <assert.h>
#include <stdio.h>
#include <string.h>

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
void parse_any_pointers(void);
void parse_declarator(void);

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

    printf("token is %s\n", current.string);
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


void read_to_first_identifier(void) {
    do {
        get_token();
        stack[sp++] = current;
    } while (current.type != TOKEN_TYPE_IDENTIFIER);
    printf("identifier is %s\n", current.string);
    get_token();
}

int main(void) {
    read_to_first_identifier();
    /*parse_declarator();*/
    return 0;
}

