
# Relatório do Projeto Zinf

## Compilando e Rodando o Projeto
Para rodar o projeto e receber instruções relacionadas a instalação de dependencias e execuçao leia o arquivo ```README.md```.

## Resumo 
Esse é um projeto final requerido como requisito de aprovação na disciplina de Algoritmos e Programação na Universidade Federal do Rio Grande do Sul (UFRGS). Este projeto é inspirado no jogo "The Legend of Zelda" (1986), um jogo de aventura RPG criado pela Nintendo. 


Este projeto foi desenvolvido através do sistema operacional Ubuntu usando a linguagem C e a biblioteca gráfica raylib. 
A estrutura inicial do projeto, contendo as bibliotecas iniciais foram adquiridas através de um fork do Quickstart raylib, diponível em: https://github.com/raylib-extras/raylib-quickstart.

Para mais informações acesse: https://github.com/SouzaLuisH/zinf

## Estruturação

A ideia de estruturação do projeto consiste no desenvolvimento da maior parte do software possivel sem o uso de bibliotecas terceiras. Dessa forma a raylib fica responsável somente pela representação grafica dos elementos que compõem o jogo, tornando qualquer alteração/atualização de bibliotecas terceiras trivial.

Portanto o projeto foi dividido em 3 grandes pastas, cada uma com suas sub pastas conforme explicado a seguir:

### **./src**
A pasta **./src** contém o arquivo ```main.c```, ele é responsavel pelo gerenciamento dos menus do jogo e também pela chamada do loop principal do jogo. Ele contém a função ```app_loop()``` responsável pela invocação de cada função e ao iniciar o jogo ela invoca a função ```game_loop()```.


### **./include**
A pasta **./include** é dividida em 2 subpastas: **./include/drivers** e **./include/game** conforme detalhado a seguir:

#### **./include/drivers**
A pasta drivers contém toda a interação com as entradas e saídas. Ela é divida em 3 partes:
-  **archive**: que contém as funções relacionadas com leitura e escrita de arquivos;
-  **graphic**: responsavel pela parte gráfica, ou seja, o desenho dos textos, sprites, animações, camera, etc.
- **keyboard**:  responsável pela aquisição das teclas pressionadas pelo usuário, tanto para controlar o personagem, quanto para navegar pelos menus.


#### **./include/game**
A pasta game tem seu arquivo ```game.c``` reponsável pelo gerenciamento geral do jogo e pelas regras de negócio da aplicação. Possui também um arquivo de defines, chamado de ```game_def.h``` que contém as definições de macros para diversas funções do jogo. Existe também o arquivo ```check.c``` que contém todas as funções e checagem do jogo como, por exemplo, chegar colisoes com a parede. Além disso é dividida em mais duas subpastas: **./include/game/player** e **./include/game/enemies** confome detalhado abaixo. 
- **player**: contém todas funções inerentes ao gerenciamento da struct que representa os dados do Player, como iniciar os dados, retornar os status de saude, pontuação, ativação da arma, etc. Além disso é responsavel pela invocação da função de desenhar o player.

- **Enemies**: contém todas funções inerentes ao gerenciamento da struct que representa os dados dos Monstros, como iniciar os dados e atualizar os dados dessas structs

### **./resources**
A pasta resources possui todos arquivos que o software interage,como imagens das texturas, os mapas do jogo e o arquivo de binario do ranking. Ele é dividido em subpastas categorizadas para melhorar a organização.

## Funcionamento

O jogo possui duas estruturas principais, ```Game_State``` e ```Player``` estruturadas da seguinte forma:

### Struct Player: 

Ela é formada por uma substructs e um enum:
 ```c 
typedef struct {
    float x;
    float y;
} Vector2D;

typedef enum { SOUTH = 0, NORTH = 1, WEST = 2, EAST = 3 } Orientation;

```
Formando uma estrutura final do seguinte formato:
```c 
typedef struct {
    Vector2D position;
    Orientation orientation;
    int score;
    int lives;
    bool hasWeapon;
    bool isWeaponActive;
    float last_damage_time;
    bool isVisible;
    bool isMoving;
} Player;
```

Nessa struct fica armazenado todos os dados referentes aos player, como vida, score, se está com a arma habilitada e etc.

