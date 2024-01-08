# Project overview
This project is about recreating our own version of bash with limited functionality.
A miniature shell :).

## Valgrind
```valgrind -s --leak-check=full --show-reachable=yes --error-limit=no --suppressions=minishell.supp ./minishell```

- ```-s```: same as --show-errpr-list=yes that shows detected errors list and suppression count at exit
- ```--leak-check=full```: shows a full list of memory leaks
- ```--show-reachable=yes```: same as ```--show-leak-kinds=all``` that shows all kinds of memory leaks
- ```--error-limit=no```: won't stop showing errors if too many
- ```--suppressions=minishell.supp```: suppresses memory leaks from in-built functions (readline and add_history)

### Permitted functions and their descriptionu
| Function        | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| readline        | Reads a line from the terminal.                                             |
| rl_clear_history| Clears the history of readline.                                             |
| rl_on_new_line  | Prepares readline for a new line of input.                                  |
| rl_replace_line | Replaces the current line buffer in readline.                               |
| rl_redisplay    | Redisplay the line buffer in readline.                                      |
| add_history     | Adds a line to the history in readline.                                     |
| printf          | Prints formatted output to stdout.                                          |
| malloc          | Allocates a block of memory.                                                |
| free            | Frees a block of allocated memory.                                          |
| write           | Writes data to a file descriptor.                                           |
| access          | Checks the accessibility of a file.                                         |
| open            | Opens a file descriptor.                                                    |
| read            | Reads data from a file descriptor.                                          |
| close           | Closes a file descriptor.                                                   |
| fork            | Creates a new process.                                                      |
| wait            | Waits for a child process to terminate.                                     |
| waitpid         | Waits for a specific child process to terminate.                            |
| wait3           | Waits for any child process, returning resource usage.                      |
| wait4           | Waits for any child process, specifying options and returning resource usage.|
| signal          | Sets a function to handle a signal.                                         |
| sigaction       | Examines or changes the action for a specific signal.                       |
| sigemptyset     | Initializes a signal set to be empty.                                       |
| sigaddset       | Adds a signal to a signal set.                                              |
| kill            | Sends a signal to a process or a group of processes.                        |
| exit            | Terminates the calling process.                                             |
| getcwd          | Gets the current working directory.                                         |
| chdir           | Changes the current working directory.                                      |
| stat            | Retrieves information about a file based on its filename.                   |
| lstat           | Like stat, but for symbolic links.                                          |
| fstat           | Retrieves information about a file based on its file descriptor.            |
| unlink          | Removes a file from the filesystem.                                         |
| execve          | Executes a program, replacing the current process.                          |
| dup             | Duplicates a file descriptor.                                               |
| dup2            | Duplicates a file descriptor to a given value.                              |
| pipe            | Creates a pair of file descriptors for interprocess communication.          |
| opendir         | Opens a directory stream.                                                   |
| readdir         | Reads a directory entry from a directory stream.                            |
| closedir        | Closes a directory stream.                                                  |
| strerror        | Returns a string describing an error code.                                  |
| perror          | Prints an error message to stderr.                                          |
| isatty          | Tests whether a file descriptor refers to a terminal.                       |
| ttyname         | Returns the name of a terminal.                                             |
| ttyslot         | Finds the slot of the current user's terminal in the utmp file.             |
| ioctl           | Manipulates the underlying device parameters of special files.              |
| getenv          | Retrieves the value of an environment variable.                             |
| tcsetattr       | Sets the parameters associated with the terminal.                           |
| tcgetattr       | Gets the parameters associated with the terminal.                           |
| tgetent         | Loads the terminal entry.                                                   |
| tgetflag        | Checks for the presence of a capability in the terminal description.        |
| tgetnum         | Retrieves a numeric capability from the terminal description.               |
| tgetstr         | Retrieves a string capability from the terminal description.                |
| tgoto           | Decodes a cursor motion string.                                             |
| tputs           | Puts a terminal string with padding.   