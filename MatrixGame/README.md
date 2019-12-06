# MatrixSide :video_game:
A vertical fixed shooter where rows of blocks gradualy descending down the screen, and the player must shoot them before they reach the bottom. The player loses if all blocks reached the bottom.

# Motivation
This project is made for the **Introduction to Robotics Course** and it is inspired from a game from the old *Brick Game 9999 in 1*. It's purpose is to combine LED Matrix, LCD Screen, Joystick and Arduino all togheter and create a replica to the game mentioned above.

## Real Game Demo
![](real-game-demo.gif)

# Gameplay
The player shoots with blocks and tries to complete rows of the matrix with them.First of all, the rows are semi completed in random patterns and are descending to the player. When the row is full it's cleaned and the score is incremented. With the advancement in score the dificulty(i.e the speed of descending rows) is incresed. The player is controlled by the joystick. The game ends when the descending blocks reaches the bottom of the matrix. 

# Controls
To move the the player you move the joystick on the **X**-axis and to shoot with one block you move the joystick on the **Y** positive axis and to shoot with 2 *(?)* blocks you press the joystick button.

# Requirments
* 8x8 LED Matrix
* LCD
* Joystick
* MAX7219 Driver
