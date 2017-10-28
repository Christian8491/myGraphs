#ifndef CGRAPH_H_INCLUDED
#define CGRAPH_H_INCLUDED

#include <cmath>

/* ================================== Graph Implementation ================================= */

template <typename N>
struct Edge;

template <typename N>
struct Node {
    int id;
    int parentId;
    N m_data[2];
    Node(int id_, N val1, N vale2);
};

template <typename N>
Node<N>::Node(int id_, N val1, N vale2) {
    id = id_;
    parentId = -1;
    m_data[0] = val1;
    m_data[1] = vale2;
}

template <typename N>
struct Edge {
    int id_dst;
    float cost;
    Edge<N>* next_edge;
    Edge(int id_){id_dst = id_; next_edge = nullptr;}
};

template <typename N>
class CGraph {
public:
    int Gsize;
    vector<Node<N>>* m_nodes;
    vector<Edge<N>*>* m_edges;
    bool m_dir = true;

public:
    CGraph();
    bool FindNode(int id_);
    bool FindNode(Node<N> node);
    void InsertNode(N valX, N valY, N id_ = -1);

    bool FindEdge(N id1, N id2);
    bool FindEdge(N srcX, N srcY, N dstX, N dstY, Node<N>*& p);
    void InsertEdge(N id1, N id2);
    void InsertEdge(N srcX, N srcY, N dstX, N dstY);

    bool hasEdge(N id);
    void RemoveNode();
    void RemoveEdge();
    int getGsize(){ return Gsize; }

    void printGraph();
};

/* Constructor of Graph */
template <typename N>
CGraph<N>::CGraph(){
    m_nodes = new vector<Node<N>>;
    m_edges = new vector<Edge<N>*>;
    Gsize = 0;
}

/* Search a node by ID and return true if find it, else return false */
template <typename N>
bool CGraph<N>::FindNode(int id_){
      if (id_ >= 0 && id_ < Gsize) return true;
      else return false;
}

/* Search a node by coordinates and return true if find it, else return false */
template <typename N>
bool CGraph<N>::FindNode(Node<N> node){
    typename vector<Node<N>>::iterator It;
    for (It = (*m_nodes).begin(); It != (*m_nodes).end(); ++It){
        if (node.m_data[0] == (*It).m_data[0] && node.m_data[1] == (*It).m_data[1] ) return true;
    }
    return false;
}

/* Insert a node into the Graph with two coordinates, always it order by ID */
template <typename N>
void CGraph<N>::InsertNode(N valX, N valY, N id_){
    //if (id_ <= -1 || id_ > Gsize) id_ = Gsize;
    //if (id_ < Gsize) return;                                      // Id is already exist

    Node<N>* newNode = new Node<N> (id_, valX, valY);
    /*
    if (FindNode(*newNode)) {
        delete[] newNode;
        return;
    }
    */
    newNode->id = id_;
    (*m_nodes).push_back(*newNode);
    (*m_edges).push_back(nullptr);
    Gsize++;
}

/* Search a specific edge by IDs and return true if find it, else return false */
template <typename N>
bool CGraph<N>::FindEdge(N id1, N id2){
    Edge<N>* ptr_edge = m_edges->at(id1);   // this is a pointer to "LIST"

    while(ptr_edge != nullptr && ptr_edge->id_dst != id2) {
        ptr_edge = ptr_edge->next_edge;
    }
    if (ptr_edge != nullptr && ptr_edge->id_dst == id2) return true;
    return false;
}


/* Insert a edge by IDs at the end of the Node */
template <typename N>
void CGraph<N>::InsertEdge(N id1, N id2){
    //if (id1 >= Gsize || id2 >= Gsize) return;
    Edge<N>* temp;

    //if (FindEdge(id1, id2)) return;
    /* on id1 */
    temp = m_edges->at(id1);
    Edge<N>* newEdge = new Edge<N>(id2);
    float dx = ( *m_nodes )[id1].m_data[0] - ( *m_nodes )[id2].m_data[0];
    float dy = ( *m_nodes )[id1].m_data[1] - ( *m_nodes )[id2].m_data[1];
    newEdge->cost = sqrt( dx * dx + dy * dy );
    newEdge->next_edge = temp;
    m_edges->at(id1) = newEdge;

    /* on id2 */
    //if (FindEdge(id2, id1)) return;
    temp = m_edges->at(id2);
    Edge<N>* newEdge2 = new Edge<N> (id1);
    newEdge2->cost = newEdge->cost;
    newEdge2->next_edge = temp;
    m_edges->at(id2) = newEdge2;

    //delete temp;
}

/* Insert a edge by coordinates at the end of the Node */
template <typename N>
void CGraph<N>::InsertEdge(N srcX, N srcY, N dstX, N dstY){
    Node<N>* p;
}


/* Has edges ? */
template <typename N>
bool CGraph<N>::hasEdge(N id_){
    if (m_edges->at(id_) == nullptr) return false;
    return true;
}

template <typename N>
void CGraph<N>::RemoveNode(){

}

template <typename N>
void CGraph<N>::RemoveEdge(){

}

/* Print vertex of the Graph and its edges */
template <typename N>
void CGraph<N>::printGraph() {
    typename vector<Node<N>>::iterator ItVect = (*m_nodes).begin();
    for (ItVect; ItVect != (*m_nodes).end(); ++ItVect) {
        cout <<"\n Adjacency list of vertex ("<< (*ItVect).m_data[0] << "," << (*ItVect).m_data[1]  <<
        ") - Id: " << (*ItVect).id << "\n->";

        Edge<N>* ptr = m_edges->at((*ItVect).id);   // this is a pointer to "LIST"
        while(ptr != nullptr) {
            cout << " " << ptr->id_dst << " -> ";
            ptr = ptr->next_edge;
        }
        cout << endl;
    }
}


CGraph<int> graph;
CGraph<int> graphCentroids;

#endif // CGRAPH_H_INCLUDED
