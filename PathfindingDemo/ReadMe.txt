AI Assignment 1 Instructions

Release Build can be found in the Release folder, The source code can be found in the PathFindingTest.zip

When the program start, you will be given options to add nodes to the grid. These will be given in the console output.
With the SDL window selected, press the appropriate number for the node type you wish to add.
Start Node (1), Goal Node (2), Object Nodes (3), Weighted Nodes (4), Threat Level (5)
The start node is green, the goal node is yellow, object nodes are bacl, weighted nodes are grey scale (based on weight), nodes with a threat level have a red scale border (based on threat)

For Object Nodes, Weighted Nodes and Threat Level, multiple nodes can be set without having to reselect a node type.

For objects nodes, clicking an object node you have placed again will toggle it between not and object and an object.

For the Weighted nodes and Threat Level, the values of the weight or threat level is set within the console output, between 0 and 25 (as a float eg. 13.25 is ok)
After setting the value, it is recommended to tab back into the SDL window, as clicking on it may place a node.

Once you have set up the grid, press 0 to select a pathing algorithm (a start and goal node must be placed)
Then choose an algorithm by pressing the appropriate number for the node type you wish to add.
Breadth First (1), Depth First (2), Best First (3), Dijkstra's (4), A* (5), A* Epsilon (6)

After choosing an algorithm, you will be asked if you want to show algorithm calculations on screen, or just see the final path.
Chhose by pressing the appropriate number for your answer
Show Calculations (1), Only show Final Path (2)

If you selected to show calculations, you will see all node explansion and paths as they are explored.
The current node orange, nodes in open have a light blue border, nodes in closed have a dark blue border, nodes in the A* Epsilon focal subset have a purple border
The lines between nodes show parent paths back to the start node

Once a path has been found, it will be highlighted in magenta. An agent will also be set to path along it. At any point after the path is found, 
you can restart the program or quit by pressing the appropriate buttons
Restart (1), Quit (2)
If a path is not found, you can restart or quit.