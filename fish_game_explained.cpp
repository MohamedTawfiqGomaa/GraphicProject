// ============================================
// لعبة السمكة - Fish Game
// لعبة تأكل فيها سمك صغير عشان تكبر وتفوز
// ============================================

// ============================================
// المكتبات الضرورية فقط
// Libraries - الأدوات اللي هنستخدمها في البرنامج
// ============================================

// هنا بنحدد إذا كنا على نظام Mac ولا Windows/Linux
#ifdef __APPLE__
// لو على Mac، نستخدم GLUT من مكانها الخاص
#define GL_SILENCE_DEPRECATION  // عشان نخفي رسائل التحذير القديمة
#include <GLUT/glut.h>
#else
// لو على Windows أو Linux، نستخدم GL العادي
#include <GL/glut.h>
#endif

// مكتبات إضافية محتاجينها
#include <cmath>       // للعمليات الرياضية زي sin, cos, sqrt
#include <ctime>       // للوقت والأرقام العشوائية
#include <cstdio>      // للطباعة وتحويل الأرقام لنصوص
#include <vector>      // للتعامل مع قوائم ديناميكية (السمك)

// ============================================
// الثوابت الأساسية
// Constants - أرقام ثابتة مش هتتغير في اللعبة
// ============================================

// أبعاد الشاشة الأولية (بالبكسل)
const int INITIAL_WINDOW_WIDTH = 1200;   // عرض الشاشة = 1200 بكسل
const int INITIAL_WINDOW_HEIGHT = 600;   // ارتفاع الشاشة = 600 بكسل

// نسبة ارتفاع المحيط من ارتفاع الشاشة (83.3%)
// يعني المياه هتاخد معظم الشاشة، والباقي سماء
const float OCEAN_HEIGHT_RATIO = 0.833f;

// إعدادات اللعبة الأساسية
const int NUM_FISH = 40;           // عدد السمك في اللعبة = 40 سمكة
const int INITIAL_TIME = 50;       // الوقت المتاح = 50 ثانية
const float FISH_SIZE = 20.0f;     // حجم السمكة الأساسي
const float COLLISION_RADIUS = 15.0f;  // المسافة اللي يحصل فيها تصادم
const float FISH_SPEED = 0.5f;     // سرعة حركة السمك
const float PI = 3.1415926f;       // العدد باي π (للدوائر والأمواج)

// أبعاد ديناميكية تتغير مع حجم النافذة
// دول مش ثوابت، هيتغيروا لو كبرنا أو صغرنا الشاشة
int windowWidth = INITIAL_WINDOW_WIDTH;      // العرض الحالي
int windowHeight = INITIAL_WINDOW_HEIGHT;    // الارتفاع الحالي
float oceanHeight = INITIAL_WINDOW_HEIGHT * OCEAN_HEIGHT_RATIO;  // ارتفاع المحيط

// ============================================
// إعدادات نمو اللاعب
// Player Growth Settings
// ============================================
const float GROWTH_INCREMENT = 0.05f;      // كل ما تاكل، تكبر بمقدار 0.05
const float INITIAL_PLAYER_SIZE = 1.0f;    // الحجم الأولي للاعب = 1x
const float MAX_PLAYER_SIZE = 2.5f;        // أقصى حجم ممكن توصله = 2.5x

// ============================================
// سلوك الأسماك الذكي
// Fish AI Behavior
// ============================================
const float FLEE_DISTANCE = 200.0f;   // المسافة اللي السمك يهرب فيها منك
const float CHASE_DISTANCE = 250.0f;  // المسافة اللي السمك الخطير يطاردك فيها
const float FLEE_SPEED = 5.5f;        // سرعة الهروب (مضروبة في السرعة العادية)
const float CHASE_SPEED = 6.3f;       // سرعة المطاردة

// ============================================
// إعدادات الأمواج
// Wave Animation Settings
// ============================================
const int WAVE_POINTS = 50;    // عدد النقاط في كل موجة (كل ما زادت كانت أنعم)
float waveOffset = 0.0f;       // إزاحة الموجة (بتزيد مع الوقت عشان تتحرك)

// ============================================
// متغيرات اللعبة
// Game State Variables - الحالة الحالية للعبة
// ============================================
bool isGameOver = false;       // هل اللعبة خلصت؟ (true = خلصت، false = لسه شغالة)
int score = 0;                 // رصيد النقاط اللي جمعتها
int gameTime = INITIAL_TIME;   // الوقت المتبقي (يبدأ من 50 ثانية)
float playerSizeScale = INITIAL_PLAYER_SIZE;  // حجم اللاعب الحالي
float prevMouseX = INITIAL_WINDOW_WIDTH / 2.0f;  // آخر موقع للماوس (للاتجاه)
bool allFishCollected = false; // هل كلت كل السمك؟ (عشان تفوز)

// ============================================
// دوال عرض النصوص على الشاشة
// Text Rendering Functions
// ============================================

/**
 * دالة رسم نص عادي على الشاشة
 * @param str: النص اللي عايزين نكتبه
 * @param x: الموقع الأفقي (من اليسار)
 * @param y: الموقع الرأسي (من تحت)
 */
void drawText(const char *str, int x, int y) {
    // حدد موقع بداية الكتابة على الشاشة
    glRasterPos2d(x, y);
    
    // اكتب كل حرف في النص واحد ورا التاني
    while (*str) {  // طول ما في حروف
        // ارسم الحرف الحالي بخط Helvetica حجم 12
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str);
        str++;  // انتقل للحرف اللي بعده
    }
}

/**
 * دالة رسم نص كبير على الشاشة
 * نفس فكرة drawText بس بخط أكبر (للعناوين)
 */
