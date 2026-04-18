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
#include <algorithm>
#include <string>

// ==================== CONSTANTS ====================
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;
const int OCEAN_HEIGHT = 500;
const int NUM_FISH = 20;
const int INITIAL_TIME = 90;
const float FISH_SIZE = 20.0f;
const float COLLISION_RADIUS = 15.0f;
const float FISH_SPEED = 0.4f;
const int WAVE_SEGMENTS = 100;
const int WAVE_SPACING = 60;
const float PI = 3.1415926f;
const int INITIAL_WINDOW_HEIGHT = 600;
const float OCEAN_HEIGHT_RATIO = 0.833f;  // نسبة ارتفاع المحيط من ارتفاع النافذة
const int INITIAL_WINDOW_WIDTH = 1200;
const int WAVE_POINTS = 50;
float waveOffset = 0.0f;
int windowWidth = INITIAL_WINDOW_WIDTH;
float oceanHeight = INITIAL_WINDOW_HEIGHT * OCEAN_HEIGHT_RATIO;

// Behavior tuning
const float SAFE_SPAWN_DISTANCE = 150.0f;
const float FLEE_DISTANCE = 220.0f;
const float CHASE_DISTANCE = 260.0f;
const float FLEE_SPEED_MULT = 1.8f;
const float CHASE_SPEED_MULT = 1.4f;

// Coral sway
const int CORAL_COUNT = 14;
const float CORAL_SWAY_SPEED = 0.025f;
float coralPhase = 0.0f;

// Fish size types - 3 SIZES
enum FishSizeType { SMALL = 0, MEDIUM = 1, LARGE = 2 };
const float SIZE_MULTIPLIERS[] = {1.0f, 1.4f, 2.0f};
const int NUM_SIZE_TYPES = 3;
const float GROWTH_INCREMENT = 0.08f;
const float INITIAL_PLAYER_SIZE = 1.0f;
const float MAX_PLAYER_SIZE = 2.5f;

// Size names for notifications
const char* SIZE_NAMES[] = {"Small Fish", "Medium Fish", "Large Fish"};

// ==================== GAME STATES ====================
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };
GameState currentState = MENU;

// ==================== GAME VARIABLES ====================
bool allYellowFishGone = false;
bool soundPlayed = false;
int score = 0;
int highScore = 0;
int gameTime = INITIAL_TIME;
float playerSizeScale = INITIAL_PLAYER_SIZE;
float prevMouseX = WINDOW_WIDTH / 2.0f;

// Biggest fish timer
bool playerIsBiggest = false;
float biggestTimer = 0.0f;
const float WIN_TIMER = 10.0f;

// Wave Animation
bool waveUp = true;
int waveDelta = 0;
const int MAX_WAVE_DELTA = 15;

// ==================== NEW FEATURES ====================
int comboCount = 0;
float comboTimer = 0.0f;
const float COMBO_TIMEOUT = 2.0f;
const float COMBO_DECAY = 0.033f;

int currentLevel = 1;
int fishEatenThisLevel = 0;
const int FISH_PER_LEVEL = 10;

float shakeIntensity = 0.0f;
float shakeOffsetX = 0.0f;
float shakeOffsetY = 0.0f;

float animTime = 0.0f;

// Track which sizes player can eat (for notifications)
bool canEatSize[NUM_SIZE_TYPES] = {true, false, false};
bool notifiedSize[NUM_SIZE_TYPES] = {true, false, false};  // Already notified

// ==================== PARTICLE SYSTEM ====================
struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float maxLife;
    float r, g, b;
    float size;

    Particle(float px, float py, float pr, float pg, float pb) {
        x = px; y = py;
        float angle = (rand() % 360) * PI / 180.0f;
        float speed = 1.0f + (rand() % 30) / 10.0f;
        vx = std::cos(angle) * speed;
        vy = std::sin(angle) * speed;
        life = maxLife = 1.0f + (rand() % 50) / 100.0f;
        r = pr; g = pg; b = pb;
        size = 3.0f + (rand() % 5);
    }

    void update() {
        x += vx;
        y += vy;
        vy += 0.05f;
        life -= 0.02f;
    }

    void draw() {
        float alpha = life / maxLife;
        glColor4f(r, g, b, alpha);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 8; i++) {
            float angle = 2.0f * PI * i / 8.0f;
            glVertex2f(x + size * alpha * std::cos(angle),
                      y + size * alpha * std::sin(angle));
        }
        glEnd();
    }

    bool isDead() { return life <= 0; }
};

std::vector<Particle> particles;

void spawnParticles(float x, float y, float r, float g, float b, int count = 15) {
    for (int i = 0; i < count; i++) {
        particles.push_back(Particle(x, y, r, g, b));
    }
}

// ==================== BUBBLE SYSTEM ====================
struct Bubble {
    float x, y;
    float size;
    float speed;
    float wobble;
    float wobblePhase;

    Bubble() { reset(); y = rand() % OCEAN_HEIGHT; }

    void reset() {
        x = rand() % WINDOW_WIDTH;
        y = -20;
        size = 3.0f + (rand() % 10);
        speed = 0.5f + (rand() % 20) / 10.0f;
        wobble = 0.5f + (rand() % 10) / 10.0f;
        wobblePhase = (rand() % 100) / 10.0f;
    }

    void update() {
        y += speed;
        x += std::sin(wobblePhase + y * 0.02f) * wobble;
        if (y > OCEAN_HEIGHT + 20) reset();
    }

    void draw() {
        float alpha = 0.3f + 0.2f * std::sin(animTime + wobblePhase);
        glColor4f(0.7f, 0.9f, 1.0f, alpha);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * i / 12.0f;
            glVertex2f(x + size * std::cos(angle), y + size * std::sin(angle));
        }
        glEnd();

        glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.5f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 6; i++) {
            float angle = PI * 0.3f + PI * 0.4f * i / 6.0f;
            float r = size * 0.4f;
            glVertex2f(x - size * 0.3f + r * std::cos(angle),
                      y + size * 0.3f + r * std::sin(angle));
        }
        glEnd();
    }
};

const int BUBBLE_COUNT = 30;
std::vector<Bubble> bubbles;

// ==================== POWER-UP SYSTEM ====================
enum PowerUpType { POWER_SPEED, POWER_SHIELD, POWER_SIZE, POWER_TIME };

struct PowerUp {
    float x, y;
    PowerUpType type;
    float bobPhase;
    bool active;

    PowerUp() : active(false), bobPhase(0) {}

    void spawn() {
        x = 100 + rand() % (WINDOW_WIDTH - 200);
        y = 100 + rand() % (OCEAN_HEIGHT - 200);
        type = static_cast<PowerUpType>(rand() % 4);
        bobPhase = (rand() % 100) / 10.0f;
        active = true;
    }

