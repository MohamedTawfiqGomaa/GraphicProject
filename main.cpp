
// ============================================
// المكتبات الضرورية فقط
// ============================================
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <ctime>
#include <cstdio>
#include <vector>

// ============================================
// الثوابت الأساسية
// ============================================
const int WINDOW_WIDTH = 1200; // عرض النافذة
const int WINDOW_HEIGHT = 600; // ارتفاع النافذة
const int OCEAN_HEIGHT = 500; // ارتفاع المحيط
const int NUM_FISH = 20; // عدد الاسماك
const int INITIAL_TIME = 50; // الوقت الابتدائي
const float FISH_SIZE = 20.0f; // حجم الاسماك
const float COLLISION_RADIUS = 15.0f; // نصف قطر التصادم
const float FISH_SPEED = 0.5f;
const float PI = 3.1415926f; // قيمة باي

// نمو اللاعب
const float GROWTH_INCREMENT = 0.05f; // نمو اللاعب
const float INITIAL_PLAYER_SIZE = 1.0f; // حجم اللاعب الابتدائي
const float MAX_PLAYER_SIZE = 2.5f; // حجم اللاعب الاقصى

// سلوك الأسماك
const float FLEE_DISTANCE = 200.0f; // المسافة الاقصى للهروب
const float CHASE_DISTANCE = 250.0f; // المسافة الاقصى للمطاردة
const float FLEE_SPEED = 1.5f; // سرعة الهروب
const float CHASE_SPEED = 1.3f; // سرعة المطاردة

// الأمواج
const int WAVE_POINTS = 50; // عدد النقاط للموجة
float waveOffset = 0.0f; // التحرك الأفقي للموجة

// ============================================
// متغيرات اللعبة
// ============================================
bool isGameOver = false; // اللعبة منتهية
int score = 0; // النقاط
int gameTime = INITIAL_TIME; // الوقت الابتدائي
float playerSizeScale = INITIAL_PLAYER_SIZE; // حجم اللاعب
float prevMouseX = WINDOW_WIDTH / 2.0f; // الموقع الافقي للماوس
bool allFishCollected = false; // جميع الاسماك مجموعة

// ============================================
// دوال عرض النصوص
//شهد
// ============================================
void drawText(const char *str, int x, int y) {
    glRasterPos2d(x, y);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str);//رسم النص بواسطة الخط العريض
        str++;//التحرك للنص التالي 
    }
}

void drawLargeText(const char *str, int x, int y) {//رسم النص بواسطة الخط العريض
    glRasterPos2d(x, y);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);//رسم النص بواسطة الخط العريض
        str++;//التحرك للنص التالي للتالي
    }
}

void drawNumber(int x, int y, int value) {//رسم الرقم
    char buffer[32];
    sprintf(buffer, "%d", value);//تحويل الرقم الى نص   
    drawText(buffer, x, y);//رسم النص بواسطة الخط العريض    
}

