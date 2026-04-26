#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ─── Structs ───────────────────────────────────────────────────────────────

typedef struct node {
    struct node *next;
    int order_number;
    int q;           // price of the order
} order;

typedef struct Queue {
    order *head, *tail;
    int size;
} Queue;

// ─── Queue functions ───────────────────────────────────────────────────────

void enqueue(Queue *queue, int order_number, int price) {
    order *new_node = (order *)malloc(sizeof(order));
    if (!new_node) {
        printf("Memory error\n");
        return;
    }
    new_node->order_number = order_number;
    new_node->q = price;
    new_node->next = NULL;

    if (queue->tail == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->size++;
}

order *dequeue(Queue *queue) {
    if (queue->head == NULL) {
        printf("Queue is empty!\n");
        return NULL;
    }
    order *t = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) queue->tail = NULL;
    queue->size--;
    return t;
}

void clear_queue(Queue *queue) {
    while (queue->head != NULL) {
        order *t = queue->head;
        queue->head = queue->head->next;
        free(t);
    }
    queue->tail = NULL;
    queue->size = 0;
}

void print_queue(Queue *queue) {
    if (queue->size == 0) {
        printf("  (no orders in queue)\n");
        return;
    }
    order *cur = queue->head;
    while (cur != NULL) {
        printf("  Order #%d — Price: %d baht\n", cur->order_number, cur->q);
        cur = cur->next;
    }
}

// ─── Main ──────────────────────────────────────────────────────────────────

int main(int argc, char **argv) {
    Queue queue = {NULL, NULL, 0};
    int order_counter = 1;

    // Optional: pre-load orders from argv
    // Usage: ./main <price1> <price2> ...
    for (int i = 1; i < argc; i++) {
        int price = atoi(argv[i]);
        if (price > 0) {
            enqueue(&queue, order_counter++, price);
            printf("[ARGV] Added Order #%d — Price: %d baht\n", order_counter - 1, price);
        }
    }

    int choice;
    int running = 1;

    printf("\n=============================\n");
    printf("   Welcome to Order Queue    \n");
    printf("=============================\n");

    while (running) {
        printf("\n--- MENU ---\n");
        printf("1. Add new order (enqueue)\n");
        printf("2. Serve next order (dequeue + payment)\n");
        printf("3. View current queue\n");
        printf("4. Close shop\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1: {
                int price;
                printf("Enter price for new order: ");
                scanf("%d", &price);
                if (price <= 0) {
                    printf("Invalid price.\n");
                    break;
                }
                enqueue(&queue, order_counter++, price);
                printf("Order #%d added to queue. (Queue size: %d)\n",
                       order_counter - 1, queue.size);
                break;
            }

            case 2: {
                if (queue.size == 0) {
                    printf("Empty queue — no orders to serve.\n");
                    break;
                }
                order *o = dequeue(&queue);
                printf("\nServing Order #%d — Price: %d baht\n", o->order_number, o->q);

                int paid;
                printf("Customer pays: ");
                scanf("%d", &paid);

                if (paid < o->q) {
                    printf("Not enough money! Need %d more baht.\n", o->q - paid);
                    // Put order back at front
                    order *temp_head = queue.head;
                    queue.head = o;
                    o->next = temp_head;
                    if (queue.tail == NULL) queue.tail = o;
                    queue.size++;
                    printf("Order returned to queue.\n");
                } else {
                    printf("Change: %d baht. Thank you!\n", paid - o->q);
                    free(o);
                    printf("Queue remaining: %d order(s)\n", queue.size);
                }
                break;
            }

            case 3: {
                printf("\nCurrent queue (%d order(s)):\n", queue.size);
                print_queue(&queue);
                break;
            }

            case 4: {
                printf("\n=== Closing Shop ===\n");
                if (queue.size > 0) {
                    printf("Warning: %d order(s) still in queue:\n", queue.size);
                    print_queue(&queue);
                } else {
                    printf("No remaining orders.\n");
                }
                printf("Clearing queue and exiting...\n");
                clear_queue(&queue);
                printf("Goodbye!\n");
                running = 0;
                break;
            }

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}