void drawLargeText(const char *str, int x, int y) {
    glRasterPos2d(x, y);  // حدد الموقع
    while (*str) {
        // ارسم بخط Times Roman حجم 24 (أكبر)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
        str++;
    }
}

/**
 * دالة رسم رقم على الشاشة
 * الكمبيوتر مش عارف يرسم أرقام، لازم نحولها لنص الأول
 * @param x, y: موقع الرقم
 * @param value: الرقم اللي عايزين نرسمه
 */
void drawNumber(int x, int y, int value) {
    char buffer[32];  // مساحة مؤقتة نحط فيها النص
    sprintf(buffer, "%d", value);  // حول الرقم لنص (مثلاً: 123 → "123")
    drawText(buffer, x, y);        // ارسم النص
}

/**
 * دالة رسم رقم عشري (مثل 1.50x)
 * بنستخدمها لعرض حجم السمكة
 */
void drawFloat(int x, int y, float value) {
    char buffer[32];
    sprintf(buffer, "%.2fx", value);  // حول لنص برقمين بعد العلامة + حرف x
    drawText(buffer, x, y);
}

/**
 * دالة رسم رقم كبير (للعناوين)
 */
void drawLargeNumber(int x, int y, int value) {
    char buffer[32];
    sprintf(buffer, "%d", value);
    drawLargeText(buffer, x, y);  // نفس الفكرة بس بخط أكبر
}

// ============================================
// رسم المحيط مع أمواج واقعية
// Ocean and Wave Rendering
// ============================================

/**
 * دالة رسم المحيط الأزرق مع أمواج متحركة
 * المحيط عبارة عن: خلفية بتدرج لوني + 3 طبقات أمواج
 */
void drawOcean() {
    // ============================================
    // الخطوة 1: رسم خلفية المحيط (مستطيل بتدرج لوني)
    // ============================================
    
    glBegin(GL_QUADS);  // ابدأ رسم شكل رباعي (مستطيل)
    
    // اللون عند أسفل المحيط (أزرق فاتح شوية)
    glColor3f(0.1f, 0.4f, 0.8f);  // R=0.1, G=0.4, B=0.8 (أزرق فاتح)
    glVertex2f(0, 0);              // الزاوية السفلية اليسرى
    glVertex2f(windowWidth, 0);    // الزاوية السفلية اليمنى
    
    // اللون عند أعلى المحيط (أزرق غامق)
    glColor3f(0.05f, 0.2f, 0.5f);  // R=0.05, G=0.2, B=0.5 (أزرق غامق)
    glVertex2f(windowWidth, oceanHeight);  // الزاوية العلوية اليمنى
    glVertex2f(0, oceanHeight);            // الزاوية العلوية اليسرى
    
    glEnd();  // خلصنا رسم المستطيل
    
    // OpenGL هيعمل تدرج تلقائي بين الألوان = تأثير واقعي للمحيط!
    
    // ============================================
    // الخطوة 2: رسم أمواج متعددة الطبقات (3 موجات)
    // كل موجة بسرعة مختلفة عشان تبان واقعية
    // ============================================
    
    // الموجة الأولى (الخلفية - بطيئة)
    // ============================================
    glColor4f(0.15f, 0.45f, 0.75f, 0.6f);  // أزرق فاتح + شفافية 60%
    glLineWidth(2.0f);  // سمك الخط = 2 بكسل
    
    glBegin(GL_LINE_STRIP);  // ابدأ رسم خط متصل من نقاط
    for (int i = 0; i <= WAVE_POINTS; i++) {
        // احسب الموقع الأفقي للنقطة الحالية
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        
        // احسب الموقع الرأسي باستخدام دالة sin (موجة جيبية)
        // sin() بتعمل منحنى يطلع وينزل = شكل الموجة
        float y = oceanHeight +                              // ارتفاع المحيط الأساسي
                  sin((x * 0.01f) + (waveOffset * 0.5f)) * 8.0f +   // موجة بطيئة
                  sin((x * 0.02f) - (waveOffset * 0.3f)) * 4.0f;    // موجة صغيرة
        
        glVertex2f(x, y);  // ارسم النقطة
    }
    glEnd();
    
    // الموجة الثانية (الوسطى - متوسطة)
    // ============================================
    glColor4f(0.2f, 0.55f, 0.85f, 0.7f);  // أزرق متوسط + شفافية 70%
    glLineWidth(2.5f);  // سمك أكبر شوية
    
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= WAVE_POINTS; i++) {
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        
        // نفس الفكرة بس بتردد وسرعة مختلفة
        float y = oceanHeight +
                  sin((x * 0.015f) + (waveOffset * 0.8f)) * 10.0f +  // موجة متوسطة
                  sin((x * 0.03f) + (waveOffset * 0.4f)) * 5.0f;
        
        glVertex2f(x, y);
    }
    glEnd();
    
    // الموجة الثالثة (الأمامية - سريعة)
    // ============================================
    glColor4f(0.25f, 0.65f, 0.95f, 0.9f);  // أزرق فاتح جداً + شفافية 90%
    glLineWidth(3.0f);  // الأسمك (الأمامية واضحة أكتر)
    
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= WAVE_POINTS; i++) {
        float x = (windowWidth / (float)WAVE_POINTS) * i;
        
        // 3 موجات مختلفة متداخلة = تأثير واقعي جداً
        float y = oceanHeight +
                  sin((x * 0.02f) + waveOffset) * 12.0f +              // موجة سريعة كبيرة
                  sin((x * 0.04f) - (waveOffset * 0.7f)) * 6.0f +     // موجة متوسطة
                  sin((x * 0.08f) + (waveOffset * 1.5f)) * 3.0f;      // موجة صغيرة سريعة
        
        glVertex2f(x, y);
    }
    glEnd();
    
    // ============================================
    // تحريك الأمواج
    // ============================================
    waveOffset += 0.03f;  // زود الإزاحة شوية كل فريم = الأمواج تتحرك
    
    // لو الإزاحة كبرت أوي، ارجعها صفر (عشان ما تفيضش)
    if (waveOffset > 2.0f * PI * 100.0f) {
        waveOffset = 0.0f;
    }
}

