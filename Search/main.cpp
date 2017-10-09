#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <pthread.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <vector>
// #include <pair>

int TAM_CELULA = 15;
int TAM_BORDA = 15;
int x_Dimension = 42;
int y_Dimension = 42;

int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
int **matrix;
std::vector< std::vector< std::pair<int, std::vector< std::pair< int,int > > > > > matrix_mirror;
// int **matrix_mirror;

bool inicializar();
void matrix_draw_loading();
void matrix_draw();
void read_keyboard();
void draw_way(std::pair<int,int> steep);
void read_map();
std::vector<std::pair<int,int>> bfs(std::pair<int,int> a, std::pair<int,int> b);
std::vector<std::pair<int,int>> search_way_teste();


bool run = true;

int color[4][3] = {
                    {137, 168, 0  }, // grass
                    {91 , 42 , 8  }, // mountain
                    {0  , 61 , 114}, // swamp
                    {226, 95 , 2  }  // fire
                  };

int main(int argc, char const *argv[]){
    srand( (unsigned)time(NULL) );
    inicializar();
    read_map();
    
    std::pair<int,int> a;
    a.first  = 22;
    a.second = 15;
    std::pair<int,int> b;
    b.first  = 23;
    b.second = 25;
    std::vector<std::pair<int,int>> way = bfs(a,b);

    while(run){
        read_keyboard();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "oiee");
        matrix_draw();
        for(int i=0; i<way.size();i++){
            draw_way(way[i]);
        }
        al_flip_display();
    }

    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}

void print_stage(){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "oiee");
    matrix_draw_loading();
    al_flip_display();
}

std::vector<std::pair<int,int>> bfs(std::pair<int,int> a, std::pair<int,int> b){
    std::vector<std::pair<int,int>> to_do;
    to_do.push_back(a);
    matrix_mirror[a.first][a.second].first = -1;
    while(true){
        std::vector<std::pair<int,int>> new_to_do;
        for(int i=0;i<to_do.size();i++){
            // if(matrix_mirror[to_do[i].first][to_do[i].second].first == -1){
                // continue;
            // }
            std::vector<std::pair<int,int>> way = matrix_mirror[to_do[i].first][to_do[i].second].second;
            way.push_back(to_do[i]);
            if(to_do[i].first == b.first && to_do[i].second == b.second){
                return way;
            }
            // Up
            if(to_do[i].first-1>=0){
                std::pair< int, int> aux;
                aux.first = to_do[i].first-1;
                aux.second = to_do[i].second;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);

                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                }
            }
            print_stage();
            // Rigth
            if(to_do[i].second+1<42){
                std::pair< int, int> aux;
                aux.first = to_do[i].first;
                aux.second = to_do[i].second+1;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);

                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                }
            }
            print_stage();
            // Left
            if(to_do[i].second-1>=0){
                std::pair< int, int> aux;
                aux.first = to_do[i].first;
                aux.second = to_do[i].second-1;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);
                    
                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                }
            }
            print_stage();
            // Down
            if(to_do[i].first+1<42){
                std::pair< int, int> aux;
                aux.first = to_do[i].first+1;
                aux.second = to_do[i].second;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);
                    
                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                }
            }
            print_stage();
        }
        to_do = new_to_do;
    }
    return to_do;
}

std::vector<std::pair<int,int>> search_way_teste(){
    std::vector<std::pair<int,int>> way;
    std::pair<int,int> aux;
    aux.first  = 1;
    aux.second = 1;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 1;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 2;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 3;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 4;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 4;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 5;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 3;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 4;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 5;
    aux.second = 6;
    way.push_back(aux);

    return way; 
}

void read_map(){
    matrix = (int**) malloc (sizeof(int*)*x_Dimension);
    for(int i = 0; i<x_Dimension;i++){
        matrix[i] = (int*) malloc (sizeof(int)*y_Dimension);
    }
    FILE *in = fopen("mapa","r");
    for(int i = 0; i<x_Dimension; i++){
        for(int j = 0; j<y_Dimension; j++){
            matrix[i][j] = fgetc(in) - 48;
            fgetc(in);
        }
    }
    for(int i = 0; i<x_Dimension; i++){
        std::vector< std::pair< int,std::vector< std::pair< int,int > > > > aux;
        for(int j = 0; j<y_Dimension; j++){
            std::pair<int,std::vector< std::pair< int,int > > > aux2;
            aux2.first = matrix[i][j];
            aux.push_back(aux2);
        }
        matrix_mirror.push_back(aux);
    }
}

void read_keyboard(){
    while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                run = false;
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
        }
    }
}

void matrix_draw_loading(){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            if(matrix_mirror[i][j].first != -1){
                al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(color[matrix_mirror[i][j].first][0], color[matrix_mirror[i][j].first][1], color[matrix_mirror[i][j].first][2]));
                // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
            }
        }
    }
}

void matrix_draw(){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
            // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
        }
    }
}

void draw_way(std::pair<int,int> steep){
    int i;
    // al_draw_filled_rectangle(TAM_BORDA+(steep.first*TAM_CELULA), TAM_BORDA+(steep.second*TAM_CELULA),TAM_BORDA+((steep.first+1)*TAM_CELULA), TAM_BORDA+((steep.second+1)*TAM_CELULA), al_map_rgb(0, 0, 0));
    al_draw_filled_circle(TAM_BORDA+(steep.second*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(steep.first*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(160, 0, 128));
}

bool inicializar(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return false;
    }

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Formigueiro");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fonte = al_load_font("Font.ttf", 15, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}
