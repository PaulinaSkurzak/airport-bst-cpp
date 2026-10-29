# Pseudocode

## Load the file
LOAD-FILE(fileName)
1. Try to open the file
2. If it doesn't open, return false
3. Read the file one line at a time
4. Add each line to a list
5. Return true

## Parse one line into fields
PARSE-LINE(line)
1. Start with an empty list of fields and an empty current field
2. Keep track of whether we're inside quotes right now, starting at no
3. Go through the line one character at a time
4. If we see a quote mark, flip inside-quotes to the opposite
5. If we see a comma and we're not inside quotes, the field is done
   Add it to the list and start a new field
6. Otherwise just add the character to the current field
7. At the end, add whatever's left as the last field
8. Return the list of fields

## Build the list of airports
BUILD-AIRPORT-LIST(lines)
1. Start with an empty list of airports and a skipped count of 0
2. For each line:
3.     Break it into fields using PARSE-LINE
4.     If the IATA code is missing, add 1 to skipped and move on
5.     Otherwise make a new Airport from the fields and add it to the list
6. Return the airport list and the skipped count

## Insert one airport into the tree
INSERT(root, airport)
1. If the tree is empty, this airport becomes the root
2. Otherwise start at the root
3. If this code matches the current node, it's a duplicate, stop
4. If this code comes before the current node's code, go left
5. If it comes after, go right
6. If that spot is empty, put the airport there
7. Otherwise keep moving and repeat from step 3

## Search
SEARCH(root, code)
1. Start at the root
2. If we hit nothing, the airport isn't here
3. If this node's code matches, we found it
4. If our code comes before this node's code, go left
5. If it comes after, go right
6. Repeat until found or we run out of tree

## Height of the tree
HEIGHT(node)
1. If the node is empty, return -1
2. Find the height of the left side
3. Find the height of the right side
4. Return 1 plus whichever is bigger

## Average depth of the tree
AVERAGE-DEPTH(root)
1. Start a total at 0 and a count at 0
2. Visit every node, tracking how deep it is
3. Add its depth to the total and add 1 to the count
4. Divide total by count

## Delete
DELETE(root, code)
1. If the tree is empty, nothing to delete
2. If the code is smaller than this node's, delete from the left
3. If bigger, delete from the right
4. Otherwise this is the one to delete
5.     No children: just remove it
6.     One child: that child takes its place
7.     Two children: find the successor, go right once then left as far as possible
8.         Copy the successor's data into this node
9.         Delete the original successor
10. Return the tree

## Validate
VALIDATE(node, low, high)
1. If the node is empty, it's valid
2. If this node's code isn't between low and high, it's broken
3. The left side must be valid, with every code smaller than this node's
4. The right side must be valid, with every code bigger than this node's
5. Return true only if both sides are good

## Range
RANGE(node, from, to, results)
1. If the node is empty, stop
2. If this node's code comes after "from," check the left side too
3. If this node's code is between "from" and "to," add it to results
4. If this node's code comes before "to," check the right side too

## Balanced Build
BUILD-BALANCED(sortedArray, start, end)
1. If start is past end, there's nothing left
2. Find the middle index
3. Make a new node from the airport at that index
4. Its left child comes from BUILD-BALANCED on everything before the middle
5. Its right child comes from BUILD-BALANCED on everything after the middle
6. Return the node