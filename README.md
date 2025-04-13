
# Algorithm Visualizer

A user-friendly tool to visualize visualizing pathfinding algorithms. Load your own custom Python algorithms, run simulations, and see them in action. Create your own maps or choose from a library of pre-built ones.



## Features

- ✅ Custom Algorithm Support – Load algorithms from the built-in library or your own Python scripts.
- ⏯️ Interactive Control – Pause and resume algorithms at any time to observe their behavior.
- 📊 Real-Time Analytics – Track explored nodes and step counts as the algorithm runs.
- 🗺️ Flexible Map Management – Import, export, and modify maps dynamically.
- ✏️ On-the-Fly Editing – Adjust maps with easy to use map editing tools.

## Screenshots
![App Screenshot](https://github.com/ahzary/Algorithm_Visualizer/blob/master/screenshot_1.PNG?raw=true)

![App Screenshot](https://github.com/ahzary/Algorithm_Visualizer/blob/master/screenshot_2.PNG?raw=true)

![App Screenshot](https://github.com/ahzary/Algorithm_Visualizer/blob/master/screenshot_3.PNG?raw=true)

## Installation

- Windows:
    1- Download the latest release [here](https://github.com/ahzary/Algorithm_Visualizer/releases).
    2- Run `Algorithm_Visualizer.exe` (Windows SmartScreen may warn; see [Notes](#notes)).
- From Source:  
    - Requirements

        - CMake ≥ 3.16
        - Qt5 or Qt6 (Widgets module)
        - Python3 (with development headers)
        - C++17 compiler

    ```bash
    git clone https://github.com/ahzary/Algorithm_Visualizer.git
    cd Algorithm_Visualizer
    mkdir build
    cd build
    cmake ..
    make
    ```

## Custom Algorithm
_this section provides the necassary info to import your own algorithms._

- The Script must be written in **Python**  
- Your Script must have a class Called **"Algorithm"**
    - A constructor that accepts:
        ```python
        def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int])
        ```
    - **map**: A 2D list of integers representing the map grid.
    - **startPoint**: [x, y] coordinate of the starting point.
    - **endPoint**: [x, y] coordinate of the goal.

- Your class must have a function called **"step"**
    - This function is called repeatedly to advance the algorithm one step.
    - It should return a **list** of tile updates: **[x, y, type]**.
-  Your class must also have a function called **"isRunning"**.
    ```python
    def isRunning(self) -> bool
    ```
    - returns **true** as long as the algorithm is still running.

- Optional Methods (Recommended)

     - These are optional but improve integration and visualization:

   - getName 
        - Returns the name of your algorithm. Useful for labeling in the UI.
            ```python
            def getName(self) -> str
            ```

   - def reconstruct_path(self) -> List[List[int]]
        - Returns a list of path tiles (e.g., [x, y, 2]) when the goal is reached.
#### Custom Algorithm example template
```python
from typing import List

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = startPoint
        self.end = endPoint
        self.running = True  # Must be managed by your algorithm

    def step(self) -> List[List[int]]:
        """Required: Return list of [x, y, tile_type] updates"""
        changes = []
        
        # Your algorithm logic here
        # Update self.running when done
        # Append changes like: changes.append([x, y, 3])  # 3=explored
        
        return changes

    def isRunning(self) -> bool:
        """Required: Return False when algorithm finishes"""
        return self.running
        
    def getName(self):
        return self.name 
        
```
## Grid type (Color code)

| Type  | Description           | Default Color  |
|-------|-----------------------|----------------|
| `1`   | Walkable tile         | ⚪️ White       |
| `2`   | Final path            | 🔵 Blue        |
| `3`   | Explored              | 🔴 Red         |
| `4`   | Obstacle              | ⚫️ Black       |
| `6`   | End / Goal point      | 🟢 Green       |
| `7`   | Currently processing  | 🟡 Yellow      |

## Demo Algorithms

- you will find 4 algorithms in the Algorithm drop-down to choose from right away
    - A*
    - Dijkstra
    - Breadth-First Search
    - Depth-First Search
_you are welcome to add more, you can fork and add more Algorithms or optimize existing ones and make a merge request_

## Maps

- maps are saved in **csv** format.
- there is a small library of small of maps which will be expanded in the future.
- you can use the **map editor** tab to modify existing ones or make a new map in minutes!

_you are welcome to add more, you can fork and add more Maps make a merge request_

#### Map editing

- to change a square type you can simply select the new type you want and then you can left click on the square to change its type
- its also possible to change multiple squares by right clicking on the first one and then draging the mouse, you can cancel the drag at any time by clicking right click again or left clicking.
- you can directly set the start and the end from the **Map interaction** tab.
- you can set empty squares and obstacles from **Map edit** tab
    - _please note that gray squares are not support in this version_

## Project Structure
```
Algorithm_Visualizer/
├── CMakeLists.txt
├── main.cpp
├── mainwindow.ui
├── include/            # Header files
├── src/                # Source files
├── resource/           # Icons, styles, assets
├── python/             # Embedded Python files
├── algorithms/         # folder for storing algorithms
├── maps/               # folder for storing maps
└── build/              # CMake build directory
```

## Roadmap
 - Implement execution time.
 - Implement path distance.
## Notes
- Windows may warn about SmartScreen,thats because the app is new and unsigned. Click 'More Info' > 'Run Anyway' to proceed.
- License
    - This is an open source project.
    - This project is licensed under the MIT License.

