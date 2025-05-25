#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_IDENTIFIER_SIZE 10
#define MAX_INTEGER_SIZE 8
#define MAX_STRING_SIZE 257
#define MAX_DATA 50

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

typedef struct {
    TokenType type;
    char name[MAX_IDENTIFIER_SIZE];
    char value[MAX_STRING_SIZE];
} Memory;


Token readToken(FILE *fp);
void writeToken(Token token, FILE *fp);
void parseStatements(FILE *fp, int fromloop);
Token getToken(FILE *fp);
void Write(FILE *fp);
void Read(FILE *fp);
void handleLoop(FILE *fp);
void DeclaraInt(FILE *fp);
void DeclaraString(FILE *fp);
int isIdentInTokens(char *value);
int getintValue(int i);
char* getsrtingValue(int i);
void removeQuotes(char *str);
void assignInt(FILE *fp,int dataindex);
void assignString(FILE *fp,int dataindex);
char* operatorProcess(int val, FILE *fp, Token operator);
char* stringProcess(char *val,FILE *fp, Token operator);
static int index = 0;
static Memory datas[MAX_DATA];

int main() {

    FILE *readp, *writep,*interp;
    Token token;
    int is_open_bracket = 0;

    readp = fopen("code.sta", "r");

    if (readp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    writep = fopen("code.lex", "w");
    interp = fopen("code.lex","r");
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
    printf("\n-->Now code is running...\n");
    parseStatements(interp,0);
    
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

void parseStatements(FILE *fp, int fromloop){
    Token token;
    do{
        token = getToken(fp);
        if(token.type ==KEYWORD){
            if (strcmp(token.value, "loop") == 0){
                handleLoop(fp);               
            }
            else if (strcmp(token.value,"int") == 0){
                DeclaraInt(fp);
            }
            else if (strcmp(token.value,"text") == 0 ){
                DeclaraString(fp);
            }
            else if (strcmp(token.value,"write")== 0){
                Write(fp);
            }
            else if (strcmp(token.value,"newLine")==0){
                printf("\n");
                token = getToken(fp);
                if (token.type != END_OF_LINE){
                    printf("\nError: End Of Line Error!\n");
                    exit(1);
                }
            }
            else if (strcmp(token.value,"read")==0)
                Read(fp);
            }
        else if (token.type == IDENTIFIER){
            int isthere = isIdentInTokens(token.value);
            if (isthere==-1){
                printf("\nError: Use of undefined identifier!\n");
                exit(1);
            }
            else if (datas[isthere].type == INT_CONST){
                assignInt(fp,isthere);
            }
            else if (datas[isthere].type == STRING_CONST){
                assignString(fp,isthere);
            }            
        }
        else if (token.type != END) {
            printf("\nError: Syntax Error!\n");
            exit(1);
        }
    }
    while (token.type !=END  && fromloop == 0);   
}

void Write(FILE *fp){
    /*ilk yazdırılan değere göre şekillenme */
    Token token = getToken(fp);

    if (token.type == STRING_CONST){
        printf("%s",token.value);
    }
    else if(token.type == INT_CONST){
        printf("%s",token.value);
    }
    else if (token.type == IDENTIFIER){
       
        int indexI = isIdentInTokens(token.value);
        
        if (datas[indexI].type == INT_CONST){
            printf("%d",getintValue(indexI));
        }
        else {
            printf("%s",getsrtingValue(indexI));
        }
    }
    else{
        printf("\nError: Write Error!\n");
        exit(1);
    }

    token= getToken(fp);
    if (token.type == COMMA){
        Write(fp);
    }
    else if (token.type!=END_OF_LINE){
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

void Read(FILE *fp){
    Token token = getToken(fp);
    char input[MAX_STRING_SIZE];

    if (token.type == STRING_CONST){
        printf("%s",token.value);
        token = getToken(fp);
        if (token.type !=COMMA){
            printf("\nError: Read Error!\n");
            exit(1);
        }
        token = getToken(fp);
    }
    if (token.type == IDENTIFIER){
        int indexI = isIdentInTokens(token.value);
        if (indexI == -1 ){
            printf("\nError: Use of undefined identifier!\n");
            exit(1);
        }
        scanf("%s", input);
        if(datas[indexI].type == INT_CONST){
            if (strspn(input, "0123456789") != strlen(input)) {
                printf("\nAppropriate int value was not entered. It was set automatically.\n");
                strcpy(input, "0");
            }
        }
        strcpy(datas[indexI].value,input); 

    }
    else {
        printf("\nError: Read Error!\n");
        exit(1);
    }
    token = getToken(fp);
    if(token.type != END_OF_LINE){
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

void handleLoop(FILE *fp){
    Token token= getToken(fp);
    long int startp;
    long int endp;
    int count;

    if (token.type ==INT_CONST){
        sscanf(token.value,"%d",&count);
    }
    else if (token.type == IDENTIFIER){
        count = getintValue(isIdentInTokens(token.value));
    }
    else {
        printf("\nError: Loop Error!\n");
        exit(1);
    }
    
    token = getToken(fp);

    if (strcmp(token.value, "times") != 0 ){
        printf("\nError: Loop Error!\n");
        exit(1);
    }
    token = getToken(fp);
    if (strcmp(token.value, "write") ==0  ){
        startp = ftell(fp);
        Write(fp);
        endp = ftell(fp);
        fseek(fp, startp, SEEK_SET);
        for (int i = 0; i<count-1;i++){
            Write(fp);
            fseek(fp, startp, SEEK_SET);
        }
        fseek(fp,endp,SEEK_SET);
        
    }
    
    else if (token.type == LEFT_CURLY_BRACKET){
        startp = ftell(fp);
        token = getToken(fp);
        while(1){
            if (token.type == END_OF_LINE){
                endp = ftell(fp);
                token = getToken(fp);
                
                if (token.type == RIGHT_CURLY_BRACKET){
                    break;
                }
            }
            else {
                endp = ftell(fp);
                token = getToken(fp);             
                if (token.type == RIGHT_CURLY_BRACKET){
                    printf("\nError: End Of Line Error!\n");
                    exit(1);
                }
            }
        }
        fseek(fp,startp,SEEK_SET);

        for(int i = 0; i<count; i++){
            fseek(fp,startp,SEEK_SET);
            while(ftell(fp)!=endp && !feof(fp)){                    
                parseStatements(fp,1);           
            }
        }
        fseek(fp,endp,SEEK_SET);
        token = getToken(fp);
    }
}

void DeclaraInt(FILE *fp){
    Token token = getToken(fp);
    
    if (token.type !=IDENTIFIER){

        printf("\nError: Invalid use case for int keyword!\n");
        exit(1);
    }

    datas[index].type = INT_CONST;
    strcpy(datas[index].name , token.value);
    strcpy(datas[index].value, "0");
    token= getToken(fp);
    
    /* Eğer nokta gelirse */
    if ((token.type == END_OF_LINE)){
        index++;
    }
    /* Eğer ilk is gelirse */
    else if (strcmp(token.value,"is")==0){
        token = getToken(fp);       
        if (token.type ==INT_CONST){
            strcpy(datas[index].value, token.value);
            index ++;
        }
        else {
            printf("\nError: Invalid declaration for  int values!\n");
            exit(1);
        }
        token = getToken(fp);
        /*Eger başlangıç değerli coklu atama yapılcaksa.*/
        if (token.type ==COMMA){
            DeclaraInt(fp);
        }
        else if(token.type!=END_OF_LINE){
            printf("\nError: Syntax Error!\n");
            exit(1);
        }
    }
    /* Eğer virgül gelirse */
    else if (token.type == COMMA){
        
        index ++; /*ilk identifier listeye eklendiği için index arttırılır.*/
        token = getToken(fp);
        if(token.type == END_OF_LINE){
            printf("\nError: Invalid multi declaration!\n");
            exit(1);
        } 
        while (1){
            if (token.type == IDENTIFIER){
                datas[index].type = INT_CONST;
                strcpy(datas[index].value, "0");
                strcpy(datas[index].name,token.value);
                index ++;
                token = getToken(fp);
                if(token.type == COMMA){
                    token = getToken(fp);
                }
                else if (token.type == END_OF_LINE) {
                    break;
                }
                else {
                    printf("\nError: Invalid multi declaration!\n");
                }
            }
            else {
                printf("\nError: Invalid multi declaration!\n");
                exit(1);
            }
        }
    }
    else {
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

void DeclaraString(FILE *fp){
    Token token = getToken(fp);
    
    if (token.type !=IDENTIFIER){

        printf("\nError: Invalid use case for text keyword!\n");
        exit(1);
    }

    datas[index].type = STRING_CONST;
    strcpy(datas[index].name , token.value);
    strcpy(datas[index].value, "");
    token= getToken(fp);
    
    /* Eğer nokta gelirse */
    if ((token.type == END_OF_LINE)){
        index++;
    }
    /* Eğer ilk is gelirse */
    else if (strcmp(token.value,"is")==0){
        token = getToken(fp);       
        if (token.type ==STRING_CONST){
            strcpy(datas[index].value, token.value);
            index ++;
        }
        else {
            printf("\nError: Invalid declaration for text values!\n");
            exit(1);
        }
        token = getToken(fp);
        if (token.type ==COMMA){
            DeclaraString(fp);
        }
        else if(token.type!=END_OF_LINE){
            printf("\nError: Syntax Error!\n");
            exit(1);
        }
    }
    /* Eğer virgül gelirse */
    else if (token.type == COMMA){
        
        index ++; /*ilk identifier listeye eklendiği için index arttırılır.*/
        token = getToken(fp);
        if(token.type == END_OF_LINE){
            printf("\nError: Invalid multi declaration!\n");
            exit(1);
        } 
        while (1){
            if (token.type == IDENTIFIER){
                datas[index].type = STRING_CONST;
                strcpy(datas[index].value, "");
                strcpy(datas[index].name,token.value);
                index ++;
                token = getToken(fp);
                if(token.type == COMMA){
                    token = getToken(fp);
                }
                else if (token.type == END_OF_LINE) {
                    break;
                }
                else {
                    printf("\nError: Invalid multi declaration!\n");
                } 
            }
            else {
                printf("\nError: Invalid multi declaration!\n");
                exit(1);
            }
        }
    }
    else {
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

/*dataindex bu metoda girilmeden önceki ilk identifierın indeksini tutar.*/
void assignInt(FILE *fp, int dataindex){
    Token token = getToken(fp);
    char str[MAX_INTEGER_SIZE];
    if (strcmp(token.value,"is")!=0){
        printf("\nError: Invalid assign!\n");
        exit(1);
    }
    token = getToken(fp);
    int val;
    if (token.type == INT_CONST){
        sscanf(token.value,"%d",&val);
    }
    else if (token.type == IDENTIFIER){
        val = getintValue(isIdentInTokens(token.value));
    }
    else {
        printf("\nError: Invalid declaration for  int values!\n");
        exit(1);
    }

    token = getToken(fp);
    if (token.type == OPERATOR){
        char *result = operatorProcess(val,fp,token);
        strcpy(datas[dataindex].value, result);
        token = getToken(fp);
        if(token.type != END_OF_LINE){
            printf("\nError: Invalid operation!\n");
            exit(1);
        }
    }
    else if (token.type == END_OF_LINE){
        sprintf(str, "%ld", val);
        strcpy(datas[dataindex].value, str);
    }
    else {
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

void assignString(FILE *fp,int dataindex){
    Token token = getToken(fp);
    if (strcmp(token.value,"is")!=0){
        printf("\nError: Invalid assign!\n");
        exit(1);
    }
    token = getToken(fp);
    char val[MAX_STRING_SIZE];
    if (token.type == STRING_CONST){
        strcpy(val,token.value);
    }
    else if (token.type == IDENTIFIER){
        strcpy(val, getsrtingValue(isIdentInTokens(token.value)));
    }
    else {
        printf("\nError: Invalid declaration for  text values!\n");
        exit(1);
    }
    token = getToken(fp);
    if (token.type == OPERATOR){
        char *result = stringProcess(val,fp,token);
        strcpy(datas[dataindex].value,result);
        token = getToken(fp);
        if(token.type != END_OF_LINE){
            printf("\nError: Invalid operation!\n");
            exit(1);
        }
    }
    else if (token.type == END_OF_LINE){
        strcpy(datas[dataindex].value, val);
    }
    else {
        printf("\nError: Syntax Error!\n");
        exit(1);
    }
}

/*Identifier tanımlanıp tanımlanmadığına bakar. Eger varsa onun datas dizisindeki indeksini döndürür.*/
int isIdentInTokens(char *value) {
    
    for (int i = 0; i < MAX_DATA; i++) {
        if (strcmp(datas[i].name, value) == 0) {
            return i; 
        }
    }
    return -1; 
}

int getintValue(int i){
    if (i == -1){
        printf("\nError: Use of undefined identifier!\n");
        exit(1);
    }
    int val;
    sscanf(datas[i].value,"%d",&val);
    return  val;
}

char* getsrtingValue(int i){
    if (i == -1){
        printf("\nError: Use of undefined identifier!\n");
        exit(1);
    }
    return  datas[i].value;
}

char* operatorProcess(int val, FILE *fp, Token operator){
    Token token = getToken(fp);
    int val2;
    int result;
    if (token.type == INT_CONST){
        sscanf(token.value,"%d",&val2);
    }
    else if (token.type == IDENTIFIER){
        int indexx = isIdentInTokens(token.value);
        val2 = getintValue(indexx);
    }
    else {
        printf("\nError: Invalid Operation!\n");
        exit(1);
    }

    if (strcmp(operator.value,"+") == 0 ){
        result = val + val2;
    }
    else if (strcmp(operator.value,"-") == 0 ){
        result = val - val2;
    }
    else if (strcmp(operator.value,"/") == 0){
        result = val /val2;
    }
    else {
        result = val* val2;
    }
    if (result< 0 ){
        result = 0;
    }
    else if (result > pow(10,MAX_INTEGER_SIZE)){
        printf("\nError: MAX_INTEGER_SIZE ERROR!\n");
        exit(1);
    }
    char *str = (char *) malloc(MAX_INTEGER_SIZE + 1);

    sprintf(str, "%d", result);
    return str;
}

char* stringProcess(char *val,FILE *fp, Token operator){
   
    char array[MAX_STRING_SIZE];
    char *result = malloc(MAX_STRING_SIZE * sizeof(char));
    Token token = getToken(fp);
    if (token.type == IDENTIFIER){
        strcpy(array,getsrtingValue(isIdentInTokens(token.value)));
    }
    else if (token.type == STRING_CONST){
        strcpy(array,token.value);
    }
    else {
        printf("\nError: Invalid Operation for texts!\n");
        exit(1);
    }
    char *ptr = strstr(val, array);

    if(strcmp(operator.value,"+") == 0){
        strcat(val, array);              
        strcpy(result,val);
    }
    else if (strcmp(operator.value,"-")==0){
        if(ptr == NULL){
            strcpy(result,val);
        }
        else {
            memmove(ptr, ptr + strlen(array), strlen(ptr + strlen(array)) + 1);
            strcpy(result,val);
        }
    }
    else {
        printf("\nError: Invalid operation for texts!\n");
        exit(1);
    }
    return result; 
}

Token getToken(FILE *fp) {
    char line[MAX_STRING_SIZE];
    Token token;

    if (fgets(line, MAX_STRING_SIZE, fp) == NULL || strlen(line) == 0) {
        
        token.type = END;
        return token;
    }
    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }
    if (strcmp(line,"EndOfLine")==0){
        token.type = END_OF_LINE;
        return token;
    }
    else if (strcmp(line,"Comma")==0){
        token.type = COMMA;
        return token;
    }
    else if (strcmp(line,"LeftCurlyBracket")==0){
        token.type = LEFT_CURLY_BRACKET;
        return token;
    }
    else if (strcmp(line,"RightCurlyBracket")==0){
        token.type = RIGHT_CURLY_BRACKET;
        return token;
    }    
    char *token_str = strtok(line, "("); 
    char *value_str = strtok(NULL, ")"); 
        
    if (token_str == NULL || value_str == NULL) {
        printf("\nError: Invalid token format.\n");
        exit(1);
    }

    if (strcmp(token_str, "Keyword") == 0) {
        token.type = KEYWORD;
    } else if (strcmp(token_str, "Identifier") == 0) {
        token.type = IDENTIFIER;
    } else if (strcmp(token_str, "IntConst") == 0) {
        token.type = INT_CONST;
    } else if (strcmp(token_str, "Operator") == 0) {
        token.type = OPERATOR;
    } 
    else if (strcmp(token_str, "String") == 0) {
        token.type = STRING_CONST;
    }
    else {
        printf("\nError: Unknown token type.\n");
        exit(1);
    }
    if(token.type==STRING_CONST){
        removeQuotes(value_str);
    }
    
    strcpy(token.value, value_str);
    return token;
}

void removeQuotes(char *str) {
    char *src = str;
    char *dst = str;

    while (*src) {
        if (*src != '"') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}