// ============================================
// فئة السمكة (Fish Class)
// هنا بنحدد كل حاجة عن السمكة: شكلها، حركتها، سلوكها
// ============================================
class Fish {
public:
    // ============================================
    // خصائص السمكة (Properties)
    // ============================================
    bool isPlayer;      // هل دي سمكة اللاعب؟ (true) ولا سمكة عادية (false)
    bool isRedFish;     // هل دي سمكة حمرا خطيرة؟
    bool isLarge;       // هل دي سمكة كبيرة؟
    float x, y;         // موقع السمكة على الشاشة (x = أفقي، y = رأسي)
    float direction;    // اتجاه حركة السمكة (موجب = يمين، سالب = شمال)
    float sizeScale;    // حجم السمكة (مضاعف: 1.0 = عادي، 1.5 = كبير)
    
    // ============================================
    // Constructor - دالة إنشاء سمكة جديدة
    // ============================================
    /**
     * @param player: لو true يبقى دي سمكة اللاعب، لو false يبقى سمكة عادية
     */
    Fish(bool player = false) : isPlayer(player) {
        if (isPlayer) {
            // ============================================
            // سمكة اللاعب
            // ============================================
            x = windowWidth / 2.0f;   // ابدأ في نص الشاشة أفقياً
            y = windowHeight / 2.0f;  // ابدأ في نص الشاشة رأسياً
            sizeScale = INITIAL_PLAYER_SIZE;  // الحجم الأولي = 1.0
            isLarge = false;          // مش كبير في البداية
            direction = FISH_SPEED;   // اتجاه البداية = يمين
        } else {
            // ============================================
            // سمكة عادية (AI)
            // ============================================
            
            // هل السمكة حمرا؟ (30% احتمال)
            isRedFish = (rand() % 10 < 3);  // rand() % 10 = رقم من 0 ل 9
                                             // لو أقل من 3 = 30% احتمال
            
            // هل السمكة كبيرة؟ (30% احتمال)
            isLarge = (rand() % 10 < 3);
            
            // حدد حجم السمكة حسب النوع
            sizeScale = isLarge ? 1.5f : 0.9f;  // كبيرة = 1.5x، صغيرة = 0.9x
            
            // حدد موقع عشوائي للسمكة
            x = rand() % windowWidth;                    // في أي مكان أفقياً
            y = rand() % (int)(oceanHeight - 50);        // في المحيط (مش في السماء!)
            
            // حدد اتجاه عشوائي (يمين أو شمال)
            direction = (rand() % 2) ? FISH_SPEED : -FISH_SPEED;
        }
    }
    
    // ============================================
    // دالة حساب نصف قطر التصادم
    // كل سمكة ليها دايرة خفية حواليها، لو دايرتين اتلامسوا = تصادم
    // ============================================
    float getCollisionRadius() const {
        return COLLISION_RADIUS * sizeScale;  // نصف القطر × الحجم
    }
    
    // ============================================
    // دالة رسم السمكة على الشاشة
    // ============================================
    void draw() {
        // ============================================
        // الخطوة 1: تحديد اللون حسب نوع السمكة
        // ============================================
        float r, g, b;  // مكونات اللون (أحمر، أخضر، أزرق)
        
        if (isPlayer) {
            // سمكة اللاعب = أزرق
            r = 0.2f; g = 0.4f; b = 1.0f;
        } else if (isRedFish) {
            // سمكة حمرا خطيرة = أحمر
            r = 1.0f; g = 0.3f; b = 0.3f;
        } else {
            // سمكة عادية = أصفر
            r = 1.0f; g = 0.9f; b = 0.2f;
        }
        
        // ============================================
        // الخطوة 2: حساب الحجم والاتجاه
        // ============================================
        
        // الحجم الحالي (لو لاعب يستخدم playerSizeScale، لو عادي يستخدم sizeScale)
        float currentScale = isPlayer ? playerSizeScale : sizeScale;
        
        // الاتجاه (-1 = شمال، +1 = يمين)
        float dir = (direction < 0) ? -1.0f : 1.0f;
        
        // أبعاد جسم السمكة
        float bodyLength = FISH_SIZE * 1.8f * currentScale;  // طول الجسم
        float bodyHeight = FISH_SIZE * 1.2f * currentScale;  // ارتفاع الجسم
        
        // ============================================
        // الخطوة 3: رسم جسم السمكة (شكل بيضاوي)
        // ============================================
        glColor3f(r, g, b);  // حدد اللون
        glBegin(GL_POLYGON);  // ابدأ رسم شكل مغلق
        
        // ارسم نصف دائرة (20 نقطة) = شكل بيضاوي
        for (int i = 0; i <= 20; i++) {
            float angle = PI * float(i) / 20.0f;  // الزاوية من 0 ل π (نصف دائرة)
            
            // احسب موقع النقطة باستخدام cos و sin
            float px = x + dir * (bodyLength * 0.4f * cos(angle));  // الموقع الأفقي
            float py = y + bodyHeight * 0.5f * sin(angle);          // الموقع الرأسي
            
            glVertex2f(px, py);  // ارسم النقطة
        }
        glEnd();
        
        // ============================================
        // الخطوة 4: رسم الذيل (شكل مروحي)
        // ============================================
        
        // موقع قاعدة الذيل (في نهاية الجسم)
        float tailBaseX = x - dir * bodyLength * 0.5f;
        float tailLength = FISH_SIZE * 0.6f * currentScale;
        
        // لون الذيل أغمق شوية من الجسم
        glColor3f(r * 0.8f, g * 0.8f, b * 0.3f);
        
        glBegin(GL_TRIANGLE_FAN);  // ارسم مروحة من المثلثات
        glVertex2f(tailBaseX, y);  // نقطة المركز (قاعدة الذيل)
        
        // ارسم 8 نقاط حوالين المركز = شكل مروحي
        for (int i = 0; i <= 8; i++) {
            // زاوية كل نقطة (من -30° ل +30°)
            float angle = (PI / 3.0f) * float(i) / 8.0f - (PI / 6.0f);
            
            // موقع النقطة
            float px = tailBaseX - dir * tailLength * cos(angle);
            float py = y + tailLength * sin(angle);
            
            glVertex2f(px, py);
        }
        glEnd();
        
        // ============================================
        // الخطوة 5: رسم العين
        // ============================================
        
        // موقع العين (في مقدمة الرأس، فوق شوية)
        float eyeX = x + dir * bodyLength * 0.35f;
        float eyeY = y + bodyHeight * 0.15f;
        float eyeSize = FISH_SIZE * 0.15f * currentScale;
        
        // بياض العين (دائرة بيضاء)
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f;  // دائرة كاملة (0 ل 2π)
            glVertex2f(eyeX + eyeSize * cos(angle), 
                      eyeY + eyeSize * sin(angle));
        }
        glEnd();
        
