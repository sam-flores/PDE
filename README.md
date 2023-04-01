# predator_prey_simulation


This program simulates a predator-prey system using partial differential equations. The predator's PDE is modeled after the heat equation where v, the predator, exhibits flow from high concentration to low concentration of v. Additionally diffusion of v is compounded by the concentration of u, the prey, at x. The two populations are spread across two spacial axes and the animation evolves with time. The initial condition's spacial distribution of u and v are set by init_u and init_v respectively. The two populations are evolved by the plot function which calculates the current state from the previous state via euler's method. The main program is in charge of calling plot to evolve the predator-prey system together in a while loop. The boundary conditions are handled by an if statement in the plot function.

to run,

- clone the codebase
- navigate to the directory in which the repository was cloned
- compile the code using "cc main.c -lm -lX11"
- run the code using "./a.out"
- a screen should pop up where you'll be able navigate the scene using the arrow keys
