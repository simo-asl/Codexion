*This project has been created as part of the 42 curriculum by mel-asla.*

# codexion

## Description

`codexion` is a concurrent simulation where multiple coder threads compete for a
limited set of shared USB dongles. Each coder repeatedly performs:

1. compile (requires two dongles)
2. debug
3. refactor

The program enforces cooldown after dongle release and fair arbitration based on
the selected scheduler:

- `fifo`: first request served first
- `edf`: earliest deadline first (`last_compile_start + time_to_burnout`)

The simulation stops when either:

- one coder burns out (fails to start compile before deadline), or
- all coders complete at least `number_of_compiles_required` compiles.

## Instructions

### Build

```bash
make
```

### Run

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Example

```bash
./codexion 5 800 200 200 200 3 20 fifo
./codexion 5 800 200 200 200 3 20 edf
```

## Blocking cases handled

- Deadlock prevention: coder threads do not directly race to lock dongles; they
	pass through a centralized request queue (heap arbitration), so circular wait
	on resource pickup is avoided.
- Coffman-related mitigation:
	- Mutual exclusion exists by design (`pthread_mutex_t` per dongle).
	- Hold-and-wait is controlled by granting both dongles together only when both
		are available.
	- No-preemption is preserved for correctness while compile is in progress.
	- Circular wait is prevented by queue-based arbitration plus ordered locking.
- Starvation prevention under EDF: tie-break on FIFO sequence number and monitor
	wakeups prevent request loss and keep ordering deterministic.
- Dongle cooldown handling: each released dongle is marked unavailable until
	`now + dongle_cooldown`.
- Precise burnout detection: a dedicated monitor thread checks deadlines in tight
	intervals and prints burnout quickly.
- Log serialization: output is protected by a logging mutex to avoid interleaved
	lines.

## Thread synchronization mechanisms

- `pthread_mutex_t` per dongle:
	protects `in_use` and cooldown timestamp updates.
- Global `request_mutex` + `pthread_cond_t request_cond`:
	coordinates queue waiting/wakeup when dongles are released or simulation ends.
- Global `state_mutex`:
	protects shared stop flag and coder progress fields used by monitor/coders.
- Global `log_mutex`:
	guarantees one complete log line at a time.

Communication model:

1. A coder inserts a request in the heap and waits on `request_cond`.
2. When dongles are released, waiting coders are broadcast-woken.
3. The top-priority eligible request atomically takes both dongles.
4. The monitor sets stop state and broadcasts to unblock waiters.

Race-condition prevention examples:

- Dongle state changes are always wrapped in dongle mutex lock/unlock.
- Stop flag and compile counters are read/written under `state_mutex`.
- Queue operations (`push/pop/remove`) are serialized under `request_mutex`.

## Resources

- POSIX threads documentation: `man pthread_create`, `man pthread_mutex_lock`,
	`man pthread_cond_wait`
- Time APIs: `man gettimeofday`, `man usleep`
- Heap / priority queue references:
	- CLRS chapter on Heaps and Priority Queues
	- https://en.wikipedia.org/wiki/Binary_heap

### AI usage

AI was used to:

- accelerate boilerplate setup (file skeletons, prototype planning)
- cross-check threading architecture against subject constraints
- review edge cases (single coder, cooldown windows, queue wakeups)

All generated output was manually reviewed, adapted, and tested.
