#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_CHILDREN 5

int main() {
  // Create the pipes
  int pipe_fds[2];
  pipe(pipe_fds);

  // Spawn child processes which run their subroutines
  for (int i = 0; i < NUM_CHILDREN; i++) {
    pid_t pid = fork();

    // Child code
    if (pid == 0) {
      // Close the read end of pipe (children won't read)
      close(pipe_fds[0]);

      // Write the child PID
      pid_t child_pid = getpid();
      write(pipe_fds[1], &child_pid, sizeof(pid_t));

      // This child is done, close the file descriptor
      close(pipe_fds[1]);
      return 0;
    }

    // No parent code in the for loop
  }

  // Parent code
  // Close write end of pipe
  close(pipe_fds[1]);

  // Keep on reading data until all child processes have closed write ends
  // If the pipe is empty and not closed, call will block
  pid_t curr_pid;
  while (read(pipe_fds[0], &curr_pid, sizeof(curr_pid))) {
    printf("Received pid %d.\n", curr_pid);
  }

  // Wait for all child processes to terminate
  for (int i = 0; i < NUM_CHILDREN; i++) {
    int status;
    wait(&status);
  }

  // Close remaining file descriptors; pipe memory is on stack, so it is
  // automatically cleaned up
  close(pipe_fds[0]);
  return 0;
}
