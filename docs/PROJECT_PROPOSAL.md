# PROJECT PROPOSAL

**on**

## Smart City Route & Infrastructure Planner

**Project Name:** SmartCity
**Team Name:** Quad Core
**Course:** CSE 232 — Algorithms
**Submission Date:** [Submission date]

### Submitted by

| Student ID | Student Name | Role |
|---|---|---|
| 20254103245 | MUNTAHA MOU JIM | Team Member |
| 20254103248 | OALID KHAN | Team Member |
| 20254103269 | MD. RAFIUL ISLAM | Team Member |
| 20254103279 | MAKHZUM-BIN-HARUN | Team Leader |

### Submitted to

**[Supervisor name]**
[Designation], Department of Computer Science and Engineering (CSE)
Bangladesh University of Business and Technology (BUBT)

---

## 01. NEEDS / PROBLEMS

A city road network connects many locations through many roads. Answering simple
questions about such a network by hand quickly becomes difficult:

- **Finding the shortest route is not straightforward.** When many locations are
  connected by many roads, it is hard to work out by hand which route between two
  places is the shortest.
- **Connectivity is hard to check.** It is not obvious which locations can be
  reached from a given place, or whether some parts of the network are cut off
  from the rest.
- **Choosing which roads are essential is difficult.** Without a systematic
  method, it is hard to find the smallest total road length that still keeps all
  locations connected.
- **Network data is difficult to manage.** Location and road information kept on
  paper or in unorganised files is hard to update, search and reuse.
- **Graph algorithms are often studied only on abstract examples.** Students
  rarely see how these algorithms apply to a familiar problem such as a city road
  network.

This project is an **educational simulation**. It does not aim to be a real-world
urban planning tool. Instead, it shows how classic graph algorithms can be
applied to a **simplified city road network**. The program represents the
network as a **weighted graph** and uses graph algorithms to explore it, find
shortest routes and identify a minimum set of roads that keeps the network
connected.

## 02. GOALS / OBJECTIVES

SmartCity is a console-based application developed in **C++17**. The aim is to
build a working, easy-to-understand program that models a simplified city road
network and demonstrates core graph algorithms on it. The program runs offline
and uses only free tools.

The specific objectives of this project are:

1. Represent the city road network as a **weighted, undirected graph** using an
   **adjacency list**, where locations are vertices and roads are edges weighted
   by distance in kilometres.
2. Allow the user to add, view, update and delete **locations** and **roads**,
   with input validation that keeps the network consistent.
3. Display the road network as a list of locations and their connected roads.
4. Explore connectivity using **Breadth-First Search (BFS)** and **Depth-First
   Search (DFS)**.
5. Find the shortest route between two locations using **Dijkstra's algorithm**.
6. Find a minimum road network using **Kruskal's algorithm** with a **Disjoint
   Set Union (DSU)**.
7. Save and load the network data using **CSV files**.
8. Test the main functions using **automated tests** that compare results with
   known correct answers.

## 03. EXISTING SYSTEM ANALYSIS

Before designing this system, we looked at existing tools related to route
finding and graph algorithms to understand their purpose and how our project
differs.

| System | Type | Focus and Relevance to Our Project |
|---|---|---|
| Google Maps | Online navigation service | Focuses on real-world navigation using real map data. It is designed for travelling, not for studying how graph algorithms work on a user-defined network. |
| VisuAlgo | Educational algorithm visualisation website | Focuses on visualising algorithms step by step on example graphs. It is designed for learning individual algorithms rather than managing a city road network. |
| Manual planning (paper or spreadsheets) | Manual records | Routes and connectivity must be worked out by hand, which is slow and error-prone when the network changes. |

These tools serve their own purposes well. SmartCity addresses a different, more
specific use case: an **offline, educational program** in which the user builds
their own simplified city road network and applies BFS, DFS, Dijkstra's
algorithm and Kruskal's algorithm to it, all within a single console
application.

## 04. FEATURES / SCOPES

The following features will be delivered within the semester. They are described
as they will work in the final version. No features beyond this list are planned
unless approved by the supervisor. Section 05 shows which features are already
completed and which are still planned.

### 04.1. Location Management

- Add a location by name. The system assigns a unique ID automatically
  (1, 2, 3, …).
- View all locations in a table showing their ID, name and number of roads.
- Update (rename) and delete locations. Deleting a location also deletes every
  road connected to it.
- A location name must not be empty, must not be repeated and must not contain a
  comma.

### 04.2. Road Management

