#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include <iostream>
#include <vector>
#include <limits.h>
#include "Input.h"
#include "AdjacencyM.h"
#include "CGraph.h"
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

#define SIZE_GRID 16

struct Path {
    int n_elements;
    vector<int> ids;
};

/* Ids of centroids */
vector<int> centroids_100   = {3,72,89,15,70,76,32,94,61,19,105,101,6,50,114,43};
vector<int> centroids_1000  = {526,159,713,26,479,636,662,808,126,170,921,69,84,335,254,79};
vector<int> centroids_2000  = {1000,295,367,15,943,1187,344,1592,813,772,447,426,594,178,474,47};
vector<int> centroids_5000  = {2561,2786,3483,3716,183,2934,3262,3870,2001,1898,4332,87,1381,1537,1199,4960};
vector<int> centroids_10000 = {5068,5684,6930,15,4606,5997,6615,7707,3981,75,8631,8169,2752,69,2298,9720};
vector<int> centroids_500k  = {235407,264361,21684,350939,228188,293237,307630,379864,191961,177484,423199,394328,126812,41706,437664,7541};
vector<int> centroids_1m    = {485083,546092,683287,744317,439338,576571,165329,774790,393616,91549,215787,805300,286925,317433,911976,60020};
vector<int> centroids_1000000 = {602100,558951,743532,593951,714495,294547,101127,64002,759588,481547,160663,435849,318625,523374,895407,243787};
vector<int> centroids_5000000 = {977998,3627326,3589203,2695440,655195,3724653,3193819,3389920,4340309,3949246,4827196,1322962,845298,1835747,1736173,4588129};
vector<int> centroids_10000000 = {453641,1977436,4396154,413726,972556,2996705,6121260,6959940,4678252,8644145,6891782,5248672,6501107,3843274,8780676,5625495};

vector<vector<Path>> short_path_centroids (SIZE_GRID, vector<Path>(SIZE_GRID));
vector<int> short_path_id;

vector<int> short_path_between_centroids;

/* Shortest path among all centroids */
void createShortestPath() {
    int k, aux;
    ifstream myfile;

    if (Nnodes == 1) myfile.open("D:/Graph_OpenGL/data/matrix_paths/100_paths.txt");
    if (Nnodes == 2) myfile.open("D:/Graph_OpenGL/data/matrix_paths/1000_paths.txt");
    if (Nnodes == 3) myfile.open("D:/Graph_OpenGL/data/matrix_paths/2000_paths.txt");
    if (Nnodes == 4) myfile.open("D:/Graph_OpenGL/data/matrix_paths/5000_paths.txt");
    if (Nnodes == 5) myfile.open("D:/Graph_OpenGL/data/matrix_paths/10000_paths.txt");
    if (Nnodes == 6) return;
    if (Nnodes == 7) return;
    if (Nnodes == 8) return;
    /*
    if (Nnodes == 6) myfile.open("D:/Graph_OpenGL/data/matrix_paths/500k_paths.txt");
    if (Nnodes == 7) myfile.open("D:/Graph_OpenGL/data/matrix_paths/1m_paths.txt");
    */
    for (int i = 0; i < SIZE_GRID; i++) {
        for (int j = 0; j < SIZE_GRID; j++) {
            myfile >> k;
            short_path_centroids[i][j].n_elements = k;
            for (int r = 0; r < k+1; r++) {
                myfile >> aux;
                short_path_centroids[i][j].ids.push_back(aux);
            }
        }
    }
}

/* Euclidean distance between two points */
float euclidean_d(int x1, int y1, int x2, int y2){
	float dist;
	dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return dist;
}

/* Select the centroids */
vector<int> select_centroids(vector<int> centroids) {
    if (Nnodes == 1) centroids = centroids_100;
    if (Nnodes == 2) centroids = centroids_1000;
    if (Nnodes == 3) centroids = centroids_2000;
    if (Nnodes == 4) centroids = centroids_5000;
    if (Nnodes == 5) centroids = centroids_10000;
    if (Nnodes == 6) centroids = centroids_1000000;
    return centroids;
}
/* Indicates the quadrant when the id is a centroid */
int num_quadr(int id) {
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);

    for (int i = 0; i < SIZE_GRID; i++) {
        if (centroids[i] == id) {
            return i;
        }
    }
    return -1;
}