    void draw() {
        if (!active) return;

        float bob = std::sin(animTime * 3 + bobPhase) * 5;
        float glow = 0.5f + 0.3f * std::sin(animTime * 5);

        glColor4f(1.0f, 1.0f, 0.5f, glow * 0.3f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 16; i++) {
            float angle = 2.0f * PI * i / 16.0f;
            glVertex2f(x + 25 * std::cos(angle), y + bob + 25 * std::sin(angle));
        }
        glEnd();

        float r, g, b;
        switch(type) {
            case POWER_SPEED:  r = 0.3f; g = 1.0f; b = 0.3f; break;
            case POWER_SHIELD: r = 0.3f; g = 0.5f; b = 1.0f; break;
            case POWER_SIZE:   r = 1.0f; g = 0.5f; b = 0.0f; break;
            case POWER_TIME:   r = 1.0f; g = 1.0f; b = 0.3f; break;
        }

        glColor3f(r, g, b);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 6; i++) {
            float angle = 2.0f * PI * i / 6.0f + animTime;
            glVertex2f(x + 12 * std::cos(angle), y + bob + 12 * std::sin(angle));
        }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        switch(type) {
            case POWER_SPEED:
                glBegin(GL_LINES);
                glVertex2f(x - 5, y + bob);
                glVertex2f(x + 5, y + bob);
                glVertex2f(x + 3, y + bob - 3);
                glVertex2f(x + 5, y + bob);
                glVertex2f(x + 3, y + bob + 3);
                glVertex2f(x + 5, y + bob);
                glEnd();
                break;
            case POWER_SHIELD:
                glBegin(GL_LINE_LOOP);
                for (int i = 0; i <= 8; i++) {
                    float angle = 2.0f * PI * i / 8.0f;
                    glVertex2f(x + 5 * std::cos(angle), y + bob + 5 * std::sin(angle));
                }
                glEnd();
                break;
            case POWER_SIZE:
                glBegin(GL_LINES);
                glVertex2f(x - 5, y + bob);
                glVertex2f(x + 5, y + bob);
                glVertex2f(x, y + bob - 5);
                glVertex2f(x, y + bob + 5);
                glEnd();
                break;
            case POWER_TIME:
                glBegin(GL_LINES);
                glVertex2f(x, y + bob);
                glVertex2f(x, y + bob + 5);
                glVertex2f(x, y + bob);
                glVertex2f(x + 4, y + bob);
                glEnd();
                break;
        }
        glLineWidth(1.0f);
    }

    bool checkCollision(float px, float py, float pradius) {
        if (!active) return false;
        float dx = px - x;
        float dy = py - y;
        return (dx * dx + dy * dy) < (pradius + 15) * (pradius + 15);
    }
};

PowerUp powerUp;
float speedBoostTimer = 0.0f;
float shieldTimer = 0.0f;
float sizeBoostTimer = 0.0f;
const float POWERUP_DURATION = 5.0f;
float powerUpSpawnTimer = 0.0f;

// ==================== FLOATING TEXT ====================
struct FloatingText {
    float x, y;
    char text[64];
    float life;
    float r, g, b;
    bool centered;
    float scale;

    FloatingText(float px, float py, const char* t, float pr, float pg, float pb, bool center = false, float s = 1.0f)
        : x(px), y(py), life(2.0f), r(pr), g(pg), b(pb), centered(center), scale(s) {
        strncpy(text, t, 63);
        text[63] = '\0';
    }

    void update() {
        y += 0.8f;
        life -= 0.015f;
    }

    void draw() {
        float alpha = life / 2.0f;
        glColor4f(r, g, b, alpha);

        void* font = (scale > 1.0f) ? GLUT_BITMAP_TIMES_ROMAN_24 : GLUT_BITMAP_HELVETICA_18;

        float drawX = x;
        if (centered) {
            int width = 0;
            for (int i = 0; text[i] != '\0'; i++) {
                width += glutBitmapWidth(font, text[i]);
            }
            drawX = x - width / 2;
        }

        glRasterPos2f(drawX, y);
        for (int i = 0; text[i] != '\0'; i++) {
            glutBitmapCharacter(font, text[i]);
        }
    }

    bool isDead() { return life <= 0; }
};

std::vector<FloatingText> floatingTexts;

