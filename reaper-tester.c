#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void child_process();
void parent_process(int);

int main()
{
  printf("Current PID is %i\n", getpid());
  printf("Forking...\n");

  int pid = fork();

  if (pid == 0)
  {
    child_process();
  }
  else if (pid > 0)
  { 
    parent_process(pid);
  }
  else
  {
    perror("fork()");
    exit(1);
  }
}

void child_process()
{
  int parent_pid = getppid();
  printf("Child: current Parent PID is %i\n", parent_pid);
  while (parent_pid == getppid())
  {
    printf("Child: waiting to be reaped by PID 1\n");
    sleep(3);
  }
  sleep(3);
  int new_parent_pid = getppid();

  if( new_parent_pid == 1 )
  {
    printf("Child: great! PID 1 adopted me :)\n");
  }
  else
  {
    printf("Child: i got reaped by PID %i, use an appropriate process management :(\n", new_parent_pid );
  }
  exit(0);
}

void parent_process(int child_pid)
{
  printf("Parent: child's pid is %i, now exiting parent...\n", child_pid);
// if (kill(child_pid, SIGKILL) != 0) perror("kill");
  sleep(5);
  exit(0);
// kernel expect us to wait for a child process, otherwise it will become a zombie :) 
// waitpid(pid, NULL, 0);
}