# Fish Game - Complete Project Documentation

## Project Overview

**Fish Game** is a 2D arcade-style game built with C++, OpenGL, and GLUT. The player controls a blue fish using mouse movement, collecting yellow fish for points while avoiding red fish. The game features animated ocean waves, sound effects, collision detection, and win/lose conditions.

**Created by:** Yehia Sharawy & Abdelrahman Sharaf  
**Date:** 21/12/2023  
**Language:** C++  
**Graphics API:** OpenGL with GLUT

---

## 📚 Libraries Used

### 1. **GLUT (OpenGL Utility Toolkit)**
- **File:** `<GL/glut.h>` (Windows) or `<GLUT/glut.h>` (macOS)
- **Purpose:** 
  - Window management and event handling
  - OpenGL context creation
  - Input handling (mouse, keyboard)
  - Timer functions for animation
  - Bitmap font rendering for text
- **Key Functions Used:**
  - `glutInit()` - Initialize GLUT
  - `glutCreateWindow()` - Create game window
  - `glutDisplayFunc()` - Set display callback
  - `glutPassiveMotionFunc()` - Handle mouse movement
  - `glutSpecialFunc()` - Handle special keys (F2)
  - `glutTimerFunc()` - Create animation timers
  - `glutMainLoop()` - Start main game loop

### 2. **OpenGL (Open Graphics Library)**
- **File:** Included via GLUT
- **Purpose:**
  - 2D graphics rendering
  - Drawing primitives (triangles, polygons, lines, points)
  - Color management
  - Blending and transparency
- **Key Functions Used:**
  - `glClear()` - Clear screen
  - `glBegin()/glEnd()` - Define drawing primitives
  - `glVertex2f()` - Define vertices
  - `glColor3f()` - Set RGB colors
  - `glEnable()/glDisable()` - Enable/disable OpenGL features
  - `gluOrtho2D()` - Set 2D orthographic projection

### 3. **C++ Standard Library - `<cmath>`**
- **Purpose:** Mathematical functions
- **Functions Used:**
  - `std::cos()` - Cosine for wave and fish calculations
  - `std::sin()` - Sine for circular shapes and waves
  - `std::abs()` - Absolute value for wave animation

### 4. **C++ Standard Library - `<cstring>`**
- **Purpose:** String manipulation
- **Functions Used:**
  - `std::strlen()` - Get string length for text rendering

### 5. **C++ Standard Library - `<cstdlib>`**
- **Purpose:** General utilities
- **Functions Used:**
  - `rand()` - Generate random numbers for fish positions and types
  - `std::srand()` - Seed random number generator

### 6. **C++ Standard Library - `<cstdio>`**
- **Purpose:** Input/output operations
- **Functions Used:**
  - `std::snprintf()` - Format score as string for display

### 7. **C++ Standard Library - `<ctime>`**
- **Purpose:** Time operations
- **Functions Used:**
  - `std::time()` - Get current time for random seed

### 8. **C++ Standard Library - `<vector>`**
- **Purpose:** Dynamic array container
- **Usage:** Store array of fish objects (`std::vector<Fish>`)

### 9. **Windows API - `<windows.h>` and `<mmsystem.h>`**
- **Platform:** Windows only (`#ifdef _WIN32`)
- **Purpose:** Sound effects
- **Functions Used:**
  - `Beep()` - Generate system beep sounds
- **Library:** `winmm.lib` (Windows Multimedia Library)

---

## 📁 Project Files

### 1. **main.cpp** (464 lines)
**Purpose:** Main source code file containing all game logic

**Structure:**
- **Lines 1-26:** Header comments and library includes
- **Lines 27-38:** Game constants (window size, fish count, speeds, etc.)
- **Lines 40-50:** Global game state variables
- **Lines 52-66:** Sound effects function
- **Lines 68-91:** Text rendering functions
- **Lines 93-130:** Wave and ocean drawing functions
- **Lines 132-271:** Fish class definition
- **Lines 273-275:** Global game objects
- **Lines 277-284:** Collision detection function
- **Lines 286-295:** Game initialization function
- **Lines 297-315:** Input handler functions
- **Lines 317-333:** Timer callback functions
- **Lines 335-445:** Main display/rendering function
- **Lines 447-463:** Main entry point

