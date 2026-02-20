*This project has been created as part of the 42 curriculum by seilkiv.*

# Philosophers

## Description

This project is an implementation of the classic Dining Philosophers problem using threads and mutexes in C.

The goal is to simulate philosophers sitting at a round table who alternate between eating, sleeping, and thinking. Each philosopher needs two forks (represented by mutexes) to eat. The simulation stops when a philosopher dies (fails to eat within `time_to_die`) or when all philosophers have eaten a required number of times (if specified).

The project focuses on:
- Thread creation and lifecycle management
- Mutex synchronization
- Avoiding data races
- Preventing deadlocks
- Accurate time handling
- Safe resource management

Each philosopher is implemented as a separate thread, and each fork is protected by a mutex.

---

## Instructions

### Compilation

Flags used:
-Wall -Wextra -Werror -pthread

Available rules:
- make
- make clean
- make fclean
- make re

Execution:
./philo `number_of_philosophers` `time_to_die` `time_to_eat` `time_to_sleep` [number_of_times_each_philosopher_must_eat]

Example:
./philo 5 800 200 200

Arguments:
- `number_of_philosophers`: number of philosophers and forks
- `time_to_die`: time (ms) before a philosopher dies without eating
- `time_to_eat`: time (ms) spent eating
- `time_to_sleep`: time (ms) spent sleeping
- `number_of_times_each_philosopher_must_eat` (optional): simulation stops if all philosophers reach this number

Technical Choices:
- Even/odd fork acquisition order to reduce deadlock risk
- Dedicated monitor thread to detect philosopher death

Mutex protection for:
- Forks
- Writing output
- Simulation stop condition
- Individual meal tracking

Resources:
- https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/
- https://42-cursus.gitbook.io/guide/3-rank-03/philosophers

AI was used as a support tool for:
- Structuring the README file
- Clarifying pthread behavior
- Reviewing concurrency logic
- Improving documentation clarity
