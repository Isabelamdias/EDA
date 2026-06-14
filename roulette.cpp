//Lista circular

#include <iostream> //new e delete() só pra c++
#include <random>
#include <ctime>
using namespace std;

#define RED     "\033[1;31m"  
#define BLACK   "\033[1;30m" 
#define WHITE   "\033[1;37m"

struct roleta{
    struct roleta *ant;
    int valor;
    int cor; // 0=verde 1=preto 2=vermelho
    struct roleta *prox;
};
typedef struct roleta *Tproleta;

bool listaVazia(Tproleta p){
    if(p != NULL){ 
        return false; 
    }else
        return true;
}

void inicializa(Tproleta *p, int valor, int cor){ //inserir
    Tproleta novo = new roleta;
    novo->valor = valor;
    novo->cor = cor;
    if(listaVazia(*p)){ //lista Vazia
        novo->prox = novo;
        novo->ant = novo;
        *p = novo;
    }else{ //lista nao vazia
        Tproleta ultimo = (*p)->ant; 
        
        novo->prox = *p;
        novo->ant = ultimo;
        ultimo->prox = novo;
        (*p)->ant = novo;
    }
}

void test(Tproleta topo){
    if(!listaVazia(topo)){
        Tproleta p = topo;
        do{
            cout << p->valor << " ";
            p = p->prox;
        }while(p != topo); //para e nao imprime o primeiro de novo
        cout << "\n";
    }else{
        cout << "Lista vazia!\n";
    }
}

void imprimeTabuleiro(int v[][2], int qnt){
    cout << "\n\n";
    for(int i = 3; i >= 1; i--){
        if(i != 2){
            cout << "   ";
        }else{
            cout << " 0 ";
        }
        for(int j = i; j < qnt; j = j + 3){
            if(v[j][1] == 1){
                cout << BLACK << " " << v[j][0] << " " << WHITE; 
            }else{
                cout << RED << " " << v[j][0] << " " << WHITE;
            }
        }
        cout << "\n";
    }
}

void ordena(int v[][2], int qnt){
    int auxVal, auxCor, j;
    for(int i = 1; i < qnt; i++){
        auxVal = v[i][0];
        auxCor = v[i][1];
        j = i - 1;
        while(j >= 0 && auxVal < v[j][0]){
            v[j + 1][0] = v[j][0];
            v[j + 1][1] = v[j][1]; 
            j--;
        }
        v[j + 1][0] = auxVal;
        v[j + 1][1] = auxCor;
    }
}

void sorte(Tproleta *p){
    int aleatorio = rand() % 37;
    Tproleta num = *p;
    int lado;
    do{
        cout << "\nGirar: ";
        cout << "\n\t1- Direita \n\t2- Esquerda";
        cout << "\nLado escolhido: ";
        cin >> lado;
    }while(lado != 1 && lado != 2);
    switch (lado)
    {
    case 1:
        for(int i = 0; i < aleatorio; i++){
            num = num->prox;
        }
        break;
    
    case 2:
        for(int i = 0; i < aleatorio; i++){
            num = num->ant;
        }
        break;
    }
    *p = num; //deixar como novo topo

}

bool validacao(int escolha, int sorteado){
    if(escolha == sorteado){
        cout << "\n\tVoce acertou!!\n";
        return true;
    }else{
        cout << "\n\tVoce errou!\n";
        return false;
    }
}

void pontuacao(int *saldo, int ponto){
    int valor = *saldo + ponto;
    if(valor < 0){
        *saldo = 0;
    }else{
        *saldo = valor;
    }
}

int menu(int saldo){
    int op = -1;
    do{
        cout << "\nSaldo: " << saldo << "\n[0] Inserir Saldo";
        cout << "\n\nDeseja apostar:\n";
        cout << "\n\t[1] Número específico (0-36)";
        cout << "\n\t[2] Cor (vermelho ou preto)";
        cout << "\n\t[3] Par ou ímpar";
        cout << "\n\t[4] Sair";
        cout << "\nOpicao escolhida: ";
        cin >> op;
    }while(op < 0 || op > 4);
    return op;
}