        // بؤبؤ العين (دائرة سودا أصغر)
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 12; i++) {
            float angle = 2.0f * PI * float(i) / 12.0f;
            // نفس الفكرة بس بحجم 60% من بياض العين
            glVertex2f(eyeX + eyeSize * 0.6f * cos(angle), 
                      eyeY + eyeSize * 0.6f * sin(angle));
        }
        glEnd();
    }
    
    // ============================================
    // دالة حركة بسيطة (للاعب فقط)
    // محمد سعيد كامل - مسؤول عن التحكم
    // ============================================
    /**
     * تحريك السمكة حسب موقع الماوس
     * @param mouseX, mouseY: موقع الماوس الحالي
     * @param prevX: موقع الماوس السابق (لحساب الاتجاه)
     */
    void move(float mouseX = 0.0f, float mouseY = 0.0f, float prevX = 0.0f) {
        if (!isPlayer) {
            // ============================================
            // حركة السمك العادي (خط مستقيم)
            // ============================================
            x += direction;  // تحرك في اتجاهك
            
            // لو خرجت من الشاشة من اليمين، ادخل من الشمال
            if (x > windowWidth) {
                x = 0;
                y = rand() % (int)(oceanHeight - 50);  // مكان رأسي جديد
            }
            
            // لو خرجت من الشمال، ادخل من اليمين
            if (x < 0) {
                x = windowWidth;
                y = rand() % (int)(oceanHeight - 50);
            }
        } else {
            // ============================================
            // حركة اللاعب (يتبع الماوس)
            // ============================================
            
            // حدد الاتجاه حسب حركة الماوس
            if (mouseX > prevX) 
                direction = FISH_SPEED;   // الماوس راح يمين = السمكة تبص يمين
            else if (mouseX < prevX) 
                direction = -FISH_SPEED;  // الماوس راح شمال = السمكة تبص شمال
            
            // السمكة تروح لمكان الماوس مباشرة
            x = mouseX;
            y = windowHeight - mouseY;  // OpenGL بيحسب من تحت، والماوس من فوق
        }
    }
    
    // ============================================
    // دالة حركة ذكية (AI للسمك العادي)
    // السمك يهرب لو أنت أكبر، أو يطاردك لو هو أكبر
    // ============================================
    /**
     * @param playerX, playerY: موقع اللاعب
     * @param playerRadius: نصف قطر تصادم اللاعب (يعني حجمه)
     */
    void moveWithBehavior(float playerX, float playerY, float playerRadius) {
        // السمكة اللاعب ما تحتاجش AI
        if (isPlayer) return;
        
        // ============================================
        // الخطوة 1: حساب المسافة بين السمكة واللاعب
        // ============================================
        float dx = playerX - x;  // المسافة الأفقية
        float dy = playerY - y;  // المسافة الرأسية
        float dist = sqrt(dx * dx + dy * dy);  // المسافة الفعلية (نظرية فيثاغورس)
        
        // نصف قطر السمكة الحالية
        float myRadius = getCollisionRadius();
        
        // الحركة الافتراضية (خط مستقيم)
        float stepX = direction;  // الخطوة الأفقية
        float stepY = 0.0f;       // الخطوة الرأسية (صفر = خط أفقي)
        
        // ============================================
        // الخطوة 2: الهروب من اللاعب الأكبر
        // ============================================


        // ===== السلوك الطبيعي ====
        bool shouldFlee  = (playerRadius > myRadius);
        bool shouldChase = (playerRadius < myRadius) && (isRedFish || isLarge);

        if (shouldFlee && dist < FLEE_DISTANCE) {
            // اللاعب أكبر مني وقريب مني = اهرب!
            
            // احسب اتجاه الهروب (عكس اتجاه اللاعب)
            stepX = -(dx / dist) * FISH_SPEED * FLEE_SPEED;
            stepY = -(dy / dist) * FISH_SPEED * FLEE_SPEED;
            
            // حدد الاتجاه (عشان السمكة تبص في اتجاه الحركة)
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }
        // ============================================
        // الخطوة 3: مطاردة اللاعب الأصغر
        // ============================================
        else if (shouldChase && dist < CHASE_DISTANCE) {
            // أنا سمكة خطيرة (حمرا أو كبيرة) واللاعب أصغر مني وقريب = اطارده!
            
            // احسب اتجاه المطاردة (نحو اللاعب)
            stepX = (dx / dist) * FISH_SPEED * CHASE_SPEED;
            stepY = (dy / dist) * FISH_SPEED * CHASE_SPEED;
            
            direction = (stepX < 0) ? -FISH_SPEED : FISH_SPEED;
        }
        
        // ============================================
        // الخطوة 4: تحريك السمكة
        // ============================================
        x += stepX;
        y += stepY;
        
        // ============================================
        // الخطوة 5: لف أفقي (لو خرجت من الشاشة)
        // ============================================
        if (x > windowWidth) x = 0;           // خرجت من اليمين = ادخل من الشمال
        if (x < 0) x = windowWidth;           // خرجت من الشمال = ادخل من اليمين
        
        // ============================================
        // الخطوة 6: تحديد عمودي (ما تخرجش من المحيط)
        // ============================================
        if (y < 20) y = 20;                           // ما تنزلش تحت خالص
        if (y > oceanHeight - 20) y = oceanHeight - 20;  // ما تطلعش فوق السطح
    }
};

