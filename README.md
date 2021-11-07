# C-projects
3 Projects created in C<br>
<i>Note: All projects were tested and writen on linux </i>

<h2>Date:</h2>
Takes input of multiple dates and sorts them into chronological order, taking into consideration leap years.
<br><br>
Compile using: <i>gcc -Wall -ansi date.c -o date</i> <br>
Execute using: <i>./date</i>

<h2>Employee:</h2>
An employee management system that allows user to add and remove employees from a database (txt file) using linked lists.
<br><br>
Compile using: <i>gcc -Wall -ansi employee.c -o employee</i>
Execute using: <i>./employee > tmp</i> where tmp is the file with the stored data.

<h2>Game of Life:</h2>
Conway's 0 player game of life simulated in C.
<br><br>

Compile using: <i>gcc -Wall -ansi game-of-life.c -o gol</i><br>
Execute using: <i>./gol [file] [width] [height] [rounds]</i><br><br>
Where:<br>
file = input file with the inital board<br>
width - width of the board<br>
height = height of the board<br>
rounds = number of interations

Input file must be a txt file in layout:
<br>First line contains the number of live cells
<br>All following lines contain the 2D coordinates of the live cells
