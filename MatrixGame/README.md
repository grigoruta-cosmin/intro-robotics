# MatrixSide :video_game:
A vertical fixed shooter where rows of blocks gradually descend down the screen, and the player must shoot them before they reach the bottom. The player loses if all blocks reached the bottom.

# Motivation
This project is made for the **Introduction to Robotics Course** and it is inspired from a game from the old *Brick Game 9999 in 1*. It's purpose is to combine LED Matrix, LCD Screen, Joystick and Arduino all togheter and create a replica to the game aforementioned.

## Real Game Demo
![](real-game-demo.gif)

# Gameplay
The player shoots blocks and tries to complete rows of the matrix with them.First of all, the rows are semi completed in random patterns and are moving downards. When one row is fully filled, it is cleared and the score increases.As the score increases, so does the difficulty of the game i.e. the speed of the descending rows. The in-game player is controlled with the joystick. The game ends when the descending blocks reach the bottom of the matrix. 

# Controls
In order to move the player, the joystick has to be moved along the **x**-axis, whereas to make it shoot, the joystick has to be moved along the **y** postivie axis. In the case of pressing the joystick button, 2 blocks will be shot.

# Requirments
* 8x8 LED Matrix
* LCD
* Joystick
* MAX7219 Driver
