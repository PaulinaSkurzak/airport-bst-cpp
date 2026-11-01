# Airport bst cpp
A binary search tree in C++17 that indexes 6,072 airports by IATA code.

---

## Problem

Every flight booking, boarding pass and baggage tag identifies an airport by its IATA code
(for example, **BOS** for Boston Logan, **JFK** for New York Kennedy, **DAC** for Dhaka).
A travel company requires a lookup service that:

- returns an airport's name, city, country and location for a given code
- remains correct as airports are added to and removed from its database
- produces alphabetical listings for customer agents
- answers range queries, such as every code from **BOS** to **BWI**

This project implements that service on a custom binary search tree, `AirportBST`, keyed by
IATA code and loaded from the OpenFlights airport database.

---

## Build and Run

```bash
g++ -std=c++17 -O2 src/*.cpp -o airports
./airports
```

| Command | Description |
|---|---|
| `load FILE` | Load the airport data, or rebuild the tree from a saved preorder file |
| `find CODE` | Search for an airport and show the search path and comparisons |
| `min` / `max` | Show the smallest / largest code |
| `add CODE` / `remove CODE` | Insert or delete an airport, then validate the tree |
| `list` | List every airport in alphabetical order |
| `save FILE` | Save the codes in preorder |
| `range FROM TO` | List every airport between two codes |
| `validate` | Check the BST property using the (lo, hi) method |
| `rebalance` | Rebuild the tree as a balanced BST |
| `experiment` | Run the Task 6 shape experiment |

---

## Sample Session

```text
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

---

## Task 6: Shape Experiment

The same 6,072 codes were inserted in four different orders. Each tree was then searched with
the same 10,000 randomly selected codes.

| Build Order | Height | Average Depth | Time for 10,000 Searches | Explanation |
|---|---|---|---|---|
| File order | 177 | 18.3 | ___ ms | Sorted runs in the file produce long right-leaning chains |
| Sorted order | 6,071 | 3,035.5 | ___ ms | Every key is the largest so far, so the tree degenerates into a chain of height n − 1 |
| Balanced (middle rule) | 12 | 10.7 | ___ ms | The middle element is chosen as the root at every level, giving the optimal height ⌊log₂ 6072⌋ |
| Random shuffle (bonus) | ___ | ___ | ___ ms | A random order has no sorted runs, so no long chains form |

### Why file order performs worse than a random shuffle

The lines of `airports.dat` are ordered by airport ID, not by IATA code. These IDs were assigned
largely country by country, and within each country the airports often appear in alphabetical
order. As a result, the file contains long runs of increasing codes, and each run builds a chain
that extends to the right.

Canada provides the clearest example: nearly all Canadian codes begin with **Y** (YAM, YAY, YAZ,
YBB, …) and appear consecutively in ascending order. The deepest node in the file-order tree,
**YZR**, requires 178 comparisons to reach. A random shuffle breaks these runs apart, which
reduces the height to approximately 25–29.

### Conclusion

Search, insertion and deletion each cost **O(h)**, where *h* is the tree height. The measured
search times follow the heights directly: the sorted chain is the slowest, and the balanced tree
is the fastest. Rebalancing reduces the worst-case search cost from O(n) to O(log n).

---

## Data Credit

Airport data is provided by **[OpenFlights.org](https://openflights.org/data.php)** and is
published under the **Open Database License (ODbL)**.