// ==================== TEXT FUNCTIONS ====================
void drawText(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    for (int i = 0; str[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
}

void drawTextMedium(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    for (int i = 0; str[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}

void drawScore(int x, int y, int value) {
    char scoreStr[32];
    std::snprintf(scoreStr, sizeof(scoreStr), "%d", value);
    glRasterPos2d(x, y);
    for (int i = 0; scoreStr[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, scoreStr[i]);
}

void drawLargeText(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    for (int i = 0; str[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}

void drawCenteredText(const char* str, int y, void* font = GLUT_BITMAP_TIMES_ROMAN_24) {
    int width = 0;
    for (int i = 0; str[i]; i++) {
        width += glutBitmapWidth(font, str[i]);
    }
    glRasterPos2d(WINDOW_WIDTH / 2 - width / 2, y);
    for (int i = 0; str[i]; i++) {
        glutBitmapCharacter(font, str[i]);
    }
}

// ==================== WAVE DRAWING ====================



void drawOcean() {
    // خلفية المحيط بتدرج بسيط
    glBegin(GL_QUADS);

    glColor3f(0.1f, 0.4f, 0.8f);
    glVertex2f(0, 0);

    glVertex2f(windowWidth, 0);
    glColor3f(0.05f, 0.2f, 0.5f);

    glVertex2f(windowWidth, oceanHeight);
    glVertex2f(0, oceanHeight);

    glEnd();

    // رسم أمواج متعددة الطبقات للواقعية

    // الموجة الأولى (الخلفية - بطيئة)

    glColor4f(0.15f, 0.45f, 0.75f, 0.6f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= WAVE_POINTS; i++) {
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        float y = oceanHeight + sin((x * 0.01f) + (waveOffset * 0.5f)) * 8.0f +
                  sin((x * 0.02f) - (waveOffset * 0.3f)) * 4.0f;
        glVertex2f(x, y);
    }
    glEnd();

    // الموجة الثانية (الوسطى - متوسطة)
    glColor4f(0.2f, 0.55f, 0.85f, 0.7f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= WAVE_POINTS; i++) {
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        float y = oceanHeight + sin((x * 0.015f) + (waveOffset * 0.8f)) * 10.0f +
                  sin((x * 0.03f) + (waveOffset * 0.4f)) * 5.0f;
        glVertex2f(x, y);
    }
    glEnd();

    // الموجة الثالثة (الأمامية - سريعة)
    glColor4f(0.25f, 0.65f, 0.95f, 0.9f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= WAVE_POINTS; i++) {
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        float y = oceanHeight + sin((x * 0.02f) + waveOffset) * 12.0f +
                  sin((x * 0.04f) - (waveOffset * 0.7f)) * 6.0f +
                  sin((x * 0.08f) + (waveOffset * 1.5f)) * 3.0f;
        glVertex2f(x, y);
    }
    glEnd();

    // تحريك الأمواج
    waveOffset += 0.03f;
    if (waveOffset > 2.0f * PI * 100.0f) {
        waveOffset = 0.0f;
    }
}

void drawCoral() {
    glPushMatrix();
    glTranslatef(shakeOffsetX, shakeOffsetY, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float baseHeight = 60.0f;
    for (int i = 0; i < CORAL_COUNT; ++i) {
        float x = 40.0f + i * (WINDOW_WIDTH / static_cast<float>(CORAL_COUNT));
        float sway = std::sin(coralPhase + i * 0.6f) * 8.0f;
        float height = baseHeight + std::sin(coralPhase * 0.8f + i) * 20.0f;

        float hue = (i * 37) % 100 / 100.0f;
        glColor4f(0.2f + hue * 0.3f, 0.5f + hue * 0.3f, 0.3f + hue * 0.2f, 0.7f);

        glBegin(GL_QUADS);
        glVertex2f(x - 6.0f + sway, 0);
        glVertex2f(x + 6.0f + sway, 0);
        glVertex2f(x + 4.0f, height);
        glVertex2f(x - 4.0f, height);
        glEnd();

        glColor4f(0.3f + hue * 0.4f, 0.8f + hue * 0.2f, 0.4f + hue * 0.2f, 0.8f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 5.0f + sway, height);
        glVertex2f(x + 5.0f + sway, height);
        glVertex2f(x + sway, height + 14.0f);
        glEnd();
    }

    glDisable(GL_BLEND);
    glPopMatrix();
}

// ==================== FISH SHAPE DRAWING FUNCTIONS ====================

// SMALL FISH - Simple round fish shape (سمكة صغيرة بسيطة)
void drawSmallFishShape(float x, float y, float scale, float dir, float tailWag, float r, float g, float b) {
    float bodyLength = FISH_SIZE * 1.5f * scale;
    float bodyHeight = FISH_SIZE * 1.0f * scale;

    // Body - oval shape
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * PI * i / 20.0f;
        float px = x + dir * bodyLength * 0.4f * std::cos(angle);
        float py = y + bodyHeight * 0.5f * std::sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Simple tail
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - dir * bodyLength * 0.4f, y);
    glVertex2f(x - dir * bodyLength * 0.8f + std::sin(tailWag) * 5, y + bodyHeight * 0.4f);
    glVertex2f(x - dir * bodyLength * 0.8f + std::sin(tailWag) * 5, y - bodyHeight * 0.4f);
    glEnd();

    // Eye
    float eyeX = x + dir * bodyLength * 0.2f;
    float eyeY = y + bodyHeight * 0.1f;
    float eyeSize = FISH_SIZE * 0.12f * scale;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++) {
        float angle = 2.0f * PI * i / 10.0f;
        glVertex2f(eyeX + eyeSize * std::cos(angle), eyeY + eyeSize * std::sin(angle));
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++) {
        float angle = 2.0f * PI * i / 10.0f;
        glVertex2f(eyeX + eyeSize * 0.5f * std::cos(angle), eyeY + eyeSize * 0.5f * std::sin(angle));
    }
    glEnd();
}

// MEDIUM FISH - Fat round fish with spots (سمكة متوسطة سمينة مع نقاط)
void drawMediumFishShape(float x, float y, float scale, float dir, float tailWag, float r, float g, float b) {
    float bodyLength = FISH_SIZE * 1.6f * scale;
    float bodyHeight = FISH_SIZE * 1.4f * scale;  // Fatter body

    // Body - more circular
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 24; i++) {
        float angle = 2.0f * PI * i / 24.0f;
        float stretch = 1.0f + 0.2f * std::cos(angle * 2);  // Slight wobble
        float px = x + dir * bodyLength * 0.35f * std::cos(angle) * stretch;
        float py = y + bodyHeight * 0.5f * std::sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Spots on body
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);
    for (int s = 0; s < 3; s++) {
        float spotX = x + dir * (bodyLength * 0.1f - s * bodyLength * 0.15f);
        float spotY = y + (s - 1) * bodyHeight * 0.2f;
        float spotSize = FISH_SIZE * 0.08f * scale;
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 8; i++) {
            float angle = 2.0f * PI * i / 8.0f;
            glVertex2f(spotX + spotSize * std::cos(angle), spotY + spotSize * std::sin(angle));
        }
        glEnd();
    }

    // Top fin
    glColor3f(r * 0.9f, g * 0.9f, b * 0.9f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - dir * bodyLength * 0.1f, y + bodyHeight * 0.45f);
    glVertex2f(x + dir * bodyLength * 0.1f, y + bodyHeight * 0.45f);
    glVertex2f(x, y + bodyHeight * 0.75f);
    glEnd();

    // Round tail
    glColor3f(r * 0.85f, g * 0.85f, b * 0.85f);
    glBegin(GL_POLYGON);
    float tailCenterX = x - dir * bodyLength * 0.5f + std::sin(tailWag) * 8;
    for (int i = 0; i <= 12; i++) {
        float angle = PI * i / 12.0f - PI/2;
        glVertex2f(tailCenterX - dir * bodyLength * 0.25f * std::cos(angle),
                  y + bodyHeight * 0.4f * std::sin(angle));
    }
    glEnd();

    // Eye - bigger
    float eyeX = x + dir * bodyLength * 0.2f;
    float eyeY = y + bodyHeight * 0.15f;
    float eyeSize = FISH_SIZE * 0.15f * scale;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 12; i++) {
        float angle = 2.0f * PI * i / 12.0f;
        glVertex2f(eyeX + eyeSize * std::cos(angle), eyeY + eyeSize * std::sin(angle));
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++) {
        float angle = 2.0f * PI * i / 10.0f;
        glVertex2f(eyeX + eyeSize * 0.5f * std::cos(angle), eyeY + eyeSize * 0.5f * std::sin(angle));
    }
    glEnd();
}

// LARGE FISH - Shark-like angular fish (سمكة كبيرة شبيهة بالقرش)
void drawLargeFishShape(float x, float y, float scale, float dir, float tailWag, float r, float g, float b) {
    float bodyLength = FISH_SIZE * 2.2f * scale;
    float bodyHeight = FISH_SIZE * 1.0f * scale;

    // Body - elongated shark shape
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    // Top curve
    glVertex2f(x + dir * bodyLength * 0.5f, y);  // Nose
    glVertex2f(x + dir * bodyLength * 0.3f, y + bodyHeight * 0.3f);
    glVertex2f(x, y + bodyHeight * 0.4f);
    glVertex2f(x - dir * bodyLength * 0.3f, y + bodyHeight * 0.25f);
    glVertex2f(x - dir * bodyLength * 0.45f, y);
    // Bottom curve
    glVertex2f(x - dir * bodyLength * 0.3f, y - bodyHeight * 0.25f);
    glVertex2f(x, y - bodyHeight * 0.35f);
    glVertex2f(x + dir * bodyLength * 0.3f, y - bodyHeight * 0.2f);
    glEnd();

    // Dorsal fin (top)
    glColor3f(r * 0.85f, g * 0.85f, b * 0.85f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - dir * bodyLength * 0.05f, y + bodyHeight * 0.35f);
    glVertex2f(x - dir * bodyLength * 0.25f, y + bodyHeight * 0.35f);
    glVertex2f(x - dir * bodyLength * 0.1f, y + bodyHeight * 0.7f);
    glEnd();

    // Side fin
    glBegin(GL_TRIANGLES);
    glVertex2f(x + dir * bodyLength * 0.1f, y - bodyHeight * 0.2f);
    glVertex2f(x - dir * bodyLength * 0.15f, y - bodyHeight * 0.25f);
    glVertex2f(x, y - bodyHeight * 0.55f);
    glEnd();

    // Tail - forked
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    float tailBaseX = x - dir * bodyLength * 0.45f;
    float tailOffset = std::sin(tailWag) * 10;
    glBegin(GL_TRIANGLES);
    // Upper fork
    glVertex2f(tailBaseX, y);
    glVertex2f(tailBaseX - dir * bodyLength * 0.35f + tailOffset, y + bodyHeight * 0.5f);
    glVertex2f(tailBaseX - dir * bodyLength * 0.15f + tailOffset * 0.5f, y + bodyHeight * 0.1f);
    // Lower fork
    glVertex2f(tailBaseX, y);
    glVertex2f(tailBaseX - dir * bodyLength * 0.35f + tailOffset, y - bodyHeight * 0.5f);
    glVertex2f(tailBaseX - dir * bodyLength * 0.15f + tailOffset * 0.5f, y - bodyHeight * 0.1f);
    glEnd();

    // Gill lines
    glColor3f(r * 0.6f, g * 0.6f, b * 0.6f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x + dir * bodyLength * 0.2f, y + bodyHeight * 0.2f);
    glVertex2f(x + dir * bodyLength * 0.15f, y - bodyHeight * 0.15f);
    glVertex2f(x + dir * bodyLength * 0.25f, y + bodyHeight * 0.15f);
    glVertex2f(x + dir * bodyLength * 0.2f, y - bodyHeight * 0.1f);
    glEnd();
    glLineWidth(1.0f);

    // Eye - small and menacing
    float eyeX = x + dir * bodyLength * 0.35f;
    float eyeY = y + bodyHeight * 0.1f;
    float eyeSize = FISH_SIZE * 0.1f * scale;

    glColor3f(1.0f, 1.0f, 0.8f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++) {
        float angle = 2.0f * PI * i / 10.0f;
        glVertex2f(eyeX + eyeSize * std::cos(angle), eyeY + eyeSize * std::sin(angle));
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 8; i++) {
        float angle = 2.0f * PI * i / 8.0f;
        glVertex2f(eyeX + eyeSize * 0.6f * std::cos(angle), eyeY + eyeSize * 0.6f * std::sin(angle));
    }
    glEnd();
}

// PLAYER FISH - Unique beautiful fish with crown (سمكة اللاعب المميزة مع تاج)
void drawPlayerFishShape(float x, float y, float scale, float dir, float tailWag) {
    float bodyLength = FISH_SIZE * 1.8f * scale;
    float bodyHeight = FISH_SIZE * 1.2f * scale;

    // Glow effect
    glColor4f(0.3f, 0.6f, 1.0f, 0.2f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * PI * i / 20.0f;
        glVertex2f(x + bodyLength * 0.6f * std::cos(angle), y + bodyHeight * 0.7f * std::sin(angle));
    }
    glEnd();

    // Body - elegant curved shape
    // Main body gradient
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 24; i++) {
        float angle = 2.0f * PI * i / 24.0f;
        float t = (std::sin(angle) + 1.0f) / 2.0f;
        glColor3f(0.1f + t * 0.2f, 0.3f + t * 0.3f, 0.8f + t * 0.2f);
        float px = x + dir * bodyLength * 0.4f * std::cos(angle);
        float py = y + bodyHeight * 0.5f * std::sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Body outline
    glColor3f(0.1f, 0.2f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 24; i++) {
        float angle = 2.0f * PI * i / 24.0f;
        float px = x + dir * bodyLength * 0.4f * std::cos(angle);
        float py = y + bodyHeight * 0.5f * std::sin(angle);
        glVertex2f(px, py);
    }
    glEnd();
    glLineWidth(1.0f);

    // Decorative stripes
    glColor4f(0.5f, 0.7f, 1.0f, 0.5f);
    for (int s = 0; s < 3; s++) {
        float stripeX = x + dir * bodyLength * (0.15f - s * 0.15f);
        glBegin(GL_LINES);
        glVertex2f(stripeX, y + bodyHeight * 0.3f);
        glVertex2f(stripeX, y - bodyHeight * 0.3f);
        glEnd();
    }

    // Flowing tail
    glBegin(GL_TRIANGLE_FAN);
    float tailBaseX = x - dir * bodyLength * 0.4f;
    glColor3f(0.2f, 0.4f, 0.9f);
    glVertex2f(tailBaseX, y);
    for (int i = 0; i <= 12; i++) {
        float angle = (PI * 0.6f) * i / 12.0f - PI * 0.3f + tailWag * 0.5f;
        float len = bodyLength * 0.5f * (1.0f + 0.2f * std::sin(i * 0.8f));
        float t = float(i) / 12.0f;
        glColor3f(0.1f + t * 0.3f, 0.3f + t * 0.4f, 0.7f + t * 0.3f);
        glVertex2f(tailBaseX - dir * len * std::cos(angle), y + len * std::sin(angle));
    }
    glEnd();

    // Top fin - elegant
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.3f, 0.5f, 1.0f);
    glVertex2f(x, y + bodyHeight * 0.45f);
    for (int i = 0; i <= 8; i++) {
        float t = float(i) / 8.0f;
        float finX = x - dir * bodyLength * 0.2f + dir * bodyLength * 0.4f * t;
        float finY = y + bodyHeight * 0.45f + bodyHeight * 0.4f * std::sin(PI * t) * (1.0f + 0.1f * std::sin(animTime * 3 + i));
        glColor3f(0.2f + t * 0.2f, 0.4f + t * 0.2f, 0.9f);
        glVertex2f(finX, finY);
    }
    glEnd();

    // Side fins
    glColor3f(0.25f, 0.45f, 0.95f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + dir * bodyLength * 0.1f, y - bodyHeight * 0.35f);
    glVertex2f(x - dir * bodyLength * 0.1f, y - bodyHeight * 0.4f);
    glVertex2f(x, y - bodyHeight * 0.65f);
    glEnd();

    // CROWN - Player's special mark!
    float crownX = x + dir * bodyLength * 0.15f;
    float crownY = y + bodyHeight * 0.55f;
    float crownSize = scale * 8.0f;

    // Crown base
    glColor3f(1.0f, 0.85f, 0.0f);  // Gold
    glBegin(GL_POLYGON);
    glVertex2f(crownX - crownSize * 1.2f, crownY);
    glVertex2f(crownX + crownSize * 1.2f, crownY);
    glVertex2f(crownX + crownSize * 1.0f, crownY + crownSize * 0.4f);
    glVertex2f(crownX - crownSize * 1.0f, crownY + crownSize * 0.4f);
    glEnd();

    // Crown points
    glBegin(GL_TRIANGLES);
    glVertex2f(crownX - crownSize * 0.8f, crownY + crownSize * 0.4f);
    glVertex2f(crownX - crownSize * 0.4f, crownY + crownSize * 0.4f);
    glVertex2f(crownX - crownSize * 0.6f, crownY + crownSize * 1.0f);

    glVertex2f(crownX - crownSize * 0.2f, crownY + crownSize * 0.4f);
    glVertex2f(crownX + crownSize * 0.2f, crownY + crownSize * 0.4f);
    glVertex2f(crownX, crownY + crownSize * 1.2f);

    glVertex2f(crownX + crownSize * 0.4f, crownY + crownSize * 0.4f);
    glVertex2f(crownX + crownSize * 0.8f, crownY + crownSize * 0.4f);
    glVertex2f(crownX + crownSize * 0.6f, crownY + crownSize * 1.0f);
    glEnd();

    // Crown gems
    glColor3f(1.0f, 0.2f, 0.2f);  // Red gem
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 6; i++) {
        float angle = 2.0f * PI * i / 6.0f;
        glVertex2f(crownX + crownSize * 0.15f * std::cos(angle),
                  crownY + crownSize * 0.6f + crownSize * 0.15f * std::sin(angle));
    }
    glEnd();

    // Eye - expressive
    float eyeX = x + dir * bodyLength * 0.28f;
    float eyeY = y + bodyHeight * 0.12f;
    float eyeSize = FISH_SIZE * 0.16f * scale;

    // Eye white
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 12; i++) {
        float angle = 2.0f * PI * i / 12.0f;
        glVertex2f(eyeX + eyeSize * std::cos(angle), eyeY + eyeSize * std::sin(angle));
    }
    glEnd();

    // Iris - blue
    glColor3f(0.2f, 0.4f, 0.9f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++) {
        float angle = 2.0f * PI * i / 10.0f;
        glVertex2f(eyeX + eyeSize * 0.6f * std::cos(angle), eyeY + eyeSize * 0.6f * std::sin(angle));
    }
    glEnd();

    // Pupil
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 8; i++) {
        float angle = 2.0f * PI * i / 8.0f;
        glVertex2f(eyeX + eyeSize * 0.3f * std::cos(angle), eyeY + eyeSize * 0.3f * std::sin(angle));
    }
    glEnd();

    // Eye shine
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 6; i++) {
        float angle = 2.0f * PI * i / 6.0f;
        glVertex2f(eyeX - eyeSize * 0.2f + eyeSize * 0.15f * std::cos(angle),
                  eyeY + eyeSize * 0.2f + eyeSize * 0.15f * std::sin(angle));
    }
    glEnd();
}

