#include "stdio.h"
#include "raylib.h"

#define RECT_COUNT 5

int is_colors_equal (Color color1, Color color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

int move_to_zero_rect (Rectangle rects[RECT_COUNT], int index) {
    Rectangle rect_to_zero = rects[index];

    for (int i = index; i >= 1; i--) {
        rects[i] = rects[i - 1];
    }

    rects[0] = rect_to_zero;
}

int main ()
{
    InitWindow(1000, 700, "shader 1");
    SetTargetFPS(75);

    Rectangle rects[RECT_COUNT];

    for (int i = 0; i < RECT_COUNT; i++) {
        rects[i].x = 50 + i * 50;
        rects[i].y = 50 + i * 50;
        rects[i].width = 150;
        rects[i].height = 150;
    }

    Color colors[RECT_COUNT] = { RED, BLUE };

    Image img = GenImageColor((int) GetScreenWidth(), (int) GetScreenHeight(), BLANK);
    Texture2D texture = LoadTextureFromImage(img);
    Color RECTS_COLOR = WHITE;
    Rectangle *PRESSED_RECTANGLE;
    int cursor_radius = 4;
    
    while (!WindowShouldClose()) {
        Rectangle cursor = { .x = GetMouseX() - cursor_radius / 2, .y = GetMouseY() - cursor_radius / 2, .width = cursor_radius * 2, .height = cursor_radius * 2 };

        PRESSED_RECTANGLE = NULL;
        RECTS_COLOR = WHITE;
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < RECT_COUNT; i++) {
                Rectangle *rect = &rects[i];
                Vector2 mouse = { GetMouseX(), GetMouseY() };
    
                if (
                    mouse.x >= rect->x && mouse.x < rect->x + rect->width
                    && mouse.y >= rect->y && mouse.y < rect->y + rect->height
                ) {
                    PRESSED_RECTANGLE = rect;
                    move_to_zero_rect(rects, i);
                    break;
                }
            }

            if (PRESSED_RECTANGLE != NULL) {
                PRESSED_RECTANGLE->x += GetMouseDelta().x;
                PRESSED_RECTANGLE->y += GetMouseDelta().y;
                
                RECTS_COLOR = RED;
            }
        }

        ImageClearBackground(&img, BLANK);

        Color *pixels = (Color *) img.data;
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                int i = y * img.width + x;
                
                int COUNT_OF_RECTS_IN_THIS_PIXEL = 0;
                Vector2 mouse = { x, y };

                for (int j = 0; j < RECT_COUNT; j++) {
                    Rectangle *rect = &rects[j];
                    if (
                        mouse.x >= rect->x && mouse.x < rect->x + rect->width
                        && mouse.y >= rect->y && mouse.y < rect->y + rect->height
                    ) {
                        COUNT_OF_RECTS_IN_THIS_PIXEL++;
                    }
                }

                if (COUNT_OF_RECTS_IN_THIS_PIXEL % 2 != 0) {
                    pixels[i] = RECTS_COLOR;
                }
            }
        }

        UpdateTexture(texture, img.data);

        BeginDrawing();
            ClearBackground(BLANK);

            DrawTexture(texture, 0, 0, WHITE);

            DrawRectangleRec(cursor, RED);
        EndDrawing();

    }
    
    // unload
    UnloadImage(img);
    UnloadTexture(texture);

    CloseWindow();

    return 0;
}