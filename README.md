# Airport bst cpp
A binary search tree in C++17 that indexes 6,072 airports by IATA code.

## Problem
A fast lookup service that returns an airport's details by code and supports insertion, deletion, ordered listing and range queries.

## Build and Run
```
g++ -std=c++17 -O2 src/*.cpp -o airports
./airports
```

## Sample Session
```
> load data/airports.dat
Loaded 6072 airports (1626 lines skipped: no IATA code). Duplicates: 0
Height: 177 Average depth: 18.3
> find BOS
Path: GKA -> … -> BOS (comparisons: 12)
BOS General Edward Lawrence Logan International Airport, Boston, United States
> find XYZ
Path: GKA -> … -> (NULL) not found (comparisons: 14)
> range BOS BWI
113 airports, 127 nodes visited
> remove BOS
Deleted BOS (case D2: only a left child, BOR) validate: OK
> rebalance
Height: 12 Average depth: 10.7
```

## Task 6: Shape Experiment
| Build Order | Height | Avg Depth | 10,000 Searches | Explanation |
|---|---|---|---|---|
| File order | 177 | 18.3 | ___ ms | Sorted runs in the file form long chains |
| Sorted order | 6,071 | 3,035.5 | ___ ms | Every key goes right, forming a chain |
| Balanced | 12 | 10.7 | ___ ms | Middle element as root gives ⌊log₂ n⌋ |
| Random shuffle | ___ | ___ | ___ ms | No sorted runs, so no long chains |

**Why file order is worse:** the file is grouped by country and often alphabetical, so increasing runs of codes (e.g. Canada's Y codes) build long chains; shuffling breaks them up.

## Data Credit

Airport data from [OpenFlights.org](https://openflights.org/data.php), Open Database License.
