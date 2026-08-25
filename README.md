# Philosophers — Concurrency and Synchronization in C

A **42 School** systems-programming project based on the Dining Philosophers problem. The goal is to coordinate multiple concurrent actors competing for shared resources while preventing data races and keeping the simulation consistent.

## What this project demonstrates

- Multithreading with POSIX threads
- Mutex-based synchronization
- Shared-resource management
- Race-condition prevention
- Precise timing and monitoring
- Thread lifecycle management
- Concurrent state inspection
- Robust cleanup of synchronization primitives and allocated memory

## Simulation model

Each philosopher runs in its own thread and alternates between:

- eating
- sleeping
- thinking

Forks are represented as shared mutex-protected resources. A separate monitoring layer detects when a philosopher exceeds the allowed time without eating or when an optional meal target has been reached.

## Program arguments

```text
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:

```bash
./philo 5 800 200 200
```

## Project structure

The implementation is split into focused modules:

- `philo_init.c` — initialization of the simulation and synchronization objects
- `philo_routine.c` — philosopher thread routine
- `philo_monitor.c` — simulation monitoring
- `philo_death.c` — starvation/death detection
- `philo_parsing.c` — argument validation
- `philo_cleanup.c` — resource cleanup
- `philo_utils.c` — timing and utility helpers

## Build

```bash
make
```

This generates:

```text
philo
```

The project is compiled with pthread support.

## Why it matters

Concurrency introduces a different category of bugs from sequential programming: a program may appear correct and still fail because of timing, unsafe shared state or an inconsistent synchronization strategy. Philosophers was an important step in learning to reason about execution that happens simultaneously rather than linearly.

## Project context

This repository is part of my **42 School software-engineering journey**. The source code is intentionally public as a record of my learning progression and technical work.

---

**Mohamed Ali Chamsa**  
Software & AI Builder · Entrepreneur · 42 School