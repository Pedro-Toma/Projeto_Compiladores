#include <stdio.h>
#include <ctype.h>
#include <string.h> // strncpy
#include <stdlib.h> // atof

// definicoes dos atomos
typedef enum{
    EOS,
    ERRO,
    VOID,
    MAIN,
    INT,
    CHAR,
    ID,
    READINT,
    WRITEINT,
    IF,
    ELSE,
    WHILE,
    INTCONST,
    CHARCONST,
    OPEN_PAR,
    CLOSE_PAR,
    OPEN_CURLY_BRACES,
    CLOSE_CURLY_BRACES,
    COMMA,
    SEMICOLON,
    ATTRIBUTION,
    AND,
    OR,
    LESS,
    LESS_EQUAL,
    EQUAL,
    DIFFERENT,
    GREATER,
    GREATER_EQUAL,
    PLUS,
    MINUS,
    MULT,
    DIV,
    ONE_LINE_COMMENT,
    MULT_LINES_COMMENT
} TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    char atributo_numero_hex[20];
    char atributo_ID[16];
} TInfoAtomo;

char *strAtomo[]={"EOS","ERRO","VOID","MAIN","INT","CHAR","ID","READINT",
"WRITEINT","IF","ELSE","WHILE","INTCONST","CHARCONST","OPEN_PAR",
"CLOSE_PAR","OPEN_CURLY_BRACES","CLOSE_CURLY_BRACES","COMMA",
"SEMICOLON","ATTRIBUTION","AND","OR","LESS","LESS_EQUAL","EQUAL",
"DIFFERENT","GREATER","GREATER_EQUAL","PLUS","MINUS","MULT","DIV",
"ONE_LINE_COMMENT","MULT_LINES_COMMENT"};
int contaLinha = 1;
char *entrada = "void main ( void ) {\n\twriteint(maior ; }\n";

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_reserv_id();
TInfoAtomo reconhece_terminais();

int main(void){
    TInfoAtomo info_atm;
    do{
        info_atm = obter_atomo();

        printf("%03d# %s ", info_atm.linha,strAtomo[info_atm.atomo]);
        printf("\n");

    }while(info_atm.atomo != ERRO && info_atm.atomo != EOS);
    printf("fim de analise lexica\n");
}

// implementacao da funcao
TInfoAtomo obter_atomo(){
    TInfoAtomo info_atomo;

    info_atomo.atomo = ERRO;

    // eliminar delimitadores
    while(*entrada == ' '|| 
          *entrada == '\n'||
          *entrada == '\r'||
          *entrada == '\t'){
        if(*entrada == '\n')
            contaLinha++;
        entrada++;
    }
    if(*entrada == '\0'){
        info_atomo.atomo = EOS;
    } else if(*entrada == '/' && *(entrada+1) == '/'){
        info_atomo.atomo = ONE_LINE_COMMENT;
    } else if(*entrada == '*' && *(entrada+1) == '/'){
        info_atomo.atomo = MULT_LINES_COMMENT; 
    } else if(isalpha(*entrada)){
        info_atomo = reconhece_reserv_id();
    } else {
        info_atomo = reconhece_terminais();
    }
    
    info_atomo.linha = contaLinha;
    return info_atomo;
}

