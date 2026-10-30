# Airport bst cpp
A binary search tree in C++17 that indexes 6,072 airports by IATA code.


## Problem
Airlines and travel agents identify airports by three-letter IATA codes. This service returns
an airport's name, city and country for a given code, supports insertion and deletion, and
answers alphabetical and range queries. It uses a custom `AirportBST` class.

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
| Sorted order | 6,071 | 3,035.5 | ___ ms | Every key goes right; the tree degenerates into a chain |
| Balanced (middle rule) | 12 | 10.7 | ___ ms | The median is chosen as the root at every level, giving ⌊log₂ n⌋ |
| Random shuffle (bonus) | ___ | ___ | ___ ms | Random order prevents long chains |

**Why file order underperforms a random shuffle:** `airports.dat` is ordered by airport ID,
and IDs were assigned country by country, often alphabetically. The resulting runs of
increasing codes each build a right-leaning chain. Canadian Y codes are the clearest example:
the deepest node, YZR, requires 178 comparisons. Shuffling breaks these runs, reducing the
height to about 25–29. Since every operation costs O(h), search time follows height.

## Data Credit

Airport data from [OpenFlights.org](https://openflights.org/data.php), Open Database License.
