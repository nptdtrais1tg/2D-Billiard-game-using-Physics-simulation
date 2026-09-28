#  2D Billiard Game Simulation in C

The project is a 2D billiards simulation developed using the C programming language to simulate complex rigid-body dynamics[cite: 1]. This is a project for the C/C++ Programming Language [ET2031E] course at the School of Electrical and Electronic Engineering, Hanoi University of Science and Technology[cite: 1].

The simulation aims to bridge the gap between theoretical physics (Mechanics and Kinematics) and interactive multimedia software, computing real-time collision detection and momentum conservation[cite: 1].

##  Features
*   **Standard Environment:** A billiard table with 6 holes and 16 balls (1 white cue ball, 15 target balls including the black 8-ball) arranged in a standard triangle formation[cite: 1, 2].
*   **User Interaction:** Controls the direction and power of the cue shot using mouse click and drag mechanics[cite: 1, 2].
*   **Real-time Physics:** Simulates rolling friction, wall collisions, and elastic ball-to-ball collisions[cite: 1].
*   **Game Logic:** Turn-based mechanics, automated point calculation, turn switching upon missing or fouling, and win/lose conditions based on the 8-ball[cite: 1, 2].

## 🎮 Gameplay and Rules
*   **Controls:** Click and hold the left mouse button on the cue ball, then drag to apply force. The direction and drag distance determine the cue ball's initial velocity vector: $v = k \cdot (P_{start} - P_{end})$[cite: 1, 2].
*   **Turn Switching:** The turn switches between Player 1 and Player 2 if a shot fails to pot a ball, or when all balls come to a complete stop on the table[cite: 1, 2].
*   **Fouls (Scratch):** If the cue ball falls into a hole, the turn passes to the opponent, and the cue ball is reset to its starting coordinates $(200, 225)$[cite: 1, 2].
*   **Win/Lose Conditions:**
    *   **Win (YOU WIN):** A player scores exactly 7 points (pots 7 target balls) and subsequently pots the 8-ball[cite: 1, 2].
    *   **Lose (YOU LOSE):** A player pots the 8-ball before reaching 7 points[cite: 1, 2].

## ⚙️ Physics Engine
The project implements a basic physics engine through the following phases:
*   **Friction & Deceleration:** A rolling friction coefficient (currently $f = 0.985$) is applied every frame to decelerate the balls: $v_{new} = v_{old} \cdot f$[cite: 1, 2].
*   **Wall Collisions:** When a ball hits the table boundaries, its velocity vector is inverted and multiplied by a restitution coefficient ($e = 1.0$)[cite: 1, 2].
*   **Ball-to-Ball Collisions:** Uses Euclidean distance for collision detection. In the current prototype phase, the system simulates simple 2D elastic collisions by swapping the velocity vectors of the two colliding balls[cite: 1, 2].
*   **Overlap Resolution:** To prevent balls from sinking into each other due to floating-point inaccuracies, a positional correction formula is applied: $Overlap = (R_1 + R_2) - Distance$ to separate the balls[cite: 1, 2].

## 🛠️ Technologies Used
*   **Language:** C[cite: 1].
*   **Graphics Library:** `raylib.h` (used to render the 800x450 window, draw basic geometry, handle mouse input, and lock the loop at 60 FPS)[cite: 1, 2].
*   **Math Library:** `raymath.h` and `math.h` (for vector and distance calculations)[cite: 2].

## 👥 Credits
The project was developed by Group 7 - Class 167803 - Semester 2025.2[cite: 1]:
*   **Students:**
    *   Nguyen Phuong Trang (Student ID: 202414668)[cite: 1]
    *   Tran Diep Linh (Student ID: 202414634)[cite: 1]
*   **Instructors:**
    *   Dang Quoc Viet[cite: 1]
    *   Do Thi Ngoc Diep[cite: 1]
