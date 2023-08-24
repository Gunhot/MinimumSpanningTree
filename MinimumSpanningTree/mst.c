#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_VERTICES 10000
#define MAX_EDGES 50000000
int Nvertex;
int Nedge;
typedef struct _edge {
	int a;
	int b;
	int weight;
}Edge;
Edge edge_min_heap[MAX_EDGES];
int heap_num;
int parent[MAX_VERTICES];
void insert_min_heap(Edge new_edge) {
	//새로운 녀석(num.key)이 parent(min_heap[i/2]보다 작을  때 parent를 child자리로 내린다.
	//parent의 자리는 child가 갖는다.
	int the_index = heap_num;
	while ((the_index > 1) && (new_edge.weight < edge_min_heap[the_index / 2].weight)) {
		edge_min_heap[the_index] = edge_min_heap[the_index / 2];
		the_index /= 2;
	}
	edge_min_heap[the_index] = new_edge;
}
Edge return_min() {
	Edge min_edge = edge_min_heap[1];
	Edge temp = edge_min_heap[heap_num--];
	int parent = 1, child = 2;
	while (child <= heap_num) {
		//더큰 노드와 비교 child가 마지막일 때는 child+1할 경우 error
		if (child != heap_num) {
			if (edge_min_heap[child].weight > edge_min_heap[child + 1].weight) child++;
		}
		if (temp.weight < edge_min_heap[child].weight) break;
		//더 작은 애가 위에 존재
		//if( edge_min_heap[child].weight < temp.weight )
		edge_min_heap[parent] = edge_min_heap[child];

		parent = child;
		child *= 2;
	}
	edge_min_heap[parent] = temp;
	return min_edge;
}
int weightedUnion(int i, int j) {
	if (i == j && i != -1) {
		return 1;
	}
	//child의 개수는 두개 합친 것
	int child_sum = parent[i] + parent[j];
	//parent[j]가 더 많을 때
	if (parent[i] > parent[j]) {
		parent[i] = j;
		parent[j] = child_sum;
	}
	//parent[i]가 더 많을 때
	else {
		parent[j] = i;
		parent[i] = child_sum;
	}
	return 0;
}
//찾으면서 1층으로 만들기
int collapsingFind(int i) {
	int root, trail, lead;
	//parent를 따라 올라가면서 root를 찾는 for문
	//root의 parent는 음수
	for (root = i; parent[root] >= 0; root = parent[root]);
	//lead는 trail보다 한 칸 위
	//trail이 parent를 root로 바꾼다
	//trail이 먼저 바뀌고 lead는 trail의 parent가 된다.
	for (trail = i; trail != root; trail = lead) {
		lead = parent[trail];
		parent[trail] = root;
	}
	return root;
}
int main(int argc, char* argv[]) {
	clock_t start = clock();
	FILE* in;
	FILE* out;
	int i;
	//input number of arguments Handling
	if (argc != 2) {
		printf("usage: ./hw3 input_filename\n");
		return 1;
	}
	//input file error Handling
	in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("The input file does not exist.\n");
		return 1;
	}

	//reading input file and making min heap
	fscanf(in, "%d", &Nvertex);
	fscanf(in, "%d", &Nedge);

	for (i = 1; i <= Nedge; i++) {
		Edge temp_edge;
		fscanf(in, "%d %d %d", &temp_edge.a, &temp_edge.b, &temp_edge.weight);
		heap_num = i;
		insert_min_heap(temp_edge);
	}


	//intializing parent
	for (i = 0; i < Nvertex; i++)parent[i] = -1;
	//write FIle
	out = fopen("hw3_result.txt", "w");

	//Kuskal's Algorithm
	int cost = 0;
	while (heap_num) {
		Edge leastEdge = return_min();
		//return 1될 경우 같은 tree소속
		if (weightedUnion(collapsingFind(leastEdge.a), collapsingFind(leastEdge.b)))continue;
		fprintf(out, "%d %d %d\n", leastEdge.a, leastEdge.b, leastEdge.weight);
		cost += leastEdge.weight;
	}
	fprintf(out, "%d\n", cost);
	int disjoint_check = 0;
	if(parent[collapsingFind(0)] == -Nvertex){
		fprintf(out, "CONNECTED\n");
	}
	else {
		fprintf(out, "DISCONNECTED\n");
	}

	
	fclose(in);
	fclose(out);


	printf("output written to hw3_result.txt\n");
	clock_t end = clock();
	printf("running time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
}
