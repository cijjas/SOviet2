#include <stdint.h>
#include "system_calls.h"
#include "tests.h"
#include "library.h"

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

void *my_process_inc(int argc, char *argv[])
{
  print("My process\n");
  return NULL;
}

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc)
{
  uint64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t processInc(uint64_t argc, char *argv[])
{
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!semOpen(SEM_ID, 1))
    {
      print("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++)
  {
    if (use_sem)
      semWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      semPost(SEM_ID);
  }

  if (use_sem)
    semClose(SEM_ID);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[])
{ //{n, use_sem, 0}
  print("la noche esta empañada\n");
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    pids[i] = createFGProcess("sync", my_process_inc, 3, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = createFGProcess("sync2", my_process_inc, 3, argvInc);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    uint64_t primerpid = pids[i];
    print("primer pid: %d\n", primerpid);
    // my_wait(pids[i]);
    // my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }
  print("Final value: %d\n", global);

  return 0;
}
