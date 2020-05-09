/****************************************************************
    Nome: Matheus Barbosa Silva
    NUSP: 11221741

    Fonte e comentários: foram utilizadas funcoes da biblioteca
    math.h para implementar o calculo do desvio padrao e estrutu
    rar as classes da tabela de visitas por tempo de permanencia.
    COLUNAS:
    [0] -> NUSP, [1] -> hora, [2] -> minuto, [3] -> tempo de perm.

****************************************************************/
#include<stdio.h>
#include<math.h>
#define MAX 100

void ordenaBubbleSort(int vet[][4], int n, int indice_busca, int ehDecresc){
    int i = 0, alt = 1, aux_nusp, aux_hor, aux_min, aux_perm;;
    while(alt == 1 && n > 1){
        alt = 0;
        for( i = 0; i < n - 1; i++ ){
            if( (!ehDecresc && vet[i][indice_busca] > vet[i+1][indice_busca]) || (ehDecresc && vet[i][indice_busca] < vet[i+1][indice_busca])){
                aux_nusp = vet[i][0];
                aux_hor = vet[i][1];
                aux_min = vet[i][2];
                aux_perm = vet[i][3];

                vet[i][0] = vet[i+1][0];
                vet[i][1] = vet[i+1][1];
                vet[i][2] = vet[i+1][2];
                vet[i][3] = vet[i+1][3];

                vet[i+1][0] = aux_nusp;
                vet[i+1][1] = aux_hor;
                vet[i+1][2] = aux_min;
                vet[i+1][3] = aux_perm;
                alt = 1;
            }
        }
        n--;
    }
}

int contaUsuarios( int vet[][4], int numUsuarios){
    //funciona a partir da matriz ordenada pelo nusp
    int nuspAnt = vet[0][0], quantUsuarios = 1, i;
    for(i = 1; i < numUsuarios; i++){
        if(vet[i][0] != nuspAnt){
            nuspAnt = vet[i][0];
            quantUsuarios++;   
        }    
    }
    return quantUsuarios;
}

double calculaMediaTempPerm(int usuarios[][4], int n){
    int i;
    double soma;
    for(i = 0; i < n; i++){
        soma += usuarios[i][3];
    }
    return (soma/n);
}

double calculaDesvioPadrao(int usuarios[][4], int n, double media){
    int i;
    double desvio, somap = 0;
    for( i = 0; i < n; i++){
        somap += (usuarios[i][3]-media)*(usuarios[i][3]-media);
    }

    desvio = sqrt(somap/n);
    return desvio;
}

void insereUsuario(int usuarios[][4], int *n, int nusp_id, int hor, int min){
    usuarios[*n][0] = nusp_id;
    usuarios[*n][1] = hor;
    usuarios[*n][2] = min;
    *n = *n + 1; 
}

int busca(int usuarios[][4], int n, int nusp_busca){
    int indice_busca = -1, i;
    for(i = 0; i <  n; i++)
        if (usuarios[i][0] == nusp_busca)
            indice_busca = i;
    return indice_busca;
}

void imprimeMatriz (int usuarios[MAX][4], int qtdUsuarios){
    int i;
    printf("\nNUSP\t  Hora de Entrada\tTempo de Permanencia (minutos)\n");
    for(i = 0; i < qtdUsuarios; i++){
        printf("%-12d %02d:%02d \t\t %d\n", usuarios[i][0], usuarios[i][1], usuarios[i][2], usuarios[i][3]);
    }
}

void imprimeHistograma (int usuarios[MAX][4], int totalUsuarios){
    int horaAtual, quantUsuarios = 0, j = 0, i;
    printf("\n***HISTOGRAMA (QUANT. DE USUARIOS POR HORA)***\n");
    for (horaAtual = 0; horaAtual < 24; horaAtual++){
        printf("%02dh: |",horaAtual);
        
        //while para inserir os usuarios que entraram na hora atual
        while(usuarios[j][1] <= horaAtual && j< totalUsuarios){
            quantUsuarios++;
            j++;
        }

        //for para retirar os usuarios que sairam na hora anterior
        for (i = 0; i < totalUsuarios && horaAtual > usuarios[i][1]; i++){
            if( (usuarios[i][3] + usuarios[i][1]*60 + usuarios[i][2])/60 == horaAtual - 1){
                quantUsuarios--;
            }
        }

        for( i = 0; i < quantUsuarios; i++){
            printf("*");
        }
        printf("\n");
    }
}

