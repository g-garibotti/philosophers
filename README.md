# Philosophers

![C](https://img.shields.io/badge/language-C-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A solution to the classic dining philosophers problem using threads and mutexes.

## Table of Contents
- [Overview](#overview)
- [The Problem](#the-problem)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Technical Challenges](#technical-challenges)
- [Performance Considerations](#performance-considerations)

## Overview

This project implements the dining philosophers problem, a classic computer science problem illustrating synchronization issues and techniques for resolving them. The implementation uses POSIX threads and mutexes to prevent race conditions and ensure proper resource sharing.

## The Problem

- N philosophers sit at a round table with N forks
- Each philosopher needs two forks to eat
- Philosophers alternate between eating, thinking, and sleeping
- If a philosopher doesn't eat within a specified time, they die
- The goal is to keep all philosophers alive while preventing deadlocks

## Features

- Thread-safe implementation using mutexes
- Accurate timestamp logging of all philosopher actions
- Deadlock prevention mechanisms
- Configurable simulation parameters
- Death monitoring system
- Clean exit handling

## Requirements

- GCC compiler
- pthread library
- Make

## Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

Example:
```bash
./philo 5 800 200 200 7
```

Parameters:
- `number_of_philosophers`: Number of philosophers and forks
- `time_to_die`: Time (ms) a philosopher can survive without eating
- `time_to_eat`: Time (ms) it takes to eat
- `time_to_sleep`: Time (ms) philosophers spend sleeping
- `number_of_times_each_philosopher_must_eat`: (Optional) Number of times each philosopher must eat

## Implementation Details

### Core Components

1. **Thread Management**
   - Each philosopher runs in a separate thread
   - Main thread monitors for death conditions
   - Clean thread creation and joining

2. **Mutex System**
   - Fork mutexes prevent multiple philosophers from using the same fork
   - Death mutex protects death status checking
   - Print mutex ensures atomic printing of status messages

3. **Resource Management**
   - Dynamic memory allocation for philosophers and forks
   - Proper cleanup on program termination
   - Prevention of memory leaks

### Key Features

1. **Death Detection**
   - Separate monitoring thread checks philosopher status
   - Maximum 10ms delay in death detection
   - Atomic death status updates

2. **Time Management**
   - Precise timing using microsecond resolution
   - Smart sleep implementation to prevent CPU hogging
   - Accurate timestamp generation

3. **Status Reporting**
   - Thread-safe status printing
   - Formatted timestamp output
   - Clear action descriptions

## Technical Challenges

1. **Race Conditions**
   - Solution: Careful mutex implementation
   - Protected shared resources
   - Atomic operations where necessary

2. **Deadlock Prevention**
   - Even/odd philosopher fork pickup ordering
   - Timeout-based resource release
   - Resource hierarchy implementation

3. **Performance Optimization**
   - Minimized mutex contention
   - Efficient thread scheduling
   - Optimized sleep calculations

## Performance Considerations

- Mutex granularity balanced for performance
- Minimized critical sections
- Efficient resource allocation/deallocation
- Smart sleeping to reduce CPU usage

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
