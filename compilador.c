/*
Gabriel Fuentes de Freitas Yamashita - 10408876
Pedro Akira Cardoso Toma - 10390171
*/

// LEXICO
#include <stdio.h>
#include <ctype.h>
#include <string.h> // strncpy
#include <stdlib.h> // atof
#include <math.h> // pow (conversao hexadecimal -> decimal)
#include <ctype.h> // verificar se caractere é ASCII

// SINTATICO
#include <stdio.h>
#include <stdlib.h>

// DECLARACOES LEXICO 
// definicoes dos atomos
typedef enum{
    EOS,
    ERROR,
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

// tipo de dado para atomos

typedef struct{
    TAtomo atomo;
    int linha;
    char atributo_ID[16];
    char caracter_const;
    int numero_const;
} TInfoAtomo;

// tipo de dado para armazenar nos da lista encadeada

typedef struct _TNo{
    char ID[16];
    int endereco;
    struct _TNo *prox;
}TNo;

// declaracao de variaveis globais
char *strAtomo[]={"EOS","ERROR","VOID","MAIN","INT","CHAR","ID","READINT",
"WRITEINT","IF","ELSE","WHILE","INTCONST","CHARCONST","OPEN_PAR",
"CLOSE_PAR","OPEN_CURLY_BRACES","CLOSE_CURLY_BRACES","COMMA",
"SEMICOLON","ATTRIBUTION","AND","OR","LESS","LESS_EQUAL","EQUAL",
"DIFFERENT","GREATER","GREATER_EQUAL","PLUS","MINUS","MULT","DIV",
"COMMENT"};
int contaLinha = 1;
char *entrada;

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

// INICIO DA FUNCOES DO SEMANTICO (Tabela de simbolos)

// Variaveis globais do semantico
TNo *simbolo_inicial = NULL;
int rotulo = 0;

int inserir_simbolo (char *ID){
    if (simbolo_inicial == NULL){
        TNo *inserido = (TNo*)malloc(sizeof(TNo));
        strncpy((*inserido).ID, ID, 16);
        (*inserido).prox = NULL;
        (*inserido).endereco = 0;
        simbolo_inicial = inserido;
        return 0;
    } else {
        TNo *atual = simbolo_inicial;
        TNo *anterior = NULL;
        int endereco = 0;
        while (atual != NULL){
            if (strcmp((*atual).ID,ID) == 0){
                printf("Erro semantico: ID '%s' duplicado ",ID);
                exit(1);
            }
            anterior = atual;
            atual = (*atual).prox;
            endereco++;
        }
        TNo *inserido = (TNo*)malloc(sizeof(TNo));;
        strncpy((*inserido).ID, ID, 16);
        (*inserido).prox = NULL;
        (*inserido).endereco = endereco;
        (*anterior).prox = inserido;
        return 0;
    }
}

int buscar_simbolo (char *ID){
    TNo *atual = simbolo_inicial;
    while (atual != NULL){
        if (strcmp((*atual).ID,ID) == 0){
            return (*atual).endereco;
        }
        atual = (*atual).prox;
    }
    printf("Erro semantico: ID '%s' nao existe", ID);
    exit(1);
}

void imprimir_tabela() {
    TNo *atual = simbolo_inicial;
    printf("TABELA DE SIMBOLOS\n");

    while (atual != NULL) {
        printf("%-10s | Endereco: %d\n", (*atual).ID, (*atual).endereco);
        atual = (*atual).prox;
    }
}

int proximo_rotulo(){
    rotulo++;
    return rotulo;
}

// FIM DA FUNCOES DO SEMANTICO (Tabela de simbolos)

// DECLARACAO FUNCAO MAIN (LEITURA DE ARQUIVO)
char* lerArquivo(char nome_arquivo[100]);

// FIM DECLARACAO MAIN

int main(int argc, char *argv[]){

    // le arquivo
    entrada = lerArquivo(argv[1]);

    info_atomo = obter_atomo(); // obtem atomo inicial
    
    lookahead = info_atomo.atomo; // lookahead eh o atomo inicial

    program(); // simbolo inicial da gramatica

    consome(EOS); // consome fim do arquivo

    // printf("\n");

    // printf("%d linhas analisadas, programa sintaticamente correto\n", contaLinha);
    return 0;
}

// IMPLEMENTACAO LEXICO
TInfoAtomo obter_atomo(){
    TInfoAtomo info_atomo;

    info_atomo.atomo = ERROR; // iniciar atomo com erro
    // eliminar delimitadores
    while(*entrada == ' '|| 
          *entrada == '\n'||
          *entrada == '\r'||
          *entrada == '\t'){
        if(*entrada == '\n')
            contaLinha++;
        entrada++;
    }
    // identifica fim de arquivo
    if(*entrada == '\0'){
        entrada++;
        info_atomo.atomo = EOS;
    // identifica comentario de uma linha
    } else if(*entrada == '/' && *(entrada+1) == '/'){
        info_atomo.atomo = COMMENT;
        info_atomo.linha = contaLinha;
        // printf("# %3d: %s\n", info_atomo.linha,strAtomo[info_atomo.atomo]);
        entrada +=2;

        // loop para consumir todo comentario
        while (*entrada != '\n'){
            entrada++;
        }
        entrada++;
        contaLinha++;
    // identifica comentarios de multiplas linhas
    } else if(*entrada == '/' && *(entrada+1) == '*'){
        info_atomo.atomo = COMMENT; 
        info_atomo.linha = contaLinha;
        // printf("# %3d: %s\n", info_atomo.linha,strAtomo[info_atomo.atomo]);
        entrada += 2;
        // consome atomos ate o fim do comentario */
        while (!(*entrada == '*' && *(entrada+1) == '/')){
            if (*entrada == '\n'){
                contaLinha++;
            }
            entrada++;
        }
        entrada += 2;
    // reconhece palavras reservadas e identificadores
    } else if(isalpha(*entrada)){
        info_atomo = reconhece_reserv_id();
    // reconhece constantes INTCONST
    } else if(*entrada == '0' && *(entrada+1) == 'x'){
        info_atomo = reconhece_const();
    // reconhece terminais da gramatica
    } else{
        info_atomo = reconhece_terminais();
    }

    return info_atomo; // retorna atomo com informacoes
}

TInfoAtomo reconhece_reserv_id(){
    TInfoAtomo info_reserv_id;
    char palavra[16]; // possivel atributo ID
    char *ini_num; // inicio de possive ID
    int qtd_letras = 0; 

    info_reserv_id.atomo = ERROR;

    ini_num = entrada; // comeco de possivel ID

    // se for maiuscula ou underline vai para identificador (q2)
    if(isupper(*entrada) || *entrada == '_'){
        entrada++; // consome letra maiuscula ou underline
        qtd_letras++;
        goto q2;
    }
    // se for minuscula verifica se eh palavra reservada
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 5;
            goto q2;
        }
        // verifica qual palavra reservada de 4 letras eh a correta
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
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
        // se proximo caractere for underline ou digito, vai para ID (q2)
        if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada)){
            entrada++; // consome letra maiuscula ou underline
            qtd_letras += 9;
            goto q2;
        }

        info_reserv_id.atomo = WRITEINT;

        return info_reserv_id;
    }

