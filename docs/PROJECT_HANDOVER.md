# Project Handover & Continuation Summary

**Smart City Route & Infrastructure Planner** — Team Quad Core
CSE 232 (Algorithm/Algorithms course — see pending decision P1) · BUBT

Repository: <https://github.com/makhzumbinharun/smart-city-route-planner>

> Written on 2026-11-04 as the permanent context for future work on this
> project. Anything marked **(verify)** was not confirmed when this file was
> written. Always check the real repository state before acting on this
> document (see section 10).

---

## READ THIS FIRST — Instructions for the Next Claude Session

**Who you are working with.** The team lead (GitHub: `makhzumbinharun`) is a
university student in a beginner C++ team. The project is a course assignment
for CSE 232 at BUBT. The whole team must be able to understand and explain
every line of code, so simplicity matters more than cleverness.

**How to work with the user**

1. Explain simply, step by step, in beginner-friendly language. Keep answers
   concise.
2. The user works on **Windows** with **VS Code**, **CMD** and **PowerShell**,
   in **C++17**. Give commands for those shells (say which shell each command
   is for).
3. The user wants to **understand a change before it is made**. Propose and
   explain first; implement only after approval.
4. **Inspect the actual repository first** (`git status`, `git log`, the
   files themselves). Do not trust this document over the real state, and do
   not repeat work that is already done.
5. **Never modify, stage, commit or push files without explicit permission.**
   Each step (edit → stage → commit → push) has been approved separately so far.
6. Report test results only when the tests were actually run, and show real
   output.

**Git safety rules (strict)**

- `dev` is the working branch. All work happens on `dev`.
- `master` must remain untouched. It changes only through a Pull Request from
  `dev` that **the user** reviews and merges, and only when the user approves a
  release.
- Never force-push, rebase, reset, amend, or rewrite history. Never delete
  branches.
- Never use `git add .` or `git add -A`. Stage files by exact path.
- Run `git diff --cached --name-only` and `git diff --cached --check` before
  every commit, and show the result.
- **No AI attribution anywhere:** no `Co-Authored-By` trailer, no "Generated
  with Claude Code", no Claude/Anthropic/AI mention in commits, PRs, code or
  docs. Use only the configured Git identity
  (`makhzumbinharun <makhzum204@gmail.com>`).

**Protected files**

- `docs/PROJECT_PROPOSAL.md` is **untracked and must stay untouched**: do not
  open, edit, format, move, delete, stage or commit it unless the user
  explicitly asks. Check it with its MD5 hash (section 4).
- Local branch `backup/dev-before-claude-removal`: do not delete.
- Working data `data/locations.csv`, `data/roads.csv`, `data/next_id.txt`: the
  user's own data; never overwrite (they are ignored by Git).

**Before you start any task**, run the "Check the state" commands in
section 5.4 and compare the result with section 4.

---

## 1. Project Overview

### 1.1 Purpose and problem

A city's road network can be modelled as a **weighted, undirected graph**:
locations are vertices, roads are edges, and each road's weight is its
distance in km. The program lets a user build and edit such a network and then
answer planning questions with classic graph algorithms:

| Question | Algorithm |
|---|---|
| Which locations can be reached from a start, and in what order? | BFS, DFS |
| What is the shortest route (total km) between two locations? | Dijkstra |
| Which roads keep every location connected with the smallest total distance? | Kruskal (+ DSU) |

### 1.2 Objectives and scope

- Represent the city with an **adjacency list**.
- Add, show, rename, delete locations; add, show, update, delete roads.
- Save and load the city automatically (CSV files).
- Implement and explain BFS, DFS, Dijkstra and Kruskal with DSU.
- Validate all input; never crash on bad input or bad files.
- Automated tests for the graph, storage and every algorithm.

**Deliberately out of scope** (decision log, `docs/development-notes.md`):
budget planning, Floyd–Warshall, Prim, Merge Sort, Greedy/0-1 Knapsack,
transport modes, fares, colours/box-drawing console UI, and the Qt GUI (parked
in `future-gui/`).

### 1.3 Technologies

