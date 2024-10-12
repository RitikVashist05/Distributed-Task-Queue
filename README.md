# Distributed Task Queue 

## Project Overview

This project implements a Distributed Task Queue System that allows clients to submit tasks with varying priorities to a server, which then distributes these tasks to a pool of worker threads based on their priority. The system is designed for concurrency, prioritization, and scalability.

## Features

- **Concurrency:** Handles multiple tasks simultaneously using multithreading.
- **Prioritization:** Tasks are processed based on priority, ensuring that higher-priority tasks are executed first.
- **Scalability:** Efficiently manages a dynamic number of tasks and worker threads.

## Project Structure

```
/DistributedTaskQueueSystem
│
├── /src
│   ├── Worker.h         # Header file for Worker class
│   ├── Worker.cpp       # Implementation of Worker class
│   ├── Client.h         # Header file for Client class
│   ├── Client.cpp       # Implementation of Client class
│   ├── Server.h         # Header file for Server class
│   ├── Server.cpp       # Implementation of Server class
│   ├── main.cpp         # Entry point of the application
│
├── /include              # Optional: Additional headers if needed
```

## Dependencies

- **C++ Standard Library:** Requires a C++ compiler supporting C++11 or later.
- **Windows Sockets API (Winsock):** For networking capabilities (specifically for the Client).

## Build Instructions

### Prerequisites

- Install MinGW or any C++ compiler that supports C++11 or later.
- Ensure that your compiler's bin directory is in the system PATH.

### Compilation

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd DistributedTaskQueueSystemWithPriority
   ```

2. Compile the code using the following command:
   ```bash
   g++ src/*.cpp -o DistributedTaskQueue -pthread
   ```

3. Run the server and client in separate terminal windows:
   - Start the server:
     ```bash
     ./DistributedTaskQueue server
     ```
   - Start the client:
     ```bash
     ./DistributedTaskQueue client "Task description" priority
     ```

## Usage

1. **Run the server** first to listen for incoming tasks.
2. **Run multiple clients** to submit tasks with the desired descriptions and priorities.
3. The server processes the tasks based on priority and assigns them to worker threads for execution.

## Key Concepts

- **Multithreading:** Utilizes threads to handle multiple tasks concurrently.
- **Mutexes:** Ensures thread safety when accessing shared resources like the task queue.
- **Priority Queue:** Implements a data structure to manage tasks based on their priority levels.

## Conclusion

This Distributed Task Queue System with Priority is a practical demonstration of concurrent programming concepts, task prioritization, and client-server architecture in C++. It provides a solid foundation for further enhancements, such as persistent task storage, improved error handling, and advanced priority algorithms.