/* Check if id is in the path between centroids - direction: q1->q2*/
bool in_path(int id, int q1, int q2){
    for (int i = 0; i < short_path_centroids[q1][q2].n_elements; i++) {
        if (short_path_centroids[q1][q2].ids[i] == id) return true;
    }
    return false;
}
/* Indicates which quadrant is the id */
int quadrant_of_centroid_near(int id) {
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);
    vector<Node<int>>* nodes = graph.m_nodes;

    int min_dist = euclidean_d(nodes->at(id).m_data[0], nodes->at(id).m_data[1], nodes->at(centroids[0]).m_data[0],nodes->at(centroids[0]).m_data[1]);
    int dist;
    int q = 0;
    for (int i = 1; i < SIZE_GRID; i++) {
        dist = euclidean_d(nodes->at(id).m_data[0], nodes->at(id).m_data[1], nodes->at(centroids[i]).m_data[0],nodes->at(centroids[i]).m_data[1]);
        if (dist < min_dist) {
            min_dist = dist;
            q = i;
        }
    }
    return q;
}

void min_path_between_centroids (int q1, vector<Node<int>>* nodes, int q2) {
    cout << "SDASd" << endl;

    short_path_between_centroids.clear();
    short_path_between_centroids.push_back(q1);
    Edge<int>* edges = graph.m_edges->at(q1);
    int min_dist = 80000;
    int x1 = nodes->at(q2).m_data[0];
    int y1 = nodes->at(q2).m_data[1];
    int id_best;
    int cont = 0;
    while(min_dist != 0 && cont < 150) {
        while(edges != nullptr) {
            if (euclidean_d(nodes->at(edges->id_dst).m_data[0], nodes->at(edges->id_dst).m_data[1], x1, y1) <= min_dist) {
                min_dist = euclidean_d(nodes->at(edges->id_dst).m_data[0], nodes->at(edges->id_dst).m_data[1], x1, y1);
                id_best = edges->id_dst;
            }
            edges = edges->next_edge;
        }

        edges = graph.m_edges->at(id_best);
        short_path_between_centroids.push_back(id_best);
        cout << "id_best: " << id_best << endl;
        cont++;

        if (min_dist != 0 && edges == nullptr) {
            edges = graph.m_edges->at(edges->next_edge->id_dst);
        }

    }
    cout << "min_dist: " << min_dist << endl;

    //for (int i = 0; i < short_path_id.size(); i++) cout << short_path_id[i] << endl;
}

void min_path_to_centroid(int id, vector<Node<int>>* nodes, int q) {
    short_path_id.clear();
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);

    int min_dist = euclidean_d(nodes->at(id).m_data[0], nodes->at(id).m_data[1], nodes->at(centroids[0]).m_data[0],nodes->at(centroids[0]).m_data[1]);
    Edge<int>* edge2 = graph.m_edges->at(id);
    int x1 = nodes->at(centroids[q]).m_data[0];
    int y1 = nodes->at(centroids[q]).m_data[1];
    short_path_id.push_back(id);

    int id_edges = edge2->id_dst;
    int id_nodes = id;
    int id_best = id;

    while (min_dist != 0) {
        while(edge2!= nullptr) {
            if (euclidean_d(nodes->at(edge2->id_dst).m_data[0], nodes->at(edge2->id_dst).m_data[1], x1, y1) < min_dist) {
                min_dist = euclidean_d(nodes->at(edge2->id_dst).m_data[0], nodes->at(edge2->id_dst).m_data[1], x1, y1);
                id_best = edge2->id_dst;
            }
            edge2 = edge2->next_edge;
        }

        edge2 = graph.m_edges->at(id_best);
        id_nodes = id_best;
        short_path_id.push_back(id_best);
        id_edges = edge2->id_dst;
    }
    /* MEJORAR SI HAY UNA DISTANCIA CORTA ENTRE UN ID Y LOS PATH ENTRE CENTROIDES */
}