| Item | Value |
|---|---|
| Language | C++17 (standard library only, no external libraries) |
| Program type | Console (command-line) menu program |
| Build system | CMake (minimum 3.21) |
| Compiler | MinGW-w64 GCC (currently WinLibs GCC 16.2.0) |
| Generator | MinGW Makefiles (main); Ninja (optional) |
| Tests | Own `check()` function in `tests/tests.cpp`, run through CTest |
| Platform | Windows 10/11 only (no Linux/macOS testing) |

### 1.4 Architecture and folder structure

```
CMakeLists.txt          Builds two programs: SmartCityPlanner and tests
include/graph.h         Edge, Road, Graph class, MAX_ROAD_DISTANCE,
                        isValidDistance(), trimName()
include/storage.h       saveCity(), loadCity()
include/algorithms.h    bfs(), dfs(), dijkstra(), kruskal(),
                        RouteResult, SpanningResult
src/graph.cpp           Graph functions (the city data)
src/storage.cpp         CSV save/load (returns messages, never prints)
src/algorithms.cpp      BFS, DFS, Dijkstra, Kruskal, DisjointSet
src/main.cpp            The menu: all keyboard input and screen output
tests/tests.cpp         Automated checks (299)
data/sample/            Sample data: 10 Dhaka locations, 14 roads, next ID 11
data/.gitkeep           Keeps the data folder in Git (see pending decision P4)
docs/                   algorithms.md, user-guide.md, development-notes.md
                        (+ untracked PROJECT_PROPOSAL.md, PROJECT_HANDOVER.md)
future-gui/             Parked Qt 6 prototype from M1 (not built, does not
                        compile as-is)
.gitignore              Ignores build folders, *.exe, working data, test_data/
.gitattributes          Line-ending rules (CSV files use LF)
```

**Key design rule:** `graph.cpp`, `storage.cpp` and `algorithms.cpp` never
print or read the keyboard. Only `main.cpp` does. This keeps the logic testable
and reusable by a possible future GUI.

CMake targets: `SmartCityPlanner` (main.cpp + graph + storage + algorithms) and
`tests` (tests.cpp + graph + storage + algorithms); `add_test(NAME tests ...)`.
Compiler warnings `-Wall -Wextra` are on.

### 1.5 Graph representation

```cpp
struct Edge { int to; double distance; };          // one road, seen from one end
struct Road { int from; int to; double distance; }; // one road, listed once

class Graph {
    std::map<int, std::string>       names;      // location ID -> name
    std::map<int, std::vector<Edge>> adjacency;  // location ID -> its roads
    int nextId;                                  // ID for the next new location
};
```

- Undirected: each road is stored in **both** locations' lists.
- At most one road between two locations; no road from a location to itself.
- IDs start at 1, are given automatically and are **never reused** (also after
  restart, thanks to `next_id.txt`).
- Public functions: `addLocation`, `addLocationWithId` (loading only),
  `renameLocation`, `removeLocation` (also removes its roads), `hasLocation`,
  `findLocationId`, `nameExists`, `getName`, `getLocationIds`, `locationCount`,
  `getNextId`, `setNextId` (can only raise the value), `addRoad`, `updateRoad`,
  `removeRoad`, `hasRoad`, `getNeighbors`, `getRoads` (each road once,
  `from < to`), `roadCount`.

**Validation rules (enforced inside `Graph`, so every caller is protected):**

- Names are trimmed (`trimName`: spaces, tabs, `\r`, `\n` at both ends), must
  not be empty, and must be unique **ignoring upper/lower case** ("BUBT",
  " bubt " and "BuBt" are the same name). The typed capitals are kept for
  display. A location may rename itself to different capitals of its own name.
  The menu also refuses commas (they would break the CSV format).
- Distances: `isValidDistance(d)` = finite, `> 0` and `<= MAX_ROAD_DISTANCE`
  (10,000 km). Rejects 0, negatives, infinity, NaN and huge values.

### 1.6 Data storage

| File | Format | Example |
|---|---|---|
| `data/locations.csv` | header `id,name`, then one location per line | `1,BUBT` |
| `data/roads.csv` | header `from,to,distance` | `1,2,2.5` |
| `data/next_id.txt` | one number: the next location ID | `11` |

- Loaded automatically at start; saved automatically after every successful
  change (`(Saved)` is printed). No manual save option.
- Paths are **relative to the current folder**, so the program must be started
  from the project root.
- Missing files → empty city. Invalid lines are **skipped with a message**
  (file, line number, reason); the next save writes clean files.
