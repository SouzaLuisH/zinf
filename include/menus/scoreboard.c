#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSCORES 5

typedef struct tipo_score
{
    char nome[20];
    int score;
} TIPO_SCORE;

void le_arquivo(TIPO_SCORE *scores, char *nome_arq)
{
    FILE *registro=fopen(nome_arq, "r");
    if(registro==NULL){
        printf("Deu algum problema no %s...", nome_arq);
        return 0;
    }
    fread(scores, sizeof(TIPO_SCORE), MAXSCORES, registro);
    fclose(registro);
}

void imprime_highscores(TIPO_SCORE *recordes)
{
    int i;
    printf("Jogador - Score \n");
    for(i=0;i<MAXSCORES;i++)
    {
        printf("%s - %d \n", recordes[i].nome, recordes[i].score);
    }
}

void escreve_arquivo(TIPO_SCORE *scores, char *nome_arq)
{
    FILE *novo=fopen(nome_arq, "w");
    fwrite(scores, sizeof(TIPO_SCORE), MAXSCORES, novo);
    fclose(novo);
}

int main(void)
{
    char arquivo[100];
    TIPO_SCORE hall_da_fama[MAXSCORES];
    TIPO_SCORE novo_placar={0};
    printf("Informe o nome do arquivo de entrada: ");
    fgets(arquivo, 100, stdin);
    int length=strlen(arquivo);

    if (arquivo[length-1]=='\n'){
        arquivo[length-1]='\0';
    }

    le_arquivo(hall_da_fama, arquivo);
    imprime_highscores(hall_da_fama);

    printf("Informe o nome de um jogador: ");
    fgets(&novo_placar.nome, 50, stdin);

    length=strlen(novo_placar.nome);
    if (novo_placar.nome[length-1]=='\n'){
        novo_placar.nome[length-1]='\0';
    }

    printf("Informe o score desse jogador: ");
    scanf("%d", &novo_placar.score);
    char ch;
    while((ch=getchar())!='\n' && (ch != EOF));
    hall_da_fama[MAXSCORES-1]=novo_placar;

    printf("Informe o nome do arquivo de saida: ");
    fgets(arquivo, 100, stdin);

    length=strlen(arquivo);
    if (arquivo[length-1]=='\n'){
        arquivo[length-1]='\0';
    }

    escreve_arquivo(hall_da_fama, arquivo);

    le_arquivo(hall_da_fama, arquivo);
    imprime_highscores(hall_da_fama);
}