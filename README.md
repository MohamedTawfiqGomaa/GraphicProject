# 🐟 Fish Game - لعبة السمك

[English](#english) | [العربية](#arabic)

---

<a name="english"></a>
## English

### 📖 Project Overview

**Fish Game** is a 2D arcade-style game built with C++, OpenGL, and GLUT. The player controls a blue fish using mouse movement, collecting yellow fish for points while avoiding red fish. The game features animated ocean waves, sound effects, collision detection, and win/lose conditions.

**Created by:** Yehia Sharawy & Abdelrahman Sharaf  
**Date:** December 21, 2023  
**Language:** C++  
**Graphics API:** OpenGL with GLUT

---

### 🎮 Game Rules

1. **Yellow Fish (Collectible):**
   - Collect for +1 point
   - Disappears when touched
   - Goal: Collect all yellow fish to win

2. **Red Fish (Enemy):**
   - Touching = Instant Game Over
   - Must be avoided at all costs

3. **Win Condition:**
   - Collect all yellow fish before time runs out

4. **Controls:**
   - **Mouse Movement:** Controls player fish position
   - **F2 Key:** Restart game

---

### 🚀 Getting Started

#### Prerequisites

- **C++ Compiler:** GCC or compatible compiler
- **OpenGL:** OpenGL development libraries
- **GLUT/freeGLUT:** GLUT implementation
- **IDE (Optional):** Code::Blocks, Visual Studio, or any C++ IDE

#### Installation

##### Windows

1. Install [MinGW-w64](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/)
2. Install freeGLUT:
   ```bash
   # Using MSYS2
   pacman -S mingw-w64-x86_64-freeglut
   ```
3. Or download freeGLUT from [SourceForge](https://sourceforge.net/projects/freeglut/)

##### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev freeglut3-dev
```

##### macOS

```bash
brew install freeglut
```

#### Building the Project

##### Using Code::Blocks

1. Open `projectt.cbp` in Code::Blocks
2. Select **Build → Build** (or press `F9`)
3. Executable will be created in `bin/Debug/`

##### Using Command Line (Linux/macOS)

```bash
g++ main.cpp -o fish_game -lGL -lGLU -lglut
```

##### Using Command Line (Windows - MinGW)

```bash
g++ main.cpp -o fish_game.exe -lopengl32 -lglu32 -lfreeglut -lwinmm
```

---

### 📁 Project Structure

```
projectt/
├── main.cpp                    # Main game source code
├── projectt.cbp               # Code::Blocks project file
├── PROJECT_DOCUMENTATION.md   # Detailed documentation
├── README.md                  # This file
├── CONTRIBUTING.md            # Contribution guidelines
└── .gitignore                 # Git ignore rules
```

---

### 🛠️ Development

#### Running the Game

After building:
- **Windows:** Run `bin/Debug/projectt.exe`
- **Linux/macOS:** Run the compiled executable

#### Game Features

✅ Animated ocean with gradient background  
✅ Smooth wave animation  
✅ Realistic fish shapes with fins and eyes  
✅ Collision detection system  
✅ Sound effects (Windows only)  
✅ Score tracking  
✅ Timer countdown  
✅ Win/lose conditions  

---

### 🤝 Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

---

### 📄 License

This project is created for educational purposes.

---

### 👥 Authors

- **Yehia Sharawy**
- **Abdelrahman Sharaf**

---

<a name="arabic"></a>
## العربية

### 📖 نظرة عامة على المشروع

**لعبة السمك** هي لعبة أركيد ثنائية الأبعاد مبنية باستخدام C++ و OpenGL و GLUT. يتحكم اللاعب بسمكة زرقاء باستخدام حركة الماوس، يجمع السمك الأصفر للحصول على نقاط مع تجنب السمك الأحمر. تتميز اللعبة بأمواج المحيط المتحركة، المؤثرات الصوتية، كشف التصادم، وشروط الفوز/الخسارة.

**المطورون:** يحيى شرواي وعبدالرحمن شرف  
**التاريخ:** 21 ديسمبر 2023  
**اللغة:** C++  
**مكتبة الرسوميات:** OpenGL مع GLUT

---

### 🎮 قواعد اللعبة

1. **السمك الأصفر (قابل للجمع):**
   - اجمعه للحصول على نقطة واحدة
   - يختفي عند لمسه
   - الهدف: اجمع كل السمك الأصفر للفوز

2. **السمك الأحمر (عدو):**
   - اللمس = انتهاء اللعبة فوراً
   - يجب تجنبه تماماً

3. **شرط الفوز:**
   - اجمع كل السمك الأصفر قبل انتهاء الوقت

4. **أزرار التحكم:**
   - **حركة الماوس:** تتحكم في موضع السمكة
   - **زر F2:** إعادة تشغيل اللعبة

---

### 🚀 البدء

#### المتطلبات الأساسية

- **مترجم C++:** GCC أو مترجم متوافق
- **OpenGL:** مكتبات تطوير OpenGL
- **GLUT/freeGLUT:** تطبيق GLUT
- **بيئة التطوير (اختياري):** Code::Blocks أو Visual Studio

#### التثبيت

##### Windows

1. ثبّت [MinGW-w64](https://www.mingw-w64.org/) أو [MSYS2](https://www.msys2.org/)
2. ثبّت freeGLUT:
   ```bash
   # باستخدام MSYS2
   pacman -S mingw-w64-x86_64-freeglut
   ```
3. أو حمّل freeGLUT من [SourceForge](https://sourceforge.net/projects/freeglut/)

##### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev freeglut3-dev
```

##### macOS

```bash
brew install freeglut
```

#### بناء المشروع

##### باستخدام Code::Blocks

1. افتح `projectt.cbp` في Code::Blocks
2. اختر **Build → Build** (أو اضغط `F9`)
3. سيتم إنشاء الملف التنفيذي في `bin/Debug/`

##### باستخدام سطر الأوامر (Linux/macOS)

```bash
g++ main.cpp -o fish_game -lGL -lGLU -lglut
```

##### باستخدام سطر الأوامر (Windows - MinGW)

```bash
g++ main.cpp -o fish_game.exe -lopengl32 -lglu32 -lfreeglut -lwinmm
```

---

### 📁 هيكل المشروع

```
projectt/
├── main.cpp                    # كود اللعبة الرئيسي
├── projectt.cbp               # ملف مشروع Code::Blocks
├── PROJECT_DOCUMENTATION.md   # توثيق مفصل
├── README.md                  # هذا الملف
├── CONTRIBUTING.md            # إرشادات المساهمة
└── .gitignore                 # قواعد Git
```

---

### 🛠️ التطوير

#### تشغيل اللعبة

بعد البناء:
- **Windows:** شغّل `bin/Debug/projectt.exe`
- **Linux/macOS:** شغّل الملف التنفيذي المُجمع

#### ميزات اللعبة

✅ محيط متحرك بخلفية متدرجة  
✅ حركة أمواج سلسة  
✅ أشكال أسماك واقعية مع زعانف وعيون  
✅ نظام كشف التصادم  
✅ مؤثرات صوتية (Windows فقط)  
✅ تتبع النقاط  
✅ عداد تنازلي للوقت  
✅ شروط الفوز/الخسارة  

---

### 🤝 المساهمة

يرجى قراءة [CONTRIBUTING.md](CONTRIBUTING.md) للاطلاع على إرشادات المساهمة وعملية إرسال طلبات السحب (Pull Requests).

---

### 📄 الترخيص

هذا المشروع تم إنشاؤه لأغراض تعليمية.

---

### 👥 المؤلفون

- **يحيى شرواي**
- **عبدالرحمن شرف**

---

## 🔗 روابط مفيدة

- [التوثيق الكامل](PROJECT_DOCUMENTATION.md)
- [إرشادات المساهمة](CONTRIBUTING.md)