// ==================== FISH CLASS ====================
class Fish {
public:
    bool isPlayer;
    bool isRedFish;
    FishSizeType sizeType;
    float x, y;
    float direction;
    float sizeScale;
    float tailPhase;

    Fish(bool player = false) : isPlayer(player), tailPhase(0) {
        if (isPlayer) {
            x = WINDOW_WIDTH / 2.0f;
            y = WINDOW_HEIGHT / 2.0f;
            sizeScale = INITIAL_PLAYER_SIZE;
            sizeType = SMALL;
            direction = FISH_SPEED;
        } else {
            isRedFish = (rand() % 10 < 3);
            int sizeRoll = rand() % 100;
            if (sizeRoll < 40) sizeType = SMALL;
            else if (sizeRoll < 75) sizeType = MEDIUM;
            else sizeType = LARGE;
            sizeScale = SIZE_MULTIPLIERS[sizeType];
            x = 0.0f; y = 0.0f;
            direction = (rand() % 2) * FISH_SPEED - (FISH_SPEED / 2.0f);
            tailPhase = (rand() % 100) / 10.0f;
        }
    }

    float getCollisionRadius() const {
        return COLLISION_RADIUS * sizeScale;
    }

    void draw() {
        float currentScale = isPlayer ? playerSizeScale : sizeScale;
        float dir = (direction < 0) ? -1.0f : 1.0f;
        float tailWag = std::sin(tailPhase) * 0.3f;

        if (isPlayer) {
            // Shield effect
            if (shieldTimer > 0) {
                float pulse = 0.5f + 0.3f * std::sin(animTime * 8);
                glColor4f(0.3f, 0.5f, 1.0f, pulse * 0.4f);
                float bodyLength = FISH_SIZE * 1.8f * currentScale;
                float bodyHeight = FISH_SIZE * 1.2f * currentScale;
                glBegin(GL_POLYGON);
                for (int i = 0; i <= 20; i++) {
                    float angle = 2.0f * PI * i / 20.0f;
                    glVertex2f(x + bodyLength * 0.8f * std::cos(angle),
                              y + bodyHeight * 0.8f * std::sin(angle));
                }
                glEnd();
            }

            // Speed boost effect
            if (speedBoostTimer > 0) {
                glColor4f(0.3f, 1.0f, 0.3f, 0.3f);
                float bodyLength = FISH_SIZE * 1.8f * currentScale;
                float bodyHeight = FISH_SIZE * 1.2f * currentScale;
                for (int i = 1; i <= 3; i++) {
                    glBegin(GL_LINE_LOOP);
                    for (int j = 0; j <= 12; j++) {
                        float angle = PI * j / 12.0f;
                        float px = x - dir * i * 15 + dir * (bodyLength * 0.3f * std::cos(angle));
                        float py = y + bodyHeight * 0.4f * std::sin(angle);
                        glVertex2f(px, py);
                    }
                    glEnd();
                }
            }

            // Draw player's unique fish
            drawPlayerFishShape(x, y, currentScale, dir, tailWag);
        } else {
            // Enemy fish colors
            float r, g, b;
            if (isRedFish) {
                float intensity = 0.7f + sizeType * 0.1f;
                r = intensity; g = 0.2f; b = 0.2f;
            } else {
                if (sizeType == SMALL) { r = 1.0f; g = 0.95f; b = 0.3f; }
                else if (sizeType == MEDIUM) { r = 1.0f; g = 0.8f; b = 0.2f; }
                else { r = 1.0f; g = 0.65f; b = 0.15f; }
            }

            // Draw based on size type with unique shapes
            switch (sizeType) {
                case SMALL:
                    drawSmallFishShape(x, y, currentScale, dir, tailWag, r, g, b);
                    break;
                case MEDIUM:
                    drawMediumFishShape(x, y, currentScale, dir, tailWag, r, g, b);
                    break;
                case LARGE:
                    drawLargeFishShape(x, y, currentScale, dir, tailWag, r, g, b);
                    break;
            }
        }

        tailPhase += isPlayer ? 0.2f : 0.15f;
    }

