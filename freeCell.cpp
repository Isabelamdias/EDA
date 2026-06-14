//Pilha e fila
//fazer o freecell - pilha e insercao e remocao no final 

#include <iostream> //new e delete() só pra c++
#include <random>
using namespace std;

#define RED     "\033[1;31m"  
#define BLACK   "\033[1;30m" 
#define WHITE   "\033[1;37m"
struct carta{
    string tipo;
    int nipe; //1-4
    int cor; //1 vermelho 2 preto
    struct carta *prox;
    
};
typedef struct carta *Tpcarta;

struct baralho{
   string tipo[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
   int qnt[13][4] = {
        {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, 
        {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, 
        {1,1,1,1}, {1,1,1,1}, {1,1,1,1}
    };
};
typedef struct baralho Tpbaralho;
string simbolos[] = {"♥", "♦", "♠", "♣"};

Tpcarta linha[8] = {NULL};
Tpcarta aux[4] = {NULL};
Tpcarta deck[4] = {NULL};
Tpbaralho dadosBaralho;

bool listaVazia(int valorOrigem){
    if(linha[valorOrigem] != NULL){
        return false;
    }else{
        return true;
    }
}

random_device rd;
mt19937 gen(rd());

void geraCartas(int indice){ //colocar cartas no deck inicial
    Tpcarta p;
    int cont = 6;
    if (indice < 4){
        cont++;
    }
    uniform_int_distribution<> distr_tipo(0, 12); 
    uniform_int_distribution<> distr_nipe(0, 3);  
    for (int i = 0; i < cont; i++) {
        int t, n;
        
        do {
            t = distr_tipo(gen);
            n = distr_nipe(gen);
        } while (dadosBaralho.qnt[t][n] == 0);

        dadosBaralho.qnt[t][n] = 0;

        p = new carta; 
        p->tipo = dadosBaralho.tipo[t]; 
        p->nipe = n + 1;        

        if (p->nipe <= 2) {
            p->cor = 1;
        } else {
            p->cor = 2;
        }

        p->prox = linha[indice];
        linha[indice] = p;
    }
    cout << "\n";
    
}

bool restricao(Tpcarta p, Tpcarta destino){
    int indice;
    if(p->cor != destino->cor){ //ver se são cores diferentes
        for(int i = 0; i < 13; i++){
            if(dadosBaralho.tipo[i] == p->tipo){
                indice = i;
            }
        }
        if(dadosBaralho.tipo[indice+1] == destino->tipo){ //ver se sao cartas seguidas
            return true;
        }
    }
    return false;
    
}
bool restricaoDeck(Tpcarta p, Tpcarta destino){
    if(p->nipe == destino->nipe){
        int indC, indD;
        for(int i = 0; i < 13; i++){
            if(dadosBaralho.tipo[i] == p->tipo){
                indC = i;
            }
        }
        if(dadosBaralho.tipo[indC - 1] == destino->tipo){
            return true;
        }
    }
    return false;
}
void movimentaDeck(int origem){
    if(linha[origem] == NULL){
        cout << "Coluna vazia\n";
        return;
    }

    Tpcarta p = linha[origem];
    int pos = p->nipe - 1; 
    bool permitido = false;

    if(deck[pos] == NULL){
        if(p->tipo == "A"){
            permitido = true;
        }
    } 
    else {
        int indice;

        for(int i = 0; i < 13; i++){
            if(dadosBaralho.tipo[i] == p->tipo){
                indice = i;
                break;
            }
        }

        if(dadosBaralho.tipo[indice - 1] == deck[pos]->tipo){
            permitido = true;
        }
    }

    if(permitido){
        linha[origem] = linha[origem]->prox;
        p->prox = deck[pos]; 
        deck[pos] = p;
    } else {
        cout << "Movimento invalido\n";
    }
}
void movimentaTabuleiro(int origem, int destino, int qnt){
    if(linha[origem] == NULL){ 
        cout << "Coluna vazia\n"; 
        return; 
    }
    Tpcarta p = linha[origem];
    if(qnt == 1){
        if(linha[destino] == NULL){  
            linha[origem] = linha[origem]->prox;
            p->prox = NULL;
            linha[destino] = p;
        }else if(restricao(p, linha[destino])){
            linha[origem] = linha[origem]->prox;
        
            p->prox = linha[destino];
            linha[destino] = p;
        }else{
            cout << "Movimentaçao nao permitida\n";
        }
    }else{
        Tpcarta pos = linha[origem];
        bool permitido = true;
        for(int i = 0; i < qnt - 1; i++){
            if(!restricao(pos, pos->prox)){
                permitido = false;
            }
            pos = pos->prox;
        }
        if(permitido){
            if(linha[destino] == NULL){  
                Tpcarta bloco = linha[origem];
                linha[origem] = pos->prox;
                pos->prox = NULL;
                linha[destino] = bloco;
            }else{ 
                permitido = restricao(pos, linha[destino]);
                if(permitido){
                    Tpcarta bloco = linha[origem];  
                    linha[origem] = pos->prox;      
                    pos->prox = linha[destino];     
                    linha[destino] = bloco;  
                }
            }
        }else{
            cout << "Seguimento invalido";
        }
    }
}
void movimentaAux(int op, int origem, int destino){
    Tpcarta p;
    bool permitido;
    if(op == 1){//tabuleiro para aux
            int pos = -1; //nenhuma vazia
            for(int i = 0; i < 4; i++){
                if(aux[i] == NULL){
                    pos = i; //pega a primeira casa vazia
                    break;
                }
            }
            if(pos != -1){
                p = linha[origem];
                linha[origem] = linha[origem]->prox;
                p->prox = NULL;
                aux[pos] = p;
            }else{
                cout << "Aux cheia\n";
            }
    }else{ 
        if(op == 2){//aux pra tabuleiro
            if(linha[destino] == NULL){
                p = aux[origem];
                aux[origem] = NULL;
                p->prox = NULL;
                linha[destino] = p;
            }else{
                permitido = restricao(aux[origem], linha[destino]);
                if(permitido){
                    p = aux[origem];
                    aux[origem] = NULL;
                    p->prox = linha[destino];
                    linha[destino] = p;
                }else{
                    cout << "Movimentacao invalida\n";
                }
            }
        }else{ //aux para deck
            if(aux[origem] == NULL){
            cout << "Aux vazia\n";
            }else{
                permitido = false;
                if(deck[destino] == NULL && aux[origem]->tipo == "A"){
                    permitido = true;
                }else{
                    permitido = restricaoDeck(aux[origem], deck[destino]);
                }
                if(permitido){
                    cout << "Ok\n";
                    p = aux[origem];
                    aux[origem] = NULL;
                    p->prox = deck[destino];
                    deck[destino] = p;
                }else{
                    cout << "Movimentacao invalida\n";
                }
            }
        }
    }
}

void imprimeAux(){
    for(int i = 0; i < 4; i++){
        if(aux[i] == NULL){
            cout << BLACK << "/ " << "    " << BLACK;
        }
        else{
            if(aux[i]->cor == 1){
                cout << RED << aux[i]->tipo  << simbolos[aux[i]->nipe - 1] << "    "  << RED ;
            }else{
                cout << BLACK << aux[i]->tipo  << simbolos[aux[i]->nipe - 1]  << "    " << BLACK;
            }
        }
    }
}
void imprimeDeck(){
    for(int i=0; i < 4; i++){
        if(deck[i] != NULL){
            if(deck[i]->cor == 1){
                cout << RED << deck[i]->tipo  << simbolos[deck[i]->nipe - 1]  << "    " << RED ;
            }else{
                cout << BLACK << deck[i]->tipo  << simbolos[deck[i]->nipe - 1] << "    " << BLACK;
            }
        }
        else{
            if(i < 2){
                cout << RED << simbolos[i] << "     " << RED;
            }else{
                cout << BLACK << simbolos[i] << "     " << BLACK;
            }
        }
    }
    cout << "\n";
}
void imprimirCarta(Tpcarta v){
    string cor[] = {"Vermelho", "Preto"};
    if(v->cor == 1){
        cout << RED << v->tipo  << simbolos[v->nipe - 1]  << RED ;
    }else{
        cout << BLACK << v->tipo  << simbolos[v->nipe - 1]  << BLACK;
    }
    if (v->tipo == "10") {
        cout << "   "; 
    } else {
        cout << "    ";
    }
}

int qntCartasColuna[8];
void qntCartas(Tpcarta v, int i){
    int cont = 0;
    while(v != NULL){
        cont++;
        v = v->prox;
    }
    qntCartasColuna[i] = cont;
}

void imprimirTabuleiro() {
    Tpcarta temp[8];
    
    for (int i = 0; i < 8; i++) {
        temp[i] = linha[i];
        qntCartas(temp[i], i);
    }
    int max = 0;

    for (int i = 0; i < 8; i++) {
        if (qntCartasColuna[i] > max) {
            max = qntCartasColuna[i];
        }
    }
    cout << WHITE << "1     2     3     4          1     2     3     4\n" << WHITE;
    imprimeAux();
    cout << "     ";
    imprimeDeck();
    cout << "\n";
    cout << WHITE << "     1     2     3     4     5     6     7     8\n" << WHITE;
    for (int nivel = 0; nivel < max; nivel++) {
        cout << WHITE << nivel + 1 << "    " << WHITE;
        for (int i = 0; i < 8; i++) {

            int altura = qntCartasColuna[i];
            int pos = altura - nivel - 1;

            if (pos >= 0) {

                Tpcarta p = linha[i];

                for (int k = 0; k < pos; k++) {
                    p = p->prox;
                }
                imprimirCarta(p);
            } else {
                cout << "      ";
            }
        }

        cout << "\n";
    }
}

bool fimdejogo(){
    int completo = 0;
    for(int i = 0; i < 4; i++){
        if(deck[i] != NULL && deck[i]->tipo == "K"){
            completo++;
        }
    }
    if(completo == 4){
        return true;
    }else{
        return false;
    }
}

int main(){
    cout << "\033[1;37;48;5;22m\033[2J\033[H"; //muda a cor do fundo, como nao fechei nao volta ao preto
    Tpcarta p[8];
    for(int i = 0; i < 8; i++){
        geraCartas(i);
    }
    cout << "\n\nFREECELL\n\n";
    imprimirTabuleiro();
    while(!fimdejogo()){
        int op, orin, dest, qnt;
        do{ 
            cout << WHITE <<"MENU\n" 
            << "[1] Mover para as celulas livres\n" 
            << "[2] Mover para a fundação\n"
            << "[3] Mover no tabuleiro\n";
            cin >> op;
        }while(op < 1 || op > 3);

        switch(op){
            case 1: 
                int op2;
                do{
                    cout << "Mover para as celulas livres\n"
                    << "[1] Colocar nas celulas livres\n"
                    << "[2] Tirar das celulas livres\n"
                    << "[3] Mover das celulas livres para as fundaçoes";
                    cin >> op2;
                }while(op2 < 1 || op2 > 3);
                int dest;
                switch (op2){
                    case 1:
                        do{
                            cout << "Origem: ";
                            cin >> orin;
                        }while(orin < 1 || orin > 8);
                        movimentaAux(op2, orin - 1, 0);
                    break;
                    case 2:
                        do{
                            cout << "Origem: ";
                            cin >> orin;
                        }while(orin < 1 || orin > 4);
                        do{
                            cout << "Destino: " << WHITE;
                            cin >> dest;
                        }while(dest < 1 || dest > 8);
                        movimentaAux(op2, orin - 1, dest - 1);
                    break;
                    case 3:
                        do{
                            cout << "Origem: ";
                            cin >> orin;
                        }while(orin < 1 || orin > 4);
                        do{
                            cout << "Destino: " << WHITE;
                            cin >> dest;
                        }while(dest < 1 || dest > 4);
                        movimentaAux(op2, orin - 1, dest - 1);
                    break; 
                }
            break;
            case 2:
                do{
                    cout << "Origem: ";
                    cin >> orin;
                }while(orin < 1 || orin > 8);
                movimentaDeck(orin - 1);
            break;
            case 3:
                do{
                    cout << "Origem: ";
                    cin >> orin;
                }while(orin < 1 || orin > 8);
                do{
                    cout << "Destino: " << WHITE;
                    cin >> dest;
                }while(dest < 1 || dest > 8);
                cout << "Quantidade de cartas: ";
                cin >> qnt;
                movimentaTabuleiro(orin - 1, dest - 1, qnt);
            break;
        }
        imprimirTabuleiro();
    }
    cout << "FIM DE JOGO!" << WHITE ;
    return 0;
}