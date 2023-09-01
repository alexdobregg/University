#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct QueueNode{
	int elem;
	struct QueueNode *next;
} QueueNode;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
} Queue;


Queue* createQueue(){
	Queue *q = (Queue *) calloc(1, sizeof(Queue));
	q->size = 0;
	q->front = q->rear = NULL;
	return q;
} 

int isQueueEmpty(Queue *q){
	return (q == NULL || q->front == NULL);
}

void enqueue(Queue *q, int elem){
	QueueNode *qnode = (QueueNode *) calloc(1, sizeof(QueueNode));
	qnode->elem = elem;
	qnode->next = NULL;
	if(isQueueEmpty(q)) {
		q->front = q->rear = qnode;
	} else {
		q->rear->next = qnode;
		q->rear = qnode;
	}
	q->size++;
}

int front(Queue* q){
	if(!isQueueEmpty(q)) {
		return q->front->elem;
	}
	return 0;
}

void dequeue(Queue* q){
	if(isQueueEmpty(q)) {
		return;
	} else {
		QueueNode *elim = q->front;
		q->front = q->front->next;
		free(elim);
		q->size--;
	}
	if(q->size == 0) {
		q->front = q->rear = NULL;
	}
}

void destroyQueue(Queue *q){
	while(q->front != NULL) {
		dequeue(q);
	}
	free(q);
}


void print_queue(Queue *q, int *p, int *c, int *dist, int *toPrint, int *count) {
    QueueNode *node;
    printf("Q = {");
    for (node = q->front; node != NULL; node = node->next) {
        printf("%c", (char)node->elem + 65);
        if (node->next != NULL) {
            printf(", ");
        }
    }
    printf("}\n");
    for (node = q->front; node != NULL; node = node->next) {
        if (toPrint[node->elem] == 0) {
            printf("d(%c) = %d", (char)node->elem + 65, dist[node->elem]);
            if (node->next != NULL) {
                printf("; ");
            }
            (*count)++;
        }
    }
    if ((*count) != 0) {
        printf("\n");    
    }

    for (node = q->front; node != NULL; node = node->next) {
        if (toPrint[node->elem] == 0) {
            if (p[node->elem] == -1) {
                printf("p(%c) = null", (char)node->elem + 65);
            } else {
                printf("p(%c) = %c", (char)node->elem + 65, (char)p[node->elem] + 65);
            }
            if (node->next != NULL) {
                printf("; ");
            }
        }
    }
    if ((*count) != 0) {
        printf("\n");    
    }

    for (node = q->front; node != NULL; node = node->next) {
        if (toPrint[node->elem] == 0) {
            printf("c(%c) = gri", (char)node->elem + 65);
            if (node->next != NULL) {
                printf("; ");
            }
        }
        toPrint[node->elem] = 1;
    }
}

void BFS(int **graph, int V, int src, int *p, int *c) {
    int *dist = (int *) calloc(V, sizeof(int));
    int *toPrint = (int *) calloc(V, sizeof(int));
    int prev;
    int lastOut;
    for (int u = 0; u < V; u++) {
        p[u] = -1;
        c[u] = 0; // alb
        dist[u] = INT_MAX;
    }
    dist[src] = 0;
    int step = 1;
    Queue *q = createQueue();
    enqueue(q, src);
    c[src] = 1; // gri

    while(q->front != NULL) {
        printf("%d) ", step++);
        int count = 0;
        print_queue(q, p, c, dist, toPrint, &count);
        int u = front(q);
        for (int v = 0; v < V; v++) {
            if (graph[u][v] == 1 && c[v] == 0) {
                dist[v] = dist[u] + 1;
                p[v] = u;
                c[v] = 1; // gri
                enqueue(q, v);
            }
        }
        if(u != 0) {
            if(count == 0) {
                printf("c(%c) = negru", (char)prev + 65);
            } else {
                printf("; c(%c) = negru", (char)prev + 65);
            }
        }
        printf("\n\n");
        c[u] = 2; // negru
        prev = u;
        lastOut = front(q);
        dequeue(q);
    }
    printf("%d) Q = {}\n", step++);
    printf("c(%c) = negru\n", (char)lastOut + 65);
    free(toPrint);
    free(dist);
    destroyQueue(q);
}


// PROBLEMA 2 - DFS

void explore(int **graph, int u, int V, int *timp, int *f, int *d, int *c, int *p) {
    d[u] = ++(*timp);
    c[u] = 1; // gri
    if (p[u] == -1) {
        printf("%d) c(%c) = gri; d(%c) = %d; p(%c) = null\n", (*timp), (char)u + 65, (char)u + 65, d[u], (char)u + 65);
    } else {
        printf("%d) c(%c) = gri; d(%c) = %d; p(%c) = %c\n", (*timp), (char)u + 65, (char)u + 65, d[u], (char)u + 65, (char)p[u] + 65);
    }
    for (int v = 0; v < V; v++) {
        if (graph[u][v] == 1 && c[v] == 0) {
            p[v] = u;
            explore(graph, v, V, timp, f, d, c, p);
        }
    }
    c[u] = 2; // negru;
    f[u] = ++(*timp);
    printf("%d) c(%c) = negru; f(%c) = %d\n", (*timp), (char)u + 65, (char)u + 65, f[u]);
}


