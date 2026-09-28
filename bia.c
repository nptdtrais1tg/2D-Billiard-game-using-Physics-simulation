#include <raylib.h>
#include <raymath.h> 
#include <math.h>

#define MAX_BALLS 16 

typedef struct {
    Vector2 pos;
    Vector2 speed;
    float radius;
    Color color;
    bool inHole; 
} Ball;

int main(void) {
    // box: start
    InitWindow(800, 450, "billiards flowchart 1:1"); 
    
    Ball balls[MAX_BALLS];
    Vector2 holes[6] = {
        {15, 15}, {400, 10}, {785, 15},   
        {15, 435}, {400, 440}, {785, 435} 
    };

    float friction = 0.985f;
    float restitution = 1.0f; 
    float k = 0.1f; 
    
    // box: initialize 1 cue ball and 15 other balls arranged in the standard triangle; i = true
    balls[0] = (Ball){{ 200, 225 }, { 0, 0 }, 15, RAYWHITE, false}; 
    int idx = 1;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col <= row; col++) {
            if (idx < MAX_BALLS) {
                balls[idx] = (Ball){{ 550.0f + row * 26, 225.0f - (row * 15) + (col * 30) }, { 0, 0 }, 15, RED, false};
                if (idx == 8) balls[idx].color = BLACK; 
                idx++;
            }
        }
    }

    bool i_turn = true; 
    bool reset_cue = false;
    
    int p1_points = 0;
    int p2_points = 0;
    int check = 16; 
    int gameOver = 0; 

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        if (check == 16 && gameOver == 0) {
            
            // box: reset_cue == true?
            if (reset_cue) {
                // box: player places the cue ball
                balls[0].pos = (Vector2){ 200, 225 };
                balls[0].speed = (Vector2){ 0, 0 };
                balls[0].inHole = false;
                reset_cue = false;
            }

            // box: player i turn
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // box: calculate the velocity vector by the cursor drag: v = k * drag_dist
                Vector2 mouse = GetMousePosition();
                balls[0].speed.x = (balls[0].pos.x - mouse.x) * k;
                balls[0].speed.y = (balls[0].pos.y - mouse.y) * k;
                check = 0; 
            }
        } 
        else if (gameOver == 0) {
            
            // box: move the cue ball, including friction factor
            for (int idx = 0; idx < MAX_BALLS; idx++) {
                if (balls[idx].inHole) continue;
                balls[idx].pos.x += balls[idx].speed.x;
                balls[idx].pos.y += balls[idx].speed.y;
                balls[idx].speed.x *= friction;
                balls[idx].speed.y *= friction;

                // box: cue ball meets wall?
                // box: change direction: v_new = -v * restitution
                if (balls[idx].pos.x >= 800 - balls[idx].radius) { balls[idx].pos.x = 800 - balls[idx].radius; balls[idx].speed.x *= -restitution; }
                if (balls[idx].pos.x <= balls[idx].radius)       { balls[idx].pos.x = balls[idx].radius;       balls[idx].speed.x *= -restitution; }
                if (balls[idx].pos.y >= 450 - balls[idx].radius) { balls[idx].pos.y = 450 - balls[idx].radius; balls[idx].speed.y *= -restitution; }
                if (balls[idx].pos.y <= balls[idx].radius)       { balls[idx].pos.y = balls[idx].radius;       balls[idx].speed.y *= -restitution; }
            }

            // box: loops (a=0; b=a+1; a==14; b==14)
            for (int a = 0; a <= 15; a++) {
                if (balls[a].inHole) continue;
                for (int b = a + 1; b <= 15; b++) { 
                    if (balls[b].inHole) continue;
                    float dist = Vector2Distance(balls[a].pos, balls[b].pos);
                    // box: ball a meets ball b? / cue ball meets ball a?
                    if (dist <= (balls[a].radius + balls[b].radius)) {
                        // box: calculate v_cue, v_a, v_b after collision
                        Vector2 temp = balls[a].speed;
                        balls[a].speed = balls[b].speed;
                        balls[b].speed = temp;
                        
                        float overlap = 0.5f * (balls[a].radius + balls[b].radius - dist);
                        balls[a].pos.x += overlap * (balls[a].pos.x - balls[b].pos.x) / dist;
                        balls[a].pos.y += overlap * (balls[a].pos.y - balls[b].pos.y) / dist;
                        balls[b].pos.x -= overlap * (balls[a].pos.x - balls[b].pos.x) / dist;
                        balls[b].pos.y -= overlap * (balls[a].pos.y - balls[b].pos.y) / dist;
                    }
                }
            }

            // box: loops (m=0; n=0; m==14; n==5)
            for (int m = 0; m <= 15; m++) {
                if (balls[m].inHole) continue;
                for (int n = 0; n <= 5; n++) {
                    // box: ball m in hole n?
                    if (CheckCollisionPointCircle(balls[m].pos, holes[n], 25)) { 
                        // box: 8 ball? / calculate point accordingly
                        if (m == 8) {
                            // box: switch turn: i = !i and win/lose conditions
                            int currentPoints = i_turn ? p1_points : p2_points;
                            if (currentPoints == 7) gameOver = 1; 
                            else gameOver = 2;                    
                        } else {
                            // box: remove ball m from table
                            balls[m].inHole = true;
                            balls[m].pos = (Vector2){ -100, -100 };
                            balls[m].speed = (Vector2){ 0, 0 };
                            
                            if (m != 0) { 
                                if (i_turn) p1_points++; 
                                else p2_points++; 
                            }

                            // box: cue ball in hole? -> reset_cue = true
                            if (m == 0) reset_cue = true;
                        }
                    }
                }
            }

            // box: loops (f=0; check=0; f==16) and v_f == 0?
            check = 0;
            for (int f = 0; f <= 15; f++) {
                if (balls[f].inHole || (fabs(balls[f].speed.x) < 0.05f && fabs(balls[f].speed.y) < 0.05f)) {
                    balls[f].speed = (Vector2){0, 0}; 
                    check += 1;
                }
            }

            if (check == 16) {
                i_turn = !i_turn;
            }
        }

        // box: render
        BeginDrawing();
            ClearBackground(DARKGREEN);
            
            for (int n = 0; n <= 5; n++) DrawCircleV(holes[n], 25, BLACK);
            
            for (int i = 0; i < MAX_BALLS; i++) {
                if (!balls[i].inHole) {
                    DrawCircleV(balls[i].pos, balls[i].radius, balls[i].color);
                    DrawCircleLines(balls[i].pos.x, balls[i].pos.y, balls[i].radius, Fade(BLACK, 0.3f));
                }
            }

            DrawText(TextFormat("p1 points: %d/7", p1_points), 10, 420, 20, WHITE);
            DrawText(TextFormat("p2 points: %d/7", p2_points), 650, 420, 20, WHITE);
            DrawText(i_turn ? "turn: player 1" : "turn: player 2", 320, 10, 20, YELLOW);

            if (check == 16 && gameOver == 0 && !balls[0].inHole && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                DrawLineV(GetMousePosition(), balls[0].pos, RAYWHITE);
            }

            if (gameOver == 1) DrawText("you win!", 300, 200, 50, GREEN);
            if (gameOver == 2) DrawText("you lose!", 280, 200, 50, RED);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}