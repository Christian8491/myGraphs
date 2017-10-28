#ifndef ADJACENCYM_H_INCLUDED
#define ADJACENCYM_H_INCLUDED

#include "Path.h"
#include "CGraph.h"

#define SIZE_GRID 16
#define INF 99999                               // Define Infinite as a large enough value.

/* ================================ ADJACENCY MATRIX ====================================== */

const int V = 119;
int graphAdj[V][V];
int dist[V][V];
const int MAX = 7;

/* A function to print the solution matrix */
void printSolution(int dist[][V]) {
    ofstream myFile2;
	myFile2.open("D:/Graph_OpenGL/data/matrix_Adjacency/1000_mat_Adj_2.txt");

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) myFile2 << "INF ";
            else {
                myFile2 << dist[i][j];
                myFile2 << " ";
            }
        }
        myFile2 << "\n";
    }
}

void floydWarshall(int graph[][V]){
    ofstream myFile3;
	myFile3.open("D:/Graph_OpenGL/data/matrix_paths/100_paths.txt");
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    int cont;
    int i = 3;
    for (int k = 0; k < V; k++) {
        for (int j = 0; j < V; j++) {
             if (dist[i][k] + dist[k][j] <= dist[i][j]) {
                 dist[i][j] = dist[i][k] + dist[k][j];
                 if (j == 89) {
                     cout << k << " ";
                 }
             }
         }
    }
    //printSolution(dist);
}

void adjacencyMat(){
    for (int i = 0; i < V; i++){
        for (int j = 0; j < V; j++) {
            graphAdj[i][j] = 0;
        }
    }

    ifstream newfile;
    newfile.open("D:/Graph_OpenGL/data/List_Adjacency/100_list_Adj.txt");
    int number_node, j;

    typename vector<Node<int>>::iterator ItVect = graph.m_nodes->begin();
    for (ItVect; ItVect != graph.m_nodes->end(); ++ItVect) {
        newfile >> number_node;
        Edge<int>* ptr = graph.m_edges->at((*ItVect).id);
        while(ptr != nullptr) {
            newfile >> j;
            graphAdj[(*ItVect).id][j] = 1;
            ptr = ptr->next_edge;
        }
    }
    //floydWarshall(graphAdj);
}

/* ================================== Adjacency matrix - Only to centroids ================= */

void centroidsMAtrix() {
    ifstream file1;
    int id_centroids[SIZE_GRID];
    int mat_centroids[SIZE_GRID][SIZE_GRID];
    file1.open("D:/Graph_OpenGL/data/centroids/centroids10000.txt");

    ofstream file2;
    file2.open("D:/Graph_OpenGL/data/matrix_centroids/10000_centr_dist.txt");

    for (int i = 0; i < SIZE_GRID; i++){
        file1 >> id_centroids[i];
    }

    for (int i = 0; i < SIZE_GRID; i++){
        for (int j = 0; j < SIZE_GRID; j++){
            mat_centroids[i][j] = dist[id_centroids[i]][id_centroids[j]];
        }
    }

    for (int i = 0; i < SIZE_GRID; i++){
        for (int j = 0; j < SIZE_GRID; j++){
            file2 << mat_centroids[i][j];
            file2 << " ";
        }
        file2 << "\n";
    }
}

#endif // ADJACENCYM_H_INCLUDED
