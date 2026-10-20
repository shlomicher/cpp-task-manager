# Task Manager

A task management system written in C++17 using a templated sorted linked list.

`SortedList<T>` is implemented as a doubly linked list. It keeps elements sorted and supports const iterators, copying, insertion, removal, filtering and applying operations.

`TaskManager` uses the list to organize tasks by priority. It can assign tasks to people, complete the highest-priority task, change priorities by task type and print task lists.

## Build and test

Requires CMake 3.16 or newer and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

`main.cpp` contains tests for the list and task manager. This is not an interactive application.

## Files

- `SortedList.h`: sorted list template and iterator implementation.
- `TaskManager.cpp/.h`: task assignment and priority management.
- `Person.cpp/.h` and `Task.cpp/.h`: people and tasks.
- `main.cpp`: test driver.

## About

Originally developed with Ofri Hacohen for Homework 3 in Introduction to Systems Programming (234124) at the Technion. The course staff provided the assignment and starter files. The build and test configuration was updated for this repository.
