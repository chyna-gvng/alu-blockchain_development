# To-Do List App with Blockchain-like Features

## Description
This C program implements a to-do list application that simulates storing, retrieving, and managing tasks as if they were stored on the Ethereum blockchain. The application uses cryptographic hashing (SHA-256) to ensure the integrity and immutability of the task data, similar to how data is secured on a blockchain.

## Features
- Add new tasks with a unique task ID, description, and status (Pending, In Progress, Completed)
- Update the status of existing tasks
- Delete tasks from the list
- Display all tasks along with their hashes, simulating a blockchain-like data structure
- Maintain the integrity of the task data by linking each task's hash to the previous task's hash, creating a hash chain

## Prerequisites
- C compiler (e.g., GCC)
- OpenSSL library for cryptographic functions

## Compiling and Running
1. Clone the repository or copy the source code file `todo_list.c` to your local machine.
2. Open a terminal or command prompt and navigate to the directory containing the source code file.
3. Compile the program using the following command:

   ```
   gcc -o todo_list todo_list.c -lssl -lcrypto
   ```

4. Run the compiled executable:

   ```
   ./todo_list
   ```

5. The program will display a menu with the following options:
   - Add Task
   - Update Task Status
   - Delete Task
   - List Tasks
   - Exit

6. Follow the on-screen instructions to interact with the to-do list application.

## Implementation Details
1. **Data Structures**: The program uses a `Task` structure to represent a single task, which includes fields for the task ID, description, status, and the task's hash value.
2. **Hashing for Data Integrity**: The `calculate_hash()` function uses the SHA-256 algorithm to generate a unique hash for each task based on its ID, description, and status, as well as the hash of the previous task. This creates a blockchain-like hash chain, ensuring the integrity of the task data.
3. **Task Management Functions**:
   - `add_task()`: Adds a new task to the list and calculates its hash based on the previous task's hash.
   - `update_task_status()`: Updates the status of an existing task and recalculates its hash based on the previous task's hash.
   - `delete_task()`: Removes a task from the list and adjusts the hash chain of the remaining tasks.
   - `list_tasks()`: Displays all the tasks and their corresponding hashes.
4. **User Interface**: The `main()` function provides a menu-driven interface for the user to interact with the to-do list application, allowing them to add, update, delete, and list tasks.

## Limitations
- The maximum number of tasks is limited by the `MAX_TASKS` constant, which is currently set to 100.
- The program does not provide any persistent storage, so the task data is only stored in memory during the program's execution.
- The program does not implement advanced blockchain features, such as proof-of-work or distributed consensus. It only simulates a basic blockchain-like data structure.
