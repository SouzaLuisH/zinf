#include "archive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_def.h"

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

int try_open_map(int stage_no) {
    char arq_path[50] = {0};
    sprintf(arq_path, MAP_PATH_PREFIX "%d.txt", stage_no);

    FILE *arq_map = fopen(arq_path, "r");
    if (arq_map == NULL) {
        return 0;
    }
    fclose(arq_map);
    return 1;
}

void save_score(const char *name, int score) {
    TIPO_SCORE new_entry;

    strncpy(new_entry.nome, name, sizeof(new_entry.nome) - 1);
    new_entry.nome[sizeof(new_entry.nome) - 1] = '\0';
    new_entry.score = score;

    int count = 0;
    TIPO_SCORE all_scores[MAX_RANKING_ITENS] = {0};

    FILE *arq = fopen("../../resources/ranking/ranking.bin", "rb");
    if (arq) {
        for (int i = 0; i < MAX_RANKING_ITENS; i++) {
            if (fread(&all_scores[i], sizeof(TIPO_SCORE), 1, arq)) {
                count++;
            }
        }
        fclose(arq);
        // If ranking is full and new score is not high enough, do nothing
        if (count == MAX_RANKING_ITENS && new_entry.score <= all_scores[MAX_RANKING_ITENS - 1].score) {
            return;
        }

        // If ranking is not full, add new entry
        if (count < MAX_RANKING_ITENS) {
            all_scores[count] = new_entry;
            count++;
        } else {
            // Replace lowest score with new entry
            all_scores[MAX_RANKING_ITENS - 1] = new_entry;
        }
        // Sort scores
        for (int i = (count - 1); i > 0; i--) {
            if (all_scores[i].score > all_scores[i - 1].score) {
                TIPO_SCORE aux = all_scores[i - 1];
                all_scores[i - 1] = all_scores[i];
                all_scores[i] = aux;
            }
        }
    } else {
        // No file exists, create new ranking with only this entry
        all_scores[0] = new_entry;
        count = 1;
    }
    // Write only the valid entries (up to MAX_RANKING_ITENS)
    arq = fopen("../../resources/ranking/ranking.bin", "wb");
    if (arq) {
        fwrite(all_scores, sizeof(TIPO_SCORE), count, arq);
        fclose(arq);
    }
}

int get_lowest_ranking_score(int *index_score) {
    FILE *f = fopen("../../resources/ranking/ranking.bin", "rb");
    int lower_score = __INT_MAX__;
    int idx = 0;
    if (f) {
        TIPO_SCORE temp;

        while (fread(&temp, sizeof(TIPO_SCORE), 1, f) == 1) {
            if (temp.score < lower_score) {
                lower_score = temp.score;
            }
            idx++;
        }
        fclose(f);
    }
    *index_score = idx;
    return lower_score;
}

int get_ranking_info(TIPO_SCORE *scores, int lenght) {
    FILE *f = fopen("../../resources/ranking/ranking.bin", "rb");
    int cnt =0;
    if (f) {
        TIPO_SCORE temp;
       
         for (int i = 0; i < lenght; i++) {
            if (fread(&scores[i], sizeof(TIPO_SCORE), 1, f)) {
                cnt++;
            }
        }
        fclose(f);
        return cnt;
    } else {
        printf("Could not open ranking file for reading.\n");
        return -1;
    }
}