void drawFloat(int x, int y, float value) {//رسم الرقم
    char buffer[32];
    sprintf(buffer, "%.2fx", value);//تحويل الرقم الى نص   
    drawText(buffer, x, y);//رسم النص بواسطة الخط العريض    
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^

// ============================================
// رسم المحيط مع أمواج واقعية
//محمد سعيد كامل
// ============================================
void drawOcean() {//رسم المحيط  
    // خلفية المحيط بتدرج بسيط
    glBegin(GL_QUADS);//رسم المستطيل
    glColor3f(0.1f, 0.4f, 0.8f);//اللون الاول
    glVertex2f(0, 0);//النقطة الاولى
    glVertex2f(WINDOW_WIDTH, 0);//النقطة الثانية
    glColor3f(0.05f, 0.2f, 0.5f);//اللون الثاني
    glVertex2f(WINDOW_WIDTH, OCEAN_HEIGHT);//النقطة الثالثة
    glVertex2f(0, OCEAN_HEIGHT);//النقطة الرابعة
    glEnd();//نهاية الرسم

    // الموجة الأولى (الخلفية - بطيئة)
    glColor4f(0.15f, 0.45f, 0.75f, 0.6f);//اللون الاول
    glLineWidth(2.0f);//عرض الخط
    glBegin(GL_LINE_STRIP);//رسم الخط
    for (int i = 0; i <= WAVE_POINTS; i++) {//التكرار لرسم الموجة
        float x = (WINDOW_WIDTH / (float)WAVE_POINTS) * i;//الموقع الافقي للموجة
        float y = OCEAN_HEIGHT + sin((x * 0.01f) + (waveOffset * 0.5f)) * 8.0f +//الموقع العمودي للموجة
                  sin((x * 0.02f) - (waveOffset * 0.3f)) * 4.0f;//الموقع العمودي للموجة
        glVertex2f(x, y);//النقطة الاولى
    }
    glEnd();//نهاية الرسم
    // الموجة الثانية (الوسطى - متوسطة)
    glColor4f(0.2f, 0.55f, 0.85f, 0.7f);//اللون الاول
    glLineWidth(2.5f);//عرض الخط
    glBegin(GL_LINE_STRIP);//رسم الخط
    for (int i = 0; i <= WAVE_POINTS; i++) {//التكرار لرسم الموجة
        float x = (WINDOW_WIDTH / (float)WAVE_POINTS) * i;//الموقع الافقي للموجة
        float y = OCEAN_HEIGHT + sin((x * 0.015f) + (waveOffset * 0.8f)) * 10.0f +//الموقع العمودي للموجة
                  sin((x * 0.03f) + (waveOffset * 0.4f)) * 5.0f;//الموقع العمودي للموجة
        glVertex2f(x, y);//النقطة الاولى
    }
    glEnd();//نهاية الرسم
    // الموجة الثالثة (الأمامية - سريعة)
    glColor4f(0.25f, 0.65f, 0.95f, 0.9f);
    glLineWidth(3.0f);//عرض الخط
    glBegin(GL_LINE_STRIP);//رسم الخط
    for (int i = 0; i <= WAVE_POINTS; i++) {//التكرار لرسم الموجة
        float x = (WINDOW_WIDTH / (float)WAVE_POINTS) * i;//الموقع الافقي للموجة
        float y = OCEAN_HEIGHT + sin((x * 0.02f) + waveOffset) * 12.0f +//الموقع العمودي للموجة
                  sin((x * 0.04f) - (waveOffset * 0.7f)) * 6.0f +//الموقع العمودي للموجة
                  sin((x * 0.08f) + (waveOffset * 1.5f)) * 3.0f;//الموقع العمودي للموجة
        glVertex2f(x, y);//النقطة الاولى
    }
    glEnd();//نهاية الرسم
    // تحريك الأمواج
    waveOffset += 0.03f;
    if (waveOffset > 2.0f * PI * 100.0f) {
        waveOffset = 0.0f;
    }
}
//^^^^^^^^^^^^^^^^^

// ============================================
// فئة السمكة
// ============================================
class Fish {
public:
    bool isPlayer;//اللاعب
    bool isRedFish;//الاسمك الاحمر
    bool isLarge;//الاسمك الكبير
    float x, y;//الموقع الافقي والعمودي
    float direction;//الاتجاه
    float sizeScale;//حجم الاسمك

    Fish(bool player = false) : isPlayer(player) {//الاسمك
        if (isPlayer) {
            x = WINDOW_WIDTH / 2.0f;//الموقع الافقي
            y = WINDOW_HEIGHT / 2.0f;//الموقع العمودي   
            sizeScale = INITIAL_PLAYER_SIZE;//حجم الاسمك
            isLarge = false;//الاسمك الكبير
            direction = FISH_SPEED;//الاتجاه
        } else {
            isRedFish = (rand() % 10 < 3);  // 30% حمراء  الاسمك الاحمر
            isLarge = (rand() % 10 < 3);     // 30% كبيرة  الاسمك الكبير
            sizeScale = isLarge ? 1.5f : 0.9f;
            x = rand() % WINDOW_WIDTH;//الموقع الافقي
            y = rand() % (OCEAN_HEIGHT - 50);//الموقع العمودي
            direction = (rand() % 2) ? FISH_SPEED : -FISH_SPEED;//الاتجاه
        }
    }

    float getCollisionRadius() const {
        return COLLISION_RADIUS * sizeScale;//نصف قطر التصادم
    }//نصف قطر التصادم

    void draw() {//رسم الاسمك
        // تحديد اللون
        float r, g, b;
        if (isPlayer) {//اللاعب
            r = 0.2f; g = 0.4f; b = 1.0f;  // أزرق اللون
        } else if (isRedFish) {//الاسمك الاحمر
            r = 1.0f; g = 0.3f; b = 0.3f;  // أحمر
        } else {//الاسمك الاحمر
            r = 1.0f; g = 0.9f; b = 0.2f;  // أصفر
        }

        float currentScale = isPlayer ? playerSizeScale : sizeScale; //حجم الاسمك
        float dir = (direction < 0) ? -1.0f : 1.0f; //الاتجاه
        float bodyLength = FISH_SIZE * 1.8f * currentScale; //طول الجسم
        float bodyHeight = FISH_SIZE * 1.2f * currentScale; //عرض الجسم

        // رسم الجسم
        glColor3f(r, g, b); //اللون
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 20; i++) { //التكرار لرسم الجسم
            float angle = PI * float(i) / 20.0f; //الزاوية
            float px = x + dir * (bodyLength * 0.4f * cos(angle)); //الموقع الافقي
            float py = y + bodyHeight * 0.5f * sin(angle); //الموقع العمودي
            glVertex2f(px, py); //النقطة الاولى
        }
        glEnd(); //نهاية الرسم

        // رسم الذيل
        float tailBaseX = x - dir * bodyLength * 0.5f; //الموقع الافقي
        float tailLength = FISH_SIZE * 0.6f * currentScale; //طول الذيل
        glColor3f(r * 0.3f, g * 0.8f, b * 0.3f); //اللون
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(tailBaseX, y); //النقطة الاولى
        for (int i = 0; i <= 8; i++) {
            float angle = (PI / 3.0f) * float(i) / 8.0f - (PI / 6.0f); //الزاوية
            float px = tailBaseX - dir * tailLength * cos(angle); //الموقع الافقي
            float py = y + tailLength * sin(angle); //الموقع العمودي
            glVertex2f(px, py); //النقطة الاولى
        }
        glEnd(); //نهاية الرسم

        // رسم العين
        float eyeX = x + dir * bodyLength * 0.35f; //الموقع الافقي
        float eyeY = y + bodyHeight * 0.15f; //الموقع العمودي
        float eyeSize = FISH_SIZE * 0.15f * currentScale; //حجم العين

        // بياض العين
        glColor3f(1.0f, 1.0f, 1.0f); //اللون
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f; //الزاوية
            glVertex2f(eyeX + eyeSize * cos(angle), eyeY + eyeSize * sin(angle)); //النقطة الاولى
        }
        glEnd(); //نهاية الرسم

        // بؤبؤ العين
        glColor3f(0.0f, 0.0f, 0.0f); //اللون
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f; //الزاوية
            glVertex2f(eyeX + eyeSize * 0.6f * cos(angle), eyeY + eyeSize * 0.6f * sin(angle)); //النقطة الاولى
        }
        glEnd(); //نهاية الرسم
    }

    void move(float mouseX = 0.0f, float mouseY = 0.0f, float prevX = 0.0f) {//التحرك
        if (!isPlayer) {
            x += direction; //الموقع الافقي
            if (x > WINDOW_WIDTH) {
                x = 0; //الموقع الافقي
                y = rand() % (OCEAN_HEIGHT - 50); //الموقع العمودي
            }
            if (x < 0) {
                x = WINDOW_WIDTH; //الموقع الافقي
                y = rand() % (OCEAN_HEIGHT - 50); //الموقع العمودي
            }
        } else {
            if (mouseX > prevX) direction = FISH_SPEED; //الاتجاه
            else if (mouseX < prevX) direction = -FISH_SPEED; //الاتجاه

            x = mouseX; //الموقع الافقي
            y = WINDOW_HEIGHT - mouseY; //الموقع العمودي
        }
    }

    void moveWithBehavior(float playerX, float playerY, float playerRadius) {
        if (isPlayer) return; //اللاعب

        float dx = playerX - x; //الموقع الافقي
        float dy = playerY - y; //الموقع العمودي
        float dist = sqrt(dx * dx + dy * dy); //المسافة

        float myRadius = getCollisionRadius(); //نصف قطر التصادم
        float stepX = direction; //الموقع الافقي
        float stepY = 0.0f; //الموقع العمودي

        // الهروب من اللاعب الأكبر
        if (playerRadius > myRadius && dist < FLEE_DISTANCE) { //الهروب من اللاعب الأكبر
            stepX = -(dx / dist) * FISH_SPEED * FLEE_SPEED;
            stepY = -(dy / dist) * FISH_SPEED * FLEE_SPEED;
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED; //الاتجاه
        }
        // مطاردة اللاعب الأصغر
        else if ((isRedFish || isLarge) && playerRadius < myRadius && dist < CHASE_DISTANCE) { //مطاردة اللاعب الأصغر
            stepX = (dx / dist) * FISH_SPEED * CHASE_SPEED; //الموقع الافقي
            stepY = (dy / dist) * FISH_SPEED * CHASE_SPEED; //الموقع العمودي    
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED; //الاتجاه
        }

        x += stepX; //الموقع الافقي
        y += stepY; //الموقع العمودي

        // لف أفقي
        if (x > WINDOW_WIDTH) x = 0; //الموقع الافقي
        if (x < 0) x = WINDOW_WIDTH; //الموقع الافقي

        // تحديد عمودي
        if (y < 20) y = 20; //الموقع العمودي
        if (y > OCEAN_HEIGHT - 20) y = OCEAN_HEIGHT - 20; //الموقع العمودي
    }
};
    

