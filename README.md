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

### **Writeup**
Our current prototype has a boat with the ability to drive around the screen using arrow keys controlled by the user.
A camera follows the boat from behind at all times. The boat has motion physics, but the turning does not yet use proper physics.
It just has acceleration, breaking and friction. The prototype also includes a power-up class which can be put in the screen, it just has no functionality yet. Our prototype has also been created with extreme modularity, so the implementation of new models or additional classes will be much easier. Lighting has also been implemented as part of the prototype. One design decision we made was to include the view of the xyz axes
in the prototype so we know where the boat is relative to the center. This is just for prototype purposes, will not be included in final submission. Finally, as part of the prototype we have looked into adding a second screen (split screen), and know how to do it, we just haven't included it as part of the prototype. We want to get more of the game built out before we actually implement split screen, but we now have the knowledge on how to do it.

Features that need to be added for the final game are a title screen, where the user can choose which boat they want to race with. Also, more boat models, which have different stats. We also need to design and implement a map for the race to take place on. We also need to add improved physics for the boats, such as collision detection with the boundaries of the map, other boats,
and powerups. Powerups is another functionality we need to implement (class is done and model is done, just no functionality implemented). Lastly, we need to implement the addition of a second screen for a second player to play on (we have researched this as mentioned above), as well as (time permitting) AI controlled boats for the user to race against.

#### **Notes:**
A list of the key bindings and the action they pertain to:

**KEYBOARD COMMANDS (case sensitive)**
* q - Quit the program
* UP_ARROW - Move boat forward
* DOWN_ARROW - Move boat backwards
* RIGHT_ARROW - Move boat to the right
* LEFT_ARROW - Move boat to the left