    void move(float mouseX = 0, float mouseY = 0, float pMouseX = 0) {
        if (isPlayer) {
            if (mouseX > pMouseX) direction = FISH_SPEED;
            else if (mouseX < pMouseX) direction = -FISH_SPEED;

            float targetX = mouseX;
            float targetY = WINDOW_HEIGHT - mouseY;

            float speedMult = (speedBoostTimer > 0) ? 1.5f : 1.0f;
            x += (targetX - x) * 0.15f * speedMult;
            y += (targetY - y) * 0.15f * speedMult;
        } else {
            x += direction;
            if (x > WINDOW_WIDTH) { x -= WINDOW_WIDTH; y = rand() % (WINDOW_HEIGHT - 100); }
            if (x < 0) { x += WINDOW_WIDTH; y = rand() % (WINDOW_HEIGHT - 100); }
        }
    }

    void moveWithBehavior(float playerX, float playerY, float playerRadius) {
        if (isPlayer) return;

        float dx = playerX - x;
        float dy = playerY - y;
        float dist2 = dx * dx + dy * dy;
        float myRadius = getCollisionRadius();

        float stepX = direction;
        float stepY = 0.0f;

        float diffMult = 1.0f + (currentLevel - 1) * 0.2f;

        if (playerRadius > myRadius * 0.9f && dist2 < FLEE_DISTANCE * FLEE_DISTANCE) {
            float norm = std::sqrt(dist2) + 0.001f;
            stepX = -(dx / norm) * FISH_SPEED * FLEE_SPEED_MULT * 2.0f * diffMult;
            stepY = -(dy / norm) * FISH_SPEED * FLEE_SPEED_MULT * 2.0f * diffMult;
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }
        else if ((isRedFish || sizeType >= MEDIUM) && playerRadius < myRadius * 0.95f
                 && dist2 < CHASE_DISTANCE * CHASE_DISTANCE) {
            float norm = std::sqrt(dist2) + 0.001f;
            stepX = (dx / norm) * FISH_SPEED * CHASE_SPEED_MULT * 2.0f * diffMult;
            stepY = (dy / norm) * FISH_SPEED * CHASE_SPEED_MULT * 2.0f * diffMult;
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }

        x += stepX;
        y += stepY;

        if (x > WINDOW_WIDTH) { x -= WINDOW_WIDTH; y = rand() % (WINDOW_HEIGHT - 100); }
        if (x < 0) { x += WINDOW_WIDTH; y = rand() % (WINDOW_HEIGHT - 100); }
        if (y < 40) y = 40;
        if (y > OCEAN_HEIGHT - 30) y = OCEAN_HEIGHT - 30;
    }
};

std::vector<Fish> fishArray;
Fish player(true);

void randomizeFishPositionAway(Fish& f) {
    if (f.isPlayer) return;
    float safeDist2 = SAFE_SPAWN_DISTANCE * SAFE_SPAWN_DISTANCE;
    for (int attempts = 0; attempts < 50; attempts++) {
        f.x = rand() % (WINDOW_WIDTH - 100);
        f.y = rand() % (WINDOW_HEIGHT - 100);
        float dx = f.x - player.x;
        float dy = f.y - player.y;
        if (dx * dx + dy * dy > safeDist2) break;
    }
}

bool checkCollision(const Fish& a, const Fish& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distanceSquared = dx * dx + dy * dy;

    float radiusA = a.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : a.getCollisionRadius();
    float radiusB = b.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : b.getCollisionRadius();
    float combinedRadius = radiusA + radiusB;

    return distanceSquared < combinedRadius * combinedRadius;
}

bool canEatFish(const Fish& player, const Fish& other) {
    float playerRadius = COLLISION_RADIUS * playerSizeScale;
    float otherRadius = other.getCollisionRadius();
    return playerRadius >= otherRadius * 0.95f;
}

// ==================== CHECK AND NOTIFY NEW EATABLE SIZES ====================
void checkNewEatableSizes() {
    float playerRadius = COLLISION_RADIUS * playerSizeScale;

    for (int i = 0; i < NUM_SIZE_TYPES; i++) {
        float fishRadius = COLLISION_RADIUS * SIZE_MULTIPLIERS[i];
        bool canNowEat = (playerRadius >= fishRadius * 0.95f);

        if (canNowEat && !canEatSize[i]) {
            canEatSize[i] = true;

            // Show notification above player
            if (!notifiedSize[i]) {
                notifiedSize[i] = true;
                char msg[64];
                snprintf(msg, sizeof(msg), "You can now eat %s!", SIZE_NAMES[i]);
                floatingTexts.push_back(FloatingText(player.x, player.y + 50, msg, 0.3f, 1.0f, 0.5f, true, 1.2f));
                spawnParticles(player.x, player.y, 0.5f, 1.0f, 0.5f, 10);
            }
        }
    }
}