- Missing/invalid `next_id.txt` → falls back to "highest ID + 1" with a note.
- Working files are ignored by Git; `data/sample/` holds the sample.
- Tests write temporary files into `test_data/` (ignored by Git).

### 1.7 Features (menu)

```
1. Add location        5. Add road
2. Show locations      6. Show roads
3. Rename location     7. Update road distance
4. Delete location     8. Delete road
9. Show city network (adjacency list)
10. Traverse network using BFS
11. Traverse network using DFS
12. Find shortest route (Dijkstra)
13. Minimum Spanning Tree (Kruskal)
0. Exit
```

- Input is read a whole line at a time; `readInt`/`readDouble` accept a number
  only if nothing else follows (`12abc` and `4.5km` are refused and asked
  again). End of input (Ctrl+Z, Enter) closes the program.
- Deleting a location asks `Are you sure? (y/n)` and also deletes its roads.
- Plain console output (`cout`, `setw` tables), no colours.

---

## 2. Algorithm Implementation

V = number of locations, E = number of roads. All in `src/algorithms.cpp`,
declared in `include/algorithms.h`. Full explanations with worked examples are
in `docs/algorithms.md`.

### 2.1 BFS — `bfs(city, startId)` → `vector<int>`

- **Purpose:** visiting order of all locations reachable from the start,
  nearest (fewest roads) first.
- **How:** `queue<int>`; a location is marked visited in a `set<int>` **when
  it is queued**, so it is never queued twice.
- **Complexity:** O(V + E) time, O(V) extra memory; sorting neighbour lists
  adds O(E log V) worst case.
- **Decisions / edge cases:** neighbours are visited in **increasing ID order**
  (`sortedNeighborIds()`), so results are identical after saving/loading.
  Unknown start → empty result. Menu option 10 also lists **unreachable**
  locations.

### 2.2 DFS — `dfs(city, startId)` and `dfsVisit(...)` → `vector<int>`

- **Purpose:** same question as BFS, but follows one path as deep as possible,
  then backtracks.
- **How:** **recursion** (`dfsVisit`), `set<int>` of visited locations.
- **Complexity:** O(V + E) time, O(V) extra memory (visited set + call stack);
  sorting adds O(E log V).
- **Decisions:** recursion chosen for clarity (networks are small); same
  increasing-ID neighbour order. Menu option 11.

### 2.3 Dijkstra — `dijkstra(city, startId, endId)` → `RouteResult`

- `RouteResult { bool found; double distance; vector<int> path; }`
- **Purpose:** route with the smallest **total distance** (not fewest roads).
- **How:** `map<int,double> dist`, `map<int,int> parent`, min-priority queue
  `priority_queue<pair<double,int>, vector<...>, greater<...>>`. Outdated queue
  entries are skipped (`d > dist[current]`); stops early when the destination
  is taken from the queue; path rebuilt from `parent` and reversed.
- **Complexity:** O((V + E) log V) time, O(V + E) extra memory.
- **Decisions / edge cases:** a parent is replaced only for a **strictly**
  shorter distance → the same route is always chosen among equal ones. Start =
  destination → distance 0, path = start. Unknown IDs or no connection →
  `found = false` ("No route exists between these locations."). Correct
  because all distances are positive; the 10,000 km cap keeps totals finite.
  Menu option 12 shows each road on the route and the total.

### 2.4 Kruskal — `kruskal(city)` → `SpanningResult`

- `SpanningResult { vector<Road> roads; double totalDistance; int components; bool connected; }`
- **Purpose:** the set of existing roads connecting all locations with the
  smallest total distance (Minimum Spanning Tree). If the network is in
  several parts, the result is a **Minimum Spanning Forest** (one tree per
  component) and the number of components is reported.
- **How:** `getRoads()` (each road once), `sort` with `isShorterRoad()`
  (distance, then `from`, then `to`), then keep a road only if `unite()`
  succeeds (DSU).
- **Complexity:** O(E log E + V log V) time (O(E log E) when connected),
  O(V + E) extra memory.
- **Decisions / edge cases:** never invents roads. Tie-break by IDs → the same
  roads are always chosen. Empty city → 0 roads, 0 components, not connected.
  One location → 1 component, connected. Selected roads = V − components.
  Menu option 13.

