1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensure that all child processed complete before the shell continue accepting user input for calling `waitpid` on every child process. In the case that I forgot to call `waitpid()`, that child process would continue to run in background consuming resources while doing nothing.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

When unused pipes are left open, the open file descriptor can cause issue in the future due to resource leak.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

`cd` is implemented as a built-on rahter than an external command because we are work with the current working process not a child process. A challenge that would arise is that we would have to find a solution to execute a command to our current process.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

We can allocate the memory for piped commands dynamically using malloc, but that could come off with a trade-off of having to ensure that every pipe is cleaned up after use.