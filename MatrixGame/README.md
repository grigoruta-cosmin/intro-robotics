# MatrixSide :video_game:
A vertical fixed shooter where rows of blocks gradually descend down the screen, and the player must shoot them before they reach the bottom. The player loses if all blocks reached the bottom.

# Motivation
This project is made for the **Introduction to Robotics Course** and it is inspired from a game from the old *Brick Game 9999 in 1*. It's purpose is to combine LED Matrix, LCD Screen, Joystick and Arduino all togheter and create a replica to the game aforementioned.

## Real Game Demo
![](real-game-demo.gif)

# Gameplay
The player shoots blocks and tries to complete rows of the matrix with them.First of all, the rows are semi completed in random patterns and are moving downards. When one row is fully filled, it is cleared and the score increases.As the score increases, so does the difficulty of the game i.e. the speed of the descending rows. The in-game player is controlled with the joystick. The game ends when the descending blocks reach the bottom of the matrix. 

# Game Menu Interactions
The menu has 4 sections: Start Game, Highscore, Settings and Info each of them can be selected with the button of the Joystick and deselected when the button is pressed again.
* The first section starts the game itself(*note* you can't exit the game when pressing the button again). In order to restart the game you have to go to the leftmost cell and do 3 swipes left, the matrix will be cleared and the score and time would reset, if you want to exit the game you have to go to the rightmost cell and do 3 swipes right on the joystick, the game will end and a message would be shown on the lcd display.
* Second section will show the higscores saved in the **EEPROM memory** of the arduino board.
* Third section let the player change the difficulty of the game, the brightness of the LED Matrix and the contrast of the LCD Display(*note * in order to change each subsection you have to swipe left or right on the joystick).
* Last section displays a little bit of informations like my github link, who was the project for and the name of the game.

### Difficulty
The difficulty of the game is represented by the speed of the descending blocks. The player can choose from:
* Easy, a new line is generated every **7** seconds.
* Medium, a new line is generated every **4** seconds.
* Hard, a new line is generated every **2.5** seconds.
**Note** Everytime the player reaches a score multiple of ten the *descending time* is decreased by 50 miliseconds.

# Controls
In order to move the player, the joystick has to be moved along the **x**-axis, whereas to make it shoot, the joystick has to be moved along the **y** postivie axis.

# Requirments
* 8x8 LED Matrix
* LCD
* Joystick
* MAX7219 Driver

# Hardware Design
* The game itself it's displayed on a **8x8 LED Matrix**. The connections are made through a **MAX7219 driver**. 
* The menu and messages are shown on a **LCD Display**.
* All interactions are made with a **Joystick**.

# Bugs
