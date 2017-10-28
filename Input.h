#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "Commom.h"

int Nnodes, ID_1 = -1, ID_2 = -1;
int type = -1;

void initializeRead() {

    /* Select the size of nodes */
    printf("Number or Nodes:");
    printf("\n 1. 100 Nodes");
    printf("\n 2. 1 000 Nodes");
    printf("\n 3. 2 000 Nodes");
    printf("\n 4. 5 000 Nodes");
    printf("\n 5. 10 000 Nodes");
    printf("\n 6. 1 000 000 Nodes");
    printf("\n 7. 5 000 000 Nodes");
    printf("\n 8. 10 000 000 Nodes");
    while (Nnodes < 1 || Nnodes > 8){
        printf("\nEnter a number:\t");
        scanf("%d", &Nnodes);
    }

    /* Select type: Serial or Parallel */
    printf("\nEnter: Serial (0) or Parallel (1)");
    while (type != 0 && type != 1) {
        printf("\nEnter a number\n");
        scanf("%d", &type);
    }

    /* Select ids */
    printf("\nEnter two ids in the range:");
    if (Nnodes == 1) {
        printf(" [0 - 118]");
        while (ID_1 < 0 || ID_1 > 118 || ID_2 < 0 || ID_2 > 118){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 2) {
        printf(" [0 - 1018]");
        while (ID_1 < 0 || ID_1 > 1018 || ID_2 < 0 || ID_2 > 1018){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if(Nnodes == 3) {
        printf(" [0 - 2015]");
        while (ID_1 < 0 || ID_1 > 2015 || ID_2 < 0 || ID_2 > 2015){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 4) {
        printf(" [0 - 5001]");
        while (ID_1 < 0 || ID_1 > 5001 || ID_2 < 0 || ID_2 > 5001){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 5) {
        printf(" [0 - 9952]");
        while (ID_1 < 0 || ID_1 > 9952 || ID_2 < 0 || ID_2 > 9952){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 6) {
        printf(" [0 - 999999]");
        while (ID_1 < 0 || ID_1 > 999999 || ID_2 < 0 || ID_2 > 999999){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 7) {
        printf(" [0 - 4999999]");
        while (ID_1 < 0 || ID_1 > 4999999 || ID_2 < 0 || ID_2 > 4999999){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }
    else if (Nnodes == 8) {
        printf(" [0 - 9999999]");
        while (ID_1 < 0 || ID_1 > 9999999 || ID_2 < 0 || ID_2 > 9999999){
            printf("\nFirst id: ");
            scanf("%d", &ID_1);
            printf("\Second id: ");
            scanf("%d", &ID_2);
        }
    }

    ifstream myFile;
    if (Nnodes == 1)
        myFile.open("/home/wilsan/_ext/myGraphs/100points.data");
        //myFile.open("D:/Graph_OpenGL/data/100points.data");
    else if (Nnodes == 2)
        myFile.open("D:/Graph_OpenGL/data/1000points.data");
    else if(Nnodes == 3)
        myFile.open("D:/Graph_OpenGL/data/2000points.data");
    else if (Nnodes == 4)
        myFile.open("D:/Graph_OpenGL/data/5000points.data");
    else if (Nnodes == 5)
        myFile.open("/home/wilsan/_ext/myGraphs/10000points.data");
    else if (Nnodes == 6)
        myFile.open("/home/wilsan/_ext/myGraphs/1000000points.data");
    else if (Nnodes == 7)
        myFile.open("D:/Graph_OpenGL/data/n_data_5000000.dat");
    else if (Nnodes == 8)
        myFile.open("D:/Graph_OpenGL/data/m_data_10000000.data");

    if (myFile.fail()){
        cerr << "The file was not found" << endl;
        return;
    }

    int n_nodes, n_edges, x, y, id1, id2;
    string name1, name2;

    myFile >> name1;
    myFile >> n_nodes;

    clock_t read_node;
    read_node = clock();
    for (int i = 0; i < n_nodes; i++){
        myFile >> x >> y;
        graph.InsertNode(x, y, i);
    }
    cout << "\nRead nodes Time: " << 1000.0 * (double)(clock() - read_node)/(double)CLOCKS_PER_SEC << " ms.\n";

    myFile >> name2;
    myFile >> n_edges;

    clock_t read_edge;
    read_edge = clock();
    for (int i = 0; i < n_edges; i++){
        myFile >> id1 >> id2 ;
        graph.InsertEdge(id1,id2);
    }
    cout << "\nRead edges Time: " << 1000.0 * (double)(clock() - read_edge)/(double)CLOCKS_PER_SEC << " ms.\n";

    if (graph.m_edges->at(ID_1) == nullptr) {
        printf("\nId 1 es un vértice aislado, seleccione otro\n");
        //exit(1);
    }

    if (graph.m_edges->at(ID_2) == nullptr) {
        printf("\nId 2 es un vértice aislado, seleccione otro\n");
        //exit(1);
    }
}

#endif // INPUT_H_INCLUDED