// ============================================
// كائنات اللعبة
// Game Objects
// ============================================
std::vector<Fish> fishArray;  // قائمة فيها كل السمك في اللعبة
Fish player(true);             // سمكة اللاعب (نمرر true عشان نقول إنها لاعب)

// ============================================
// كشف التصادم (Collision Detection)
// ============================================

/**
 * دالة فحص التصادم بين سمكتين
 * التصادم = لو الدائرتين الخفيتين حوالين السمكتين اتلامسوا
 * 
 * @param a, b: السمكتين اللي عايزين نشوف لو اصطدموا
 * @return: true لو في تصادم، false لو مفيش
 */
bool checkCollision(const Fish& a, const Fish& b) {
    // ============================================
    // الخطوة 1: حساب المسافة بين السمكتين
    // ============================================
    float dx = a.x - b.x;  // الفرق الأفقي
    float dy = a.y - b.y;  // الفرق الرأسي
    float distSq = dx * dx + dy * dy;  // مربع المسافة (أسرع من حساب الجذر)
    
    // ============================================
    // الخطوة 2: حساب نصف قطر كل سمكة
    // ============================================
    // لو سمكة لاعب، استخدم playerSizeScale، لو عادية استخدم getCollisionRadius
    float radiusA = a.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : a.getCollisionRadius();
    float radiusB = b.isPlayer ? (COLLISION_RADIUS * playerSizeScale) : b.getCollisionRadius();
    
    // مجموع نصف القطرين
    float combinedRadius = radiusA + radiusB;
    
    // ============================================
    // الخطوة 3: مقارنة المسافة بمجموع الأنصاف
    // ============================================
    // لو المسافة أقل من المجموع = الدائرتين متداخلتين = تصادم!
    return distSq < (combinedRadius * combinedRadius);
}

/**
 * دالة فحص إذا كان اللاعب يستطيع أكل السمكة
 * 
 * @param player: سمكة اللاعب
 * @param other: السمكة الأخرى
 * @return: true لو اللاعب أكبر ويقدر ياكلها
 */
bool canEatFish(const Fish& player, const Fish& other) {
    float playerRadius = COLLISION_RADIUS * playerSizeScale;  // حجم اللاعب
    float otherRadius = other.getCollisionRadius();           // حجم السمكة الأخرى
    
    // تقدر تاكلها لو حجمك على الأقل 95% من حجمها
    return playerRadius >= otherRadius * 0.95f;
}

// ============================================
// تهيئة اللعبة (Game Initialization)
// ============================================

/**
 * دالة تجهيز اللعبة من البداية
 * بتتنادى في أول البرنامج ولما تدوس F2 (إعادة اللعب)
 */
void initGame() {
    // ============================================
    // الخطوة 1: تحديد لون الخلفية (السماء)
    // ============================================
    glClearColor(0.07f, 0.01f, 0.75f, 1.0f);  // أزرق غامق (سماء ليلية)
    
    // ============================================
    // الخطوة 2: إعادة تعيين متغيرات اللاعب
    // ============================================
    playerSizeScale = INITIAL_PLAYER_SIZE;    // الحجم = 1.0
    prevMouseX = windowWidth / 2.0f;          // الماوس في النص
    
    // ============================================
    // الخطوة 3: إنشاء السمك من جديد
    // ============================================
    fishArray.clear();  // امسح كل السمك القديم
    
    // اخلق 40 سمكة جديدة
    for (int i = 0; i < NUM_FISH; ++i) {
        fishArray.push_back(Fish());  // Fish() = سمكة عادية جديدة
    }
    
    // ============================================
    // الخطوة 4: إعداد نظام الإحداثيات (Coordinate System)
    // ============================================
    glMatrixMode(GL_PROJECTION);  // نمط المصفوفة = Projection
    glLoadIdentity();             // ابدأ من الصفر
    
    // حدد نظام الإحداثيات 2D
    // gluOrtho2D = نظام متعامد (مش منظور 3D)
    // من (0,0) في اليسار-تحت لـ (windowWidth, windowHeight) في اليمين-فوق
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}

// ============================================
// دالة تحديث حجم النافذة (Window Reshape)
// ============================================

