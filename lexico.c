// LEXICO
#include <stdio.h>
#include <ctype.h>
#include <string.h> // strncpy
#include <stdlib.h> // atof
#include <math.h> // pow (conversao hexadecimal -> decimal)

// SINTATICO
#include <stdio.h>
#include <stdlib.h>

// DECLARACOES LEXICO 
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
    COMMENT
} TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    char atributo_numero_hex[20];
    char atributo_ID[16];
    char caracter_const;
    int numero_const;
} TInfoAtomo;

// declaracao de variaveis globais
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
TInfoAtomo reconhece_const();
TInfoAtomo reconhece_terminais();
// FIM DAS DECLARACOES DO LEXICO

// DECLARACOES SINTATICO
// variavel global
TAtomo lookahead;
TInfoAtomo info_atomo;

// SINTATICO - prototipacao de funcao
void program();
void compound_stmt();
void var_decl();
void type_specifier();
void var_decl_list();
void variable_id();
void stmt();
void assig_stmt();
void cond_stmt();
void while_stmt();
void expr();
void conjunction();
void comparison();
void relation();
void sum();
void term();
void factor();
void consome( TAtomo atomo );

// FIM DECLARACOES SINTATICO

int main(void){
    TInfoAtomo info_atm;
    do{
        info_atm = obter_atomo();

        printf("%03d# %s ", info_atm.linha,strAtomo[info_atm.atomo]);
        printf("\n");

    }while(info_atm.atomo != ERRO && info_atm.atomo != EOS);
    printf("fim de analise lexica\n");
}