// ============================================
// كائنات اللعبة
// ============================================
std::vector<Fish> fishArray;//الاسماك
Fish player(true);//اللاعب

// ============================================
// كشف التصادم
//مروه
// ============================================
//
bool checkCollision(const Fish& a, const Fish& b) {//التصادم
    float dx = a.x - b.x;//الموقع الافقي
    float dy = a.y - b.y;//الموقع العمودي
    float distSq = dx * dx + dy * dy;//المسافة

    float radiusA = a.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : a.getCollisionRadius();//نصف قطر التصادم
    float radiusB = b.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : b.getCollisionRadius();//نصف قطر التصادم
    float combinedRadius = radiusA + radiusB;//نصف قطر التصادم

    return distSq < (combinedRadius * combinedRadius);//التصادم
}

bool canEatFish(const Fish& player, const Fish& other) {//التصادم
    float playerRadius = COLLISION_RADIUS * playerSizeScale;//نصف قطر التصادم
    float otherRadius = other.getCollisionRadius();//نصف قطر التصادم
    return playerRadius >= otherRadius * 0.95f;//التصادم
}
////^^^^^^^^^^^

// ============================================
// تهيئة اللعبة
//شهد
// ============================================
void initGame() {//التهيئة اللعبة
    glClearColor(0.07f, 0.01f, 0.75f, 1.0f);//اللون الاول والثاني والثالث والرابع   
    playerSizeScale = INITIAL_PLAYER_SIZE;//حجم الاسمك
    prevMouseX = WINDOW_WIDTH / 2.0f;//الموقع الافقي

    fishArray.clear();//مسح الاسماك
    for (int i = 0; i < NUM_FISH; ++i) {//التكرار لرسم الاسماك
        fishArray.push_back(Fish());//رسم الاسمك
    }

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// ============================================
// التحكم
//محمد سعيد كامل
// ============================================
void mouseMove(int x, int y) {//التحرك بالماوس
    float mouseX = static_cast<float>(x);//الموقع الافقي
    float mouseY = static_cast<float>(y);//الموقع العمودي
    player.move(mouseX, mouseY, prevMouseX);//التحرك الافقي
    prevMouseX = mouseX;//الموقع الافقي
}
//^^^^^^^^^^^^^^^^^^^^^^^
void keyboard(int key, int x, int y) {//التحرك باللوحة
    if (key == GLUT_KEY_F2) {
        isGameOver = false;//اللعبة المنتهية
        gameTime = INITIAL_TIME;//الوقت الابتدائي
        score = 0;//النقاط
        allFishCollected = false;//الاسماك المجمعة
        playerSizeScale = INITIAL_PLAYER_SIZE;//حجم الاسمك
        prevMouseX = WINDOW_WIDTH / 2.0f;//الموقع الافقي

        fishArray.clear();//مسح الاسماك
        for (int i = 0; i < NUM_FISH; ++i) {//التكرار لرسم الاسماك
            fishArray.push_back(Fish());//رسم الاسمك
        }
    }
    glutPostRedisplay();//اعادة العرض
}

// ============================================
// المؤقتات
// ساره
// ============================================
void animationTimer(int value) {//التحرك بالمؤقت
    glutPostRedisplay();
    glutTimerFunc(50, animationTimer, 0);
}

void gameTimer(int value) {//التحرك بالمؤقت
    if (!isGameOver && gameTime > 0) {
        --gameTime;//الوقت المنقضي
        if (gameTime == 0) {//الوقت المنقضي
            isGameOver = true;//اللعبة المنتهية
        }
    }
    glutPostRedisplay();//اعادة العرض
    glutTimerFunc(1000, gameTimer, 0);
}
/// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// ============================================
// العرض الرئيسي
// ============================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);//مسح اللون

    glEnable(GL_BLEND);//تمكين التداخل
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//التداخل

    drawOcean();//رسم المحيط

    if (!isGameOver) {
        // خلفية UI
        glColor3f(0.0f, 0.0f, 0.0f);//اللون الاول والثاني والثالث
        glBegin(GL_QUADS);
        glVertex2f(0, 560);//النقطة الاولى
        glVertex2f(150, 560);//النقطة الثانية
        glVertex2f(150, 600);//النقطة الثالثة
        glVertex2f(0, 600);//النقطة الرابعة
        glEnd();//نهاية الرسم

        glBegin(GL_QUADS);
        glVertex2f(1050, 560);//النقطة الاولى
        glVertex2f(1200, 560);//النقطة الثانية
        glVertex2f(1200, 600);//النقطة الثالثة
        glVertex2f(1050, 600);//النقطة الرابعة  
        glEnd();//نهاية الرسم

        // النصوص
        glColor3f(1.0f, 1.0f, 1.0f);//اللون الاول والثاني والثالث
        drawText("Score:", 10, 585);//النص  
        glColor3f(1.0f, 1.0f, 0.0f);//اللون الاول والثاني والثالث
        drawNumber(60, 585, score);//الرقم

        glColor3f(1.0f, 1.0f, 1.0f);//اللون الاول والثاني والثالث
        drawText("Size:", 10, 570);//النص  
        glColor3f(0.3f, 1.0f, 0.8f);//اللون الاول والثاني والثالث
        drawFloat(50, 570, playerSizeScale);//الرقم

        glColor3f(1.0f, 1.0f, 1.0f);//اللون الاول والثاني والثالث       
        drawText("Time:", 1070, 575);//النص  
        glColor3f(0.3f, 1.0f, 0.3f);//اللون الاول والثاني والثالث
        drawNumber(1120, 575, gameTime);//الرقم

        // رسم اللاعب
        player.draw();//رسم اللاعب

        // تحريك ورسم الأسماك
        float playerRadius = COLLISION_RADIUS * playerSizeScale;//نصف قطر التصادم

        for (size_t i = 0; i < fishArray.size(); ++i) {
            fishArray[i].moveWithBehavior(player.x, player.y, playerRadius);//التحرك بالسلوك

            if (checkCollision(player, fishArray[i])) {//التصادم
                if (fishArray[i].isRedFish) {
                    if (!canEatFish(player, fishArray[i])) {//التصادم
                        isGameOver = true;
                        break;
                    } else {
                        score += 3;//النقاط
                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT * 2.0f;//حجم الاسمك
                            if (playerSizeScale > MAX_PLAYER_SIZE)
                                playerSizeScale = MAX_PLAYER_SIZE;//حجم الاسمك
                        }
                        fishArray.erase(fishArray.begin() + i);//مسح الاسمك
                        --i;//التكرار لرسم الاسمك
                    }
                } else {
                    if (canEatFish(player, fishArray[i])) {//التصادم
                        int points = fishArray[i].isLarge ? 2 : 1;//النقاط
                        score += points;//النقاط

                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT;//حجم الاسمك
                            if (playerSizeScale > MAX_PLAYER_SIZE)
                                playerSizeScale = MAX_PLAYER_SIZE;//حجم الاسمك
                        }

                        fishArray.erase(fishArray.begin() + i);//مسح الاسمك
                        --i;//التكرار لرسم الاسمك
                    } else {
                        isGameOver = true;//اللعبة المنتهية
                        break;
                    }
                }
            } else {
                fishArray[i].draw();//رسم الاسمك
            }
        }

        // فحص الفوز
        if (fishArray.empty()) {
            allFishCollected = true;//الاسماك المجمعة
            isGameOver = true;//اللعبة المنتهية
        }

    } else {
        // شاشة نهاية اللعبة
        glColor3f(0.0f, 0.0f, 0.0f);//اللون الاول والثاني والثالث   
        glBegin(GL_QUADS);
        glVertex2f(200, 150);//النقطة الاولى
        glVertex2f(1000, 150);//النقطة الثانية
        glVertex2f(1000, 450);//النقطة الثالثة
        glVertex2f(200, 450);//النقطة الرابعة
        glEnd();//نهاية الرسم

        if (allFishCollected) {
            glColor3f(0.3f, 1.0f, 0.3f);//اللون الاول والثاني والثالث
            drawLargeText("YOU WIN!", 520, 400);//النص
            glColor3f(1.0f, 1.0f, 0.5f);
            drawLargeText("Final Score:", 480, 280);//النص
            glColor3f(1.0f, 1.0f, 0.0f);//اللون الاول والثاني والثالث
            drawNumber(640, 280, score);//الرقم
        } else {
            glColor3f(1.0f, 0.3f, 0.3f);//اللون الاول والثاني والثالث
            drawLargeText("GAME OVER!", 500, 400);//النص    
            glColor3f(1.0f, 0.8f, 0.5f);
            drawLargeText("Score:", 520, 280);//النص        
            glColor3f(1.0f, 0.5f, 0.0f);//اللون الاول والثاني والثالث
            drawNumber(610, 280, score);
        }

        glColor3f(0.8f, 0.8f, 0.8f);//اللون الاول والثاني والثالث
        drawLargeText("Press F2 to Play Again", 450, 200);//النص
    }

    glDisable(GL_BLEND);
    glutSwapBuffers();
}

// ============================================
// الدالة الرئيسية
// ============================================
int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));//التحرك بالمؤقت

    glutInit(&argc, argv);//التحرك بالمؤقت
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//التحرك بالمؤقت
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);//التحرك بالمؤقت
    glutInitWindowPosition(250, 200);//التحرك بالمؤقت
    glutCreateWindow("Fish Game - Clean Version");//التحرك بالمؤقت

    initGame();//التهيئة اللعبة

    glutPassiveMotionFunc(mouseMove);//التحرك بالماوس
    glutSpecialFunc(keyboard);//التحرك باللوحة
    glutTimerFunc(0, animationTimer, 0);//التحرك بالمؤقت
    glutTimerFunc(0, gameTimer, 0);//التحرك بالمؤقت
    glutDisplayFunc(display);//التحرك بالمؤقت

    glutMainLoop();//التحرك بالمؤقت
    return 0;
}