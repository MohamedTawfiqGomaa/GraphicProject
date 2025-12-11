//
//  main.cpp
//  Fish-Fillet-Project
//
//  Created by Yehia Sharawy on 21/12/2023.
//

// Libraries
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

// Game Constants
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;
const int OCEAN_HEIGHT = 500;
const int NUM_FISH = 20;
const int INITIAL_TIME = 20;
const float FISH_SIZE = 20.0f;
const float COLLISION_RADIUS = 15.0f;
const float FISH_SPEED = 0.4f;
const int WAVE_SEGMENTS = 100;
const int WAVE_SPACING = 60;
const float PI = 3.1415926f;

// Behavior tuning
const float SAFE_SPAWN_DISTANCE = 150.0f;   // Minimum distance from player when spawning fish
const float FLEE_DISTANCE = 220.0f;         // Distance at which smaller fish start fleeing
const float CHASE_DISTANCE = 260.0f;        // Distance at which bigger/red fish start chasing
const float FLEE_SPEED_MULT = 1.8f;         // Speed multiplier for fleeing fish
const float CHASE_SPEED_MULT = 1.4f;        // Speed multiplier for chasing fish

// Coral sway
const int CORAL_COUNT = 14;
const float CORAL_SWAY_SPEED = 0.025f;
float coralPhase = 0.0f;

// Fish size types
enum FishSizeType {
    SMALL = 0,    // 0.8x size
    MEDIUM = 1,   // 1.0x size
    LARGE = 2     // 1.5x size
};

const float SIZE_MULTIPLIERS[] = {0.8f, 1.0f, 1.5f};
const float GROWTH_INCREMENT = 0.05f;  // Player grows by this amount per fish eaten
const float INITIAL_PLAYER_SIZE = 1.0f;
const float MAX_PLAYER_SIZE = 2.5f;

// Game State
bool isGameOver = false;
int score = 0;
int gameTime = INITIAL_TIME;
bool allYellowFishGone = false;
bool soundPlayed = false;
float playerSizeScale = INITIAL_PLAYER_SIZE;  // Player growth scale
float prevMouseX = WINDOW_WIDTH / 2.0f;  // Previous mouse X position for direction calculation

// Wave Animation
bool waveUp = true;
int waveDelta = 0;
const int MAX_WAVE_DELTA = 15;

// Sound Effects (non-blocking, shorter duration to reduce delay)
void playSound(int type) {
#ifdef _WIN32
    switch(type) {
        case 0: Beep(800, 50); break;   // Collect yellow fish - shorter
        case 1: Beep(300, 80); break;  // Hit red fish - shorter
        case 2: Beep(200, 150); break;  // Game over - shorter
        case 3: 
            Beep(523, 100);
            Beep(659, 100);
            Beep(784, 150);
            break;  // Win - shorter
    }
#endif
}

// Text Display Functions
void drawText(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    for (int i = 0; i < static_cast<int>(std::strlen(str)); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
}

void drawScore(int x, int y, int value) {
    glRasterPos2d(x, y);
    if (value < 0) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '-');
        value = -value;
    }
    char scoreStr[32];
    std::snprintf(scoreStr, sizeof(scoreStr), "%d", value);
    for (int i = 0; scoreStr[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, scoreStr[i]);
}

void drawLargeText(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    for (int i = 0; i < static_cast<int>(std::strlen(str)); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}

// Wave Drawing
void drawWave(float cx, float cy, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < WAVE_SEGMENTS; i++) {
        float theta = 2.0f * PI * float(i) / float(WAVE_SEGMENTS);
        float x = radius * std::cos(theta - 44.92f);
        float y = radius * std::sin(theta - 44.92f);
        glVertex2f(x + cx, y + cy + waveDelta);
    }
    glEnd();
}

void drawOcean() {
    // Ocean background gradient
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.4f, 0.8f);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glColor3f(0.05f, 0.2f, 0.5f);
    glVertex2f(WINDOW_WIDTH, OCEAN_HEIGHT);
    glVertex2f(0, OCEAN_HEIGHT);
    glEnd();

    // Animated waves
    glColor3f(0.15f, 0.5f, 0.9f);
    for (int i = 0; i <= WINDOW_WIDTH; i += WAVE_SPACING) {
        drawWave(i, OCEAN_HEIGHT, 40);
    }
    
    glColor3f(0.2f, 0.6f, 1.0f);
    for (int i = 30; i <= WINDOW_WIDTH; i += WAVE_SPACING) {
        drawWave(i, OCEAN_HEIGHT, 35);
    }
    
    // Update wave animation
    if (std::abs(waveDelta) > MAX_WAVE_DELTA) waveUp = !waveUp;
    waveDelta += waveUp ? 1 : -1;
}