// ==================== SIZE INDICATOR UI - SIMPLIFIED ====================
void drawSizeIndicator() {
    float startX = 10;
    float startY = WINDOW_HEIGHT - 35;
    float spacing = 100;

    // Background panel
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(5, startY - 25);
    glVertex2f(5 + NUM_SIZE_TYPES * spacing + 20, startY - 25);
    glVertex2f(5 + NUM_SIZE_TYPES * spacing + 20, startY + 25);
    glVertex2f(5, startY + 25);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Can Eat:", startX, startY + 10);

    float playerRadius = COLLISION_RADIUS * playerSizeScale;

    // Draw each size type with checkmark or X
    for (int i = 0; i < NUM_SIZE_TYPES; i++) {
        float iconX = startX + 60 + i * spacing;
        float iconY = startY;

        bool canEat = canEatSize[i];

        // Fish type name
        if (canEat) {
            glColor3f(0.3f, 1.0f, 0.3f);
        } else {
            glColor3f(1.0f, 0.3f, 0.3f);
        }

        drawText(SIZE_NAMES[i], iconX - 15, iconY + 8);

        // Checkmark or X
        if (canEat) {
            glColor3f(0.0f, 1.0f, 0.0f);
            glLineWidth(3.0f);
            glBegin(GL_LINES);
            glVertex2f(iconX - 10, iconY - 12);
            glVertex2f(iconX, iconY - 18);
            glVertex2f(iconX, iconY - 18);
            glVertex2f(iconX + 15, iconY - 5);
            glEnd();
            glLineWidth(1.0f);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            glVertex2f(iconX - 5, iconY - 8);
            glVertex2f(iconX + 10, iconY - 18);
            glVertex2f(iconX + 10, iconY - 8);
            glVertex2f(iconX - 5, iconY - 18);
            glEnd();
            glLineWidth(1.0f);
        }
    }

    // Player size display
    char sizeStr[32];
    snprintf(sizeStr, sizeof(sizeStr), "Your Size: %.1fx", playerSizeScale);
    glColor3f(0.5f, 0.8f, 1.0f);
    drawText(sizeStr, WINDOW_WIDTH - 120, WINDOW_HEIGHT - 25);
}

// ==================== MINIMAP ====================
void drawMinimap() {
    float mapX = WINDOW_WIDTH - 110;
    float mapY = OCEAN_HEIGHT - 110;
    float mapW = 100;
    float mapH = 80;

    glColor4f(0.0f, 0.0f, 0.3f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(mapX, mapY);
    glVertex2f(mapX + mapW, mapY);
    glVertex2f(mapX + mapW, mapY + mapH);
    glVertex2f(mapX, mapY + mapH);
    glEnd();

    glColor4f(0.5f, 0.5f, 1.0f, 0.8f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(mapX, mapY);
    glVertex2f(mapX + mapW, mapY);
    glVertex2f(mapX + mapW, mapY + mapH);
    glVertex2f(mapX, mapY + mapH);
    glEnd();

    for (const auto& fish : fishArray) {
        float fx = mapX + (fish.x / WINDOW_WIDTH) * mapW;
        float fy = mapY + (fish.y / OCEAN_HEIGHT) * mapH;

        if (fish.isRedFish) glColor3f(1.0f, 0.3f, 0.3f);
        else glColor3f(1.0f, 1.0f, 0.3f);

        glPointSize(2.0f + fish.sizeScale);
        glBegin(GL_POINTS);
        glVertex2f(fx, fy);
        glEnd();
    }

    float px = mapX + (player.x / WINDOW_WIDTH) * mapW;
    float py = mapY + (player.y / OCEAN_HEIGHT) * mapH;
    glColor3f(0.3f, 0.5f, 1.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(px, py);
    glEnd();

    if (powerUp.active) {
        float pux = mapX + (powerUp.x / WINDOW_WIDTH) * mapW;
        float puy = mapY + (powerUp.y / OCEAN_HEIGHT) * mapH;
        glColor3f(0.0f, 1.0f, 0.5f);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        glVertex2f(pux, puy);
        glEnd();
    }
}

// ==================== UI DRAWING ====================
void drawProgressBar(float x, float y, float w, float h, float value, float maxVal,
                     float r, float g, float b) {
    glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    float fillW = w * (value / maxVal);
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + fillW, y);
    glVertex2f(x + fillW, y + h);
    glVertex2f(x, y + h);
    glEnd();

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawHUD() {
    // Score panel (bottom left)
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(5, 5);
    glVertex2f(160, 5);
    glVertex2f(160, 55);
    glVertex2f(5, 55);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Score:", 10, 40);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawScore(55, 40, score);

    glColor3f(0.7f, 0.7f, 0.7f);
    drawText("High:", 90, 40);
    glColor3f(0.5f, 1.0f, 0.5f);
    drawScore(125, 40, highScore);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Level:", 10, 25);
    glColor3f(0.3f, 1.0f, 1.0f);
    drawScore(50, 25, currentLevel);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Fish:", 80, 25);
    glColor3f(1.0f, 0.8f, 0.3f);
    drawScore(110, 25, fishArray.size());

    // Time panel
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - 85, 5);
    glVertex2f(WINDOW_WIDTH - 5, 5);
    glVertex2f(WINDOW_WIDTH - 5, 40);
    glVertex2f(WINDOW_WIDTH - 85, 40);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Time:", WINDOW_WIDTH - 80, 25);
    glColor3f(gameTime > 10 ? 0.3f : 1.0f, gameTime > 10 ? 1.0f : 0.3f, 0.3f);
    drawScore(WINDOW_WIDTH - 40, 25, gameTime);

    // Biggest fish timer
    if (playerIsBiggest) {
        float barWidth = 200;
        float barX = WINDOW_WIDTH / 2 - barWidth / 2;
        float barY = 520;

        float pulse = 0.7f + 0.3f * std::sin(animTime * 5);

        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(barX - 10, barY - 25);
        glVertex2f(barX + barWidth + 10, barY - 25);
        glVertex2f(barX + barWidth + 10, barY + 20);
        glVertex2f(barX - 10, barY + 20);
        glEnd();

        glColor3f(pulse, 1.0f, pulse);
        drawTextMedium("BIGGEST FISH!", barX + 40, barY + 5);

        drawProgressBar(barX, barY - 20, barWidth, 15, biggestTimer, WIN_TIMER, 0.3f, 1.0f, 0.3f);

        char timeStr[32];
        snprintf(timeStr, sizeof(timeStr), "%.1fs", WIN_TIMER - biggestTimer);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(barX + barWidth / 2 - 15, barY - 17);
        for (int i = 0; timeStr[i]; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, timeStr[i]);
    }

    // Combo display
    if (comboCount > 1) {
        glColor4f(1.0f, 0.8f, 0.0f, comboTimer / COMBO_TIMEOUT);
        char comboStr[32];
        snprintf(comboStr, sizeof(comboStr), "COMBO x%d!", comboCount);
        drawTextMedium(comboStr, WINDOW_WIDTH / 2 - 40, 480);
    }

    // Power-up indicators
    float indicatorY = 60;
    float indicatorX = 170;
    if (speedBoostTimer > 0) {
        glColor3f(0.3f, 1.0f, 0.3f);
        drawText("SPEED", indicatorX, indicatorY);
        drawProgressBar(indicatorX + 45, indicatorY - 3, 40, 10, speedBoostTimer, POWERUP_DURATION, 0.3f, 1.0f, 0.3f);
        indicatorX += 100;
    }
    if (shieldTimer > 0) {
        glColor3f(0.3f, 0.5f, 1.0f);
        drawText("SHIELD", indicatorX, indicatorY);
        drawProgressBar(indicatorX + 50, indicatorY - 3, 40, 10, shieldTimer, POWERUP_DURATION, 0.3f, 0.5f, 1.0f);
        indicatorX += 105;
    }
    if (sizeBoostTimer > 0) {
        glColor3f(1.0f, 0.5f, 0.0f);
        drawText("SIZE+", indicatorX, indicatorY);
        drawProgressBar(indicatorX + 40, indicatorY - 3, 40, 10, sizeBoostTimer, POWERUP_DURATION, 1.0f, 0.5f, 0.0f);
    }
}

// ==================== MENU SCREEN ====================
void drawMenu() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH / 2 - 280, 120);
    glVertex2f(WINDOW_WIDTH / 2 + 280, 120);
    glVertex2f(WINDOW_WIDTH / 2 + 280, 480);
    glVertex2f(WINDOW_WIDTH / 2 - 280, 480);
    glEnd();

    float titleBob = std::sin(animTime * 2) * 10;
    glColor3f(0.3f, 0.7f, 1.0f);
    drawCenteredText("FISH FRENZY", 430 + titleBob);

    glColor3f(1.0f, 1.0f, 0.5f);
    drawCenteredText("Press SPACE to Start", 350, GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.8f, 0.8f, 0.8f);
    drawCenteredText("Controls:", 300, GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.7f, 0.7f, 0.7f);
    drawCenteredText("Mouse - Move your fish", 280, GLUT_BITMAP_HELVETICA_12);
    drawCenteredText("P - Pause game", 260, GLUT_BITMAP_HELVETICA_12);
    drawCenteredText("F2 - Restart game", 240, GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.5f, 1.0f, 0.8f);
    drawCenteredText("Goal: Become the BIGGEST fish for 10 seconds to WIN!", 200, GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.8f, 0.8f, 0.5f);
    drawCenteredText("Eat smaller fish to grow! Each fish type has unique shape!", 180, GLUT_BITMAP_HELVETICA_12);

    if (highScore > 0) {
        glColor3f(0.5f, 1.0f, 0.5f);
        char hsStr[64];
        snprintf(hsStr, sizeof(hsStr), "High Score: %d", highScore);
        drawCenteredText(hsStr, 150, GLUT_BITMAP_HELVETICA_12);
    }
}