/**
 * بتتنادى لما تكبر أو تصغر الشاشة
 * @param width, height: الأبعاد الجديدة
 */
void reshape(int width, int height) {
    // ============================================
    // الخطوة 1: تحديث المتغيرات العامة
    // ============================================
    windowWidth = width;
    windowHeight = height;
    oceanHeight = windowHeight * OCEAN_HEIGHT_RATIO;  // المحيط = 83.3% من الارتفاع
    
    // ============================================
    // الخطوة 2: تحديث منطقة الرسم (Viewport)
    // ============================================
    glViewport(0, 0, width, height);  // ارسم على كل النافذة
    
    // ============================================
    // الخطوة 3: تحديث نظام الإحداثيات
    // ============================================
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);  // أبعاد جديدة
    
    glMatrixMode(GL_MODELVIEW);  // ارجع للنمط العادي
    glLoadIdentity();
    
    glutPostRedisplay();  // أعد رسم الشاشة
}

// ============================================
// التحكم (Controls)
// ============================================

/**
 * دالة حركة الماوس
 * بتتنادى كل ما الماوس يتحرك
 */
void mouseMove(int x, int y) {
    // حول الإحداثيات من int لـ float
    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(y);
    
    // حرك سمكة اللاعب لموقع الماوس
    player.move(mouseX, mouseY, prevMouseX);
    
    // احفظ الموقع الحالي عشان نحسب الاتجاه في المرة الجاية
    prevMouseX = mouseX;
}

/**
 * دالة الكيبورد (المفاتيح الخاصة)
 * بتتنادى لما تدوس مفاتيح زي F1, F2, الأسهم
 */
void keyboard(int key, int x, int y) {
    // ============================================
    // مفتاح F2 = إعادة اللعب (Restart)
    // ============================================
    if (key == GLUT_KEY_F2) {
        // أعد تعيين كل المتغيرات
        isGameOver = false;
        gameTime = INITIAL_TIME;
        score = 0;
        allFishCollected = false;
        playerSizeScale = INITIAL_PLAYER_SIZE;
        prevMouseX = windowWidth / 2.0f;
        
        // امسح السمك القديم واخلق سمك جديد
        fishArray.clear();
        for (int i = 0; i < NUM_FISH; ++i) {
            fishArray.push_back(Fish());
        }
    }
    
    glutPostRedisplay();  // أعد رسم الشاشة
}

// ============================================
// المؤقتات (Timers)
// ============================================

/**
 * مؤقت الرسوم المتحركة (Animation Timer)
 * بيتنادى كل 50 ميللي ثانية = 20 فريم في الثانية
 */
void animationTimer(int value) {
    glutPostRedisplay();  // قول لـ OpenGL ارسم فريم جديد
    
    // نادي نفسك تاني بعد 50ms
    glutTimerFunc(50, animationTimer, 0);
}

/**
 * مؤقت اللعبة (Game Timer - العد التنازلي)
 * بيتنادى كل ثانية
 */
void gameTimer(int value) {
    // لو اللعبة لسه شغالة والوقت متبقي
    if (!isGameOver && gameTime > 0) {
        --gameTime;  // نقص ثانية
        
        // لو الوقت خلص
        if (gameTime == 0) {
            isGameOver = true;  // اللعبة خلصت!
        }
    }
    
    glutPostRedisplay();  // أعد الرسم
    
    // نادي نفسك تاني بعد 1000ms (ثانية)
    glutTimerFunc(1000, gameTimer, 0);
}

// ============================================
// العرض الرئيسي (Main Display Function)
// هنا بيتم رسم كل حاجة على الشاشة
// ============================================

/**
 * دالة العرض الرئيسية
 * بتتنادى كل فريم (20 مرة في الثانية)
 */