// Coral / seaweed sway for extra depth
void drawCoral() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.3f, 0.7f, 0.4f, 0.6f);

    float baseHeight = 60.0f;
    for (int i = 0; i < CORAL_COUNT; ++i) {
        float x = 40.0f + i * (WINDOW_WIDTH / static_cast<float>(CORAL_COUNT));
        float sway = std::sin(coralPhase + i * 0.6f) * 8.0f;
        float height = baseHeight + std::sin(coralPhase * 0.8f + i) * 20.0f;

        glBegin(GL_QUADS);
        glVertex2f(x - 6.0f + sway, 0);
        glVertex2f(x + 6.0f + sway, 0);
        glVertex2f(x + 4.0f, height);
        glVertex2f(x - 4.0f, height);
        glEnd();

        // Add a lighter tip
        glColor4f(0.4f, 0.9f, 0.5f, 0.7f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 5.0f + sway, height);
        glVertex2f(x + 5.0f + sway, height);
        glVertex2f(x + sway, height + 14.0f);
        glEnd();
        glColor4f(0.3f, 0.7f, 0.4f, 0.6f);
    }

    glDisable(GL_BLEND);
}

// Fish Class
class Fish {
public:
    bool isPlayer;
    bool isRedFish;
    FishSizeType sizeType;
    float x, y;
    float direction;
    float sizeScale;  // Individual size scale for this fish
    
    Fish(bool player = false) : isPlayer(player) {
        if (isPlayer) {
            x = WINDOW_WIDTH / 2.0f;
            y = WINDOW_HEIGHT / 2.0f;
            sizeScale = INITIAL_PLAYER_SIZE;
            sizeType = MEDIUM;
            direction = FISH_SPEED;  // Default direction for player (right)
        } else {
            isRedFish = (rand() % 10 < 3);  // 30% red fish, 70% yellow fish
            // Randomly assign size type: 40% small, 40% medium, 20% large
            int sizeRoll = rand() % 10;
            if (sizeRoll < 4) {
                sizeType = SMALL;
            } else if (sizeRoll < 8) {
                sizeType = MEDIUM;
            } else {
                sizeType = LARGE;
            }
            sizeScale = SIZE_MULTIPLIERS[sizeType];
            // Position will be assigned later ensuring safe spawn distance from player
            x = 0.0f;
            y = 0.0f;
            direction = (rand() % 2) * FISH_SPEED - (FISH_SPEED / 2.0f);
        }
    }
    
    // Get effective size for collision detection
    float getCollisionRadius() const {
        return COLLISION_RADIUS * sizeScale;
    }
    
    void draw() {
        // Set color based on fish type
        float r, g, b;
        if (isPlayer) {
            r = 0.2f; g = 0.4f; b = 1.0f;  // Blue player
        } else if (isRedFish) {
            r = 1.0f; g = 0.3f; b = 0.3f;  // Red enemy
        } else {
            // Yellow collectible with slight color variation based on size
            if (sizeType == SMALL) {
                r = 1.0f; g = 0.95f; b = 0.3f;  // Lighter yellow for small
            } else if (sizeType == MEDIUM) {
                r = 1.0f; g = 0.9f; b = 0.2f;   // Standard yellow
            } else {
                r = 1.0f; g = 0.85f; b = 0.15f; // Darker yellow for large
            }
        }
        
        // Use player's size scale if this is the player, otherwise use fish's own scale
        float currentScale = isPlayer ? playerSizeScale : sizeScale;
        
        float dir = (direction < 0) ? -1.0f : 1.0f;
        float bodyLength = FISH_SIZE * 1.8f * currentScale;
        float bodyHeight = FISH_SIZE * 1.2f * currentScale;
        
        // Draw fish body (oval/ellipse shape)
        glColor3f(r, g, b);
        glBegin(GL_POLYGON);
        int segments = 20;
        for (int i = 0; i <= segments; i++) {
            float angle = PI * float(i) / float(segments);
            float px = x + dir * (bodyLength * 0.4f * std::cos(angle));
            float py = y + bodyHeight * 0.5f * std::sin(angle);
            glVertex2f(px, py);
        }
        glEnd();
        
        // Draw outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= segments; i++) {
            float angle = PI * float(i) / float(segments);
            float px = x + dir * (bodyLength * 0.4f * std::cos(angle));
            float py = y + bodyHeight * 0.5f * std::sin(angle);
            glVertex2f(px, py);
        }
        glEnd();
        