q2:
    // verifica se o ID passou de 15 letras
    if(qtd_letras > 15){
        return info_reserv_id;
    }
    // se proximo caractere for letra, underline ou digito continua lendo ID (q2)
    if(isalpha(*entrada) || *entrada == '_' || isdigit(*entrada) ){
        entrada++;// consome letra minuscula
        qtd_letras++;
        goto q2;
    }

    // copia ID para atributo ID
    int tamanho = entrada - ini_num;
    strncpy(info_reserv_id.atributo_ID,ini_num,tamanho);
    info_reserv_id.atributo_ID[tamanho] = '\0'; // insere '\0' ao final da palavra
    info_reserv_id.atomo = ID;
    return info_reserv_id;
}

TInfoAtomo reconhece_const(){
    TInfoAtomo info_id;
    info_id.atomo = ERROR;
    char *ini_num;
    char hexa[16];
    // verifica se eh um CHARCONST 
    // if(*entrada == '\'' && isascii(*(entrada+1)) && *(entrada+2) == '\''){
    //    info_id.atomo = CHARCONST;
    //    info_id.caracter_const = *(entrada+1);
    //    entrada += 3;
    // verifica se eh um INTCONST (hexadecimal)} 
    // if (*entrada == '0' && *(entrada+1) == 'x'){
    entrada += 2;
    // se nao for A,B,C,D,E,F, digito retorna erro
    if (*entrada == 'A' ||
        *entrada == 'B' ||
        *entrada == 'C' ||
        *entrada == 'D' ||
        *entrada == 'E' ||
        *entrada == 'F' ||
        isdigit(*entrada)){
        ini_num = entrada;
        entrada++;
        int posicao = 0; // verifica posicao para calculo posterior
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

        // pega constante INTCONST
        int tamanho = entrada - ini_num;
        strncpy(hexa,ini_num,tamanho);
        hexa[tamanho]='\0';
        int i = 0;
        int decimal = 0;
        int representacao;
        // verfica letra e sua representacao
        while(tamanho > i){
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
                representacao = hexa[i] - '0'; // subtrai ASCII de hexa[i] por ASCII de '0' para obter número
            }
            // calcula decimal
            decimal += representacao * pow(16,posicao);
            i++;
            posicao--;
        }
        info_id.numero_const = decimal;
    }   
    return info_id;
}