void visitasPorMinuto(int usuarios[][4], int quantUsuarios){
    int amp, i, j = 0, quant = 0, min, max;
    double k;
    min = usuarios[0][3];
    max = usuarios[quantUsuarios-1][3];

    printf("\n***VISITAS POR TEMPO DE PERMANENCIA***\n\n");
    printf("INTERVALO (MINUTOS) \t QUANT. DE USUARIOS\n");
    k = ceil(1 + 3.3 * log10(quantUsuarios));
    amp = ceil((max - min)/k);
    for( i = 0; i < k; i++){
        while (j < quantUsuarios && usuarios[j][3] <= min + amp){
            if( (usuarios[j][3] >= min && usuarios[j][3] < min + amp) || (j == quantUsuarios - 1 && usuarios[j][3] <= min + amp )){
                quant++;
            }
            j++;
        }
        if( j == quantUsuarios){
            printf("%4d |--| %4d \t\t %3d \n", min, min+amp, quant);
        }else{
            printf("%4d |--  %4d \t\t %3d\n", min, min+amp, quant);
        }
        
        min += amp;
        quant = 0;
    }

}

int main(){
    FILE * arquivo_de_entrada;
    int entraousai, nusp, horas, minutos, usuarios[MAX][4], num_usuarios = 0, i, quantUsuariosDif = 0;
    int posEntradaUsuario, horInicial, minInicial, executar = 1;
    double mediaTempPerm = 0, desvioPadrao = 0;
    char nome_arq[80];

    printf("Bem-vindo ao Sistema Estatı́stico de Uso do Salão de Estudos\n");
    printf("Biblioteca 'Carlos Benjamim de Lyra' - IME-USP\n");

    while(executar == 1){
        for (i = 0; i < MAX; i++){
            usuarios[i][0] = 0;
            usuarios[i][1] = 0;
            usuarios[i][2] = 0;
            usuarios[i][3] = 0;
            num_usuarios = 0;
        }
    
        printf("Digite o nome do arquivo de dados: ");
        scanf("%s", &nome_arq);

        arquivo_de_entrada = fopen(nome_arq, "r");
        while (!feof(arquivo_de_entrada)){
            if (fscanf(arquivo_de_entrada, "%d %d %d:%d", &entraousai, &nusp, &horas, &minutos) != 4) continue;
            if(entraousai == 0){
                insereUsuario(usuarios, &num_usuarios, nusp, horas, minutos);
            }else{
                posEntradaUsuario = busca(usuarios, num_usuarios, nusp);
                if(posEntradaUsuario == -1){
                    //entrou no dia anterior e saiu no dia atual, não encontrado na matriz (conta a partir de 00:00h)
                    insereUsuario(usuarios, &num_usuarios, nusp, 0, 0);
                    posEntradaUsuario = num_usuarios - 1;
                }
                horInicial = usuarios[posEntradaUsuario][1];
                minInicial = usuarios[posEntradaUsuario][2];
                usuarios[posEntradaUsuario][3] = (horas-horInicial)*60 + (minutos - minInicial);            
            }
        }

        for( i = 0; i < num_usuarios; i++){
            if(usuarios[i][3] == 0){
                usuarios[i][3] = (23-usuarios[i][1])*60 + (59 - usuarios[i][2]);
            }
        }

        //ordenacao crescente pelo nusp
        ordenaBubbleSort(usuarios, num_usuarios, 0, 0);
        mediaTempPerm = calculaMediaTempPerm(usuarios, num_usuarios);
        desvioPadrao = calculaDesvioPadrao(usuarios, num_usuarios, mediaTempPerm);

        quantUsuariosDif = contaUsuarios(usuarios, num_usuarios);

        printf("\nNO PERIODO TIVEMOS %d USUARIOS DIFERENTES E %d VISITAS\n", quantUsuariosDif, num_usuarios);
        printf("A MEDIA DE PERMANENCIA NO SALAO FOI DE %.2lf MINUTOS COM DESVIO PADRAO DE %.2lf\n\n", mediaTempPerm, desvioPadrao);

        printf("***LISTA DE USUARIOS DO SALAO DE LEITURA***\n");
        imprimeMatriz(usuarios, num_usuarios);

        //ordenacao decrescente pelo tempo de perm
        ordenaBubbleSort(usuarios, num_usuarios, 3, 1);
        printf("\n***VISITAS ORDENADAS PELO TEMPO DE PERMANENCIA***\n");
        imprimeMatriz(usuarios, num_usuarios);

        //ordenação crescente pela hora
        ordenaBubbleSort(usuarios, num_usuarios, 1, 0);
        imprimeHistograma(usuarios, num_usuarios);

        //ordenacao crescente pelo tempo de perm
        ordenaBubbleSort(usuarios, num_usuarios, 3, 0);
        visitasPorMinuto(usuarios, num_usuarios);

        printf("\nDeseja executar novamente? (1 - SIM, 0 - NÃO) \n");
        scanf("%d", &executar);
    }
}