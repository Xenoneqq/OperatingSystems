# Operating Systems

Programs written in C solving various Operating Systems-related problems, created as part of a college course covering key concepts and implementations.

## How to boot up programs

Using ``linux`` or ``wsl`` is suggested when working with this project. Install all the required compilers using the command:
```bash
sudo apt update  
sudo apt install build-essential 
```
You can compile the programs manually or use make files which are explained in the Make File section.

### 📄 Make Files
The programs use ``Make files`` to provide an easy method of testing, compiling and removing binary files. To use them enter the programs directory and use one of the following commands:
- ``make`` - used to make a simple compilation
- ``make run`` - used to run the compiled program
- ``make test`` - used to compile and run the program
- ``make clean`` - used to remove binary files (compiled programs, result files, generated data)

### 📂 File Hierarchy
Program directories are organized into classes, each focusing on a specific topic related to operating systems. The directories are named ``lab`` followed by a number that corresponds to the class they were created for. The programs are split into two directories:

#### exercises
- contains simple programs that explain different operations that are possible in c

#### solutions
- contains soultions for each class exercise