### 2. **projectt.cbp** (51 lines)
**Purpose:** Code::Blocks project file

**Contains:**
- Project configuration for Code::Blocks IDE
- Compiler settings (GCC)
- Linker settings (OpenGL, GLUT, Windows libraries)
- Build targets (Debug and Release)
- Source file references
- Library paths and dependencies

**Key Settings:**
- **Compiler:** GCC (GNU Compiler Collection)
- **Libraries Linked:**
  - `freeglut` - GLUT implementation
  - `opengl32` - OpenGL for Windows
  - `glu32` - OpenGL Utility Library
  - `winmm` - Windows Multimedia (for sound)
  - `gdi32` - Windows Graphics Device Interface

### 3. **projectt.depend**
**Purpose:** Code::Blocks dependency tracking file
- Automatically generated
- Tracks file dependencies for build system
- Ensures proper compilation order

### 4. **bin/Debug/projectt.exe**
**Purpose:** Compiled executable (Windows)
- Debug build of the game
- Can be run directly
- Contains debug symbols for debugging

### 5. **obj/Debug/main.o**
**Purpose:** Object file
- Compiled object code from main.cpp
- Intermediate build file
- Linked to create final executable

---

## 🎮 Game Architecture

### Game Constants
```cpp
WINDOW_WIDTH = 1200      // Screen width in pixels
WINDOW_HEIGHT = 600      // Screen height in pixels
OCEAN_HEIGHT = 500       // Ocean area height
NUM_FISH = 20            // Total number of fish spawned
INITIAL_TIME = 10        // Game time limit in seconds
FISH_SIZE = 20.0f        // Base size of fish
COLLISION_RADIUS = 15.0f // Collision detection radius
FISH_SPEED = 0.4f        // Speed of AI fish movement
```

### Game State Variables
- `isGameOver` - Boolean flag for game over state
- `score` - Player's current score
- `gameTime` - Remaining time in seconds
- `allYellowFishGone` - Win condition check
- `soundPlayed` - Prevents duplicate sound on game over

### Wave Animation System
- `waveUp` - Direction of wave movement (up/down)
- `waveDelta` - Current wave offset
- `MAX_WAVE_DELTA = 15` - Maximum wave displacement

---

## 🐟 Fish Class

### Properties
- `isPlayer` - Boolean: true if this is the player fish
- `isRedFish` - Boolean: true if enemy fish (red)
- `x, y` - Float coordinates (position)
- `direction` - Float: movement direction and speed

### Methods

#### Constructor `Fish(bool player)`
- Initializes fish position and type
- Player fish: starts at center
- AI fish: random position and type (red/yellow)

#### `void draw()`
- Renders the fish using OpenGL primitives
- **Components:**
  1. **Body:** Oval/ellipse shape (20 segments)
  2. **Tail Fin:** Fan-shaped using triangle fan
  3. **Dorsal Fin:** Top fin (triangle)
  4. **Pelvic Fin:** Bottom fin (triangle)
  5. **Eye:** Circular with white and black pupil
  6. **Outline:** Black border for definition
- **Colors:**
  - Player: Blue (0.2, 0.4, 1.0)
  - Red Fish: Red (1.0, 0.3, 0.3)
  - Yellow Fish: Yellow (1.0, 0.9, 0.2)

#### `void move(float mouseX, float mouseY)`
- **Player fish:** Follows mouse cursor
- **AI fish:** Moves horizontally, wraps around screen edges
- Respawns at random Y position when leaving screen

---

## 🎯 Game Mechanics

### Collision Detection
```cpp
bool checkCollision(const Fish& a, const Fish& b)
```
- Uses squared distance calculation (efficient, no square root)
- Compares distance squared to collision radius squared
- Returns true if fish are within collision range

### Gameplay Rules

1. **Yellow Fish (Collectible):**
   - Collect for +1 point
   - Disappears when touched
   - Goal: Collect all yellow fish to win

