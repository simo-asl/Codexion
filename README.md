*This project has been created as part of the 42 curriculum by mel-asla.*

# Codexion

## Description

Codexion is a C concurrency simulation in which one thread per coder competes
for adjacent USB dongles. A custom heap arbitrates each dongle using FIFO or
earliest-deadline-first (EDF), while a separate monitor stops the simulation on
burnout or once every coder reaches the requested compile count.

## Instructions

Build with `make`, then run:

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
time_to_refactor number_of_compiles_required dongle_cooldown fifo|edf
```

All times are milliseconds. `make clean`, `make fclean`, and `make re` are also
available. Run `make test` for the scheduler, integration, stress, cooldown,
timing, Valgrind, and DRD checks (the last two run when Valgrind is installed).

## Resources

- POSIX Threads Programming: mutexes and condition variables
- Linux manual pages for `pthread_create`, `pthread_mutex_lock`, and
  `pthread_cond_timedwait`
- Introduction to Algorithms: binary heaps and priority queues
- Liu and Layland, *Scheduling Algorithms for Multiprogramming in a Hard-Real-
  Time Environment*: deadline scheduling background

AI was used to review the supplied implementation, derive edge and stress
tests, check synchronization risks, and help simplify the final code. Every
generated or adapted part was reviewed against the subject and tested locally.

## Blocking cases handled

Dongle pairs are locked in address order, breaking circular wait and preventing
deadlock. Each request is placed in both adjacent heaps before acquisition;
FIFO tickets prevent queue jumping and EDF deadlines prioritize the coder at
greatest immediate risk. A released dongle remains unavailable until its
cooldown timestamp. The monitor checks deadlines at short intervals, while one
output mutex serializes logs and makes burnout the
final message. Resource contenders sleep on the specific blocking dongle and
wake on queue, ownership, release, or cooldown changes. A generation counter
prevents lost wakeups. The one-coder case takes its only dongle once and never
attempts to lock it twice.

## Thread synchronization mechanisms

Each dongle has a `pthread_mutex_t` protecting its busy flag, cooldown, and
heap. Two dongles are always locked in the same address order. A shared state
mutex protects start/stop state, request tickets, compile counts, and burnout
deadlines. Its condition variable forms the start barrier, interrupts timed
work on shutdown, and avoids long blind sleeps. A separate log mutex prevents
output interleaving. Each dongle condition variable targets only its adjacent
waiters, avoiding global wakeup storms. Coders never read shared mutable state
outside the corresponding mutex.