void DFS(int **graph, int V, int *f, int *d, int *c, int *p) {
    for (int u = 0; u < V; u++) {
        c[u] = 0; // alb
        p[u] = -1; // null
        f[u] = 0;
        d[u] = 0;
    }
    int timp = 0;
    for (int u = 0; u < V; u++) {
        if (c[u] == 0) {
            explore(graph, u, V, &timp, f, d, c, p);
        }
    }
    printf("\nLista este:");
    for (int u = 0; u < V; u++) {
        printf(" %c: (%d, %d);", (char)u + 65, d[u], f[u]);
    }
    printf("\n");
}






int main() {
    // ALOCARE MEMORIE PENTRU REZOLVARE LABORATOR.

    int V_lab = 10; 

    // Timpul de finalizare
    int *f_lab = (int *) calloc(V_lab, sizeof(int));

    // Timpul de descoperire
    int *d_lab = (int *) calloc(V_lab, sizeof(int));

    // Culoarea: 0 alb, 1 gri, 2 negru
    int *c_lab = (int *) calloc(V_lab, sizeof(int));

    // Parinte;
    int *p_lab = (int *) calloc(V_lab, sizeof(int));

    int **graphLab = (int **) calloc(V_lab, sizeof(int *));
    for (int i = 0; i < V_lab; i++) {
        graphLab[i] = (int *) calloc(V_lab, sizeof(int));
    }

    // A: B, C, H
  	graphLab[0][1] = 1;
  	graphLab[0][2] = 1;
  	graphLab[0][7] = 1;
    // B: D, E, H, I
  	graphLab[1][3] = 1;
  	graphLab[1][4] = 1;
  	graphLab[1][7] = 1;
  	graphLab[1][8] = 1;
    // C: D
  	graphLab[2][3] = 1;
    // D: E, J
  	graphLab[3][4] = 1;
  	graphLab[3][9] = 1;
    // E: F, G
  	graphLab[4][5] = 1;
  	graphLab[4][6] = 1;
    // F: G
  	graphLab[5][6] = 1;
    // G: -
    // H: I
  	graphLab[7][8] = 1;
    // I: A
  	graphLab[8][0] = 1;
    // J: -

    /* -------------------------------------------------- */

    // ALOCARE MEMORIE PENTRU REZOLVARE CURS.

    int V_curs = 12;

    // Timpul de finalizare
    int *f_curs = (int *) calloc(V_curs, sizeof(int));

    // Timpul de descoperire
    int *d_curs = (int *) calloc(V_curs, sizeof(int));

    // Culoarea: 0 alb, 1 gri, 2 negru
    int *c_curs = (int *) calloc(V_curs, sizeof(int));

    // Parinte;
    int *p_curs = (int *) calloc(V_curs, sizeof(int));

    int **graphCurs = (int **) calloc(V_curs, sizeof(int *));
    for (int i = 0; i < V_curs; i++) {
        graphCurs[i] = (int *) calloc(V_curs, sizeof(int));
    }

    // A: B, G
    graphCurs[0][1] = 1;
  	graphCurs[0][6] = 1;
    // B: H
  	graphCurs[1][7] = 1;
    // C: D, E
  	graphCurs[2][3] = 1;
  	graphCurs[2][4] = 1;
    // D: -
    // E: F
  	graphCurs[4][5] = 1;
    // F: -
    // G: B, C
  	graphCurs[6][1] = 1;
    graphCurs[6][2] = 1;
    // H: A
  	graphCurs[7][0] = 1;
    // I: A, J, K
  	graphCurs[8][0] = 1;
  	graphCurs[8][9] = 1;
  	graphCurs[8][10] = 1;
    // J: K
  	graphCurs[9][10] = 1;
    // K: L
  	graphCurs[10][11] = 1;
    // L: -

    // BFS
    printf("1. BFS:\n\n");
    printf("GRAPH LABORATOR \n");
    BFS(graphLab, V_lab, 0, p_lab, c_lab);
    printf("\nGRAPH CURS\n");
    BFS(graphCurs, V_curs, 0, p_curs, c_curs);
    printf("\n");

    // DFS
    printf("2. DFS:\n\n");
    printf("GRAPH LABORATOR \n");
    DFS(graphLab, V_lab, f_lab, d_lab, c_lab, p_lab);
    printf("\nGRAPH CURS\n");
    DFS(graphCurs, V_curs, f_curs, d_curs, c_curs, p_curs);

    // Dezalocare memorie
    for (int i = 0; i < V_lab; i++) {
        free(graphLab[i]);
    }
    free(graphLab);
    free(p_lab);
    free(c_lab);
    free(d_lab);
    free(f_lab);

    for (int i = 0; i < V_curs; i++) {
        free(graphCurs[i]);
    }
    free(graphCurs);
    free(p_curs);
    free(c_curs);
    free(d_curs);
    free(f_curs);

    return 0;
}