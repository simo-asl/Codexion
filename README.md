*This project has been created as part of the 42 curriculum by mel-asla.*

# Codexion

## Description

Codexion is a multithreaded C simulation where coders share USB dongles. Each coder needs two adjacent dongles to compile, then debug and refactor. Dongle access is scheduled with either FIFO or EDF using a custom heap.

The simulation stops when a coder burns out or when every coder reaches the required number of compiles.

## Instructions

Build the project:

```bash
make
```

Run it with:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown fifo|edf
```

All times are in milliseconds.

Other Makefile rules:

```bash
make clean
make fclean
make re
```

## Resources

- POSIX threads documentation (`pthread_create`, mutexes and condition variables)
- Linux manual pages for `gettimeofday` and `usleep`
- Binary heap / priority queue documentation
- https://dev.to/yel-bakk/codexion-4fk8
- EDF scheduling references

AI was used to review synchronization logic, suggest edge and stress tests, and help simplify the code. The final implementation was reviewed and tested against the project subject.

## Blocking cases handled

- **Deadlock:** dongle pairs are always locked in the same order, preventing circular wait, one of Coffman's deadlock conditions.
- **Starvation:** FIFO uses request order, while EDF gives priority to the earliest burnout deadline with deterministic tie-breaking.
- **Cooldown:** a released dongle cannot be reused before its cooldown expires.
- **Burnout:** a dedicated monitor checks coder deadlines and stops the simulation when needed.
- **Logging:** a log mutex prevents messages from being mixed together.
- **One coder:** with only one dongle available and two required for compiling, the coder cannot compile and eventually burns out. The implementation avoids trying to acquire the same dongle twice.

## Thread synchronization mechanisms

Each dongle has a `pthread_mutex_t` protecting its state, cooldown timestamp, and priority queue. Dongle condition variables wake coders when resource state changes.

A shared state mutex protects simulation stop/start state, request tickets, compile counters, and burnout deadlines. Its condition variable is used for startup and interruptible waits.

A separate log mutex serializes output. The monitor and coder threads communicate only through state protected by the shared mutex, preventing races on deadlines, completion state, and simulation termination.