// ==================== PAUSE SCREEN ====================
void drawPause() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCenteredText("PAUSED", 350);

    glColor3f(0.8f, 0.8f, 0.8f);
    drawCenteredText("Press P to Resume", 300, GLUT_BITMAP_HELVETICA_18);
    drawCenteredText("Press F2 to Restart", 270, GLUT_BITMAP_HELVETICA_18);
}

// ==================== GAME OVER SCREEN ====================
void drawGameOver() {
    if (!soundPlayed) {
        soundPlayed = true;
        if (score > highScore) highScore = score;
    }

    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(200, 100);
    glVertex2f(1000, 100);
    glVertex2f(1000, 500);
    glVertex2f(200, 500);
    glEnd();

    if (allYellowFishGone) {
        glColor3f(0.3f, 1.0f, 0.3f);
        drawCenteredText("VICTORY!", 420);

        glColor3f(0.5f, 1.0f, 0.8f);
        drawCenteredText("You became the biggest fish!", 370, GLUT_BITMAP_HELVETICA_18);
    } else {
        glColor3f(1.0f, 0.3f, 0.3f);
        drawCenteredText("GAME OVER", 420);
    }

    char scoreStr[64];
    snprintf(scoreStr, sizeof(scoreStr), "Final Score: %d", score);
    glColor3f(1.0f, 1.0f, 0.5f);
    drawCenteredText(scoreStr, 320, GLUT_BITMAP_HELVETICA_18);

    snprintf(scoreStr, sizeof(scoreStr), "Level Reached: %d", currentLevel);
    glColor3f(0.3f, 0.8f, 1.0f);
    drawCenteredText(scoreStr, 290, GLUT_BITMAP_HELVETICA_18);

    snprintf(scoreStr, sizeof(scoreStr), "Final Size: %.2fx", playerSizeScale);
    glColor3f(0.5f, 0.8f, 1.0f);
    drawCenteredText(scoreStr, 260, GLUT_BITMAP_HELVETICA_18);

    if (score >= highScore && score > 0) {
        float flash = 0.5f + 0.5f * std::sin(animTime * 5);
        glColor3f(1.0f, flash, 0.0f);
        drawCenteredText("NEW HIGH SCORE!", 220, GLUT_BITMAP_HELVETICA_18);
    }

    glColor3f(0.8f, 0.8f, 0.8f);
    drawCenteredText("Press F2 to Play Again", 160, GLUT_BITMAP_HELVETICA_18);
    drawCenteredText("Press SPACE for Menu", 130, GLUT_BITMAP_HELVETICA_18);
}

// ==================== INITIALIZATION ====================
void initGame() {
    glClearColor(0.07f, 0.01f, 0.75f, 1.0f);

    playerSizeScale = INITIAL_PLAYER_SIZE;
    prevMouseX = WINDOW_WIDTH / 2.0f;
    player.x = WINDOW_WIDTH / 2.0f;
    player.y = WINDOW_HEIGHT / 2.0f;

    score = 0;
    gameTime = INITIAL_TIME;
    currentLevel = 1;
    fishEatenThisLevel = 0;
    comboCount = 0;
    comboTimer = 0;
    allYellowFishGone = false;
    soundPlayed = false;

    playerIsBiggest = false;
    biggestTimer = 0.0f;

    // Reset eatable sizes tracking
    canEatSize[0] = true;  // Can eat small from start
    canEatSize[1] = false;
    canEatSize[2] = false;
    notifiedSize[0] = true;  // Already know about small
    notifiedSize[1] = false;
    notifiedSize[2] = false;

    speedBoostTimer = 0;
    shieldTimer = 0;
    sizeBoostTimer = 0;
    powerUpSpawnTimer = 0;
    powerUp.active = false;

    shakeIntensity = 0;

    particles.clear();
    floatingTexts.clear();

    fishArray.clear();
    for (int i = 0; i < NUM_FISH; ++i) {
        Fish f;
        randomizeFishPositionAway(f);
        fishArray.push_back(f);
    }

    if (bubbles.empty()) {
        for (int i = 0; i < BUBBLE_COUNT; i++) {
            bubbles.push_back(Bubble());
        }
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}

void resetToMenu() {
    currentState = MENU;
    initGame();
}

// ==================== INPUT HANDLERS ====================
void mouseMove(int x, int y) {
    if (currentState != PLAYING) return;

    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(y);
    player.move(mouseX, mouseY, prevMouseX);
    prevMouseX = mouseX;
}

void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_F2) {
        initGame();
        currentState = PLAYING;
    }
    glutPostRedisplay();
}

void keyboardNormal(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (currentState == MENU) {
            initGame();
            currentState = PLAYING;
        } else if (currentState == GAME_OVER) {
            resetToMenu();
        }
    }
    if (key == 'p' || key == 'P') {
        if (currentState == PLAYING) currentState = PAUSED;
        else if (currentState == PAUSED) currentState = PLAYING;
    }
    if (key == 27) {  // ESC
        if (currentState == PLAYING) currentState = PAUSED;
        else if (currentState == PAUSED) resetToMenu();
    }

    glutPostRedisplay();
}

// ==================== TIMERS ====================
void animationTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, animationTimer, 0);
}

