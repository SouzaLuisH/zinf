#ifndef ARCHIVE_C
#define ARCHIVE_C

int try_open_map(int stage_no);
int read_map_archive(char *map, char *arq_nome);
void save_score(const char *name, int score);

#endif