### Struct Game_State: 

Ela é formada pela struct Vector2D, a mesma do player, e outras duas novas substructs:

```c
typedef struct {
    Vector2D position;
    Orientation orientation;
    int lives;
    int reward;
    bool isEnable;
    float last_change_time;
} Enemies;
```
 ```c 
typedef struct {
    Vector2D position;
    bool isEnable;
} Element;
 ```

Formando uma estrutura final do seguinte formato:
```c
typedef struct { 
    int n_walls;
    Vector2D *walls;
    int n_monsters;
    Enemies *monsters;
    int n_lives;
    Element *lives;
    int n_weapons;
    Element *weapons;
    int current_stage;
} Game_State;
```

Essa struct possui toda a informação do mapa como os dados dos inimigos, posição das armas, vidas, paredes, fase atual, etc.

## Preenchimento das Structs

Em game.c quando a função ```init_game_data``` é invocada ela le um arquivo txt e o transforma em uma matriz de caracteres, após isso invoca a função ```player_init_status```, ```get_player_initial_position``` e ```get_initial_position_of_all_elements```. As duas primeiras funções usam a struct player e a matriz de caracteres para definir a posição inicial do jogador e seus status iniciais. A terceira função lida com a inicialização da struct Game_State. 

### get_initial_position_of_all_elements
 Essa função conta quantos inimigos, paredes, vidas, e monstros há na matriz de caracteres que representa o mapa. Após isso ocorre uma alocação dinamica de memoria de acordo com a quantidade de cada item, isso garante que o jogo funcione com qualquer quantidade de qualquer elemento. Após é simplemente preenchido os itens de cada tipo conforme a necessidade de cada estrutura.


 ## Interação dos Elementos


### Checagem
O arquivo ```check.c``` contém as funções responsaveis pela verificação da interação entre os diferentes tipos de elementos do jogo. São elas:

```c
bool check_wall_colision(Vector2D position, Game_State *map);
bool check_life_colision(Vector2D position, int *life_position, Game_State *map);
bool check_weapon_colision(Vector2D position, int *weapon_position, Game_State *map);
bool check_monster_player_colision(Vector2D player_position, Vector2D monster_position);
bool check_monster_weapon_colision(Player *player, Vector2D monster_position);
bool check_win_condition(Game_State *map);
bool check_defeat_condition(Player player);
bool check_user_active_weapon(uint8_t input);
bool check_is_available_ranking(int i_score);
```
Elas somente verificam o status de algum tipo de evento, como, por exemplo, checar se dois elementos colidiram ou se o jogo atingiu uma condição de vitoria.

As funções de colisão são baseadas no algoritmo chamado AABB, usado para verificar se a area de dois blocos estão sobrepostos, usamos isso pois nossos elementos movem-se livremente pelo mapa, impossibilitando uma abordagem mais simples, como somente verificar as coordenadas (x,y) de cada elemento.

Esse algoritmo pode ser exemplificado com a função ```check_wall_colision```:

```c
bool check_wall_colision(Vector2D position, Game_State *map) {
    const float HITBOX_OFFSET = (TILE_SIZE - PLAYER_HITBOX_SIZE) / 2.0f;

    // Define a área de colisão (hitbox) do jogador, centralizada
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_walls; i++) {
        float wall_x = map->walls[i].x;
        float wall_y = map->walls[i].y;
        float wall_width = TILE_SIZE;
        float wall_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < wall_x + wall_width && player_hitbox_x + player_hitbox_width > wall_x && player_hitbox_y < wall_y + wall_height && player_hitbox_y + player_hitbox_height > wall_y) {
            return true;
        }
    }

    // Verifica colisão com as bordas do mapa
    float map_width_pixels = MAP_WIDTH * TILE_SIZE;
    float map_height_pixels = MAP_HEIGHT * TILE_SIZE;

    if (player_hitbox_x < 0 || player_hitbox_x + player_hitbox_width > map_width_pixels) return true;
    if (player_hitbox_y < 0 || player_hitbox_y + player_hitbox_height > map_height_pixels) return true;

    return false;
}

```
 O algoritmo basicamente verifica as posições das coordenadas relativas a cada elemento, retornando true ou false dependendo da sobreposição ou não dos elementos.

