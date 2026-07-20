
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main () {
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int window_width = 800;
	int window_height = 450;

	const int camera_width = 160;
	const int camera_height = 90;

	Camera2D camera = {0};
	camera.zoom = 1.0f;
	camera.offset = (Vector2){ window_width / 2, window_height / 2 };

	RenderTexture2D application_surface = LoadRenderTexture(camera_width, camera_height);


	// Create the window and OpenGL context
	InitWindow(window_width, window_height, "Hello Raylib");

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

		camera.target = position;

		// drawing
		BeginDrawing();
		ClearBackground(MAGENTA);
		BeginMode2D(camera);


		//DrawTexture(the_batter, position, 100, WHITE);

		// Setup the back buffer for drawing (clear color and depth buffers)
		//ClearBackground(BLACK);
		DrawRectangle((int)position.x, (int)position.y, 32, 32, RED);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		EndMode2D();
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