void display() {
    // ============================================
    // الخطوة 1: تنظيف الشاشة
    // ============================================
    glClear(GL_COLOR_BUFFER_BIT);  // امسح كل حاجة قديمة
    
    // ============================================
    // الخطوة 2: تفعيل الشفافية (Transparency)
    // ============================================
    glEnable(GL_BLEND);  // فعّل الشفافية
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // طريقة المزج
    
    // ============================================
    // الخطوة 3: رسم المحيط والأمواج
    // ============================================
    drawOcean();
    
    // ============================================
    // الخطوة 4: حسب حالة اللعبة
    // ============================================
    if (!isGameOver) {
        // ============================================
        // اللعبة شغالة - ارسم كل حاجة
        // ============================================
        
        // حساب معامل التكبير (عشان الUI يتناسب مع حجم الشاشة)
        float scaleX = (float)windowWidth / INITIAL_WINDOW_WIDTH;
        float scaleY = (float)windowHeight / INITIAL_WINDOW_HEIGHT;
        

        // ============================================
        // رسم خلفية UI (المستطيلات السودا)
        // ============================================
        
        // حساب مواقع الخلفيات (متناسبة مع الحجم)
        float uiTop = 600.0f * scaleY;
        float uiBottom = 560.0f * scaleY;
        float uiLeftWidth = 150.0f * scaleX;
        float uiRightWidth = 150.0f * scaleX;
        
        // خلفية اليسار (Score و Size)
        glColor3f(0.0f, 0.0f, 0.0f);  // لون أسود
        glBegin(GL_QUADS);
        glVertex2f(0, uiBottom);
        glVertex2f(uiLeftWidth, uiBottom);
        glVertex2f(uiLeftWidth, uiTop);
        glVertex2f(0, uiTop);
        glEnd();
        
        // خلفية اليمين (Time)
        glBegin(GL_QUADS);
        glVertex2f(windowWidth - uiRightWidth, uiBottom);
        glVertex2f(windowWidth, uiBottom);
        glVertex2f(windowWidth, uiTop);
        glVertex2f(windowWidth - uiRightWidth, uiTop);
        glEnd();
        
        
        // ============================================
        // رسم النصوص (Score, Size, Time)
        // ============================================
        
        // Score (النقاط)
        glColor3f(1.0f, 1.0f, 1.0f);  // أبيض
        drawText("Score:", 10 * scaleX, 585 * scaleY);
        glColor3f(1.0f, 1.0f, 0.0f);  // أصفر
        drawNumber(60 * scaleX, 585 * scaleY, score);
        
        // Size (حجم اللاعب)
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Size:", 10 * scaleX, 570 * scaleY);
        glColor3f(0.3f, 1.0f, 0.8f);  // أخضر فاتح
        drawFloat(50 * scaleX, 570 * scaleY, playerSizeScale);
        
        // Time (الوقت المتبقي)
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Time:", 1070 * scaleX, 575 * scaleY);
        glColor3f(0.3f, 1.0f, 0.3f);  // أخضر
        drawNumber(1120 * scaleX, 575 * scaleY, gameTime);
        
        // ============================================
        // رسم سمكة اللاعب
        // ============================================
        player.draw();
        
        // ============================================
        // تحريك ورسم السمك + كشف التصادم
        // ============================================
        
        float playerRadius = COLLISION_RADIUS * playerSizeScale;  // حجم اللاعب
        
        // لف على كل السمك
        for (size_t i = 0; i < fishArray.size(); ++i) {
            // حرك السمكة بسلوك ذكي (هروب/مطاردة)
            fishArray[i].moveWithBehavior(player.x, player.y, playerRadius);
            
            // ============================================
            // فحص التصادم مع اللاعب
            // ============================================
            if (checkCollision(player, fishArray[i])) {
                // في تصادم!
                
                if (fishArray[i].isRedFish) {
                    // ============================================
                    // سمكة حمرا (خطيرة)
                    // ============================================
                    
                    if (!canEatFish(player, fishArray[i])) {
                        // السمكة الحمرا أكبر منك = Game Over!
                        isGameOver = true;
                        break;  // اطلع من اللوب
                    } else {
                        // أنت أكبر من السمكة الحمرا = كسبت!
                        score += 3;  // 3 نقاط
                        
                        // كبر بسرعة (ضعف السرعة العادية)
                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT * 2.0f;
                            if (playerSizeScale > MAX_PLAYER_SIZE)
                                playerSizeScale = MAX_PLAYER_SIZE;  // ما تزيدش عن الحد الأقصى
                        }
                        
                        // امسح السمكة من القائمة
                        fishArray.erase(fishArray.begin() + i);
                        --i;  // رجع الـ index عشان ما تضيعش سمكة
                    }
                } else {
                    // ============================================
                    // سمكة عادية (صفرا)
                    // ============================================
                    
                    if (canEatFish(player, fishArray[i])) {
                        // تقدر تاكلها!
                        
                        // احسب النقاط (كبيرة = 2، صغيرة = 1)
                        int points = fishArray[i].isLarge ? 2 : 1;
                        score += points;
                        
                        // كبر بشكل عادي
                        if (playerSizeScale < MAX_PLAYER_SIZE) {
                            playerSizeScale += GROWTH_INCREMENT;
                            if (playerSizeScale > MAX_PLAYER_SIZE)
                                playerSizeScale = MAX_PLAYER_SIZE;
                        }
                        
                        // امسح السمكة
                        fishArray.erase(fishArray.begin() + i);
                        --i;
                    } else {
                        // السمكة أكبر منك = Game Over!
                        isGameOver = true;
                        break;
                    }
                }
            } else {
                // مفيش تصادم = ارسم السمكة عادي
                fishArray[i].draw();
            }
        }
        
        // ============================================
        // فحص الفوز (هل كلت كل السمك؟)
        // ============================================
        if (fishArray.empty()) {
            // مفيش سمك خالص = فزت!
            allFishCollected = true;
            isGameOver = true;
        }
        
    } else {
        // ============================================
        // اللعبة خلصت - ارسم شاشة النهاية
        // ============================================
        
        // حساب معامل التكبير
        float scaleX = (float)windowWidth / INITIAL_WINDOW_WIDTH;
        float scaleY = (float)windowHeight / INITIAL_WINDOW_HEIGHT;
        
        // ============================================
        // رسم صندوق الرسالة (مستطيل أسود كبير)
        // ============================================
        float dialogLeft = 200.0f * scaleX;
        float dialogRight = 1000.0f * scaleX;
        float dialogTop = 450.0f * scaleY;
        float dialogBottom = 150.0f * scaleY;
        
        glColor3f(0.0f, 0.0f, 0.0f);  // أسود
        glBegin(GL_QUADS);
        glVertex2f(dialogLeft, dialogBottom);
        glVertex2f(dialogRight, dialogBottom);
        glVertex2f(dialogRight, dialogTop);
        glVertex2f(dialogLeft, dialogTop);
        glEnd();
        
        // ============================================
        // الرسالة (فوز أو خسارة)
        // ============================================
        if (allFishCollected) {
            // ============================================
            // فزت! (كلت كل السمك)
            // ============================================
            glColor3f(0.3f, 1.0f, 0.3f);  // أخضر فاتح
            drawLargeText("YOU WIN!", 520 * scaleX, 400 * scaleY);
            
            glColor3f(1.0f, 1.0f, 0.5f);  // أصفر فاتح
            drawLargeText("Final Score:", 480 * scaleX, 280 * scaleY);
            
            glColor3f(1.0f, 1.0f, 0.0f);  // أصفر
            drawLargeNumber(640 * scaleX, 280 * scaleY, score);
        } else {
            // ============================================
            // خسرت! (سمكة أكبر صدمتك أو الوقت خلص)
            // ============================================
            glColor3f(1.0f, 0.3f, 0.3f);  // أحمر
            drawLargeText("GAME OVER!", 500 * scaleX, 400 * scaleY);
            
            glColor3f(1.0f, 0.8f, 0.5f);  // برتقالي فاتح
            drawLargeText("Score:", 520 * scaleX, 280 * scaleY);
            
            glColor3f(1.0f, 0.5f, 0.0f);  // برتقالي
            drawLargeNumber(610 * scaleX, 280 * scaleY, score);
        }
        
        // ============================================
        // رسالة إعادة اللعب
        // ============================================
        glColor3f(0.8f, 0.8f, 0.8f);  // رمادي فاتح
        drawLargeText("Press F2 to Play Again", 450 * scaleX, 200 * scaleY);
    }
    
    // ============================================
    // الخطوة 5: تعطيل الشفافية
    // ============================================
    glDisable(GL_BLEND);
    
    // ============================================
    // الخطوة 6: عرض الإطار (Swap Buffers)
    // ============================================
    glutSwapBuffers();  // اعرض الفريم اللي رسمناه (Double Buffering)
}

