#ifndef ARCHIVE_H
#define ARCHIVE_H

typedef struct tipo_score {
  char nome[20];
  int score;
} TIPO_SCORE;

int try_open_map(int stage_no);
int read_map_archive(char *map, char *arq_nome);
void save_score(const char *name, int score);
int get_lowest_ranking_score(int *index_score);
int get_ranking_info(TIPO_SCORE *scores, int lenght);

#endif