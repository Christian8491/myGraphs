/*
 * Author: christian Córdova Estrada
 * Copy the folder Graph in D, after run
 * Select a number of nodes {1,2,3,4,5} to create a Graph with
 * {100,1000,2000,50000,10000} vertex
 *
 * Select two valid ids within the range shown
 */

#include "Commom.h"
#include "CGraph.h"
#include "Path.h"
#include "AdjacencyM.h"
#include "myOpenGL.h"
#include "Input.h"

int main() {
    /* To read the input data */
    initializeRead();

    /* To search and find the shortest path between two any ids */
    //createShortestPath();
    //create_paths_list();


    precalc();

   // vector<Node<int>>* nodes = graph.m_nodes;
    // min_path_between_centroids (602100, nodes, 558951);


    /* To drawn the graph */
    //initializeOpenGL();

    return 0;
}