// ============================================
// الدالة الرئيسية (Main Function)
// نقطة البداية - من هنا البرنامج يبدأ
// ============================================
int main(int argc, char *argv[]) {
    // ============================================
    // الخطوة 1: تهيئة المولد العشوائي
    // ============================================
    // استخدم الوقت الحالي كـ seed عشان الأرقام تكون مختلفة كل مرة
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // ============================================
    // الخطوة 2: تهيئة GLUT (مكتبة النوافذ والرسوم)
    // ============================================
    glutInit(&argc, argv);  // جهز GLUT
    
    // حدد نمط العرض:
    // GLUT_RGB = استخدم ألوان RGB
    // GLUT_DOUBLE = استخدم Double Buffering (ارسم على شاشة خفية عشان ما يترعششش)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    
    // ============================================
    // الخطوة 3: إنشاء النافذة
    // ============================================
    windowWidth = INITIAL_WINDOW_WIDTH;
    windowHeight = INITIAL_WINDOW_HEIGHT;
    oceanHeight = windowHeight * OCEAN_HEIGHT_RATIO;
    
    glutInitWindowSize(windowWidth, windowHeight);  // حجم النافذة
    glutInitWindowPosition(250, 200);  // موقع النافذة على الشاشة (x=250, y=200)
    glutCreateWindow("Fish Game - Desktop Application");  // عنوان النافذة
    
    // ============================================
    // الخطوة 4: تهيئة اللعبة
    // ============================================
    initGame();  // جهز كل شيء (السمك، الألوان، إلخ)
    
    // ============================================
    // الخطوة 5: ربط الدوال بالأحداث (Event Callbacks)
    // ============================================
    
    // لما حجم النافذة يتغير، نادي دالة reshape
    glutReshapeFunc(reshape);
    
    // لما الماوس يتحرك (بدون ضغط أزرار)، نادي دالة mouseMove
    glutPassiveMotionFunc(mouseMove);
    
    // لما تدوس مفاتيح خاصة ( F2 )، نادي دالة keyboard
    glutSpecialFunc(keyboard);
    
    // مؤقت الأنيميشن (كل 50ms)
    glutTimerFunc(0, animationTimer, 0);
    
    // مؤقت اللعبة (كل 1000ms = ثانية)
    glutTimerFunc(0, gameTimer, 0);
    
    // الدالة اللي ترسم الشاشة
    glutDisplayFunc(display);
    
    // ============================================
    // الخطوة 6: اللوب الرئيسي (Main Loop)
    // ============================================
    // ده لوب لا نهائي - اللعبة فاضلة شغالة لحد ما تقفل النافذة
    // GLUT بيتعامل مع كل الأحداث (ماوس، كيبورد، مؤقتات) أوتوماتيكياً
    glutMainLoop();
    
    return 0;  // ده مش هيوصله أبداً لأن glutMainLoop ما بتخلصش
}



/*
# ملخص اللعبه
الفكرة الأساسية:

أنت سمكة زرقاء صغيرة
في المحيط 40 سمكة:

أصفر: عادية (تقدر تاكلها تكبر)
حمرا: خطيرة (لو هي أكبر منك هتاكلك)


لازم تكبر وتاكل كل الس
عايزك بقا تعملي الكود كامل وفيه الشرح بالتفصيل على كل جزء وكل سطر بالطريقة دي كدا على شكل تعليقات9:40 PMمك في 50 ثانية
4. لو سمكة أكبر منك صدمتك = Game Over
5. لو كلت كل السمك = You Win!
نظام النقاط:

سمكة صغيرة = 1 نقطة
سمكة كبيرة = 2 نقطة
سمكة حمرا = 3 نقاط

نظام النمو:

كل ما تاكل تكبر شوية
لو وصلت لحجم 2.5x تبقى عملاق!


🎨 التقنيات المستخدمة

OpenGL/GLUT: للرسم ثلاثي الأبعاد (بس بنستخدمه 2D)
Double Buffering: عشان الصورة متترعشش
Sine/Cosine: لرسم الدوائر والأمواج
Vector: لتخزين السمك بشكل ديناميكي
Collision Detection: كشف التصادم بالمسافة
AI Behavior: السمك يهرب أو يطارد حسب الحجم

*/