TInfoAtomo reconhece_reserv_id(){
    TInfoAtomo info_reserv_id;
    char palavra[16];
    char *ini_num;
    int qtd_letras = 0;

    info_reserv_id.atomo = ERRO;

    ini_num = entrada;

    if(isupper(*entrada) || *entrada == '_'){
        entrada++; // consome letra maiuscula ou underline
        qtd_letras++;
        goto q2;
    }
    if(islower(*entrada)){
        goto q1;
    }
    return info_reserv_id; // erro lexico

q1:
    // reconhece palavras reservadas com 2 letras
    strncpy(palavra, entrada, 2);
    palavra[2] = '\0';
    if(strcmp(palavra, "if") == 0){
        for(int i = 0; i < 2; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 3;
            goto q2;
        }
        info_reserv_id.atomo = IF;
        return info_reserv_id;
    }

    // reconhece palavras reservadas com 3 letras
    strncpy(palavra, entrada, 3);
    palavra[3] = '\0';
    if(strcmp(palavra, "int") == 0){
        for(int i = 0; i < 3; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 4;
            goto q2;
        }
        info_reserv_id.atomo = INT;
        return info_reserv_id;
    }

    // reconhece palavras reservadas com 4 letras
    strncpy(palavra, entrada, 4);
    palavra[4] = '\0';
    if(strcmp(palavra, "char") == 0 || strcmp(palavra, "else") == 0 || 
       strcmp(palavra, "main") == 0 || strcmp(palavra, "void") == 0){
        for(int i = 0; i < 4; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 5;
            goto q2;
        }
        if(strcmp(palavra, "char") == 0){
            info_reserv_id.atomo = CHAR;
        } else if(strcmp(palavra, "else") == 0){
            info_reserv_id.atomo = ELSE;
        } else if(strcmp(palavra, "main") == 0){
            info_reserv_id.atomo = MAIN;
        } else if(strcmp(palavra, "void") == 0){
            info_reserv_id.atomo = VOID;
        }

        return info_reserv_id;
    }

    // reconhece palavras reservadas com 5 letras
    strncpy(palavra, entrada, 5);
    palavra[5] = '\0';
    if(strcmp(palavra, "while") == 0){
        for(int i = 0; i < 5; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 6;
            goto q2;
        }

        info_reserv_id.atomo = WHILE;

        return info_reserv_id;
    }

    // reconhece palavras reservadas com 7 letras
    strncpy(palavra, entrada, 7);
    palavra[7] = '\0';
    if(strcmp(palavra, "readint") == 0){
        for(int i = 0; i < 7; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 8;
            goto q2;
        }

        info_reserv_id.atomo = READINT;

        return info_reserv_id;
    }

    // reconhece palavras reservadas com 8 letras
    strncpy(palavra, entrada, 8);
    palavra[8] = '\0';
    if(strcmp(palavra, "writeint") == 0){
        for(int i = 0; i < 8; i++){
            entrada++;
        }
        if(isalpha(*entrada) || *entrada == '_'){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 9;
            goto q2;
        }

        info_reserv_id.atomo = WRITEINT;

        return info_reserv_id;
    }

q2:
    if(qtd_letras > 15){
        return info_reserv_id;
    }
    if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada) ){
        entrada++;// consome letra minuscula
        qtd_letras++;
        goto q2;
    }
    palavra[entrada - ini_num] = '\0';
    strncpy(info_reserv_id.atributo_ID,ini_num,entrada - ini_num);

    info_reserv_id.atomo = ID;

    return info_reserv_id;
}

TInfoAtomo reconhece_terminais(){
    TInfoAtomo info_id;
    info_id.atomo = ERRO;

    if(*entrada == '(')
        info_id.atomo = OPEN_PAR;
    if(*entrada == ')')
        info_id.atomo = CLOSE_PAR;
    if(*entrada == '{')
        info_id.atomo = OPEN_CURLY_BRACES;
    if(*entrada == '}')
        info_id.atomo = CLOSE_CURLY_BRACES;
    if(*entrada == ';')
        info_id.atomo = SEMICOLON;
    if(*entrada == ',')
        info_id.atomo = COMMA;
    if(*entrada == '|' && *(entrada+1) == '|'){
        entrada++;
        info_id.atomo = OR;
    }
    if(*entrada == '&' && *(entrada+1) == '&'){
        entrada++;
        info_id.atomo = AND;
    }
    if(*entrada == '=' && *(entrada+1) == '='){
        entrada++;
        info_id.atomo = EQUAL;
    }
    if(*entrada == '=')
        info_id.atomo = ATTRIBUTION;
    if(*entrada == '>' && *(entrada+1) == '='){
        entrada++;
        info_id.atomo = GREATER_EQUAL;
    }    
    if(*entrada == '>')
        info_id.atomo = GREATER;
    if(*entrada == '<' && *(entrada+1) == '='){
        entrada++;
        info_id.atomo = LESS_EQUAL;
    }    
    if(*entrada == '<')
        info_id.atomo = LESS;
    if(*entrada == '+')
        info_id.atomo = PLUS;
    if(*entrada == '-')
        info_id.atomo = MINUS;
    if(*entrada == '*')
        info_id.atomo = MULT;
    if(*entrada == '/')
        info_id.atomo = DIV;
    entrada++;

    return info_id;
}