
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main () {
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// Load a texture from the resources directory
	Texture the_batter = LoadTexture("spr_batter_down.png");
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Vector2 position = {0};
	float speed = 1.0f;

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		const bool up = IsKeyDown(KEY_UP);
		const bool down = IsKeyDown(KEY_DOWN);
		const bool left = IsKeyDown(KEY_LEFT);
		const bool right = IsKeyDown(KEY_RIGHT);

		const int horizontal_direction = right - left;
		const int vertical_direction = down - up;

		position.x += horizontal_direction * speed;
		position.y += vertical_direction * speed;

		// drawing
		BeginDrawing();

		DrawRectangle((int)position.x, (int)position.y, 32, 32, RED);

		//DrawTexture(the_batter, position, 100, WHITE);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(the_batter);
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
