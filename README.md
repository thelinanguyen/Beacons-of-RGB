# Beacons of RGB 🌈
## GUI of the project

<img width="749" height="739" alt="GUI2" src="https://github.com/user-attachments/assets/b5d1aa82-8828-484b-b8dd-f7d347ac38d1" />
<img width="1278" height="1281" alt="GUI3" src="https://github.com/user-attachments/assets/81b94823-56b2-4aa8-bc43-491053fba21c" />


## Project Overview

Beacons of RGB is a C++20 project developed as part of a Data Structures and Algorithms course. The system simulates a network of "beacons" that transmit and combine colored light rays through direct paths and an underground network of optical fibers.The primary objective is the implementation of highly efficient data structures and algorithms to handle large-scale datasets, aiming for an average time complexity of $\Theta(n \log n)$ or better for most operations. The project emphasizes the effective use of the C++ Standard Template Library (STL) and custom algorithmic design. 💡

## Key Features
* Beacon Management
  * Identification: Beacons are managed using unique BeaconID strings.
  * Attributes: Each beacon maintains a name, $(x, y)$ coordinates, and a specific RGB color.
  * Sorting & Queries: Implements efficient searching and sorting based on name (alphabetical), brightness, and coordinates.
* Light Propagation & Color Physics
  * Light Beams: Beacons can emit a single combined light ray to another beacon.
  * Color Blending: When multiple rays enter a beacon, the output color is calculated as the average of the incoming rays and the beacon's inherent color
  * Brightness Calculation: Brightness is determined using the sRGB approximation formula: Brightness = 3*R + 6*G + B 
 
* Optical Fiber Network
  * Graph Structure: A network of bidirectional optical fibers connects various coordinate points
  * Cost Analysis: Each fiber has a designated "cost" representing the time delay (refractive index simulation).
  * Pathfinding Algorithms:
    * Any Route: Rapid identification of any viable path between two points.
    * Least Intersections: Finds the path with the minimum number of fibers (BFS).
    * Fastest Route: Determines the path with the lowest cumulative cost (Dijkstra's algorithm).
    * Cycle Detection: Identifies loops within the fiber network to prevent infinite propagation.

## Technical Specifications
* Language: C++20
* Supports both a CLI (g++) and a Qt-based Graphical User Interface.
* Performance Constraints: Efficiency is a critical evaluation metric. Designed to handle dynamic data scaling without significant performance degradation.

## Program Structure
<pre>
├── datastructures.hh     # Public interface (provided incompletely, to complete by the student)
├── datastructures.cc     # Core implementation (student work)
├── course_code/          # Provided framework & UI files
└── README.md
</pre>

## How to run
Compile and run using either:
* Command-line (g++) for text-based interaction
* QtCreator / qmake for graphical visualization

Behavior and functionality are identical in both modes.