/* ==================================== DIJKSTRA ======================================== */
/*
const int V_ = 119;//119 1019 2016 5002 9953;
int graphDij[V_][V_];


int minDistance(int dist[], bool sptSet[]) {
   int min = INT_MAX, min_index;          // Initialize min value
    for (int v = 0; v < V_; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

   return min_index;
}

// Function to print shortest path from source to j using parent array
void printPath(int parent[], int j) {
    if (parent[j] == -1) return;              // Base Case : If j is source
    printPath(parent, parent[j]);               // ERROR
    printf("%d ", j);
}

// A utility function to print the constructed distance array
void printSolution(int dist[], int n, int parent[], int src) {
    printf("Vertex\t  \tPath");
    for (int i = 1; i < V_; i++) {
        if (i == 3 || i == 72 || i == 89 || i == 15 || i == 70)
        printf("\n%d -> %d \t %d ", src, i, src);
        printPath(parent, i);
    }
}

/* CREATE DIJKSTRA FILES
void myDijkstra() {
    for (int i = 0; i < V_; i++){
        for (int j = 0; j < V_; j++) {
            graphDij[i][j] = 0;
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
            graphDij[(*ItVect).id][j] = 1;
            ptr = ptr->next_edge;
        }
    }

    /* DIJKSTRA
    int src = 3;
    int dist_[V_];
    bool sptSet[V_];
    int parent[V_];

    for (int i = 0; i < V_; i++) {
        parent[src] = -1;
        dist_[i] = INT_MAX;
        sptSet[i] = false;
    }
    dist_[src] = 0;
   for (int count = 0; count < V_-1; count++) {
        int u = minDistance(dist_, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < V_; v++) {
            if (!sptSet[v] && graphDij[u][v] && dist_[u] + graphDij[u][v] < dist_[v]) {
                parent[v]  = u;
                dist_[v] = dist_[u] + graphDij[u][v];
            }
        }
   }
    printSolution(dist_, V_, parent, src);
}

/* ============================= CREATE LIST_ADJACENCY ================================ */

struct Pair
{
    int nodeId;
    int d;// dijkstra value

    Pair( int nodeId, int d )
    {
        this->nodeId = nodeId;
        this->d = d;
    }
};


struct PairComparator
{
    inline bool operator() ( const Pair& p1, const Pair& p2 )
    {
        return p1.d > p2.d;
    }
};

typedef std::priority_queue< Pair,
                             vector< Pair >,
                             PairComparator > PriorityQueue;

struct ResultDijkstra
{
    vector<float> dists;
    vector<int> parents;
};

ResultDijkstra dijsktra_pq( int srcId, const CGraph<int> &pGraph )
{
    vector<float> vDists;
    vector<int> vParents;
    for ( int q = 0; q < pGraph.Gsize; q++ )
    {
        if ( q != srcId )
        {
            vDists.push_back( INFINITY );
            vParents.push_back( -1 );
        }
        else
        {
            vDists.push_back( 0 );
            vParents.push_back( srcId );
        }
    }

    PriorityQueue myPriorityQueue;

    for ( int q = 0; q < pGraph.Gsize; q++ )
    {
        myPriorityQueue.push( Pair( q, vDists[q] ) );
    }

    while ( !myPriorityQueue.empty() )
    {
        int uId = ( myPriorityQueue.top() ).nodeId;
        myPriorityQueue.pop();

        // Traverse the adjacency list for every edge
        Edge<int>* edge = ( *(pGraph.m_edges) )[uId];
        while ( edge != NULL )
        {
            int vId = edge->id_dst;

            if ( vDists[vId] > vDists[uId] + edge->cost )
            {
                vDists[vId] = vDists[uId] + edge->cost;
                vParents[vId] = uId;
                myPriorityQueue.push( Pair( vId, vDists[vId] ) );
            }

            edge = edge->next_edge;
        }

    }

    ResultDijkstra result;
    result.dists = vDists;
    result.parents = vParents;

    return result;
}

void precalc()
{
    cout << "precalculating ...." << endl;
    ofstream file_out;
    file_out.open( "/home/wilsan/_ext/myGraphs/test_precalc.txt" );

    vector<int> centroids;
    centroids = select_centroids( centroids );
    cout << "? -> " << centroids.size() << endl;
    for ( int q = 0; q < centroids.size(); q++ )
    {
        cout << "precalc for centroid " << ( q + 1 ) << endl;
        int centroidId = centroids[q];
        ResultDijkstra _res = dijsktra_pq( centroidId, graph );
        for ( int p = 0; p < centroids.size(); p++ )
        {
            vector<int> vPath;

            int vId = centroids[p];

            if ( vId == centroidId )
            {
                vPath.push_back( vId );
            }
            else
            {
                while ( true )
                {
                    vPath.push_back( vId );
                    vId = _res.parents[vId];

                    if ( vId == centroidId )
                    {
                        vPath.push_back( vId );
                        break;
                    }
                }
            }
                
            std::reverse( vPath.begin(), vPath.end() );

            file_out << vPath.size() - 1 << "\t";
            for ( int s = 0; s < vPath.size(); s++ )
            {
                file_out << vPath[s] << " ";
            }
            file_out << endl;
        }

        file_out << endl;
    }

    file_out.close();

    cout << "done" << endl;
}

