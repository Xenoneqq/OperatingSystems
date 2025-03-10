# LAB 1
This folder contains three programs written in c. To run them enter their derectory and use the command `make`. This will compile the program and create an `.exe` file. You can also use `make clean` command to remove all the compiled files.

## Hello world
First program is `helloworld`. The program as expected writes out "Hello world!". To compile the program use `make` command.

## Multiplier
The program asks the user to input two numbers. The result of the program is the multiplication of these numers.

## Countdown - Final exercise
The program `countdown.c` counts from 10 to 0 displaying them in console. The programs make file contains a `debug` instruction that can be used with the command `make debug`. This will create a file that saves debug information and will also lunch the `.exe` in `gdb`. This command requires `gdb` to be installed. On linux you can do so by using the command: 
```bash
sudo apt install gdb
```
The countdown exercise requires to debug it and complete certain tasks. All the tasks, commands and explanations can be found [here](countdown/README.md)