### 2.5 Disjoint Set Union — `struct DisjointSet` in `src/algorithms.cpp`

- `map<int,int> parent`, `map<int,int> rank`; `makeSet(id)`, `findRoot(id)`
  (with **path compression**), `unite(a, b)` (with **union by rank**; returns
  `false` if already in the same set — this is how Kruskal detects a cycle).
- **Why maps:** location IDs are not 0..V−1 (deleted IDs leave gaps).
- **Complexity:** almost constant per operation (inverse Ackermann) plus
  O(log V) for map lookups.
- Minor open suggestion B6: the struct could be made file-only (`static` /
  unnamed namespace) like the other helpers.

---

## 3. Development History

All commits are on `dev` (dated 2026-11-04). Test counts below were measured
by rebuilding each commit with GCC 16.2.0 when this document was written
(see section 6).

| Milestone | Commit | Message |
|---|---|---|
| — | `8374c4c` | Initialize project repository (this is `master`) |
| M1 | `78f875e` | M1: Project foundation with CMake, core library, Qt window and tests |
| S1 | `6f76f77` | S1: Simple console program with locations, roads and network display |
| S2 | `c3fc761` | S2: Update/delete operations and CSV save/load |
| S3 | `f416196` | S3: BFS and DFS network traversal |
| S4 | `85f59c7` | S4: Dijkstra's shortest route |
| S5 | `99fa2e4` | S5: Kruskal's minimum spanning tree |
| S6.1 | — | Read-only audit (no commit) |
| S6.2 | `7bcae7a` | S6.2: Name and distance validation improvements |
| S6.3 | `14e269f` | S6.3: Documentation consistency and final review |
| S6.4 | `bf0ce8a` | S6.4: Final documentation update |

**M1 — Project foundation (`78f875e`).** CMake project, a core library, a Qt 6
window and tests. The original M1 commit (`9ec1ac7`) contained AI attribution;
the user amended it manually to `78f875e` (the original is kept on the local
branch `backup/dev-before-claude-removal`). After M1, the project was
re-scoped to a simple CLI: Qt code moved to `future-gui/`, separate `include/`
and `src/` kept, only Kruskal (no Prim), plain console without colours.

**S1 — Console program (`6f76f77`).** `Graph` class (adjacency list), add/show
locations and roads, show network, plain menu, `readInt`/`readDouble`, tests.

**S2 — Edit, delete, save/load (`c3fc761`).** Rename/delete locations (with
confirmation, roads removed too), update/delete roads, `storage.cpp` with
CSV save/load, automatic saving, invalid-line handling, sample data. Fix
requested by the user: IDs of deleted locations are never reused
(`next_id.txt`, `setNextId`).

**S3 — BFS and DFS (`f416196`).** `algorithms.cpp`, BFS (queue), DFS
(recursion), sorted neighbour order, unreachable locations listed, menu 10/11.

**S4 — Dijkstra (`85f59c7`).** `RouteResult`, priority queue with outdated-entry
skip, early stop, strict-improvement parent rule, menu 12; docs table corrected.

**S5 — Kruskal (`99fa2e4`).** `DisjointSet`, `SpanningResult`, MST/MSF with
component count, ID tie-break, test helper `isSpanningForest()`, menu 13;
complexity statement in docs corrected.

**S6.1 — Audit (no commit).** Full read-only review. No Critical/High issues.
Two Medium issues (fixed in S6.2): A1 near-duplicate names; A2 huge distances
(1e308) made Dijkstra totals overflow to infinity, so a connected pair showed
"No route". Other findings: see section 6.3.

**S6.2 — Validation (`7bcae7a`).** `trimName`, `findLocationId`,
case-insensitive `nameExists`, rename may change its own capitals,
`MAX_ROAD_DISTANCE` (10,000 km) and `isValidDistance` used by `Graph`, menu and
CSV loader. Files: `include/graph.h`, `src/graph.cpp`, `src/main.cpp`,
`src/storage.cpp`, `tests/tests.cpp` + 4 docs. +63 checks → **299 passed, 0
failed**. Manual tests: CSV with duplicate-looking names (skipped with notes,
rest trimmed), roads of 10,000.5 and 1e308 km skipped, menu refused `  bubt  `,
saved ` Gulshan ` as `Gulshan`, Farmgate → FARMGATE rename allowed, 10,000.01
and 10,001 refused, exactly 10,000 accepted, Dijkstra/Kruskal correct, restart
reloaded clean data.