TInfoAtomo reconhece_terminais(){
    TInfoAtomo info_id;
    info_id.atomo = ERROR;

    // reconhce todos os terminais da gramatica
    if(*entrada == '('){
        info_id.atomo = OPEN_PAR;
        entrada++;
    } else if(*entrada == ')'){
        info_id.atomo = CLOSE_PAR;
        entrada++;
    } else if(*entrada == '{'){
        info_id.atomo = OPEN_CURLY_BRACES;
        entrada++;
    } else if(*entrada == '}'){
        info_id.atomo = CLOSE_CURLY_BRACES;
        entrada++;
    } else if(*entrada == ';'){
        info_id.atomo = SEMICOLON;
        entrada++;
    } else if(*entrada == ','){
        info_id.atomo = COMMA;
        entrada++;
    } else if(*entrada == '|' && *(entrada+1) == '|'){
        info_id.atomo = OR;
        entrada+=2;
    } else if(*entrada == '&' && *(entrada+1) == '&'){
        info_id.atomo = AND;
        entrada+=2;
    } else if(*entrada == '=' && *(entrada+1) == '='){
        info_id.atomo = EQUAL;
        entrada+=2;
    } else if(*entrada == '='){
        info_id.atomo = ATTRIBUTION;
        entrada++;
    } else if(*entrada == '>' && *(entrada+1) == '='){
        info_id.atomo = GREATER_EQUAL;
        entrada+=2;
    } else if(*entrada == '>'){
        info_id.atomo = GREATER;
        entrada++;
    } else if(*entrada == '<' && *(entrada+1) == '='){
        entrada+=2;
        info_id.atomo = LESS_EQUAL;
    } else if(*entrada == '<'){
        info_id.atomo = LESS;
        entrada++;
    } else if(*entrada == '+'){
        info_id.atomo = PLUS;
        entrada++;
    } else if(*entrada == '-'){
        info_id.atomo = MINUS;
        entrada++;
    } else if(*entrada == '*'){
        info_id.atomo = MULT;
        entrada++;
    } else if(*entrada == '/'){
        info_id.atomo = DIV;
        entrada++;
    }
    return info_id;
}
// FIM IMPLEMENTACAO LEXICO

