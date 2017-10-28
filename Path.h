#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include "Commom.h"
#include "Input.h"
#include "AdjacencyM.h"
#include "CGraph.h"

using namespace std;

#define SIZE_GRID 16
#define FACTOR 1000

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
vector<int> min_path_between_id_to_centroid;            // Path: id1 -> q1
vector<int> short_path_between_centroids;               // Path: q2  -> q2

/* Shortest path among all centroids */
void createShortestPath() {
    int k, aux;
    ifstream myfile;

    if (Nnodes == 1) myfile.open("D:/Graph_OpenGL/data/matrix_paths/test_precalc.txt");
    if (Nnodes == 2) myfile.open("D:/Graph_OpenGL/data/matrix_paths/1000_paths.txt");
    if (Nnodes == 3) myfile.open("D:/Graph_OpenGL/data/matrix_paths/2000_paths.txt");
    if (Nnodes == 4) myfile.open("D:/Graph_OpenGL/data/matrix_paths/5000_paths.txt");
    if (Nnodes == 5) myfile.open("D:/Graph_OpenGL/data/matrix_paths/test_precalc10000.txt");
    if (Nnodes == 6) myfile.open("D:/Graph_OpenGL/data/matrix_paths/test_precalc1000000.txt");
    if (Nnodes == 7) myfile.open("D:/Graph_OpenGL/data/matrix_paths/test_precalc5000000.txt");
    if (Nnodes == 8) myfile.open("D:/Graph_OpenGL/data/matrix_paths/test_precalc10000000.txt");

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
    if (Nnodes == 7) centroids = centroids_5000000;
    if (Nnodes == 8) centroids = centroids_10000000;
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
/*
void min_path_between_centroids (int q1, vector<Node<int>>* nodes, int q2) {
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
*/


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

/* ============================= PRE - CALCULUS - MATRIX PATHS ================================ */

struct Pair {
    int nodeId;
    int d;
    Pair(int nodeId_, int d_) {nodeId = nodeId_; d = d_;}
};

struct PairComparator {
    inline bool operator() (const Pair& p1, const Pair& p2) { return p1.d > p2.d; }
};

typedef std::priority_queue<Pair, vector<Pair>, PairComparator> PriorityQueue;

struct ResultDijkstra {
    vector<float> dists;
    vector<int> parents;
};

ResultDijkstra dijsktra_pq(int srcId, const CGraph<int> &pGraph) {
    /* Initialize */
    vector<float> vDists(pGraph.Gsize, INFINITY);
    vector<int> vParents(pGraph.Gsize, -1);

    vDists[srcId] = 0;
    vParents[srcId] = srcId;

    PriorityQueue myPriorityQueue;
    const int _size = pGraph.Gsize/FACTOR;

    for (int i = 0; i < _size ; i++) {
        myPriorityQueue.push(Pair(i, vDists[i]));
    }

    while (!myPriorityQueue.empty()) {
        int uId = (myPriorityQueue.top()).nodeId;
        myPriorityQueue.pop();

        /* Traverse the adjacency list for every edge */
        Edge<int>* edge = pGraph.m_edges->at(uId);
        while (edge != nullptr) {
            int vId = edge->id_dst;
            if (vDists[vId] > vDists[uId] + edge->cost) {
                vDists[vId] = vDists[uId] + edge->cost;
                vParents[vId] = uId;
                myPriorityQueue.push(Pair( vId, vDists[vId]));
            }
            edge = edge->next_edge;
        }
    }

    ResultDijkstra result;
    result.dists = vDists;
    result.parents = vParents;

    return result;
}

/* To pre-calculus */
void pre_calculus() {
    ofstream file_out;
    file_out.open( "D:/Graph_OpenGL/data/matrix_paths/test_precalc10000000.txt" );

    vector<int> centroids;
    centroids = select_centroids(centroids);

    for (int q = 0; q < centroids.size(); q++) {
        cout << "pre calculus for centroid " << (q + 1) << endl;
        int centroidId = centroids[q];
        ResultDijkstra _res = dijsktra_pq(centroidId, graph);
        for (int p = 0; p < centroids.size(); p++) {
            vector<int> vPath;
            int vId = centroids[p];

            if (vId == centroidId) vPath.push_back(vId);
            else {
                while (true) {
                    vPath.push_back(vId);
                    vId = _res.parents[vId];

                    if (vId == centroidId) {
                        vPath.push_back(vId);
                        break;
                    }
                }
            }

            std::reverse(vPath.begin(), vPath.end());
            file_out << vPath.size() - 1 << "\t";
            for (int s = 0; s < vPath.size(); s++) {
                file_out << vPath[s] << " ";
            }
            file_out << endl;
        }
        file_out << endl;
    }
    file_out.close();

    cout << "done" << endl;
}


/* Path between id an his centroid */
void min_path_to_centroid(int id, vector<Node<int>>* nodes, int q) {
    short_path_id.clear();
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);

    int x1 = nodes->at(q).m_data[0];
    int y1 = nodes->at(q).m_data[1];

    int min_dist = euclidean_d(nodes->at(id).m_data[0], nodes->at(id).m_data[1], x1, y1);
    cout << "min_dist: "  << min_dist << endl;
    Edge<int>* edges = graph.m_edges->at(id);
    short_path_id.push_back(id);
    int id_best = id, id_best2 = id;
    int dist_aux, cont = 0;
    int id_aux;

    while (min_dist != 0 && cont < 10000) {
        while(edges!= nullptr) {
            dist_aux =  euclidean_d(nodes->at(edges->id_dst).m_data[0], nodes->at(edges->id_dst).m_data[1], x1, y1);
            if (dist_aux < min_dist) {
                min_dist = dist_aux;
                id_best = edges->id_dst;
            }
            edges = edges->next_edge;
        }
        cont++;

        edges = graph.m_edges->at(id_best);
        short_path_id.push_back(id_best);
        id_best2 = id_best;
/*
        if (id_best2 != id_best) {
            edges = graph.m_edges->at(id_best);
            short_path_id.push_back(id_best);
            id_best2 = id_best;
            cont = 0;
        } else {
            edges = graph.m_edges->at(id_best);
            id_aux = edges->id_dst;
            min_dist = euclidean_d(graph.m_nodes->at(id_aux).m_data[0],graph.m_nodes->at(id_aux).m_data[1], x1,y1);
        }
*/
    }

    cout << "min_dist: "  << min_dist << endl;
    cout << "short_path_id: " << short_path_id.size() << endl;
}

/* Calculate the min path between two points -- NOT OPTIMAL */
void calculate_min_path(int id, int q) {
    min_path_between_id_to_centroid.clear();
    clock_t start;
    start = clock();

    ResultDijkstra _res = dijsktra_pq(id, graph);
    while (true) {
        min_path_between_id_to_centroid.push_back(q);
        q = _res.parents[q];

        if (q == id) {
            min_path_between_id_to_centroid.push_back(q);
            break;
        }
    }

    cout << "Find Path id to Centroid Time: " << 1000.0 * (double)(clock() - start)/(double)CLOCKS_PER_SEC << " ms.\n";
}

/* =========================== CREATE LIST ADJACENCY =================================== */

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
        file_out << "\n";
    }
    file_out.close();
}

/* =========================== CREATE DIJSKTRA MATRIX =================================== */
/*
const int T = 470246;
int graphDijkstra[T][1000];

void create_dijktra_file() {

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
}
*/

#endif // PATH_H_INCLUDED
