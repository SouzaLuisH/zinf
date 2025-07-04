#include "archive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_def.h"

typedef struct tipo_score {
    char nome[20];
    int score;
} TIPO_SCORE;

int read_map_archive(char *map, char *arq_nome) {
    int i = 0, j = 0;
    char r_char = 0;
    FILE *arq_map = fopen(arq_nome, "r");
    if (arq_map == NULL) {
        if (DEBUG_PRINTS) {
            printf("ERRO! O mapa está com algum problema.");
        }
        return 1;
    }

    while ((r_char = fgetc(arq_map)) != EOF) {
        if (r_char != '\r' && r_char != '\n' && r_char != '\0') {
            if (i < MAP_WIDTH * MAP_HEIGHT) {
                *map = r_char;
                map++;
                i++;
            }
        }
    }
    fclose(arq_map);
    return 0;
}

void save_score(const char *name, int score) {
    TIPO_SCORE entry;
    strncpy(entry.nome, name, sizeof(entry.nome) - 1);
    entry.nome[sizeof(entry.nome) - 1] = '\0';
    entry.score = score;
    FILE *f = fopen("../../resources/ranking/ranking.bin", "ab");
    if (f) {
        fwrite(&entry, sizeof(TIPO_SCORE), 1, f);
        fclose(f);
    }
}

void print_ranking(void) {
    FILE *f = fopen("../../resources/ranking/ranking.bin", "rb");
    if (f) {
        printf("Ranking file contents:\n");
        TIPO_SCORE temp;
        int idx = 0;
        while (fread(&temp, sizeof(TIPO_SCORE), 1, f) == 1) {
            printf("%d: %s - %d\n", idx++, temp.nome, temp.score);
        }
        fclose(f);
    } else {
        printf("Could not open ranking file for reading.\n");
    }
}
