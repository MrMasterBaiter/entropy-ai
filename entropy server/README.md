# README #

Welcome to the Entropy AI tournament manager. 
For rules of the game visit : [wikipedia: entropy board game](https://en.wikipedia.org/wiki/Entropy_(1977_board_game))

This is the part of the mega AI-championship in the course [Artificial Intelligence, Autumn 2015 - running now](http://www.cse.iitd.ac.in/~mausam/courses/csl333/autumn2015/), exceptionally taught by Prof. Mausam every year at [IIT DELHI](http://www.iitd.ac.in). (championship commencing soon)

Before going further, please read the few simple rules of the game.

### A Snapshot of server cli ###
![Snap1](https://dl.dropboxusercontent.com/u/35429234/Screen%20Shot%202015-08-25%20at%204.05.03%20am.png)

### Problem statement for an AI developer ###
* Write a program in any language of your choice which has the following I/O specs:
* * All input is taken from stdin and all output is sent to stdout
* * These 2 examples shall clear the I/O specs ("stdin: " and "stdout: " are not a part of I/O):


Example 1:

```
#!python

stdin: 5
stdin: ORDER
stdin: 0 0 B
stdout: 0 0 1 1 
...

```

Example 2:

```
#!python

stdin: 5
stdin: CHAOS
stdin: B
stdout: 0 0
stdin: 0 0 1 1
stdin: A 
...

```

Explanation:
The first 2 lines define the board size and player's role.
The next lines depends on role:

For order:

1. Read chaos's move: "x y C", (C is a single alphabet in upper case A-Z, representing color)
1. Print "a b c d" which is your move: (a, b) => (c, d)
1. Goto (1) till game over...

For chaos: 

1. Read "C" (C is a single alphabet in upper case A-Z, representing color)
1. Print "x y" which are the coordinates of the sq where you want to place the piece
1. Read "a b c d" which is order's move
1. Goto (1) till game over...


## **IMPORTANT:** ##
* Your program needs to flush output buffers at every newline. this can be done by using **endl** at every move in c++ and using the -u flag for python - **python -u code.py**
* **common mistake**: Since, stdin and stdout are reserved for communicating with the tournament program, please do not redirect stderr to stdout, and you can use stderr (and **not** stdout) for personal debugging purposes.

### Setting up (for a student participating in the game) ###

* Steps for a test run (no extra code):
* Make sure you use python2 and not 3.
* In different terminals do:
* * sh startServer.sh - see server/config.txt for more options
* * cd client; python client.py 
* * Interact with server's terminal for making the 2 clients(same copy of the basic AI program) to play bot vs ai

* Further:
* * myAI.py is a python program which adheres to the I/O specs and plays kiddishly
* * You can write it in any language, make it intelligent and play against other players and also against the TA-AI which is built into the server. The TA-AI which you currently see is Random.


### Feedback & Suggestions ###
We humbly welcome all suggestions, bug reports and feedback. You can shoot them to:

* Mayank Raj <mayank.raj.iitd@gmail.com>
* Shreya Rajpal <shreya.rajpal@gmail.com>