- Add a two-way road between two existing locations, with its distance in km.
- View all roads in a table showing both locations and the distance.
- Update a road's distance and delete roads.
- Validation rules: both locations must exist, a road cannot connect a location
  to itself, the distance must be a positive number, and only one road is allowed
  between the same two locations.

### 04.3. City Network Display

- Display the network as an adjacency list: each location followed by the
  locations it is directly connected to, with the road distances.

### 04.4. Network Exploration (BFS and DFS)

- The user chooses a start location.
- **BFS** visits locations level by level using a **queue**.
- **DFS** follows one path as far as possible before backtracking, using
  **recursion** (or an equivalent stack-based approach).
- The program displays the visiting order and the locations reachable from the
  start location.
- Time complexity of both algorithms with an adjacency list: **O(V + E)**, where
  V is the number of locations and E is the number of roads.

### 04.5. Shortest Route (Dijkstra's Algorithm)

- The user selects a start location and a destination.
- Dijkstra's algorithm, using a **priority queue**, will find the shortest route.
  The algorithm gives correct results because all road distances are positive.
- The program displays the full route as a sequence of locations, and the total
  distance.
- A clear message is shown when the destination cannot be reached.
- Time complexity with an adjacency list: **O((V + E) log V)**.

### 04.6. Minimum Road Network (Kruskal's Algorithm)

- Kruskal's algorithm **sorts the existing roads** by distance and selects the
  shortest roads that do not form a cycle. A **Disjoint Set Union (DSU)** is used
  to detect cycles.
- The algorithm only selects from roads that already exist in the graph; it does
  not create new roads.
- If the network is **connected**, the result is a **Minimum Spanning Tree
  (MST)**: the set of roads with the smallest total length that connects all
  locations.
- If the network is **disconnected**, the result is a **Minimum Spanning Forest
  (MSF)**: one minimum spanning tree for each connected part of the network.
- The program displays the selected roads and their total length.
- Time complexity: **O(E log E)**, mainly for sorting the roads.

### 04.7. CSV Data Storage

- Locations and roads will be stored in two CSV files: `locations.csv` and
  `roads.csv`.
- The program will load the saved data when it starts and save the data after
  changes, so that the network is kept between program runs.
- Missing or invalid files will be handled with clear messages.
- A small sample network of Dhaka locations will be provided for testing and
  demonstration. **The sample distances are illustrative values, not actual
  measured distances.**

## 05. DEVELOPMENT MODEL

We have chosen the **Incremental Development Model** for this project.

In this model, the system is built in separate stages. Each stage adds a new
working part to the existing system and is designed, coded and tested before the
next stage begins.

We chose this model for the following reasons:

- The project has clearly separate parts: graph and data management, network
  display, BFS and DFS, Dijkstra's algorithm and Kruskal's algorithm. Each part
  can be built and tested on its own.
- After every increment the program works and can be demonstrated, even if time
  becomes short near the end of the semester.
- Errors are easier to find and fix within a small part of the program.
- Team members can work on different parts at the same time.
- Each completed increment can be shown to the supervisor for feedback before the
  next one begins.

The project is being built in five increments:

| Increment | Content | Status |
|---|---|---|
| Increment 01 | Graph structure, location and road management (add/view), menu, network display and tests | Completed |
| Increment 02 | Update/delete operations, CSV save/load and sample data | Planned |
| Increment 03 | BFS and DFS network exploration | Planned |
| Increment 04 | Dijkstra's shortest route | Planned |
| Increment 05 | Kruskal's algorithm, final integration, testing and documentation | Planned |

**Increment 01 (completed)** currently provides:

- A C++ graph structure using an adjacency list.
- Adding and viewing locations.
- Adding and viewing roads.
- Displaying the city network.
- A console menu with input validation.
- Automated tests: 39 checks, all passing.

BFS, DFS, Dijkstra's algorithm, Kruskal's algorithm and CSV storage are **not yet
implemented**; they are planned for Increments 02–05.

```
 Increment 01        Increment 02        Increment 03      Increment 04      Increment 05
 [COMPLETED]         [PLANNED]           [PLANNED]         [PLANNED]         [PLANNED]
 Graph, Add/View --> Update/Delete,  --> BFS & DFS    -->  Dijkstra     -->  Kruskal, Final
 Menu, Tests         CSV, Sample Data                                        Testing & Docs
      |                   |                   |                 |                 |
 Design & Code       Design & Code       Design & Code     Design & Code     Design & Code
 Test & Review       Test & Review       Test & Review     Test & Review     Test & Review
      <------------------------- Feedback / Revision Loop ------------------------->
```