void gameTimer(int value) {
    if (currentState == PLAYING && gameTime > 0) {
        --gameTime;
        if (gameTime == 0) {
            currentState = GAME_OVER;
            soundPlayed = false;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000, gameTimer, 0);
}

// ==================== CHECK IF PLAYER IS BIGGEST ====================
bool checkPlayerIsBiggest() {
    float playerRadius = COLLISION_RADIUS * playerSizeScale;
    for (const auto& fish : fishArray) {
        if (!fish.isPlayer) {
            float fishRadius = fish.getCollisionRadius();
            if (fishRadius >= playerRadius) {
                return false;
            }
        }
    }
    return fishArray.size() > 0;
}

// ==================== MAIN DISPLAY ====================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    animTime += 0.033f;
    coralPhase += CORAL_SWAY_SPEED;

    // Screen shake
    if (shakeIntensity > 0) {
        shakeOffsetX = (rand() % 100 / 50.0f - 1.0f) * shakeIntensity;
        shakeOffsetY = (rand() % 100 / 50.0f - 1.0f) * shakeIntensity;
        shakeIntensity *= 0.9f;
        if (shakeIntensity < 0.5f) shakeIntensity = 0;
    } else {
        shakeOffsetX = shakeOffsetY = 0;
    }

    drawOcean();
    drawCoral();

    // Bubbles
    glPushMatrix();
    glTranslatef(shakeOffsetX, shakeOffsetY, 0);
    for (auto& bubble : bubbles) {
        if (currentState == PLAYING) bubble.update();
        bubble.draw();
    }
    glPopMatrix();

    if (currentState == MENU) {
        drawMenu();
    }
    else if (currentState == PLAYING || currentState == PAUSED) {
        glPushMatrix();
        glTranslatef(shakeOffsetX, shakeOffsetY, 0);

        if (currentState == PLAYING) {
            // Update timers
            if (speedBoostTimer > 0) speedBoostTimer -= 0.033f;
            if (shieldTimer > 0) shieldTimer -= 0.033f;
            if (sizeBoostTimer > 0) {
                sizeBoostTimer -= 0.033f;
                if (sizeBoostTimer <= 0) {
                    playerSizeScale = std::max(INITIAL_PLAYER_SIZE, playerSizeScale - 0.5f);
                }
            }

            if (comboTimer > 0) {
                comboTimer -= COMBO_DECAY;
                if (comboTimer <= 0) comboCount = 0;
            }

            powerUpSpawnTimer += 0.033f;
            if (!powerUp.active && powerUpSpawnTimer > 8.0f + (rand() % 100) / 20.0f) {
                powerUp.spawn();
                powerUpSpawnTimer = 0;
            }

            // Check for new eatable sizes
            checkNewEatableSizes();

            // Check if player is biggest
            playerIsBiggest = checkPlayerIsBiggest();

            if (playerIsBiggest) {
                biggestTimer += 0.033f;
                if (biggestTimer >= WIN_TIMER) {
                    allYellowFishGone = true;
                    currentState = GAME_OVER;
                    soundPlayed = false;
                }
            } else {
                biggestTimer = 0.0f;
            }
        }

        powerUp.draw();

        // Power-up collision
        if (currentState == PLAYING && powerUp.active) {
            float pr = COLLISION_RADIUS * playerSizeScale;
            if (powerUp.checkCollision(player.x, player.y, pr)) {
                spawnParticles(powerUp.x, powerUp.y, 0.5f, 1.0f, 0.5f, 20);

                switch (powerUp.type) {
                    case POWER_SPEED:
                        speedBoostTimer = POWERUP_DURATION;
                        floatingTexts.push_back(FloatingText(powerUp.x, powerUp.y, "SPEED!", 0.3f, 1.0f, 0.3f));
                        break;
                    case POWER_SHIELD:
                        shieldTimer = POWERUP_DURATION;
                        floatingTexts.push_back(FloatingText(powerUp.x, powerUp.y, "SHIELD!", 0.3f, 0.5f, 1.0f));
                        break;
                    case POWER_SIZE:
                        sizeBoostTimer = POWERUP_DURATION;
                        playerSizeScale = std::min(MAX_PLAYER_SIZE, playerSizeScale + 0.5f);
                        floatingTexts.push_back(FloatingText(powerUp.x, powerUp.y, "SIZE UP!", 1.0f, 0.5f, 0.0f));
                        break;
                    case POWER_TIME:
                        gameTime += 15;
                        floatingTexts.push_back(FloatingText(powerUp.x, powerUp.y, "+15 SEC!", 1.0f, 1.0f, 0.3f));
                        break;
                }
                powerUp.active = false;
            }
        }

        player.draw();

        // Fish logic
        float playerRadius = COLLISION_RADIUS * playerSizeScale;
        for (int i = 0; i < static_cast<int>(fishArray.size()); ++i) {
            if (currentState == PLAYING) {
                fishArray[i].moveWithBehavior(player.x, player.y, playerRadius);
            }

            if (checkCollision(player, fishArray[i]) && currentState == PLAYING) {
                Fish& fish = fishArray[i];

                if (fish.isRedFish) {
                    if (!canEatFish(player, fish) && shieldTimer <= 0) {
                        shakeIntensity = 15.0f;
                        currentState = GAME_OVER;
                        soundPlayed = false;
                        break;
                    } else if (canEatFish(player, fish) || shieldTimer > 0) {
                        int points = 5 * (comboCount > 0 ? comboCount : 1);
                        score += points;

                        comboCount++;
                        comboTimer = COMBO_TIMEOUT;

                        spawnParticles(fish.x, fish.y, 1.0f, 0.3f, 0.3f, 20);

                        char pointStr[16];
                        snprintf(pointStr, sizeof(pointStr), "+%d", points);
                        floatingTexts.push_back(FloatingText(fish.x, fish.y, pointStr, 1.0f, 1.0f, 0.0f));

                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT * 2.0f;
                        }

                        fishEatenThisLevel++;
                        fishArray.erase(fishArray.begin() + i);
                        --i;
                    }
                } else {
                    if (canEatFish(player, fish)) {
                        int basePoints = 1;
                        if (fish.sizeType == MEDIUM) basePoints = 3;
                        else if (fish.sizeType == LARGE) basePoints = 6;

                        int points = basePoints * (comboCount > 0 ? comboCount : 1);
                        score += points;

                        comboCount++;
                        comboTimer = COMBO_TIMEOUT;

                        spawnParticles(fish.x, fish.y, 1.0f, 1.0f, 0.3f);

                        char pointStr[16];
                        snprintf(pointStr, sizeof(pointStr), "+%d", points);
                        floatingTexts.push_back(FloatingText(fish.x, fish.y, pointStr, 1.0f, 1.0f, 0.0f));

                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT * (1 + fish.sizeType * 0.5f);
                        }

                        fishEatenThisLevel++;
                        fishArray.erase(fishArray.begin() + i);
                        --i;
                    } else if (shieldTimer <= 0) {
                        shakeIntensity = 15.0f;
                        currentState = GAME_OVER;
                        soundPlayed = false;
                        break;
                    }
                }
            } else {
                fishArray[i].draw();
            }
        }

        // Level progression
        if (fishEatenThisLevel >= FISH_PER_LEVEL) {
            currentLevel++;
            fishEatenThisLevel = 0;
            gameTime += 15;

            floatingTexts.push_back(
                FloatingText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "LEVEL UP!", 0.3f, 1.0f, 1.0f, true, 1.5f));

            int newFish = NUM_FISH + currentLevel * 2;
            for (int i = fishArray.size(); i < newFish; ++i) {
                Fish f;
                f.isRedFish = (rand() % 10 < 2 + currentLevel);
                randomizeFishPositionAway(f);
                fishArray.push_back(f);
            }
        }

        // Spawn new fish if too few
        if (fishArray.size() < 5 && currentState == PLAYING) {
            for (int i = 0; i < 5; i++) {
                Fish f;
                randomizeFishPositionAway(f);
                fishArray.push_back(f);
            }
        }

        // Particles
        for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
            particles[i].update();
            particles[i].draw();
            if (particles[i].isDead()) {
                particles.erase(particles.begin() + i);
                --i;
            }
        }

        // Floating texts
        for (int i = 0; i < static_cast<int>(floatingTexts.size()); ++i) {
            floatingTexts[i].update();
            floatingTexts[i].draw();
            if (floatingTexts[i].isDead()) {
                floatingTexts.erase(floatingTexts.begin() + i);
                --i;
            }
        }

        glPopMatrix();

        drawSizeIndicator();
        drawHUD();
        drawMinimap();

        if (currentState == PAUSED) {
            drawPause();
        }
    }
    else if (currentState == GAME_OVER) {
        glPushMatrix();
        glTranslatef(shakeOffsetX, shakeOffsetY, 0);
        player.draw();
        for (auto& fish : fishArray) {
            fish.draw();
        }
        glPopMatrix();

        drawGameOver();
    }

    glDisable(GL_BLEND);
    glutSwapBuffers();
}

// ==================== MAIN ====================
int main(int argc, char *argv[]) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fish Frenzy");

    initGame();

    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseMove);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardNormal);
    glutTimerFunc(0, animationTimer, 0);
    glutTimerFunc(0, gameTimer, 0);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