2. **Red Fish (Enemy):**
   - Touching = Instant Game Over
   - Must be avoided at all costs

3. **Win Condition:**
   - Collect all yellow fish before time runs out
   - Display "YOU WIN!" message

4. **Lose Conditions:**
   - Touch a red fish (instant loss)
   - Time runs out without collecting all yellow fish
   - Display "YOU FAILED!" message

### Controls
- **Mouse Movement:** Controls player fish position
- **F2 Key:** Restart game (resets score, time, fish array)

---

## 🎨 Rendering System

### Rendering Order (Z-order)
1. **Ocean Background** - Gradient blue background
2. **Waves** - Animated sine-wave water surface (2 layers)
3. **UI Elements** - Score and time displays
4. **Fish** - Player and AI fish
5. **Game Over Screen** - Overlay with messages

### Drawing Functions

#### `drawOcean()`
- Creates gradient background (light to dark blue)
- Draws animated waves using polygon shapes
- Updates wave animation each frame

#### `drawWave(float cx, float cy, float radius)`
- Draws individual wave segment
- Uses polygon with 100 segments for smooth curve
- Offset by `waveDelta` for animation

#### `drawText()`, `drawScore()`, `drawLargeText()`
- Render text using GLUT bitmap fonts
- Support for different font sizes
- Position-based text placement

---

## ⏱️ Timer System

### Animation Timer (`animationTimer`)
- **Frequency:** Every 20ms (~50 FPS)
- **Purpose:** Continuous screen updates
- **Function:** Calls `glutPostRedisplay()` to refresh screen

### Game Timer (`gameTimer`)
- **Frequency:** Every 1000ms (1 second)
- **Purpose:** Countdown game time
- **Function:** Decrements `gameTime`, triggers game over at 0

---

## 🔊 Sound System

### Sound Types
1. **Type 0:** Collect yellow fish
   - Frequency: 800 Hz
   - Duration: 50ms
   - High-pitched beep

2. **Type 1:** Hit red fish
   - Frequency: 300 Hz
   - Duration: 80ms
   - Low-pitched beep

3. **Type 2:** Game over
   - Frequency: 200 Hz
   - Duration: 150ms
   - Low beep

4. **Type 3:** Win
   - Three-tone melody
   - Notes: C (523 Hz), E (659 Hz), G (784 Hz)
   - Creates victory sound

### Implementation
- Windows-only (uses `Beep()` function)
- Non-blocking (short durations)
- Plays once per event (controlled by `soundPlayed` flag)

---

## 🔄 Game Loop Flow

```
1. Initialize (main)
   ├─ Seed random number generator
   ├─ Initialize GLUT
   ├─ Create window
   ├─ Set up callbacks
   └─ Start main loop

2. Main Loop (glutMainLoop)
   ├─ Display Function (called ~50 times/second)
   │  ├─ Clear screen
   │  ├─ Draw ocean and waves
   │  ├─ Update fish positions
   │  ├─ Check collisions
   │  ├─ Update score/time
   │  ├─ Draw UI
   │  └─ Swap buffers
   │
   ├─ Animation Timer (every 20ms)
   │  └─ Trigger display update
   │
   ├─ Game Timer (every 1000ms)
   │  └─ Decrement time
   │
   └─ Input Handlers
      ├─ Mouse Movement → Update player position
      └─ F2 Key → Restart game
```

---

## 🎯 Key Algorithms

### 1. Wave Animation
```cpp
if (std::abs(waveDelta) > MAX_WAVE_DELTA) waveUp = !waveUp;
waveDelta += waveUp ? 1 : -1;
```
- Oscillates between -15 and +15
- Creates smooth up/down wave motion

### 2. Collision Detection
```cpp
float distanceSquared = dx * dx + dy * dy;
float collisionDistance = (COLLISION_RADIUS * 2.0f) * (COLLISION_RADIUS * 2.0f);
return distanceSquared < collisionDistance;
```
- Avoids expensive square root calculation
- Compares squared distances

### 3. Fish Wrapping
```cpp
if (x > WINDOW_WIDTH) {
    x -= WINDOW_WIDTH;
    y = static_cast<float>(rand() % (WINDOW_HEIGHT - 100));
}
```
- Fish reappears on opposite side
- Random Y position for variety

