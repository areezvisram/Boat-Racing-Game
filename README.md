## **SFWRENG 3GC3 - Final Project**
#### Group 15
#### Areez Visram - visraa7 - 400239951
#### Neil Lobo - lobon4 - 400242014

### **Platform Specifications**
* OS: Developed on both Mac OS Big Sur Version 11.6, Apple Silicon M1 Chip and Windows 10 10.0.19042 Build 19042

### **Program Execution**
Using the makefile, the program can be executed via:
* `all`: Run `make`

To remove the executable run:
* `clean`: Run `make clean`

Please note that a lot of external classes were created in the `classes` folder. Please compile this program using the
makefile provided as part of the submission as it has been changed to compile correctly with our file structure.

### **Main Program**
**Description**:
For our final project, we decided to make a racing game. In this game, you are a boat on a water racetrack that is at the bottom of a canyon. Your goal is to complete the track as fast as possible. If you choose the 1 player mode, your time will be displayed upon completion. If you choose the 2 player mode, the player who completes the race first win!

When starting the game, your first choice is 1 player or 2 player mode. From there you must pick your boat and material. Than the game is started!

**Prototype Differences:**
++ Added materials and textures to all objects in scene
++ Added menus to initialize the game in different states
++ Improved physics
++ Added checkpoints and main game loop
++ Added custom collision detection for checkpoints
++ Added timer and ability to restart
++ Added more boats
++ Added attributes to boats

-- No collisions with walls (so please stay inside the track at all times!)
-- No collectable upgrades on map

**External Libraries Used**:
* iostream
    * Used for basic input and output
* random
    * Used in the generation of random values within a passed range
* vector
    * Use of vectors
* string
    * Easier use of strings
* fstream
    * Used in object loader
* chrono
    * Used in timing the race
#### **Instructions and Key Bindings:**
* Run the program
* Use mouse to select one player or two players on first screen
* On next screen, click on arrows to toggle through boats
* Click materials to change material
* Click done when complete
* For race:
**KEYBOARD COMMANDS (case sensitive)**
* q - Quit the program (at any time)
* r - Restart race (only when completed)
* UP_ARROW - Move player one boat forward
* DOWN_ARROW - Move player one boat backwards
* RIGHT_ARROW - Move player one boat to the right
* LEFT_ARROW - Move player one boat to the left
If 2 player:
* w - Move player two boat forward
* s - Move player two boat backwards
* d - Move player two boat to the right
* a - Move player two boat to the left

#### **Boat Models**
* Speedboat - https://free3d.com/3d-model/wakeboard-boat-v1--979173.html
* Fishing boat - https://free3d.com/3d-model/boat-low-poly-431802.html
* Small pirate ship - https://www.turbosquid.com/3d-models/free-ship-3d-model/378524
* Big pirate ship - https://www.turbosquid.com/3d-models/historic-ship-3d-model/671135