// IMPLEMENTACAO SINTATICO

// <program> ::= void main ‘(‘ void ‘)’ <compound_stmt>
void program(){
    printf("\tINPP\n");
    consome(VOID);
    consome(MAIN);
    consome(OPEN_PAR);
    consome(VOID);
    consome(CLOSE_PAR);
    compound_stmt();
    printf("\tPARA\n\n");
    imprimir_tabela();
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
    variable_id(); // identificador
    int qtd_variaveis = 1;
    while(lookahead == COMMA){
        consome(COMMA);
        variable_id();
        qtd_variaveis++;
    }
    printf("\tAMEM %d\n", qtd_variaveis);
}

// <variable_id> ::= id [ ‘=’ <expr> ]
void variable_id(){
    inserir_simbolo(info_atomo.atributo_ID);
    int endereco = buscar_simbolo(info_atomo.atributo_ID);
    consome(ID);
    if (lookahead == ATTRIBUTION){
        consome(ATTRIBUTION);
        expr();
        printf("\tARMZ %d\n", endereco);
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
        printf("\tLEIT\n");
        consome(READINT);
        consome(OPEN_PAR);
        int endereco = buscar_simbolo(info_atomo.atributo_ID);
        printf("\tARMZ %d\n", endereco);
        consome(ID);
        consome(CLOSE_PAR);
        consome(SEMICOLON);
    } else{
        consome(WRITEINT);
        consome(OPEN_PAR);
        expr();
        consome(CLOSE_PAR);
        consome(SEMICOLON);
        printf("\tIMPR\n");
    }
}

// <assig_stmt> ::= id ‘=’ <expr> ‘;’ 
void assig_stmt(){
    int endereco = buscar_simbolo(info_atomo.atributo_ID);
    consome(ID);
    consome(ATTRIBUTION);
    expr();
    consome(SEMICOLON);
    printf("\tARMZ %d\n", endereco);
}

// <cond_stmt> ::= if ‘(‘ <expr> ‘)’ <stmt> [ else <stmt> ]
void cond_stmt(){
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    consome(IF);
    consome(OPEN_PAR);
    expr();
    consome(CLOSE_PAR);
    printf("\tDSVF L%d\n", L1);
    stmt();
    printf("\tDSVS L%d\n", L2);
    printf("L%d:\tNADA\n", L1);
    if (lookahead == ELSE){
        consome(ELSE);
        stmt();
    }
    printf("L%d:\tNADA\n", L2);
}

// <while_stmt> ::= while ‘(‘ <expr> ‘)’ <stmt>
void while_stmt(){
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    printf("L%d:\tNADA\n", L1);
    consome(WHILE);
    consome(OPEN_PAR);
    expr();
    consome(CLOSE_PAR);
    printf("\tDSVF L%d\n", L2);
    stmt();
    printf("\tDSVS L%d\n", L1);
    printf("L%d:\tNADA\n", L2);
}

// <expr> ::= <conjunction> { ‘||’ <conjunction> }
void expr(){
    conjunction();
    while(lookahead == OR){
        consome(OR);
        conjunction();
        printf("\tDISJ\n");
    }
}

// <conjunction> ::= <comparison> { ‘&&’ <comparison> }
void conjunction(){
    comparison();
    while(lookahead == AND){
        consome(AND);
        comparison();
        printf("\tCONJ\n");
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
        char comp = lookahead;
        relation();
        sum();
        if (comp == GREATER){
            printf("\tCMMA\n");
        } else if (comp == GREATER_EQUAL){
            printf("\tCMAG\n");
        } else if (comp == EQUAL){
            printf("\tCMIG\n");
        } else if (comp == DIFFERENT){
            printf("\tCMDG\n");
        } else if (comp == LESS){
            printf("\tCMME\n");
        } else {
            printf("\tCMEG\n");
        }
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
        char op = lookahead;
        consome(lookahead);
        term();
        if (op == PLUS){
            printf("\tSOMA\n");
        } else{
            printf("\tSUBT\n");
        }
    }
}