int main(){ //roleta europeia
    cout << "\033[1;37;48;5;22m\033[2J\033[H"; //muda o fundo

    srand(time(0));
    Tproleta topo = NULL;
    int baralho[37][2] = { // {valor, cor} 0=verde 1=preto 2=vermelho
    {0,  0},
    {32, 2}, {15, 1}, {19, 2}, {4,  1}, {21, 2}, {2,  1},
    {25, 2}, {17, 1}, {34, 2}, {6,  1}, {27, 2}, {13, 1},
    {36, 2}, {11, 1}, {30, 2}, {8,  1}, {23, 2}, {10, 1},
    {5,  2}, {24, 1}, {16, 2}, {33, 1}, {1,  2}, {20, 1},
    {14, 2}, {31, 1}, {9,  2}, {22, 1}, {18, 2}, {29, 1},
    {7,  2}, {28, 1}, {12, 2}, {35, 1}, {3,  2}, {26, 1}
};
    for(int i = 0; i < 37; i++){
        inicializa(&topo, baralho[i][0], baralho[i][1]);
    }
    ordena(baralho, 37);

    int saldo = 0;
    int valor;
    int aposta = 0;
    int escolha = -1;
    int op = menu(saldo);
    int num, cor;
    while(op != 4){
        if(op == 0){
            do{
                cout << "Quanto deseja inserir: ";
                cin >> valor;
            }while(valor <= 0);
            saldo += valor;
        }else{
            if(saldo > 0){
                imprimeTabuleiro(baralho, 37);
                do{
                    cout << "\nQuanto deseja apostar: ";
                    cin >> aposta;
                }while(aposta <= 0 || aposta > saldo);
                switch (op){
                case 1: //terminei
                    do{
                        cout << "\nEscolha o valor do Numero (0-36): ";
                        cin >> escolha;
                    }while(escolha < 0 || escolha > 36);
                    sorte(&topo);
                    num = topo->valor;
                    cout << "Numero sorteado: " << num;
                    if(validacao(escolha, num)){ //acertou
                        pontuacao(&saldo, aposta*10);
                    }else{
                        pontuacao(&saldo, -aposta);
                    }
                    break;
                case 2:
                    do{
                        cout << "\nEscolha uma cor";
                        cout << "\n\t1- Preto \n\t2- Vermelho";
                        cout << "\nOpçao escolhida: ";
                        cin >> escolha;
                    }while(escolha != 1 && escolha != 2);
                    sorte(&topo);
                    cor = topo->cor;
                    cout << "\nNumero sorteado: " << topo->valor;
                    cout << "\nCor sorteada: ";
                    if(cor == 0){
                        cout << "verde";
                    }else{
                        if(cor == 1){
                            cout << "preto";
                        }else{
                            cout << "vermelho";
                        }
                        }
                    if(validacao(escolha, cor)){ //acertou
                        pontuacao(&saldo, aposta*2);
                    }else{
                        pontuacao(&saldo, -aposta);
                    }
                    
                    break;
                case 3:
                    do{
                        cout << "\nEscolha ";
                        cout << "\n\t1- Impar \n\t2- Par";
                        cout << "\nOpçao escolhida: ";
                        cin >> escolha;
                    }while(escolha != 1 && escolha != 2);
                    sorte(&topo);
                    num = topo->valor;
                    cout << "\nNumero sorteado: " << num;
                    if(num != 0){
                        if(num%2 == 0){ //so entra se par
                            num = 2;
                        }else{
                            num = 1;
                        }
                    }
                    if(validacao(escolha, num)){ //acertou
                        pontuacao(&saldo, aposta*2);
                    }else{
                        pontuacao(&saldo, -aposta);
                    }
                    break;
                }
            }else{
                cout << "\n\tSaldo insuficiente\n";
            }
        }
        op = menu(saldo); 
    }   
    cout << "\n\nObrigada por jogar";
    cout << "\nSeu saldo foi de: " << saldo;
    return 0;
}