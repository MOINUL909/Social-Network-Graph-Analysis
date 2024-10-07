#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
UserId=Moinul
Password=Moinul909
*/

// Define login credentials
#define USER_ID "Moinul"
#define PASSWORD "Moinul909"

// Structure to represent a node (person) in the graph
typedef struct Node {
    int id;             // Unique ID of the person
    char name[50];      // Name of the person
    struct Node* next;  // Pointer to the next node (person)
} Node;

// Structure to represent an adjacency list
typedef struct AdjList {
    Node* head;         // Pointer to the head of the adjacency list
} AdjList;

// Structure to represent a graph
typedef struct Graph {
    int numVertices;    // Number of vertices (people) in the graph
    AdjList* array;     // Array of adjacency lists
    int* visited;       // Array to track visited nodes during traversal
} Graph;

// Function to verify login credentials
int login() {
    char inputUserId[50];
    char inputPassword[50];

    printf("Enter User ID: ");
    scanf("%s", inputUserId);

    printf("Enter Password: ");
    scanf("%s", inputPassword);

    // Check if the entered credentials match the predefined credentials
    if (strcmp(inputUserId, USER_ID) == 0 && strcmp(inputPassword, PASSWORD) == 0) {
        printf("Login successful!\n\n");
        return 1;  // Return 1 for successful login
    } else {
        printf("Invalid User ID or Password.\n");
        return 0;  // Return 0 for failed login
    }
}

// Function to create a new node
Node* createNode(int id, const char* name) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with 'numVertices' vertices
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->array = (AdjList*)malloc(numVertices * sizeof(AdjList));
    graph->visited = (int*)malloc(numVertices * sizeof(int));

    for (int i = 0; i < numVertices; i++) {
        graph->array[i].head = NULL;
        graph->visited[i] = 0;  // Initialize all vertices as not visited
    }

    return graph;
}

// Function to add an edge to the graph (undirected)
void addEdge(Graph* graph, int src, int dest, const char* destName) {
    // Add an edge from src to dest
    Node* newNode = createNode(dest, destName);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Add an edge from dest to src (because it's an undirected graph)
    Node* newNode2 = createNode(src, "");
    newNode2->next = graph->array[dest].head;
    graph->array[dest].head = newNode2;
}

// Function to print the adjacency list of the graph
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        Node* temp = graph->array[v].head;
        printf("Person %d (%s): ", v, temp ? temp->name : "Unknown");
        while (temp) {
            printf("-> %d ", temp->id);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Depth-First Search (DFS) utility function to explore the graph
void DFS(Graph* graph, int v) {
    // Mark the current node as visited and print it
    graph->visited[v] = 1;
    printf("%d ", v);

    // Recur for all the vertices adjacent to this vertex
    Node* temp = graph->array[v].head;
    while (temp) {
        int connectedVertex = temp->id;
        if (!graph->visited[connectedVertex]) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

// Function to find and print all connected components in the graph
void findConnectedComponents(Graph* graph) {
    printf("Connected components in the graph:\n");
    for (int v = 0; v < graph->numVertices; v++) {
        if (!graph->visited[v]) {
            DFS(graph, v);
            printf("\n");
        }
    }
}

// Function to detect cycles in the graph using DFS
int detectCycleUtil(Graph* graph, int v, int parent) {
    graph->visited[v] = 1;

    Node* temp = graph->array[v].head;
    while (temp) {
        int connectedVertex = temp->id;

        if (!graph->visited[connectedVertex]) {
            if (detectCycleUtil(graph, connectedVertex, v)) {
                return 1;
            }
        } else if (connectedVertex != parent) {
            return 1;  // Cycle detected
        }
        temp = temp->next;
    }
    return 0;
}

// Function to check if a graph contains a cycle
int detectCycle(Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        graph->visited[v] = 0;
    }

    for (int v = 0; v < graph->numVertices; v++) {
        if (!graph->visited[v]) {
            if (detectCycleUtil(graph, v, -1)) {
                return 1;
            }
        }
    }
    return 0;
}

// Function to perform Breadth-First Search (BFS) and find the shortest path
void shortestPath(Graph* graph, int startVertex, int endVertex) {
    int* distance = (int*)malloc(graph->numVertices * sizeof(int));
    int* queue = (int*)malloc(graph->numVertices * sizeof(int));
    int front = 0, rear = 0;

    for (int i = 0; i < graph->numVertices; i++) {
        distance[i] = -1;
    }

    // Start BFS from the startVertex
    queue[rear++] = startVertex;
    distance[startVertex] = 0;

    while (front < rear) {
        int currentVertex = queue[front++];

        Node* temp = graph->array[currentVertex].head;
        while (temp) {
            int adjVertex = temp->id;

            if (distance[adjVertex] == -1) {
                distance[adjVertex] = distance[currentVertex] + 1;
                queue[rear++] = adjVertex;

                // Stop if we reached the endVertex
                if (adjVertex == endVertex) {
                    printf("Shortest path length from %d to %d: %d\n", startVertex, endVertex, distance[endVertex]);
                    free(distance);
                    free(queue);
                    return;
                }
            }
            temp = temp->next;
        }
    }

    printf("No path exists between %d and %d\n", startVertex, endVertex);
    free(distance);
    free(queue);
}

// Main function to demonstrate social network graph analysis
int main() {
    // Login mechanism
    if (login()) {
        printf("Access granted. You can use the social network graph analysis.\n\n");

        int numVertices = 6;  // Number of people (vertices)
        Graph* graph = createGraph(numVertices);

        // Adding relationships (edges) between people
        addEdge(graph, 0, 1, "Alice");
        addEdge(graph, 0, 2, "Bob");
        addEdge(graph, 1, 2, "Charlie");
        addEdge(graph, 1, 3, "David");
        addEdge(graph, 2, 3, "Eve");
        addEdge(graph, 3, 4, "Frank");
        addEdge(graph, 4, 5, "Grace");

        // Print the graph (social network)
        printGraph(graph);

        // Find and print all connected components
        findConnectedComponents(graph);

        // Check if the graph contains a cycle
        if (detectCycle(graph)) {
            printf("The social network graph contains a cycle.\n");
        } else {
            printf("The social network graph does not contain any cycles.\n");
        }

        // Find the shortest path between two people
        shortestPath(graph, 0, 5);

        // Free allocated memory
        for (int i = 0; i < graph->numVertices; i++) {
            Node* temp = graph->array[i].head;
            while (temp) {
                Node* next = temp->next;
                free(temp);
                temp = next;
            }
        }
        free(graph->array);
        free(graph->visited);
        free(graph);

    } else {
        printf("Access denied. Exiting program.\n");
    }

    return 0;
}