**S6.3 — Documentation consistency (`14e269f`).** README status, removed
non-existent `docs/project-proposal.pdf` reference, Ninja made optional with a
MinGW Makefiles alternative, S5 marked Done, S6.2 milestone row, name-rule row.
Verified with both generators: 0 warnings, 299 passed.

**S6.4 — Final documentation update (`bf0ce8a`, committed and pushed by the
user).** README section "Windows: Build and Run (CMD)" (8 steps, safe sample
copy), development notes and user guide updated to the WinLibs GCC 16.2.0 /
CMake 4.4.4 toolchain (old Qt-installer toolchain kept as labelled history),
requirements list cleaned up. The documented CMD steps were run on a fresh
export: build without warnings, safe copy did not overwrite existing data,
tests passed.

---

## 4. Current Git and GitHub Status (when this file was written)

| Item | Value |
|---|---|
| Repository | <https://github.com/makhzumbinharun/smart-city-route-planner> |
| Remote | `origin` (HTTPS) |
| Current branch | `dev` |
| Latest commit | `bf0ce8a4c6e54833c3476bc38a7136f720d5fcd6` — "S6.4: Final documentation update" |
| `origin/dev` | `bf0ce8a` (in sync, 0 ahead / 0 behind) |
| `master` = `origin/master` | `8374c4c5acf4fbad9f83b77ecb31211cca873880` — "Initialize project repository" (never touched) |
| Local-only branch | `backup/dev-before-claude-removal` at `9ec1ac7` (do not delete) |
| Untracked | `docs/PROJECT_PROPOSAL.md`, `docs/PROJECT_HANDOVER.md` (this file) |
| Modified tracked files | none |
| Git identity | `makhzumbinharun <makhzum204@gmail.com>` |

`docs/PROJECT_PROPOSAL.md` MD5: `b0fd494416b48564c404cc0985b552cc`. If the
hash changes, someone edited the proposal.

No PR from `dev` to `master` has been opened yet **(verify on GitHub)**.
Whether GitHub branch-protection settings are enabled is unknown **(verify)**;
the rules above are followed by agreement regardless.

---

## 5. Development Environment

### 5.1 Current setup (team lead's PC)

| Item | Value |
|---|---|
| OS | Windows 11 Pro |
| Compiler | WinLibs standalone MinGW-w64 GCC 16.2.0 (UCRT, POSIX threads, SEH), target `x86_64-w64-mingw32` |
| Location | `S:\mingw64\bin` (on the Windows **System PATH**) |
| CMake | 4.4.4 — `S:\mingw64\bin\cmake.exe` (also `ctest.exe` there) |
| Make | GNU Make 4.4.1 — `S:\mingw64\bin\mingw32-make.exe` |
| Ninja | `S:\mingw64\bin\ninja.exe` (optional generator; version not recorded) |
| Editor / shells | VS Code; CMD (main), PowerShell; Git Bash is also present |
| Project folder | `P:\BUBT\1-4\CSE 232\smart-city-route-planner` |
| Build folder | `build-mingw` (MinGW Makefiles); `build` is used for Ninja |

**Previous toolchain (history only, removed):** GCC 15.1.0, CMake 3.30.5 and
Ninja 1.12.1 from the Qt Online Installer in `S:\Qt\Tools\...`. M1–S6.3 were
built with it. Qt, Qt Creator, MSYS2 and the old MinGW are no longer installed
and are **not** needed.

### 5.2 Build, run and test — CMD (main way)

```cmd
cd /d "P:\BUBT\1-4\CSE 232\smart-city-route-planner"
cmake -S . -B build-mingw -G "MinGW Makefiles"
cmake --build build-mingw
if not exist data mkdir data
if not exist data\locations.csv copy data\sample\locations.csv data\locations.csv
if not exist data\roads.csv copy data\sample\roads.csv data\roads.csv
if not exist data\next_id.txt copy data\sample\next_id.txt data\next_id.txt
build-mingw\SmartCityPlanner.exe
ctest --test-dir build-mingw --output-on-failure
```

Configure once; after code changes only `cmake --build build-mingw`. Success
looks like `100% tests passed out of 1` (CTest runs one program, `tests.exe`,
which prints `299 passed, 0 failed`).