*Figure 01: Incremental Development Model — Smart City Route & Infrastructure Planner*

## 06. FEASIBILITY ANALYSIS & POSSIBLE RISKS

### 06.1. Feasibility Analysis

**Technical Feasibility**
The system uses C++17, the C++ Standard Template Library (STL) and simple file
handling. The team members have studied programming and basic data structures in
their coursework, and the algorithms used are standard topics in an algorithms
course. The
required tools (GCC, CMake and Visual Studio Code) are free and available on
Windows. No paid software or special hardware is required.

**Economic Feasibility**
All software tools are free and open source. Development is done on the team
members' existing personal computers. No server, hosting or software licence is
needed.

**Operational Feasibility**
The system is a menu-driven console application. Users choose options by number
and type names and distances when asked, so only basic computer skills are
needed.

### 06.2. Possible Risks

- **Incorrect algorithm results:** an error in an algorithm could produce wrong
  routes or totals. This will be reduced by automated tests that compare the
  results with answers worked out by hand on small example networks.
- **Data file errors:** a missing or damaged CSV file could cause data loss or
  incorrect data. The program will check the files when loading and report any
  problems clearly.
- **Scope creep:** the team may be tempted to add extra features. The feature
  list in Section 04 is fixed and will only change with the supervisor's
  approval.
- **Team coordination issues:** four members working on different parts may make
  conflicting changes. This will be managed by agreeing on the shared graph
  structure first and by using Git and GitHub for version control.
- **Academic schedule conflicts:** exams and other coursework may reduce the time
  available. The incremental model ensures that a working program exists after
  each increment.

## 07. REQUIREMENTS

**Hardware Requirements**

- Any PC or laptop with at least 2 GB of RAM.
- At least 1 GB of free disk space for the compiler, tools and project files.
- Standard keyboard and monitor.

**Software Requirements**

- Operating system: Windows 10 or Windows 11.
- Compiler: GCC (MinGW-w64) with C++17 support.
- Build system: CMake 3.21 or newer. Any build tool supported by CMake can be
  used; the team uses Ninja, which is optional.
- Editor: Visual Studio Code with the C/C++ extension.
- Version control: Git and GitHub.

**Programming Language**

- C++ (C++17 standard).
- Standard library headers such as `iostream`, `string`, `vector`, `map`,
  `queue`, `algorithm`, `sstream`, `iomanip` and `cmath`.

**Data Storage**

- Locations and roads will be stored in CSV text files, read and written using
  C++ file streams (`ifstream` and `ofstream`).

**Internet Access**

- The application will operate completely offline after installation. Internet
  access may be required for downloading development tools and collaborating
  through GitHub.

### 07.1. Source Code Repository

The project source code is maintained on GitHub for version control and team
collaboration.

**Repository:** https://github.com/makhzumbinharun/smart-city-route-planner

GitHub is used for:

- Source code management.
- Team collaboration.
- Progress tracking.
- Backup and version history.
- Documentation storage.

## 08. ACTIVITY AND TIME SCHEDULE / PROJECT BREAKDOWN WITH TIME ESTIMATION

The table below shows the estimated **total effort of the whole team** (not the
effort of each member). All values are approximate.

| # | Stages / Tasks | Estimated Effort (man-hours) | Status |
|---|---|---|---|
| **Stage 1** | **Analysis and Design** | | |
| 1.1 | Requirements analysis and feature finalisation | 06 | Completed |
| 1.2 | System design, module breakdown and work plan | 06 | Completed |
| 1.3 | Graph data structure and CSV file format design | 06 | In progress |
| 1.4 | Flowchart and algorithm design | 08 | Planned |
| **Stage 2** | **Implementation** | | |
| 2.1 | Graph structure, adding and viewing locations and roads, menu | 16 | Completed (Increment 01) |
| 2.2 | Update/delete operations and CSV save/load | 12 | Planned |
| 2.3 | BFS and DFS traversal | 10 | Planned |
| 2.4 | Dijkstra's shortest route | 12 | Planned |
| 2.5 | Kruskal's minimum spanning tree with DSU | 12 | Planned |
| **Stage 3** | **Testing and Quality Assurance** | | |
| 3.1 | Unit testing for each module | 10 | In progress |
| 3.2 | Integration testing and bug fixing | 08 | Planned |
| **Stage 4** | **Deployment** | | |
| 4.1 | Project report and documentation writing | 08 | Planned |
| 4.2 | Presentation and viva preparation | 06 | Planned |
| | **Total Estimated Team Effort** | **120 man-hours** | |