### 4. Win Condition Check
```cpp
allYellowFishGone = true;
for (const auto& fish : fishArray) {
    if (!fish.isPlayer && !fish.isRedFish) {
        allYellowFishGone = false;
        break;
    }
}
```
- Iterates through all fish
- Checks if any yellow fish remain
- Sets win flag if none found

---

## 🛠️ Build Configuration

### Compiler Settings
- **Compiler:** GCC (GNU Compiler Collection)
- **Standard:** C++ (implicit C++11 features)
- **Warnings:** `-Wall` (all warnings enabled)
- **Debug:** `-g` (debug symbols)
- **Optimization:** `-O2` (Release mode)

### Linked Libraries
1. **freeglut** - GLUT implementation
2. **opengl32** - OpenGL core library
3. **glu32** - OpenGL utilities
4. **winmm** - Windows multimedia (sound)
5. **gdi32** - Graphics device interface

### Build Targets
- **Debug:** Development build with debug symbols
- **Release:** Optimized build for distribution

---

## 📊 Performance Considerations

### Optimizations
1. **Squared Distance:** Collision detection avoids square root
2. **Efficient Loops:** Early exit in win condition check
3. **Short Sound Durations:** Non-blocking audio
4. **Fixed Array Size:** Predictable memory usage
5. **50 FPS:** Balanced performance and smoothness

### Memory Management
- Uses `std::vector` for dynamic fish array
- Automatic memory management
- Fish removed from vector on collision

---

## 🎮 Game Features Summary

✅ **Graphics:**
- Animated ocean with gradient background
- Smooth wave animation
- Realistic fish shapes with fins and eyes
- Color-coded UI elements

✅ **Gameplay:**
- Mouse-controlled player
- Collect yellow fish for points
- Avoid red fish (instant game over)
- Time limit challenge
- Win/lose conditions

✅ **Audio:**
- Sound effects for all game events
- Different tones for different actions
- Victory melody

✅ **User Interface:**
- Real-time score display
- Countdown timer
- Game over screen
- Restart functionality

---

## 🔧 Platform Compatibility

### Windows
- ✅ Fully supported
- ✅ Sound effects work
- ✅ All features available

### macOS
- ✅ Graphics work
- ⚠️ Sound effects disabled (no Beep API)
- ✅ Gameplay fully functional

### Linux
- ✅ Graphics work
- ⚠️ Sound effects disabled
- ✅ Gameplay fully functional

---

## 📝 Code Style

- **Naming:** camelCase for functions, UPPER_CASE for constants
- **Organization:** Logical grouping of related functions
- **Comments:** Clear explanations for complex sections
- **Constants:** All magic numbers defined as constants
- **Type Safety:** Proper use of static_cast and type conversions

---

## 🚀 Future Enhancement Ideas

1. **Difficulty Levels:** Adjust fish speed and count
2. **Power-ups:** Special fish with bonuses
3. **High Score System:** Save best scores
4. **Particle Effects:** Visual feedback on collisions
5. **Background Music:** Continuous soundtrack
6. **Multiple Levels:** Progressive difficulty
7. **Fish Varieties:** Different sizes and behaviors
8. **Mobile Support:** Touch controls

---

## 📖 Usage Instructions

### Compiling
1. Open `projectt.cbp` in Code::Blocks
2. Select Build → Build (or F9)
3. Executable created in `bin/Debug/`

### Running
1. Run `projectt.exe` from `bin/Debug/`
2. Or use Code::Blocks Run button (F10)

### Playing
1. Move mouse to control blue fish
2. Collect yellow fish for points
3. Avoid red fish (game over if touched)
4. Collect all yellow fish to win
5. Press F2 to restart

---

## 📄 License & Credits

**Developers:**
- Yehia Sharawy
- Abdelrahman Sharaf

**Technologies:**
- OpenGL (Graphics)
- GLUT (Window Management)
- C++ Standard Library
- Windows API (Sound)

**Date:** December 21, 2023

---

*End of Documentation*