### 5.3 The same in PowerShell

```powershell
Set-Location "P:\BUBT\1-4\CSE 232\smart-city-route-planner"
cmake -S . -B build-mingw -G "MinGW Makefiles"
cmake --build build-mingw
ctest --test-dir build-mingw --output-on-failure
.\build-mingw\tests.exe | Select-Object -Last 1      # shows "299 passed, 0 failed"

# Copy sample data only where no working file exists (never overwrites)
foreach ($f in 'locations.csv','roads.csv','next_id.txt') {
    if (-not (Test-Path "data\$f")) { Copy-Item "data\sample\$f" "data\$f" }
}
.\build-mingw\SmartCityPlanner.exe

# Clean rebuild (delete only the build folder)
Remove-Item -Recurse -Force build-mingw
cmake -S . -B build-mingw -G "MinGW Makefiles"; if ($?) { cmake --build build-mingw }

# Optional: Ninja in a separate folder
cmake -S . -B build -G Ninja; if ($?) { cmake --build build }
```

### 5.4 Common Git operations (PowerShell)

```powershell
# Check the state (run before every task)
git branch --show-current
git status --short
git log --oneline -5
git fetch origin
git rev-list --left-right --count origin/dev...dev     # "0 0" = in sync
git rev-parse --short master origin/master              # must stay 8374c4c
Get-FileHash docs\PROJECT_PROPOSAL.md -Algorithm MD5    # B0FD4944...B552CC

# Commit (only after approval) — stage exact paths, never "git add ."
git add -- README.md docs/user-guide.md
git diff --cached --check
git diff --cached --name-only
git commit -m "S7: Short description"

# Push (only after separate approval)
git push origin dev
```

Pull Request (only when the user decides to release): open
<https://github.com/makhzumbinharun/smart-city-route-planner/compare/master...dev>,
base `master`, compare `dev`. The **user** reviews and merges it.

### 5.5 Problems met and their solutions

| Problem | Solution |
|---|---|
| `liblto_plugin.dll: error loading plugin`, `Permission denied`, "An Application Control policy has blocked this file" | Windows Smart App Control blocked the compiler and new programs. The user investigated and then turned Smart App Control off. Not a code problem. |
| `libstdc++-6.dll was not found` when starting the program | No static linking; start from a terminal where `S:\mingw64\bin` is on the PATH. |
| `'g++' is not recognized...` | Add `S:\mingw64\bin` to PATH, then open a **new** terminal (old terminals keep the old PATH; this also happened with tool sessions after the toolchain switch). |
| Saved city not loaded | Start the program from the project root (relative `data/` paths). |
| CMake error when changing generator | A build folder supports one generator; delete it or use a different folder. |
| CTest summary text differs | CMake 4.4.4 prints `100% tests passed out of 1`; CMake 3.30 printed `100% tests passed, 0 tests failed out of 1`. Both mean success. |
| `LF will be replaced by CRLF` warnings | Harmless; line endings are handled by `.gitattributes`. |
| DOCX generation through Word automation was very slow | Stopped by the user; DOCX/PDF of the proposal postponed to near submission. |
| AI attribution in the first M1 commit | User amended it manually; rule since then: no attribution, no further history rewrites. |

---

## 6. Testing and Quality Assurance

### 6.1 Automated tests

`tests/tests.cpp` uses `check(condition, description)` (prints PASS/FAIL;
`main()` returns 1 on any failure, so CTest fails). Coverage: locations,
roads, rename/delete, CSV save/load (empty city, ID gaps, missing files, a
file with 16 invalid lines, no header, impossible save folder, ID persistence),
BFS/DFS (known orders, cycles, disconnected parts, stability after
save/load), Dijkstra (same start/destination, unknown IDs, unreachable, longer
direct road vs shorter multi-road route, ties, decimals, after edits and
reload), Kruskal (empty, one/two locations, known MSTs, equal distances, forests,
after edits and reload; every result checked by `isSpanningForest()`), and
S6.2 names/distances.

Checks per milestone (each commit rebuilt with GCC 16.2.0 on 2026-11-04):

