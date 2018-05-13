# (Smooth) Slider

## What this repository is about

* Just for fun 
  - I'm in the process of *learning* C++
  - Using SFML as the GUI component (still unpolished)
* Educational
  - (Failing) TD-Leaf λ temporal difference learning - in progress
  - Adversarial search (Minimax with alpha-beta pruning, Iterative deepening minimax, etc ...)

## What this game is about:

The board has the following layout:

```
 (Player V's goal)
+----------+
|H         |
|H         | (player H's goal)
|H         |
|  V  V  V |
+----------+
```

The aim of the game is simple - given a NxN board, move all your pieces across the opposite side of the board.

The rules are:
  - `H` is allowed to move North, South, and East
  - `V` is allowed to move North, West, and East
  
(i.e. pieces are not allowed to move "backwards" or diagonally)

**note** that most of the learning algorithms are not working as expected and the logic needs to be reworked.
 
 more instructions coming soon on compilation and running the GUI module.
  
