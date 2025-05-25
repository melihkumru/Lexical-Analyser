#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_IDENTIFIER_SIZE 10
#define MAX_INTEGER_SIZE 8
#define MAX_STRING_SIZE 257

typedef enum {
    IDENTIFIER,
    INT_CONST,
    OPERATOR,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    STRING_CONST,
    KEYWORD,
    END_OF_LINE,
    COMMA,
    END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_STRING_SIZE];
} Token;

Token readToken(FILE *fp);
void writeToken(Token token, FILE *fp);

int main() {
    FILE *readp, *writep;
    Token token;
    int is_open_bracket = 0;

    readp = fopen("code.sta", "r");

    if (readp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    writep = fopen("code.lex", "w");
    
    if (writep == NULL) {
        printf("Error creating file.\n");
        fclose(readp);
        return 1;
    }
    /*Processed part of lex analysis.*/
    do {
        token = readToken(readp);
        /*The section where incorrect use of parentheses is also discussed.*/
        if (token.type==LEFT_CURLY_BRACKET){
            is_open_bracket += 1;
        }
        if (token.type== RIGHT_CURLY_BRACKET){
            if (is_open_bracket==0){
                printf("Error: INVALID CURLY BRACKET!");
                exit(1);
            }
            is_open_bracket -= 1;
        }
        writeToken(token, writep);
    } while (token.type != END);
    if(is_open_bracket!=0){
        printf("Error: INVALID CURLY BRACKET!");
        exit(1);
    }
    fclose(readp);
    fclose(writep);
    printf("Lexical analysis complete.\n");
    return 0;
}

Token readToken(FILE *fp) {
    char c;
    Token token;
    int index = 0;
    /*Pass all spaces*/
    do{
        c = fgetc(fp);
    } while (isspace(c));

    /*Is it end of file*/
    if (c == EOF) {
        token.type = END;
        return token;
    }
    
    /* Is it identifier or keyword*/
    if (isalpha(c)) { 
        do {
            token.value[index++] = c;
            c = fgetc(fp);
        } while (isalnum(c) || c == '_');

        token.value[index] = '\0';
        ungetc(c,fp);
        token.type = KEYWORD; 
        if (strcmp(token.value, "int") != 0 && strcmp(token.value, "text") != 0 &&
            strcmp(token.value, "is") != 0 && strcmp(token.value, "loop") != 0 &&
            strcmp(token.value, "times") != 0 && strcmp(token.value, "read") != 0 &&
            strcmp(token.value, "write") != 0 && strcmp(token.value, "newLine") != 0) {
            
            if(strlen(token.value) > MAX_IDENTIFIER_SIZE){
                printf("Error: IDENTIFIER_SIZE\n");
                exit(1);
            }
            token.type = IDENTIFIER;
        }
    }

    /*Is it int-const*/ 
    else if (isdigit(c)) { 
        do {
            token.value[index++] = c;
            c = fgetc(fp);
        } while (isdigit(c));

        token.value[index] = '\0';
        if (strlen(token.value)>MAX_INTEGER_SIZE){
            printf("Error: MAX_INTEGER_SIZE\n");
            exit(1);
        }
        ungetc(c,fp);
        token.type = INT_CONST;
    } 
    
    /*Is it operator*/ 
    else if (c == '+' || c == '-' || c == '*' || c == '/') { 
        if(c =='/'){
            char nextChar = fgetc(fp);
            if (nextChar == '*'){
                do{
                    c=fgetc(fp);
                } while ((c!='*' && c!=EOF)||c=='\r');
                
                if (c=='*'){
                    c=fgetc(fp);
                    if(c =='/'){
                        return readToken(fp);
                    }
                    else {
                        printf("Error: Comment Line is not terminated!");
                        exit(1);
                    }
                }
                else {
                    printf("Error: Comment Line is not terminated!");
                    exit(1);
                }
            }
            else {
                ungetc(nextChar, fp);
                token.value[index++] = c;
                token.value[index] = '\0';
                token.type = OPERATOR;
            }
        }
        else{
            token.value[index++] = c;
            token.value[index] = '\0';
            token.type = OPERATOR;
        }
    }
    
    /*Is it left curly bracket*/
    else if (c == '{') {
        token.value[index++] = c;
        token.value[index] = '\0';
        token.type = LEFT_CURLY_BRACKET;
    } 
    
    /*Is it right curly bracket*/
    else if (c == '}') { 
        token.value[index++] = c;
        token.value[index] = '\0';
        token.type = RIGHT_CURLY_BRACKET;
    }

    /*Is it text*/
    else if (c == '"') { 
        token.value[index++] = c;
        c = fgetc(fp);
        while (c != '"' && c != EOF) {
            token.value[index++] = c;
            c = fgetc(fp);
            if(c==EOF){
                printf("Error: String constant not terminated.");
                exit(1);
            }
        }
        if (c == EOF || index > MAX_STRING_SIZE) {
            printf("Error: MAX_STRING_SIZE\n");
            exit(1);
        }
        token.value[index++] = c;
        token.value[index] = '\0';
        token.type = STRING_CONST;
    }

    /*Is it end of line */
    else if (c == '.') { 
        token.value[index++] = c;
        token.value[index] = '\0';
        token.type = END_OF_LINE;
    }
    
    /*Is it comma*/
    else if (c == ',') { 
        token.value[index++] = c;
        token.value[index] = '\0';
        token.type = COMMA;
    }

    /*Is it invalid char*/
    else {
        printf("Error: Invalid character.\n");
        exit(1);
    }

    return token;
}

void writeToken(Token token, FILE *fp) {
    switch (token.type) {
        case IDENTIFIER:
            fprintf(fp, "Identifier(%s)\n", token.value);
            break;
        case INT_CONST:
            fprintf(fp, "IntConst(%s)\n", token.value);
            break;
        case OPERATOR:
            fprintf(fp, "Operator(%s)\n", token.value);
            break;
        case LEFT_CURLY_BRACKET:
            fprintf(fp, "LeftCurlyBracket\n");
            break;
        case RIGHT_CURLY_BRACKET:
            fprintf(fp, "RightCurlyBracket\n");
            break;
        case STRING_CONST:
            fprintf(fp, "String(%s)\n", token.value);
            break;
        case KEYWORD:
            fprintf(fp, "Keyword(%s)\n", token.value);
            break;
        case END_OF_LINE:
            fprintf(fp, "EndOfLine\n");
            break;
        case COMMA:
            fprintf(fp, "Comma\n");
            break;
    }
}