        // Draw tail fin (fan shape) at the back of the fish
        float tailBaseX = x - dir * bodyLength * 0.5f;                     // back of the body
        float tailLength = FISH_SIZE * 0.6f * currentScale;               // tail size scales with fish
        glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(tailBaseX, y);
        for (int i = 0; i <= 8; i++) {
            float angle = (PI / 3.0f) * float(i) / 8.0f - (PI / 6.0f);
            float px = tailBaseX - dir * tailLength * std::cos(angle);     // extend tail opposite to head
            float py = y + tailLength * std::sin(angle);
            glVertex2f(px, py);
        }
        glEnd();
        
        // Draw tail outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(tailBaseX, y);
        for (int i = 0; i <= 8; i++) {
            float angle = (PI / 3.0f) * float(i) / 8.0f - (PI / 6.0f);
            float px = tailBaseX - dir * tailLength * std::cos(angle);
            float py = y + tailLength * std::sin(angle);
            glVertex2f(px, py);
        }
        glEnd();
        
        // Draw top fin (dorsal fin)
        glColor3f(r * 0.9f, g * 0.9f, b * 0.9f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + dir * bodyLength * 0.1f, y + bodyHeight * 0.4f);
        glVertex2f(x + dir * bodyLength * 0.3f, y + bodyHeight * 0.6f);
        glVertex2f(x + dir * bodyLength * 0.2f, y + bodyHeight * 0.5f);
        glEnd();
        
        // Draw bottom fin (pelvic fin)
        glBegin(GL_TRIANGLES);
        glVertex2f(x + dir * bodyLength * 0.1f, y - bodyHeight * 0.4f);
        glVertex2f(x + dir * bodyLength * 0.3f, y - bodyHeight * 0.6f);
        glVertex2f(x + dir * bodyLength * 0.2f, y - bodyHeight * 0.5f);
        glEnd();
        
        // Draw eye (circle)
        float eyeX = x + dir * bodyLength * 0.35f;  // Move eye towards the head/front
        float eyeY = y + bodyHeight * 0.15f;
        float eyeSize = FISH_SIZE * 0.15f * currentScale;
        
        // Eye white
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f;
            glVertex2f(eyeX + eyeSize * std::cos(angle), eyeY + eyeSize * std::sin(angle));
        }
        glEnd();
        
        // Eye pupil
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f;
            glVertex2f(eyeX + eyeSize * 0.6f * std::cos(angle), eyeY + eyeSize * 0.6f * std::sin(angle));
        }
        glEnd();
    }
    
    void move(float mouseX = 0.0f, float mouseY = 0.0f, float prevMouseX = 0.0f) {
        if (!isPlayer) {
            x += direction;
            if (x > WINDOW_WIDTH) {
                x -= WINDOW_WIDTH;
                y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
            }
            if (x < 0) {
                x += WINDOW_WIDTH;
                y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
            }
        } else {
            // Update direction based on mouse movement
            if (mouseX > prevMouseX) {
                direction = FISH_SPEED;  // Moving right
            } else if (mouseX < prevMouseX) {
                direction = -FISH_SPEED;  // Moving left
            }
            // If mouseX == prevMouseX, keep current direction
            
            x = mouseX;
            y = WINDOW_HEIGHT - mouseY;
        }
    }

    // Movement with simple behavior: flee smaller fish, chase bigger/red fish
    void moveWithBehavior(float playerX, float playerY, float playerRadius) {
        if (isPlayer) return;

        float dx = playerX - x;
        float dy = playerY - y;
        float dist2 = dx * dx + dy * dy;
        float myRadius = getCollisionRadius();

        // Default drift
        float stepX = direction;
        float stepY = 0.0f;

        // Flee if player is bigger and close (applies to ALL fish - yellow and red)
        if (playerRadius > myRadius * 0.9f && dist2 < FLEE_DISTANCE * FLEE_DISTANCE) {
            float norm = std::sqrt(dist2) + 0.001f;
            stepX = -(dx / norm) * FISH_SPEED * FLEE_SPEED_MULT * 2.0f;
            stepY = -(dy / norm) * FISH_SPEED * FLEE_SPEED_MULT * 2.0f;
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }
        // Chase only if this fish is bigger (red or large) and player is smaller
        else if ((isRedFish || sizeType == LARGE) && playerRadius < myRadius * 0.95f && dist2 < CHASE_DISTANCE * CHASE_DISTANCE) {
            float norm = std::sqrt(dist2) + 0.001f;
            stepX = (dx / norm) * FISH_SPEED * CHASE_SPEED_MULT * 2.0f;
            stepY = (dy / norm) * FISH_SPEED * CHASE_SPEED_MULT * 2.0f;
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }

        x += stepX;
        y += stepY;

        // Wrap horizontally
        if (x > WINDOW_WIDTH) {
            x -= WINDOW_WIDTH;
            y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
        }
        if (x < 0) {
            x += WINDOW_WIDTH;
            y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
        }

        // Clamp vertical movement to stay in ocean
        if (y < 40) y = 40;
        if (y > OCEAN_HEIGHT - 30) y = OCEAN_HEIGHT - 30;
    }
};