// <term> ::= <factor> { ( ‘*’ | ‘/’ ) <factor> }
void term(){
    factor();
    while(lookahead == MULT || lookahead == DIV){
        char op = lookahead;
        consome(lookahead);
        factor();
        if (op == MULT){
            printf("\tMULT\n");
        } else{
            printf("\tDIVI\n");
        }
    }
}

// <factor> ::= intconst | id | ‘(‘ <expr> ‘)’
void factor(){
    if (lookahead == INTCONST) {
        printf("\tCRCT %d\n", info_atomo.numero_const);
        consome(lookahead);
    } else if(lookahead == ID){
        int endereco = buscar_simbolo(info_atomo.atributo_ID);
        printf("\tCRVL %d\n", endereco);
        consome(lookahead);
    }else{
        consome(OPEN_PAR);
        expr();
        consome(CLOSE_PAR);
    }
}

void consome( TAtomo atomo ){
    // se for comentario obtem proximo atomo e atualiza lookahead
    while ( lookahead == COMMENT){
        info_atomo = obter_atomo();
        // consome(info_atomo.atomo);
        lookahead = info_atomo.atomo;
    }
    // se for igual ao atomo a ser consumido
    if( lookahead == atomo ){
        info_atomo.linha = contaLinha;
        // print para variaveis com valores adicionais
        // if (lookahead == ID){
        //   printf("# %3d: %s | %s\n", info_atomo.linha,strAtomo[info_atomo.atomo],info_atomo.atributo_ID);
        // } else if (lookahead == INTCONST){
        //     printf("# %3d: %s | %d\n", info_atomo.linha,strAtomo[info_atomo.atomo],info_atomo.numero_const);
        // } else if (lookahead == CHARCONST){
        //     printf("# %3d: %s | %c\n", info_atomo.linha,strAtomo[info_atomo.atomo],info_atomo.caracter_const);
        // } else{
        //     printf("# %3d: %s\n", info_atomo.linha,strAtomo[info_atomo.atomo]);
        // }
        // obtem proximo atomo e atualiza lookahead
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
        if ( lookahead == COMMENT){
            info_atomo = obter_atomo();
            lookahead = info_atomo.atomo;
        }
    }
    // se nao for o atomo a ser consumido, gera erro
    else{
        printf("# %3d: Erro sintatico: esperado [%s] encontrado [%s]\n", contaLinha, strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}

// FIM IMPLEMENTACAO SINTATICO

// IMPLEMENTACAO DE LEITURA DE ARQUIVO
char* lerArquivo(char nome_arquivo[100]){
    FILE *arquivo;

    arquivo = fopen(nome_arquivo,"r"); // abre arquivo em modo leitura

    // verifica se eh possivel ler arquivo
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo \"teste.txt\".");
        return NULL;
    }

    int tamanho_arquivo;

    fseek(arquivo, 0, SEEK_END); // vai até o final do arquivo
    tamanho_arquivo = ftell(arquivo); // retorna posição atual do ponteiro (no fim do arquivo)

    rewind(arquivo); // volta o ponteiro para o comeco do arquivo

    char* conteudo;

    // aloca espaço para o conteudo do arquivo (tamanho do arquivo + EOS)
    conteudo = (char*) malloc(tamanho_arquivo+1);

    char caractere;
    int i = 0;

    // le cada caractere ate o final do arquivo
    while((caractere = fgetc(arquivo)) != EOF){
        conteudo[i] = caractere; // guarda o caractere na string conteudo
        i++; // move uma posição na string conteudo
    }
    conteudo[i] = '\0'; // armazena EOS

    fclose(arquivo);

    return conteudo;
}

// FIM DA IMPLEMENTACAO DE LEITURA DE ARQUIVO