### Gerenciamento

 O arquivo ```game.c``` possui diversas funções handle, cada uma responsavel pela interação de um tipo de elemento do mapa e estão listadas a seguir:

 ```c
void handle_extra_lifes(Player *player, Game_State *map);
void handle_weapon_elements(Player *player, Game_State *map);
void handle_player_damage(Player *player, int value_of_damage);
void handle_player_monster_interation(Player *player, Game_State *map);
void handle_player_movement(Player *player, Game_State *map, uint8_t key_pressed);
void handle_monster_movement(Game_State *map_data);
```
As funções que servem para detectar interações simples, como pegar arma, colidir com um vida extra e etc, usam funções de check para verificar e manipular as estruturas, como ,por exemplo, a função ```handle_extra_lifes```:

```c
void handle_extra_lifes(Player *player, Game_State *map) {
    int life_position = 0;

    if (check_life_colision(player->position, &life_position, map)) {
        if (map->lives[life_position].isEnable) {
            map->lives[life_position].isEnable = false;
            increment_player_lives(player, 1);
            if (DEBUG_PRINTS) printf("\nPLAYER LIVES: %d\n", player->lives);
        }
    }
}
```
As funções de colisões mais complexas como, por exemplo, a colisão de um monstro com o player, seguem o mesmo principio, porém com algumas verificações para garantir a correta manipulação das structs de acordo com a lógica proposta pelo trabalho, como, por exemplo, a função ```handle_player_monster_interation```

```c
void handle_player_monster_interation(Player *player, Game_State *map) {
    for (int i = 0; i < map->n_monsters; i++) {
        Enemies *monster = &map->monsters[i];

        if (monster->isEnable && check_monster_player_colision(player->position, monster->position)) {
            handle_player_damage(player, 1);
        }

        if (monster->isEnable && check_monster_weapon_colision(player, monster->position)) {
            monster->isEnable = false;
            player->score += monster->reward;
        }
    }
}
```
### Movimentação do Jogador

A função ```handle_player_movement``` processa o movimento do jogador com base nas teclas pressionadas. Ela determina a direção desejada e calcula o vetor de movimento. Caso o jogador se mova na diagonal, o vetor é normalizado utilizando a distância euclidiana (dividindo os componentes por √2) para manter a velocidade constante. A função atualiza a orientação do jogador e tenta movê-lo separadamente nos eixos X e Y, verificando previamente colisões com paredes para evitar movimentos inválidos. 

### Movimentação Randomica dos Monstros

A função ```handle_monster_movement``` atualiza o movimento dos monstros no jogo. Para cada monstro, ela verifica se é hora de mudar a direção com base em um temporizador aleatório. Em seguida, calcula uma nova posição com base na direção atual e na velocidade. Se a nova posição não colide com uma parede, o monstro se move; caso contrário, o temporizador é zerado para forçar a troca de direção no próximo ciclo.


### Mudança de Fase

A função ```handle_stage_transition``` gerencia a transição entre fases do jogo. Quando o jogador vence uma fase, ela verifica se há uma próxima disponível. Se houver, os dados da nova fase são carregados; caso contrário, o jogo exibe a tela de vitória e, após confirmação do jogador, exibe o ranking final. A função utiliza um estado interno para controlar o fluxo entre jogar, exibir vitória e mostrar o ranking.


### Loop Principal do Jogo

A função ```game_loop``` representa o ciclo principal do jogo. Ela inicializa os dados do jogador e do mapa no início de uma nova fase, atualiza a lógica do jogo (como transições de fase, derrota e interação com inimigos), processa a entrada do jogador e movimentação dos personagens, além de gerenciar elementos do cenário e desenhar a interface gráfica. Todo o sistema é estruturado em torno de uma câmera que acompanha o jogador. Ao final, o loop retorna um valor de controle para o fluxo do jogo.

### Manipulação do ScoreBoard
A função ```save_score``` no arquivo ```archive.c``` gerencia o ranking de pontuações. Ela carrega os dados existentes de um arquivo binário, insere a nova pontuação caso haja espaço ou se for superior à menor registrada, e reordena os itens em ordem decrescente. Por fim, salva o ranking atualizado no arquivo, garantindo que apenas as melhores pontuações sejam mantidas.