// Game Objects
std::vector<Fish> fishArray;
Fish player(true);

// Ensure fish spawn away from player to avoid unfair instant collisions
void randomizeFishPositionAway(Fish& f) {
    if (f.isPlayer) return;
    int attempts = 0;
    float safeDist2 = SAFE_SPAWN_DISTANCE * SAFE_SPAWN_DISTANCE;
    while (attempts < 50) {
        f.x = static_cast<float>(rand() % (WINDOW_WIDTH - 100));
        f.y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
        float dx = f.x - player.x;
        float dy = f.y - player.y;
        if (dx * dx + dy * dy > safeDist2) break;
        attempts++;
    }
}

// Collision Detection - uses size-based radii
bool checkCollision(const Fish& a, const Fish& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distanceSquared = dx * dx + dy * dy;
    
    float radiusA = a.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : a.getCollisionRadius();
    float radiusB = b.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : b.getCollisionRadius();
    float combinedRadius = radiusA + radiusB;
    float collisionDistance = combinedRadius * combinedRadius;
    
    return distanceSquared < collisionDistance;
}

// Check if player can eat the other fish (player must be larger)
bool canEatFish(const Fish& player, const Fish& other) {
    float playerRadius = COLLISION_RADIUS * playerSizeScale;
    float otherRadius = other.getCollisionRadius();
    return playerRadius >= otherRadius * 0.95f;  // Slight forgiveness to reduce unfair losses
}

// Initialization
void initGame() {
    glClearColor(0.07f, 0.01f, 0.75f, 1.0f);
    playerSizeScale = INITIAL_PLAYER_SIZE;  // Reset player size
    prevMouseX = WINDOW_WIDTH / 2.0f;  // Initialize previous mouse position
    fishArray.clear();
    for (int i = 0; i < NUM_FISH; ++i) {
        Fish f;
        randomizeFishPositionAway(f);
        fishArray.push_back(f);
    }
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}

// Input Handlers
void mouseMove(int x, int y) {
    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(y);
    player.move(mouseX, mouseY, prevMouseX);
    prevMouseX = mouseX;  // Update previous mouse position
}

void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_F2) {
        isGameOver = false;
        gameTime = INITIAL_TIME;
        score = 0;
        allYellowFishGone = false;
        soundPlayed = false;
        playerSizeScale = INITIAL_PLAYER_SIZE;  // Reset player size
        prevMouseX = WINDOW_WIDTH / 2.0f;  // Reset previous mouse position
        fishArray.clear();
        for (int i = 0; i < NUM_FISH; ++i) {
            Fish f;
            randomizeFishPositionAway(f);
            fishArray.push_back(f);
        }
    }
    glutPostRedisplay();
}

// Timers
void animationTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, animationTimer, 0);
}

