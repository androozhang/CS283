1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Calling `fork` provides us with a child process to call `execvp`. This way the parent process can overlook what is happening inside the child process where we are calling `execvp`.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  `fork()` returns -1 when it fails. My implementation checks for a value that is not 0 and prints an error message and waits.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It looks in `$PATH ` to find commands to execute.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling `wait()` pauses the execution of the parent process until the child process terminates.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  `WEXITSTATUS()` returns the return value of the child process.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I use a flag to determine when I am dealing with a quoted argument or not. This is necessary because I need to check whether or not to deal with spaces appropriately base off whether it's quoted or not.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I didn't make that many changes to my parsing logic. The only additional thing was taking care of quoted arguments, which also ended up as an unexpected challenge. Besides that, there weren't any other unexpected challenges.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are used for quick communication that happens asynchronously while IPC is not and is used for more complex communication with larger data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL forces a process to terminate which can use be for terminating unresponsive processes. SIGTERM asks for a process to terminate as well, but allows it to cleanup first. SIGINT is a signal requested by the user to terminate a process.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  A process pauses when it receives SIGSTOP. It can not be caught or ignored like SIGINT because a paused process it is a signal that allows the system to pause any process at a given time.