void create_list_adjacency() {
    ofstream file_out;
    file_out.open("D:/Graph_OpenGL/data/List_Adjacency/10000000_list_Adj.txt");

    for (int i = 0; i < graph.Gsize; i++) {
        file_out << graph.m_nodes->at(i).id;
        file_out << " ";
        Edge<int>* edges = graph.m_edges->at(i);
        while (edges != nullptr) {
            file_out << edges->id_dst;
            file_out << " ";
            edges = edges->next_edge;
        }
        //file_out << -1;           // Optional
        file_out << "\n";
    }

    file_out.close();

}

/* =========================== CREATE DIJSKTRA MATRIX =================================== */
//const int T = 470246;
//int graphDijkstra[T][1000];

void create_dijktra_file() {
/*
    int** graphDijkstra = new int*[T];
    for(int i = 0; i < T; ++i)
        graphDijkstra[i] = new int[1000];

     for (int i = 0; i < T; i++){
        for (int j = 0; j < T; j++) {
            graphDijkstra[i][j] = 0;
        }
    }

    ifstream newfile;
    newfile.open("D:/Graph_OpenGL/data/List_Adjacency/500k_list_Adj.txt");
    int number_node, j;

    typename vector<Node<int>>::iterator ItVect = graph.m_nodes->begin();
    for (ItVect; ItVect != graph.m_nodes->end(); ++ItVect) {
        newfile >> number_node;
        Edge<int>* ptr = graph.m_edges->at((*ItVect).id);
        while(ptr != nullptr) {
            newfile >> j;
            graphDijkstra[(*ItVect).id][j] = 1;
            ptr = ptr->next_edge;
        }
    }

    ofstream out_file;
    out_file.open("C:/Users/Lab-Maestria-12.helpdesk-PC/Desktop/proof/Dijkstra_matrix/500k_dijk.txt");

    for (int i = 0; i < T; i++){
        for (int j = 0; j < T; j++) {
            out_file << graphDijkstra[i][j];
        }
    }

*/
}


/* ========================================== CREATE PATHS ============================== */

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < Nnodes; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}


void printPath(int parent[], int j) {
    if (parent[j]==-1) return;
    printPath(parent, parent[j]);
    printf("%d ", j);
}


int printSolution(int dist[], int n, int parent[]) {
    int src = 0;
    printf("Vertex\t  Distance\tPath");
    for (int i = 0; i < n; i++) {
        printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
        printPath(parent, i);
    }
}


void dijkstra(vector<vector<int>> graph, int src, int size) {
    int dist[size];
    bool sptSet[size];
    int parent[size];

    parent[src] = -1;
    for (int i = 0; i < size; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;
    int p = 0;

    for (int count = 0; count < size; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int k = 0; k < size; k++) {
            for (int i = 1; i < graph[k].size(); i++) {
                if (k == graph[p][i]) {
                    if (!sptSet[k] && dist[u] + 1 < dist[k]) {
                        parent[k] = u;
                        dist[k] = dist[u] + 1;        // + 1
                    }
                }
            }
        }
        p++;

    }

    printSolution(dist, size, parent);
}

void create_paths_list() {
    /* Let us create the example graph discussed above */
/*
    int size = Nnodes;
    vector<vector<int>> graph (size);

    ifstream readile;
    readile.open("D:/Graph_OpenGL/data/List_Adjacency/5k_list_Adj_1.txt");
*/

    vector<vector<int>> graph (9);
    for (int i = 0; i < 9; i++) {
        graph[i].push_back(i);
    }

    graph[0].push_back(1); graph[0].push_back(7);
    graph[1].push_back(0); graph[1].push_back(2); graph[1].push_back(7);
    graph[2].push_back(1); graph[2].push_back(3); graph[2].push_back(5); graph[2].push_back(8);
    graph[3].push_back(2); graph[3].push_back(4); graph[3].push_back(5);
    graph[4].push_back(3); graph[4].push_back(5);
    graph[5].push_back(2); graph[5].push_back(3); graph[5].push_back(4); graph[5].push_back(6);
    graph[6].push_back(5); graph[6].push_back(7); graph[6].push_back(8);
    graph[7].push_back(0); graph[7].push_back(1); graph[7].push_back(6); graph[7].push_back(8);
    graph[8].push_back(2); graph[8].push_back(6); graph[8].push_back(7);
/*
    int ids, j;
    for (int i = 5; i < 6; i++) {
        readile >> ids;
        readile >> j;
        while(j != -1) {
            graph[i].push_back(j);
            readile >> j;
        }
    }

    //cout << graph[65][1] << endl;
    cout << graph[0][2] << endl;
    cout << graph[0][3] << endl;
    cout << graph[0][4] << endl;

    cout << graph[1][1] << endl;
    cout << graph[1][2] << endl;
*/
    dijkstra(graph, 0, 9);

}

#endif // PATH_H_INCLUDED
