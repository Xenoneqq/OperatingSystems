# COUNTDOWN - Program explanation

To run the program use `make` command. This will create a `.exe` file which can be executed using the command:
```bash
./countdown.exe
```
The makefile contains the following commands:
- `make` - compiles the program and creates `countdown.exe` file.
- `make clean` - clears the directory of any compilation files.
- `make debug` - compiles the program and creates `countdown.exe` file that keeps all of the debug data including variables.

## 🐛 Debugging exercise
The exercise requires to perform debugging tasks. To do them you are required to use a debugger. For this exercise I used `gdb` to install it use the command

```bash
sudo apt install gdb
```

After installing the gdb you can use the `make debug` command to compile the program in debug mode.

### Adding a breakpoint

Adding a breakpoint can be done using the command:
```sh
# adds a breakpoint in line :5
break 5
b 5
```
To check the currently set breakpoints use the command:
```sh
# displays all of the existing breakpoints
info breakpoints
```
This will print out a list of all the breakpoints and their numeric ID. 

### Removing a breakpoint
You can remove a specific breakpoint using the following command:
```sh
# removes the breakpoint of ID = 1
delete 1
# to remove all breakpoints use:
delete breakpoints
```
To continue running the program you can use one of the following commands:
```sh
# to run the code line by line use:
step
# to execute the program up to the next breakpoint use:
continue
```

### Displaying variables
To display a variable at any given moment you can use the command
```sh
print variable_name
```
if you used the `-g` option while compiling this should work. Also be sure to use a breakpoint as not setting one will run the program from start to end without giving you an option to debug anything.