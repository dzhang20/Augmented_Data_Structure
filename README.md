# Augmented_Data_Structure
### note: should have trace.txt in the same directory
## Result of reuse distance is in result.txt
## Usage:
- $g++ -o avl -Wall -O3 AVL.cpp
- $./avl

OR
- $./run.sh && ./avl

## Design:
### Using AVL tree as base data structure with augmented field: 
### 					h_e: most recent index
### 					h_n: number of nodes under its subtree
### For calculating the reuse distance of one node: basically we need to count how many nodes are on the left of target node in an Inorder traversal output. Thus, we need to keep track of each "turns" when walk from root to the target node and follow "left add, right minus" rule.