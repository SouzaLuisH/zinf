
#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir



/*
    Pour faire:
	Je besoin quelques modules:
	 - Un pour lire les fichers texte de la carte;
	 - Un pour interprète le clés du keyboard;
	 - Un pour imprimer le éléments au display;
	 - Un pour contrôler le ennemis position;
	 - Un pour validè le logiquè du jeu;
	 - Un pour validè le phase de jeu;
	 - Un pour lire les fichers binaries pour ranking du joueurs.


	 Je peux commecer le project avec le plus simple module.
	 Je vous commecer avec le module pour interprète le clés du keyboard, suivant
	 Je vous implementè le module pour imprimer les informations au display.

	Quand je terminé cette modules, je peux créer les images e textures the le project besoin pour
	être beau et amusant. Je vous utilisèr le software que ce t'appel Figma.


*/




int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Hello Raylib");

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	int i = 0;
	while (!WindowShouldClose())		
	{
		i++;
		BeginDrawing();

		ClearBackground(BLACK);
		if(i<=60){
			DrawRectangle(500, 400, 25,25, GREEN);

		}
		else{
			DrawRectangle(500, 400, 20,20, WHITE);
		}
		
		if(i==120) i=0;

		EndDrawing();
	}


	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}
