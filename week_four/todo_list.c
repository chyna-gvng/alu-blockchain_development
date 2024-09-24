#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_TASKS 100

typedef enum { PENDING, IN_PROGRESS, COMPLETED } TaskStatus;

// Task structure to hold task information
typedef struct {
    int task_id;
    char description[100];
    TaskStatus status;
    unsigned char hash[SHA256_DIGEST_LENGTH];
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

// Function to calculate the hash of a task based on its data and the previous task's hash
void calculate_hash(Task* task, const unsigned char* prev_hash) {
    unsigned char input[1024];
    // Combine the task's ID, description, status, and the previous task's hash into a single input buffer
    sprintf((char*)input, "%d%s%d%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            task->task_id, task->description, task->status,
            prev_hash[0], prev_hash[1], prev_hash[2], prev_hash[3],
            prev_hash[4], prev_hash[5], prev_hash[6], prev_hash[7],
            prev_hash[8], prev_hash[9], prev_hash[10], prev_hash[11],
            prev_hash[12], prev_hash[13], prev_hash[14], prev_hash[15],
            prev_hash[16], prev_hash[17], prev_hash[18], prev_hash[19],
            prev_hash[20], prev_hash[21], prev_hash[22], prev_hash[23],
            prev_hash[24], prev_hash[25], prev_hash[26], prev_hash[27],
            prev_hash[28], prev_hash[29], prev_hash[30], prev_hash[31]);
    
    // Calculate the hash using SHA-256 and store it in the task's hash field
    SHA256(input, strlen((char*)input), task->hash);
}

// Function to add a new task
void add_task(int task_id, char* description, TaskStatus status) {
    // Check if the maximum number of tasks has been reached
    if (task_count >= MAX_TASKS) {
        printf("Error: Maximum number of tasks reached.\n");
        return;
    }

    // Initialize the new task with the given information
    tasks[task_count].task_id = task_id;
    strcpy(tasks[task_count].description, description);
    tasks[task_count].status = status;
    
    // Calculate the hash of the new task based on the previous task's hash
    unsigned char prev_hash[SHA256_DIGEST_LENGTH] = {0};
    if (task_count > 0) {
        memcpy(prev_hash, tasks[task_count - 1].hash, SHA256_DIGEST_LENGTH);
    }
    calculate_hash(&tasks[task_count], prev_hash);

    task_count++;
    printf("Task added successfully.\n");
}

// Function to update the status of a task
void update_task_status(int task_id, TaskStatus new_status) {
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].task_id == task_id) {
            tasks[i].status = new_status;
            
            // Calculate the hash of the updated task based on the previous task's hash
            unsigned char prev_hash[SHA256_DIGEST_LENGTH] = {0};
            if (i > 0) {
                memcpy(prev_hash, tasks[i - 1].hash, SHA256_DIGEST_LENGTH);
            }
            calculate_hash(&tasks[i], prev_hash);

            printf("Task status updated successfully.\n");
            return;
        }
    }
    printf("Error: Task not found.\n");
}

// Function to delete a task
void delete_task(int task_id) {
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].task_id == task_id) {
            found = 1;
            // Shift the remaining tasks to maintain the hash chain
            for (int j = i; j < task_count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            task_count--;
            printf("Task deleted successfully.\n");
            break;
        }
    }
    if (!found) {
        printf("Error: Task not found.\n");
    }
}

// Function to list all the tasks and their hashes
void list_tasks() {
    printf("Tasks:\n");
    for (int i = 0; i < task_count; i++) {
        printf("Task ID: %d, Description: %s, Status: %s, Hash: ", tasks[i].task_id, tasks[i].description,
               tasks[i].status == PENDING ? "Pending" : (tasks[i].status == IN_PROGRESS ? "In Progress" : "Completed"));
        for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
            printf("%02x", tasks[i].hash[j]);
        }
        printf("\n");
    }
}

int main() {
    int choice, task_id;
    char description[100];
    TaskStatus status;

    while (1) {
        printf("\nTo-Do List Menu:\n");
        printf("1. Add Task\n");
        printf("2. Update Task Status\n");
        printf("3. Delete Task\n");
        printf("4. List Tasks\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter task ID: ");
                scanf("%d", &task_id);
                printf("Enter task description: ");
                scanf(" %[^\n]s", description);
                printf("Enter task status (0 - Pending, 1 - In Progress, 2 - Completed): ");
                scanf("%d", &status);
                add_task(task_id, description, (TaskStatus)status);
                break;
            case 2:
                printf("Enter task ID: ");
                scanf("%d", &task_id);
                printf("Enter new status (0 - Pending, 1 - In Progress, 2 - Completed): ");
                scanf("%d", &status);
                update_task_status(task_id, (TaskStatus)status);
                break;
            case 3:
                printf("Enter task ID: ");
                scanf("%d", &task_id);
                delete_task(task_id);
                break;
            case 4:
                list_tasks();
                break;
            case 5:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