*Status labels reflect the repository at the time of writing. Task 2.1 is
confirmed by the completed Increment 01 code, and task 3.1 by the existing
automated tests. The statuses of tasks 1.1–1.3 are based on the project
documentation and require confirmation by the team.*

## 09. BUDGET

This project does not require any financial investment. All tools, compilers and
software are free and open source, and development is done on the team members'
existing personal computers.

| Project Item | Cost |
|---|---|
| Software (GCC, CMake, Visual Studio Code, Git) | Free |
| Development computers | Existing personal computers |
| Database or server licence | Not required |
| Hosting or internet services for the application | Not required |
| **Total estimated development cost** | **BDT 0** |

The total excludes optional printing and personal expenses.

## 10. DEVELOPMENT TEAM & STAKEHOLDERS

### 10.1. Development Team

The responsibilities below are a **proposed allocation** and may be adjusted by
the team during the project.

| Student ID | Student Name | Role | Proposed Responsibilities |
|---|---|---|---|
| 20254103279 | MAKHZUM-BIN-HARUN | Team Leader | Team coordination, system design, graph data structure, menu program and integration |
| 20254103269 | MD. RAFIUL ISLAM | Team Member | Update/delete operations, CSV file handling, testing and documentation |
| 20254103245 | MUNTAHA MOU JIM | Team Member | BFS and DFS implementation and testing |
| 20254103248 | OALID KHAN | Team Member | Dijkstra's and Kruskal's algorithm implementation and testing |

### 10.2. Stakeholders

| Stakeholder | Role in System | Interest |
|---|---|---|
| Students and learners | Main users | Want to see how graph algorithms find routes, check connectivity and select a minimum road network on a simplified city model |
| Course Supervisor | Academic evaluator | Reviews project progress and evaluates the correct use of algorithms and data structures |
| Development team | Developers | Want to gain practical experience in implementing and testing graph algorithms in C++ |

## 11. LIMITATIONS & FUTURE WORKS

### 11.1. Limitations

The following are known limitations of this version of the system:

- The program has a console-based interface only; there is no graphical user
  interface.
- The network is an abstract graph, not a geographical map. Location coordinates
  and real map data are not used.
- The sample road distances are illustrative values, not measured city data.
- Each road has only one weight: its distance in km. Travel time, fare, traffic
  and construction cost are not considered.
- All roads are undirected (two-way), and only one road is allowed between the
  same two locations.
- The application is single-user and does not support concurrent access.
- Windows is the target platform for development and testing. Other operating
  systems are not a project requirement.

### 11.2. Future Works

The following features are **not part of this project** but could be added in
future versions:

- A graphical user interface using Qt 6, reusing the same graph and algorithm
  code.
- Additional road weights, such as travel time, fare and construction cost.
- Directed (one-way) roads and multiple transport modes between locations.
- Comparison with other algorithms, such as Prim's algorithm for minimum spanning
  trees and the Floyd–Warshall algorithm for all-pairs shortest paths.
- Infrastructure budget planning, such as selecting projects within a limited
  budget.
- Importing real map data and displaying the network graphically.

## 12. REFERENCES

1. T. H. Cormen, C. E. Leiserson, R. L. Rivest and C. Stein, *Introduction to
   Algorithms*, 4th ed. Cambridge, MA: MIT Press, 2022.
2. E. W. Dijkstra, "A note on two problems in connexion with graphs,"
   *Numerische Mathematik*, vol. 1, pp. 269–271, 1959.
3. J. B. Kruskal, "On the shortest spanning subtree of a graph and the traveling
   salesman problem," *Proceedings of the American Mathematical Society*,
   vol. 7, no. 1, pp. 48–50, 1956.
4. B. Stroustrup, *The C++ Programming Language*, 4th ed. Boston, MA:
   Addison-Wesley, 2013.
5. R. S. Pressman and B. R. Maxim, *Software Engineering: A Practitioner's
   Approach*, 8th ed. New York, NY: McGraw-Hill Education, 2014. (Referenced for
   the Incremental Development Model.)
6. Free Software Foundation, *GCC, the GNU Compiler Collection*. Available:
   https://gcc.gnu.org
7. Kitware, *CMake Documentation*. Available: https://cmake.org/documentation
8. Microsoft, *Visual Studio Code Documentation*. Available:
   https://code.visualstudio.com/docs

---

Copyright © 2026 Quad Core. All rights reserved.