void gameTimer(int value) {
    if (!isGameOver && gameTime > 0) {
        --gameTime;
        if (gameTime == 0) {
            isGameOver = true;
            soundPlayed = false;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000, gameTimer, 0);
}

// Main Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    coralPhase += CORAL_SWAY_SPEED;
    drawOcean();
    drawCoral();
    
    if (!isGameOver) {
        // UI Background
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        // Score box
        glVertex2f(0, 580);
        glVertex2f(120, 580);
        glVertex2f(120, 600);
        glVertex2f(0, 600);
        glEnd();
        glBegin(GL_QUADS);
        // Time box
        glVertex2f(1080, 580);
        glVertex2f(1200, 580);
        glVertex2f(1200, 600);
        glVertex2f(1080, 600);
        glEnd();
        
        // Score and Time
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Score:", 5, 585);
        glColor3f(1.0f, 1.0f, 0.0f);
        drawScore(45, 585, score);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Size:", 5, 570);
        glColor3f(0.3f, 1.0f, 0.8f);
        char sizeStr[16];
        std::snprintf(sizeStr, sizeof(sizeStr), "%.2fx", playerSizeScale);
        glRasterPos2d(40, 570);
        for (int i = 0; sizeStr[i] != '\0'; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, sizeStr[i]);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Time:", 1100, 570);
        glColor3f(0.3f, 1.0f, 0.3f);
        drawScore(1160, 570, gameTime);
        
        // Draw player
        player.draw();
        
        // Update and draw fish
        float playerRadius = COLLISION_RADIUS * playerSizeScale;
        for (int i = 0; i < static_cast<int>(fishArray.size()); ++i) {
            fishArray[i].moveWithBehavior(player.x, player.y, playerRadius);
            if (!fishArray[i].isPlayer && checkCollision(player, fishArray[i])) {
                if (fishArray[i].isRedFish) {
                    // Red fish: Game Over only if player is smaller or equal
                    if (!canEatFish(player, fishArray[i])) {
                        playSound(1);
                        isGameOver = true;
                        soundPlayed = false;
                        break;  // Exit loop immediately
                    } else {
                        // Player is big enough to eat red fish!
                        score += 3;  // Bonus points for eating red fish
                        playSound(0);
                        // Player grows when eating larger fish
                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT * 2.0f;
                            if (playerSizeScale > MAX_PLAYER_SIZE) {
                                playerSizeScale = MAX_PLAYER_SIZE;
                            }
                        }
                        fishArray.erase(fishArray.begin() + i);
                        --i;
                    }
                } else {
                    // Yellow fish: Check if player can eat it
                    if (canEatFish(player, fishArray[i])) {
                        // Collect points and remove fish
                        int points = 1;
                        if (fishArray[i].sizeType == MEDIUM) points = 2;
                        else if (fishArray[i].sizeType == LARGE) points = 3;
                        
                        score += points;
                        playSound(0);
                        
                        // Player grows when eating fish
                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT;
                            if (playerSizeScale > MAX_PLAYER_SIZE) {
                                playerSizeScale = MAX_PLAYER_SIZE;
                            }
                        }
                        
                        fishArray.erase(fishArray.begin() + i);
                        --i;  // Adjust index after erasing
                    } else {
                        // Player is too small - game over
                        playSound(1);
                        isGameOver = true;
                        soundPlayed = false;
                        break;
                    }
                }
            } else {
                fishArray[i].draw();
            }
        }
        
        // Check win condition - all fish (yellow and red) must be collected
        bool allFishGone = true;
        for (const auto& fish : fishArray) {
            if (!fish.isPlayer) {  // Check if any non-player fish exists
                allFishGone = false;
                break;
            }
        }
        // End game immediately if all fish are collected
        if (allFishGone) {
            allYellowFishGone = true;  // Set to true for win condition
            isGameOver = true;
            soundPlayed = false;
        }
    } else {
        // Game Over Screen
        if (!soundPlayed) {
            playSound(allYellowFishGone ? 3 : 2);
            soundPlayed = true;
        }
        
        // Background
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(200, 100);
        glVertex2f(1000, 100);
        glVertex2f(1000, 500);
        glVertex2f(200, 500);
        glEnd();
        
        // Messages
        if (allYellowFishGone) {
            glColor3f(0.3f, 1.0f, 0.3f);
            drawLargeText("YOU WIN!", 500, 450);
            glColor3f(1.0f, 1.0f, 0.5f);
            drawLargeText("Max Score: ", 500, 260);
            glColor3f(1.0f, 1.0f, 0.0f);
            drawScore(620, 260, score);
        } else {
            glColor3f(1.0f, 0.3f, 0.3f);
            drawLargeText("YOU FAILED!", 500, 450);
            glColor3f(1.0f, 0.8f, 0.5f);
            drawLargeText("Max Score: ", 500, 260);
            glColor3f(1.0f, 0.5f, 0.0f);
            drawScore(620, 260, score);
        }
        
        glColor3f(0.8f, 0.8f, 0.8f);
        drawLargeText("Play Again by pressing -> F2", 450, 160);
        glColor3f(0.6f, 0.6f, 0.6f);
        drawText("Created By Yehia Sharawy & Abdelrahman Sharaf", 900, 10);
    }
    
    glDisable(GL_BLEND);
    glutSwapBuffers();
}

// Main
int main(int argc, char *argv[]) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 200);
    glutCreateWindow("Fish Game");
    initGame();
    glutPassiveMotionFunc(mouseMove);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, animationTimer, 0);
    glutTimerFunc(0, gameTimer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