| Commit | Checks |
|---|---|
| S1 `6f76f77` | 39 passed, 0 failed |
| S2 `c3fc761` | 142 passed, 0 failed |
| S3 `f416196` | 172 passed, 0 failed |
| S4 `85f59c7` | 202 passed, 0 failed |
| S5 `99fa2e4` | 236 passed, 0 failed |
| S6.2 `7bcae7a` | 299 passed, 0 failed |
| S6.4 `bf0ce8a` (current) | 299 passed, 0 failed |

**Latest result:** current code → **299 passed, 0 failed**; CTest
`100% tests passed out of 1`; build with `-Wall -Wextra` gives **0 warnings,
0 errors** with both MinGW Makefiles and Ninja.

### 6.2 Manual testing done

- The program was tried manually during the milestone reviews, but a detailed
  written record exists only for S6.2 (section 3). Earlier manual results are
  not recorded here **(verify if needed for the report)**.
- S6.4: the README CMD steps were executed exactly as written on a fresh
  export (configure, build, safe sample copy twice, run, CTest, rebuild).

### 6.3 Known limitations and open audit items (not fixed, all Low/Suggestion)

| ID | Item |
|---|---|
| A3 | Distances are shown with 2 decimals: 0.001 km is stored correctly but displayed as `0.00 km`. |
| A4 | Saving rewrites the CSV files directly; a crash or power loss during a save could leave a half-written file (the loader would then skip damaged lines with notes). |
| B1 | "Is this exactly a number?" parsing exists in both `main.cpp` and `storage.cpp` (small, deliberate duplication). |
| B2 | `future-gui/` no longer compiles as-is (refers to the removed `core/project_info.h`). Keep or remove? (P3) |
| B3 | `data/.gitkeep` is no longer needed because `data/sample/` keeps the folder. (P4) |
| B4 | `main.cpp` (621 lines) is the largest file; splitting is not necessary. |
| B5 | Names longer than about 20 characters, or non-English names, push table columns out of line. |
| B6 | `DisjointSet` could be made file-only (`static`/unnamed namespace). |
| C1 | The proposal is outdated (progress section, no mention of `next_id.txt`). Needs the user's go-ahead to edit. |
| C4 | Course title wording: docs say "CSE 232 — Algorithms"; the user sometimes writes "Algorithm". (P1) |
| — | DFS uses recursion; a very large network (thousands of locations in one chain) could exceed the call stack. Not a concern for course-size data. |
| — | Windows only; Linux/macOS not tested. |

### 6.4 Still to verify before submission

- A final **clean clone** of `dev` builds and passes tests (planned step).
- The documentation examples (BFS order, Dijkstra BUBT → Gulshan 15.00 km,
  Kruskal 9 roads / 35.20 km on the sample data) still match the program
  output **(verify before the demo)**.
- The proposal content against the final program (C1).

---

## 7. Documentation and Proposal

| File | Tracked? | State |
|---|---|---|
| `README.md` | Yes | Up to date (S6.4): description, features, structure, data files, requirements, CMD build steps, PowerShell/Ninja alternative, docs table (proposal and report "to be added before submission"). |
| `docs/algorithms.md` | Yes | Complete: adjacency list, BFS, DFS, Dijkstra, Kruskal, DSU, complexity summary, worked examples. Checked against the code in S6.3. |
| `docs/user-guide.md` | Yes | Up to date (S6.4): CMD build steps, menu, every option, data files, troubleshooting. |
| `docs/development-notes.md` | Yes | Up to date (S6.4): current + earlier toolchain, code walkthrough, tests, Git workflow, decision log, milestones (lists up to S6.2 and "S6 In progress"; S6.3/S6.4 rows not added yet). |
| `docs/PROJECT_PROPOSAL.md` | **No (untracked)** | Written during this project from the format of a sample proposal the user provided, with the Quad Core team roster; reviewed twice. **Not known to be submitted or approved.** Partly outdated (C1). DOCX/PDF versions to be made near submission, only when the user asks. **Do not touch without permission.** |
| `docs/PROJECT_HANDOVER.md` | **No (untracked)** | This file. Whether to commit it is the user's decision (P6). |

---

## 8. Team

| Student ID | Name | Role |
|---|---|---|
| 20254103279 | MAKHZUM-BIN-HARUN | **Team Leader**; repository owner; reviews and merges Pull Requests |
| 20254103245 | MUNTAHA MOU JIM | Member |
| 20254103248 | OALID KHAN | Member |
| 20254103269 | MD. RAFIUL ISLAM | Member |