// IMPLEMENTACAO LEXICO
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
        entrada++;
        info_atomo.atomo = EOS;
    } else if(*entrada == '/' && *(entrada+1) == '/'){
        entrada +=2 ;
        while (*entrada != '\n'){
            reconhece_terminais();
        }
        entrada++;
        info_atomo.atomo = COMMENT;
    } else if(*entrada == '/' && *(entrada+1) == '*'){
        entrada += 2;
        while (*entrada != '*' && *(entrada+1) == '/'){
            reconhece_terminais();
        }
        entrada += 2;
        info_atomo.atomo = COMMENT; 
    } else if(isalpha(*entrada)){
        info_atomo = reconhece_reserv_id();
    } else if(*entrada == '\'' || *entrada == '0'){
        info_atomo = reconhece_const();
    } else{
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
        entrada += 2;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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
        entrada += 3;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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
        entrada += 4;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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
        entrada += 5;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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
        entrada += 7;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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
        entrada += 8;
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
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

TInfoAtomo reconhece_const(){
    TInfoAtomo info_id;
    info_id.atomo = ERRO;
    char *ini_num;
    char hexa[16];

    if(*entrada == '\'' && isalpha(*(entrada+1)) && *(entrada+2) == '\''){
        info_id.atomo = CHARCONST;
        info_id.caracter_const = *(entrada+1);
        entrada += 3;
    } if (*entrada == '0' && *(entrada+1) == 'x'){
        entrada += 2;
        if (*entrada == 'A' ||
            *entrada == 'B' ||
            *entrada == 'C' ||
            *entrada == 'D' ||
            *entrada == 'E' ||
            *entrada == 'F' ||
            isdigit(*entrada)){
            ini_num = entrada;
            entrada++;
            int posicao = 0;
            info_id.atomo = INTCONST;
            while(*entrada == 'A' ||
                *entrada == 'B' ||
                *entrada == 'C' ||
                *entrada == 'D' ||
                *entrada == 'E' ||
                *entrada == 'F' ||
                isdigit(*entrada)){
                entrada++;
                posicao++;
            }
            strncpy(hexa,ini_num,entrada - ini_num);
            hexa[entrada - ini_num]='\0';
            int i = 0;
            int decimal = 0;
            int representacao;
            while(hexa[i] != '\0'){
                if (hexa[i] == 'A'){
                    representacao = 10;
                } else if (hexa[i] == 'B'){
                    representacao = 11;
                } else if (hexa[i] == 'C'){
                    representacao = 12;
                } else if (hexa[i] == 'D'){
                    representacao = 13;
                } else if (hexa[i] == 'E'){
                    representacao = 14;
                } else if (hexa[i] == 'F'){
                    representacao = 15;
                } else {
                    representacao = hexa[i];
                }
                decimal += representacao * pow(16,posicao);
                posicao--;
            }
            info_id.numero_const = decimal;
        }   
    }
    return info_id;
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
// FIM IMPLEMENTACAO LEXICO

// IMPLEMENTACAO SINTATICO

// <program> ::= void main ‘(‘ void ‘)’ <compound_stmt>
void program(){
    consome(VOID);
    consome(MAIN);
    consome(OPEN_PAR);
    consome(VOID);
    consome(CLOSE_PAR);
    compound_stmt();
}

// <compound_stmt> ::= ‘{‘ <var_decl> { <stmt> } ‘}’
void compound_stmt(){
    consome(OPEN_CURLY_BRACES);
    var_decl();
    while(lookahead == OPEN_CURLY_BRACES ||
          lookahead == ID ||
          lookahead == IF ||
          lookahead == WHILE ||
          lookahead == READINT ||
          lookahead == WRITEINT){
        stmt();
    }
    consome(CLOSE_CURLY_BRACES);
}

// <var_decl> ::= [ <type_specifier> <var_decl_list> ‘;’ ]
void var_decl(){
    if(lookahead == INT){
        type_specifier();
        var_decl_list();
        consome(SEMICOLON);
    }
}

// <type_specifier> ::= int | char
void type_specifier(){
    if(lookahead == INT){
        consome(INT);
    } else{
        consome(CHAR);
    }
}

// <var_decl_list> ::= <variable_id> { ‘,’ <variable_id> }
void var_decl_list(){
    variable_id();
    while(lookahead == COMMA){
        consome(COMMA);
        variable_id();
    }
}

// <variable_id> ::= id [ ‘=’ <expr> ]
void variable_id(){
    consome(ID);
    if (lookahead == ATTRIBUTION){
        consome(ATTRIBUTION);
        expr();
    }
}

/* <stmt> ::= <compound_stmt> |
              <assig_stmt> |
              <cond_stmt> |
              <while_stmt> |
              readint ‘(‘ id ‘)’ ‘;’ |
              writeint ‘(‘ <expr> ‘)’ ‘;’ |
*/
void stmt(){
    if(lookahead == OPEN_CURLY_BRACES){
        compound_stmt();
    } else if (lookahead == ID){
        assig_stmt();
    } else if (lookahead == IF){
        cond_stmt();
    } else if (lookahead == WHILE){
        while_stmt();
    } else if (lookahead == READINT){
        consome(READINT);
        consome(OPEN_PAR);
        consome(ID);
        consome(CLOSE_PAR);
        consome(SEMICOLON);
    } else{
        consome(WRITEINT);
        consome(OPEN_PAR);
        expr();
        consome(CLOSE_PAR);
        consome(SEMICOLON);
    }
}

// <assig_stmt> ::= id ‘=’ <expr> ‘;’ 
void assig_stmt(){
    consome(ID);
    consome(ATTRIBUTION);
    expr();
    consome(SEMICOLON);
}

// <cond_stmt> ::= if ‘(‘ <expr> ‘)’ <stmt> [ else <stmt> ]
void cond_stmt(){
    consome(IF);
    consome(OPEN_PAR);
    expr();
    consome(CLOSE_PAR);
    stmt();
    if (lookahead == ELSE){
        consome(ELSE);
        stmt();
    }
}

// <while_stmt> ::= while ‘(‘ <expr> ‘)’ <stmt>
void while_stmt(){
    consome(WHILE);
    consome(OPEN_PAR);
    expr();
    consome(CLOSE_PAR);
    stmt();
}

// <expr> ::= <conjunction> { ‘||’ <conjunction> }
void expr(){
    conjunction();
    while(lookahead == OR){
        consome(OR);
        conjunction();
    }
}

// <conjunction> ::= <comparison> { ‘&&’ <comparison> }
void conjunction(){
    comparison();
    while(lookahead == AND){
        consome(AND);
        comparison();
    }
}

// <comparison> ::= <sum> [ <relation> <sum> ] 
void comparison(){
    sum();
    if (lookahead == GREATER       ||
        lookahead == GREATER_EQUAL ||
        lookahead == EQUAL         ||
        lookahead == DIFFERENT     ||
        lookahead == LESS          ||
        lookahead == LESS_EQUAL){
        relation();
        sum();
    }
}

// <relation> ::= “<” | “<=” | “==” | “!=” | “>” | “>=”
void relation(){
    if (lookahead == GREATER       ||
        lookahead == GREATER_EQUAL ||
        lookahead == EQUAL         ||
        lookahead == DIFFERENT     ||
        lookahead == LESS){
        consome(lookahead);
    }else{
        consome(LESS_EQUAL);
    }
}

// <sum> ::= <term> { (‘+’ | ‘-’) <term> }
void sum(){
    term();
    while(lookahead == PLUS || lookahead == MINUS){
        consome(lookahead);
        term();
    }
}

// <term> ::= <factor> { ( ‘*’ | ‘/’ ) <factor> }
void term(){
    factor();
    while(lookahead == MULT || lookahead == DIV){
        consome(lookahead);
        factor();
    }
}

// <factor> ::= intconst | charconst | id | ‘(‘ <expr> ‘)’
void factor(){
    if (lookahead == INTCONST ||
        lookahead == CHARCONST ||
        lookahead == ID){
        consome(lookahead);
    }else{
        consome(OPEN_PAR);
        expr();
        consome(CLOSE_PAR);
    }
}

void consome( TAtomo atomo ){
    if( lookahead == atomo ){
        
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else{
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}

// FIM IMPLEMENTACAO SINTATICO