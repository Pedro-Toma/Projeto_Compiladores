Participantes:

Gabriel Fuentes de Freitas Yamashita - 10408876
Pedro Akira Cardoso Toma - 10390171

Processo de Desenvolvimento:

Começamos vendo as aulas dos respectivos analisadores léxico e sintático e lemos os slides, e também nos baseamos nos arquivos disponibilizados pelo professor na pasta zipada "projeto".
Implementamos uma função para a leitura do arquivo teste.txt, que abre o mesmo em modo de leitura e o ponteiro que indica o seu índice o percorre até o final, para saber a quantidade de caracteres que o arquivo possuí, o ponteiro a partir de rewind volta ao seu inicio, alocando o espaço de memória para uma variável string armazenar todo o conteúdo e assim dar continuidade para armazena-lo.
O analisador léxico possui uma função principal obter_atomo() que serve para eliminar os delimitadores e fazer as atualizações necessárias para as variáveis de 'entrada' e 'contaLinha', e também identifica os comentários de múltiplas linhas e de uma linha, essa função também chama outras 3 funções, para reconhecer terminais, palavras reservadas, identificadores e contestantes charconst e intconst.
Nós identificamos como átomos para o analisador léxico: EOS, ERROR, COMMENT e os caracteres que representam os terminais da gramática.
Já para o analisador sintático, foram implementadas as funções que possuem chamadas de funções dentro de si, com o intuito de reconhecer as sequências corretas pertinentes a lógica do conteúdo disponibilizado por meio dos átomos, pelo analisador léxico, através da gramatica EBNF disponibilizada pelo pdf do projeto.  

Principais Dificuldades:

Na implementação da função consome(), tivemos dificuldade no momento de ignorar o conteúdo que ficava entre os comentários de múltiplas linhas e de linhas únicas, pois em muitos testes percebemos que o tratamento o qual estávamos fazendo travava o resto da execução da análise sintática do arquivo, já que não havíamos reparado na atribuição incorreta do lookahead a qual havíamos feito.
E tratar comentários no meio do código, também por conta da função consome.

Como rodar:

possuir a linguagem c instalada na maquina e compilar no terminal do seu desktop o seguinte comando

gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
./compilador