Individual responsibilities of the members have **not** been defined in this
project so far **(to be decided by the team)**.

---

## 9. Remaining Work and Roadmap

### 9.1 Finished

All program features, all four algorithms with DSU, validation (S6.2), 299
automated tests, README, algorithms, user guide and development notes.

### 9.2 Fastest path to submission (recommended order)

1. **Decide the pending items** P1–P6 below (minutes, no code).
2. **Optional small doc fix:** add S6.3/S6.4 rows to the milestone table in
   `docs/development-notes.md`.
3. **Clean-clone check:** clone `dev` into a new folder, build, run CTest,
   run the demo script once.
4. **Update the proposal** (C1) — only with the user's go-ahead — then make
   the DOCX/PDF.
5. **Write the final report** (structure below), with screenshots of the
   console.
6. **Prepare the presentation and rehearse the demo.**
7. **Pull Request `dev` → `master`** when the user approves the release; the
   user merges it.

No new features are needed. The Low items (A3–B6) are optional.

### 9.3 Suggested final report structure

Use the course's official template if one is given. Otherwise:

1. Title page (course, team, members and IDs, supervisor, date)
2. Abstract
3. Introduction and problem statement
4. Objectives and scope (in/out of scope)
5. System design: architecture (`include/`, `src/`, menu vs logic), graph
   representation, data files
6. Algorithms: BFS, DFS, Dijkstra, Kruskal + DSU — idea, pseudocode, worked
   example, complexity (reuse `docs/algorithms.md`)
7. Implementation: tools, file structure, validation rules, persistence
8. Testing: automated tests (299 checks), manual tests, sample results
9. Results: screenshots of each menu option on the sample data
10. Limitations and future work (section 6.3; optional Qt GUI)
11. Conclusion
12. References; appendix (user guide summary, repository link)

### 9.4 Presentation (about 10–12 slides)

Title & team → problem → objectives → graph model (adjacency list picture) →
BFS/DFS → Dijkstra → Kruskal + DSU → validation & data storage → testing →
live demo → limitations/future work → Q&A. Each member should be able to
explain one algorithm and the overall design (assignment of topics: team
decision).

### 9.5 Live demo plan (about 5 minutes)

1. Show `ctest --test-dir build-mingw --output-on-failure` passing.
2. Start with the sample data; option 2 (locations) and 9 (network).
3. Option 10 BFS and 11 DFS from BUBT (ID 1); point out the different orders.
4. Option 12 Dijkstra BUBT → Gulshan (expected 15.00 km via Mirpur 10,
   Agargaon, Farmgate).
5. Option 13 Kruskal (expected 9 of 14 roads, 35.20 km, connected).
6. Validation: add location ` bubt ` (refused as duplicate), road of
   20000 km (refused), `12abc` as a number (asked again).
7. Delete a road or location, run Kruskal again (forest / changed total),
   exit, restart → data reloaded (persistence).

Use a **copy** of the sample data for the demo so the real working data is not
changed, and rehearse with the exact build that will be shown.

### 9.6 Optional future improvement: Qt GUI

`future-gui/` holds the M1 Qt 6 Widgets prototype. Because the logic files
never print or read input, a GUI could reuse `graph`, `storage` and
`algorithms` unchanged and replace only `main.cpp`. It needs Qt to be
reinstalled and `core/project_info.h` references removed. **Not needed for
submission.**

### 9.7 Pending decisions (need the user's approval)

| ID | Decision |
|---|---|
| P1 | Official course title: "Algorithm" or "Algorithms" — then use it everywhere. |
| P2 | Update the proposal (C1), its final file name, whether to commit it, and when to create DOCX/PDF. |
| P3 | Keep `future-gui/` in the submission or remove it. |
| P4 | Delete `data/.gitkeep` or keep it. |
| P5 | Fix any optional Low items (A3–B6) or leave them as documented limitations. |
| P6 | Commit `docs/PROJECT_HANDOVER.md` or keep it local/untracked. |
| P7 | When to open the `dev` → `master` Pull Request. |
| P8 | Keep or delete the local branch `backup/dev-before-claude-removal`. |

---

Copyright © 2026 Quad Core. All rights reserved.
