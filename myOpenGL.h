#ifndef MYOPENGL_H_INCLUDED
#define MYOPENGL_H_INCLUDED

#include "Path.h"
#include "Input.h"
#include "Commom.h"

#define DISPLAC 4
#define PI 3.141592
#define SIZE_EDGE 0.3
#define MAIN_EDGE 2.8
#define AUX_EDGE 1.6
#define CENTER_1 5
#define CENTER_2 15
#define PARAL_NOD 100               //100
#define RADIUS_PARAL 100

int DISPLAC2 = 50;
/* ==================================    OpenGL    ========================================= */

void start() {
    glMatrixMode(GL_PROJECTION);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    if (Nnodes == 6 || Nnodes == 7 || Nnodes == 8) gluOrtho2D(-10, 20000, -10, 20000);
    else gluOrtho2D(-10, 910 , -10, 910);
}

/* Draw regular quadrants of width 250 */
void draw_quadrant(){
    glColor4f(0.8, 0.0, 0.0, 0.01);
    if (Nnodes == 6 || Nnodes == 7 || Nnodes == 8) {
        glBegin(GL_LINES);
            glVertex2f(5000,-10); glVertex2f(5000,20000);
            glVertex2f(10000,-10); glVertex2f(10000,20000);
            glVertex2f(15000,-10); glVertex2f(15000,20000);
            glVertex2f(-10,5000); glVertex2f(20000,5000);
            glVertex2f(-10,10000); glVertex2f(20000,10000);
            glVertex2f(-10,15000); glVertex2f(20000,15000);
        glEnd();
    }
    else {
        glBegin(GL_LINES);
            glVertex2f(230,-10); glVertex2f(230,920);
            glVertex2f(460,-10); glVertex2f(460,920);
            glVertex2f(690,-10); glVertex2f(690,920);
            glVertex2f(-10,230); glVertex2f(920,230);
            glVertex2f(-10,460); glVertex2f(920,460);
            glVertex2f(-10,690); glVertex2f(920,690);
        glEnd();
    }
    glFlush();
}

/* Draw edges by each vertex */
void edges(int x0, int y0, int i){
    glColor3f(0.2, 0, 0.92);
    Edge<int>* ptr_e = graph.m_edges->at(i);
    vector<Node<int>>* ptr_n = graph.m_nodes;
    int id_edges;

    while (ptr_e != nullptr) {
        id_edges = ptr_e->id_dst;
        glLineWidth((GLfloat)SIZE_EDGE);
        glBegin(GL_LINES);
            glVertex2f(x0,y0);
            glVertex2f(ptr_n->at(id_edges).m_data[0],ptr_n->at(id_edges).m_data[1]);
        glEnd();
        ptr_e = ptr_e->next_edge;
    }
    glLineWidth(1.0f);
    glFlush();
}

/* Function that call drawCircles by each vertex */
void draw_edges() {
    vector<Node<int>>* ptr = graph.m_nodes;
    int size = graph.Gsize;
    for (int i = 0; i < size; i++) {
        edges(ptr->at(i).m_data[0], ptr->at(i).m_data[1], i);
    }
}

/* Draw circles of radius 4.0 */
void circles(int cx, int cy, float radius) {
    glBegin(GL_POINTS);
    float calx, caly;
    for (double i = 0.0; i < 2*PI; i+=0.01) {
        calx = radius * cos(i);
        caly = radius * sin(i);
        glVertex2f(cx + calx, cy + caly);
    }
    glEnd();
    glFlush();
}

/* Function that call circles by each vertex */
void draw_vertex() {
    float radius = 0.5;
    glColor3f(0, 0.94, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < graph.Gsize; i++) {
        if (graph.m_edges->at(i) != nullptr) {
            glVertex2i(graph.m_nodes->at(i).m_data[0],graph.m_nodes->at(i).m_data[1]);
        }
    }
    glEnd();
    glFlush();
}

/* Function to draw two circles to parallel */
void draw_circles_to_parallel() {
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);
    int id_center1 = centroids[CENTER_1];
    int id_center2 = centroids[CENTER_2];
    glColor3f(0.8, 0.4, 0);
    glLineWidth((GLfloat)MAIN_EDGE);
    circles(graph.m_nodes->at(id_center1).m_data[0], graph.m_nodes->at(id_center1).m_data[1], RADIUS_PARAL);
    circles(graph.m_nodes->at(id_center2).m_data[0], graph.m_nodes->at(id_center2).m_data[1], RADIUS_PARAL);
    glLineWidth(1.0f);
}

/* Function to draw vertex (nodes) to the parallel search */
void draw_nodes_to_parallel(vector<int>& nodes_1, vector<int>& nodes_2) {
    vector<int> centroids(SIZE_GRID);
    centroids = select_centroids(centroids);

    int id_center1 = centroids[CENTER_1];
    int id_center2 = centroids[CENTER_2];
    int x0 = graph.m_nodes->at(id_center1).m_data[0];
    int y0 = graph.m_nodes->at(id_center1).m_data[1];
    int x1 = graph.m_nodes->at(id_center2).m_data[0];
    int y1 = graph.m_nodes->at(id_center2).m_data[1];
    int cont = 0;
    for (int i = 0; i < graph.Gsize; i++) {
        if (euclidean_d(x0, y0, graph.m_nodes->at(i).m_data[0], graph.m_nodes->at(i).m_data[1]) < RADIUS_PARAL) {
            nodes_1[cont] = i;
            cont++;
        }
        if (cont == PARAL_NOD) break;
    }

    cont = 0;
    for (int i = 0; i < graph.Gsize; i++) {
        if (euclidean_d(x1, y1, graph.m_nodes->at(i).m_data[0], graph.m_nodes->at(i).m_data[1]) < RADIUS_PARAL) {
            nodes_2[cont] = i;
            cont++;
        }
        if (cont == PARAL_NOD) break;
    }

    /* Print the nodes */
    float radius = 1.6;
    glColor3f(1, 0, 0);
    for (int i = 0; i < PARAL_NOD; i++) {
        circles(graph.m_nodes->at(nodes_1[i]).m_data[0], graph.m_nodes->at(nodes_1[i]).m_data[1], radius);
        circles(graph.m_nodes->at(nodes_2[i]).m_data[0], graph.m_nodes->at(nodes_2[i]).m_data[1], radius);
    }
}

/* Calculate the distance between two points */
float euclidian(float x1, float y1, float x2, float y2){
    float dist;
    dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return dist;
}

/* show Id */
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos3f(x, y, 0);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

/*  Shows the ids of the centroids */
void draw_centroids() {
    vector<int> centroids(SIZE_GRID);
    if (Nnodes == 1) centroids = centroids_100;
    if (Nnodes == 2) centroids = centroids_1000;
    if (Nnodes == 3) centroids = centroids_2000;
    if (Nnodes == 4) centroids = centroids_5000;
    if (Nnodes == 5) centroids = centroids_10000;
    if (Nnodes == 6) centroids = centroids_1000000;
    if (Nnodes == 7) centroids = centroids_5000000;
    if (Nnodes == 8) centroids = centroids_10000000;
    int aux;
    if (Nnodes != 7 && Nnodes != 8 && Nnodes != 6) DISPLAC2 = 3;
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    for (int i = 0; i < centroids.size(); i++) {
        aux = centroids[i];
        glVertex2i(graph.m_nodes->at(aux).m_data[0]-DISPLAC2,graph.m_nodes->at(aux).m_data[1]-DISPLAC2);
        glVertex2i(graph.m_nodes->at(aux).m_data[0]-DISPLAC2,graph.m_nodes->at(aux).m_data[1]+DISPLAC2);
        glVertex2i(graph.m_nodes->at(aux).m_data[0]+DISPLAC2,graph.m_nodes->at(aux).m_data[1]+DISPLAC2);
        glVertex2i(graph.m_nodes->at(aux).m_data[0]+DISPLAC2,graph.m_nodes->at(aux).m_data[1]-DISPLAC2);
    }
    glEnd();
    glFlush();
}

/*  Shows the ids of the Ids selected */
void draw_ids(){
    glColor4f(1, 1, 1, 1);
    void * font = GLUT_BITMAP_HELVETICA_18;
    int k1 = ID_1, k2 = ID_2;

    /* To id 1 */
    if (k1 == ID_1) {
        string res;
        stringstream convert;
        convert << k1;
        res = convert.str();
        string temp_str = convert.str();
        char* char_t = (char*)temp_str.c_str();
        renderBitmapString(graph.m_nodes->at(k1).m_data[0]+4, graph.m_nodes->at(k1).m_data[1]+2, (void *)font, char_t);
    }

    glColor3f(0.8, 0.2, 0.7);                       // Color of the square: light blue
    glBegin(GL_QUADS);                              // To drawn a square
        glVertex2i(graph.m_nodes->at(k1).m_data[0]-DISPLAC,graph.m_nodes->at(k1).m_data[1]-DISPLAC);
        glVertex2i(graph.m_nodes->at(k1).m_data[0]-DISPLAC,graph.m_nodes->at(k1).m_data[1]+DISPLAC);
        glVertex2i(graph.m_nodes->at(k1).m_data[0]+DISPLAC,graph.m_nodes->at(k1).m_data[1]+DISPLAC);
        glVertex2i(graph.m_nodes->at(k1).m_data[0]+DISPLAC,graph.m_nodes->at(k1).m_data[1]-DISPLAC);
    glEnd();

    /* To id 2 */
    glColor4f(1, 1, 1, 1);
    if (k2 == ID_2) {
        string res;
        stringstream convert;
        convert << k2;
        res = convert.str();
        string temp_str = convert.str();
        char* char_t = (char*)temp_str.c_str();
        renderBitmapString(graph.m_nodes->at(k2).m_data[0]+4, graph.m_nodes->at(k2).m_data[1]+2, (void *)font, char_t);
    }

    glColor3f(0.8, 0.2, 0.7);
    glBegin(GL_QUADS);                              // To drawn a square
        glVertex2i(graph.m_nodes->at(k2).m_data[0]-DISPLAC,graph.m_nodes->at(k2).m_data[1]-DISPLAC);
        glVertex2i(graph.m_nodes->at(k2).m_data[0]-DISPLAC,graph.m_nodes->at(k2).m_data[1]+DISPLAC);
        glVertex2i(graph.m_nodes->at(k2).m_data[0]+DISPLAC,graph.m_nodes->at(k2).m_data[1]+DISPLAC);
        glVertex2i(graph.m_nodes->at(k2).m_data[0]+DISPLAC,graph.m_nodes->at(k2).m_data[1]-DISPLAC);
    glEnd();
    glFlush();

}

/* Find the centroids of each quadrant */
void centroids(int x0, int x1, int y0, int y1) {
    float cent_x = 0, cent_y = 0;
    int X, Y, cont = 0;
    vector<Node<int>>* ptr = graph.m_nodes;
    vector<Node<int>>* nodes_quad = new vector<Node<int>>;
    for (int i = 0; i < graph.Gsize; i++) {
        X = ptr->at(i).m_data[0];
        Y = ptr->at(i).m_data[1];
        if (X >= x0 && X < x1 && Y > y0 && Y <= y1) {
            nodes_quad->push_back(ptr->at(i));
            cent_x += X;
            cent_y += Y;
            cont++;
        }
    }

    cent_x /= cont;
    cent_y /= cont;

    /* Find the closest vertex to the centroid */
    int sizeV = nodes_quad->size();
    vector<float> distances;
    vector<Node<int>>* nodes_quad_with_edges = new vector<Node<int>>;
    for (int i = 0; i < sizeV; i++) {
        if (graph.hasEdge(nodes_quad->at(i).id)) {
            nodes_quad_with_edges->push_back(nodes_quad->at(i));
            distances.push_back(euclidian(cent_x, cent_y, nodes_quad->at(i).m_data[0], nodes_quad->at(i).m_data[1]));
        }
    }

    /* Find the minimum distance */
    int k = 0;
    float min_dist = distances[0];
    for(int i = 1; i < distances.size(); i++) {
        if (distances[i] <= min_dist) {
            min_dist = distances[i];
            k = i;
        }
    }

    Node<int> newNode = nodes_quad_with_edges->at(k);
    graphCentroids.InsertNode(newNode.m_data[0], newNode.m_data[1], newNode.id);
    cout << "id: " << newNode.id << endl;

    delete nodes_quad;
    delete nodes_quad_with_edges;
}

/* Call centroids SIZE_GRID times */
void calculateCentroids() {
/*
    centroids(0,230,690,920);                   //I  Q
    centroids(230,460,690,920);
    centroids(460,690,690,920);
    centroids(690,921,690,920);                 // IV  Q

    centroids(0,230,460,690);
    centroids(230,460,460,690);
    centroids(460,690,460,690);
    centroids(690,921,460,690);                 // VIII  Q

    centroids(0,230,230,460);
    centroids(230,460,230,460);                 // ..
    centroids(460,690,230,460);
    centroids(690,921,230,460);                 // XII  Q

    centroids(0,230,-1,230);
    centroids(230,460,-1,230);
    centroids(460,690,-1,230);
    centroids(690,921,-1,230);                   // XVI  Q



    centroids(0,5000,15000,20000);                   //I  Q
    centroids(5000,10000,15000,20000);
    centroids(10000,15000,15000,20000);
    centroids(15000,20001,15000,20000);                 // IV  Q

    centroids(0,5000,10000,15000);
    centroids(5000,10000,10000,15000);
    centroids(10000,15000,10000,15000);
    centroids(15000,20001,10000,15000);                 // VIII  Q

    centroids(0,5000,5000,10000);
    centroids(5000,10000,5000,10000);                 // ..
    centroids(10000,15000,5000,10000);
    centroids(15000,20001,5000,10000);                 // XII  Q

    centroids(0,5000,-1,5000);
    centroids(5000,10000,-1,5000);
    centroids(10000,15000,-1,5000);
    centroids(15000,20001,-1,5000);


    centroids(0,1250,3750,5000);                   //I  Q
    centroids(1250,2500,3750,5000);
    centroids(2500,3750,3750,5000);
    centroids(3750,5001,3750,5000);                 // IV  Q

    centroids(0,1250,2500,3750);
    centroids(1250,2500,2500,3750);
    centroids(2500,3750,2500,3750);
    centroids(3750,5001,2500,3750);                 // VIII  Q

    centroids(0,1250,1250,2500);
    centroids(1250,2500,1250,2500);                 // ..
    centroids(2500,3750,1250,2500);
    centroids(3750,5001,1250,2500);                 // XII  Q

    centroids(0,1250,-1,1250);
    centroids(1250,2500,-1,1250);
    centroids(2500,3750,-1,1250);
    centroids(3750,5001,-1,1250);
*/
}

/* Show the path between two selected ids (for the moment only centroids) */
void draw_shortest_path(int id1, int id2) {
    if (Nnodes == 6 || Nnodes == 7 || Nnodes == 8) return;
    glColor3f(1, 1, 1);
    vector<Node<int>>* nodes = graph.m_nodes;
    int q1 = -1, q2 = -1;                                              // Quadrants
    q1 = num_quadr(id1);
    q2 = num_quadr(id2);

    /* Case N 1 - Both are Centroids */
    if (q1 != -1 && q2 != -1) {
        int num_paths = short_path_centroids[q1][q2].n_elements;
        int aux1, aux2;
        for (int i = 0; i < num_paths; i++){
            aux1 = short_path_centroids[q1][q2].ids[i];
            aux2 = short_path_centroids[q1][q2].ids[i+1];
            glLineWidth((GLfloat)MAIN_EDGE);
            glBegin(GL_LINES);
            glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
            glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
            glEnd();

        }
        glLineWidth(1.0f);
    }

    /* Case N 2 - id2 is a centroid */
    else if (q1 == -1 && q2 != -1) {
        /* Calculate in which quadrant is q1 */
        q1 = quadrant_of_centroid_near(id1);

        if (in_path(id1, q1, q2)) {
            int k = 0;
            while(id1 != short_path_centroids[q1][q2].ids[k]) {
                k++;
            }

            int num_paths = short_path_centroids[q1][q2].n_elements;
            int aux1, aux2;
            for (int i = k; i < num_paths; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
        }

        else if (in_path(id1, q2, q1)) {
            int k = 0;
            while(id1 != short_path_centroids[q2][q1].ids[k]) {
                k++;
            }
            int num_paths = short_path_centroids[q2][q1].n_elements;
            int aux1, aux2;
            for (int i = 0; i < k; i++){
                aux1 = short_path_centroids[q2][q1].ids[i];
                aux2 = short_path_centroids[q2][q1].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
        }
        else {
            min_path_to_centroid(id1, nodes, q1);

            /* Path: from centroid of q1 to centroid of q2 */
            int num_paths = short_path_centroids[q1][q2].n_elements;
            int aux1, aux2;
            glColor3f(1, 1, 1);
            for (int i = 0; i < num_paths; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
            glLineWidth(1.0f);

            /* Path: from id1 to centroid of q1 */
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);                          // OTHER COLOR
                glBegin(GL_LINES);
                glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);
        }

    }

    /* Case N 3 - id1 is a centroid */
    else if (q1 != -1 && q2 == -1) {
        /* Calculate in which quadrant is q2 */
        q2 = quadrant_of_centroid_near(id2);

        if (in_path(id2, q2, q1)) {
            int k = 0;
            while(id2 != short_path_centroids[q2][q1].ids[k]) {
                k++;
            }
            int num_paths = short_path_centroids[q2][q1].n_elements;
            int aux1, aux2;
            for (int i = k; i < num_paths; i++){
                aux1 = short_path_centroids[q2][q1].ids[i];
                aux2 = short_path_centroids[q2][q1].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
        }
        else if (in_path(id2, q1, q2)) {
            int k = 0;
            while(id2 != short_path_centroids[q1][q2].ids[k]) {
                k++;
            }
            int num_paths = short_path_centroids[q1][q2].n_elements;
            int aux1, aux2;
            for (int i = 0; i < k; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
        }
        else {
            min_path_to_centroid(id2, nodes, q2);

            /* Path: from centroid of q1 to centroid of q2 */
            int num_paths = short_path_centroids[q1][q2].n_elements;
            int aux1, aux2;
            glColor3f(1, 1, 1);
            for (int i = 0; i < num_paths; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
            glLineWidth(1.0f);

            /* Path: from id2 to centroid 2 */
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);                          // OTHER COLOR
                glBegin(GL_LINES);
                glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
        }

        glLineWidth(1.0f);
    }

    /* Case N 4 - There is not Centroids */
    else {
        q1 = quadrant_of_centroid_near(id1);
        q2 = quadrant_of_centroid_near(id2);

        /* id2 belongs q2->q1 and id1 belongs q2->q1 */
        if (in_path(id2, q2, q1) == true && in_path(id1, q2, q1) == true) {
            int k = 0, m = 0;
            while(id2 != short_path_centroids[q2][q1].ids[k]) k++;
            while(id1 != short_path_centroids[q2][q1].ids[m]) m++;
            int aux1, aux2;
            glColor3f(1, 1, 1);
            for (int i = k; i < m; i++){
                aux1 = short_path_centroids[q2][q1].ids[i];
                aux2 = short_path_centroids[q2][q1].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);
        }

        /* id1 belongs q1->q2 and id2 belongs q1->q2 */
        else if (in_path(id1, q1, q2) == true && in_path(id2, q1, q2) == true) {
            int k = 0, m = 0;
            while(id2 != short_path_centroids[q1][q2].ids[k]) k++;
            while(id1 != short_path_centroids[q1][q2].ids[m]) m++;
            int aux1, aux2;
            glColor3f(1, 1, 1);
            for (int i = m; i < k; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

        }

        /* id2 belongs q2->q1 and id1 not belongs q2->q1 */
        else if (in_path(id2, q2, q1) == true && in_path(id1, q2, q1) == false) {
            int k = 0;
            while(id2 != short_path_centroids[q2][q1].ids[k]) k++;
            int aux1, aux2;
            int num_paths = short_path_centroids[q2][q1].n_elements;
            glColor3f(1, 1, 1);
            for (int i = k; i < num_paths; i++){
                aux1 = short_path_centroids[q2][q1].ids[i];
                aux2 = short_path_centroids[q2][q1].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

            /* Path: id1 to centroid of q1 */
            min_path_to_centroid(id1, nodes, q1);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                    glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

        }

        /* id1 belongs q1->q2 and id2 not belongs q1->q2 */
        else if (in_path(id1, q1, q2) == true && in_path(id2, q1, q2) == false) {
            int m = 0;
            while(id1 != short_path_centroids[q1][q2].ids[m]) m++;
            int aux1, aux2;
            int num_paths = short_path_centroids[q1][q2].n_elements;
            glColor3f(1, 1, 1);
            for (int i = m; i < num_paths; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

            /* Path: id2 to centroid of q2 */
            min_path_to_centroid(id2, nodes, q2);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                    glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

        }

        /* id2 belongs q1->q2 but id1 not belongs q1->q2 */
        else if (in_path(id2, q1, q2) == true && in_path(id1, q1, q2) == false) {
            int k = 0;
            while(id2 != short_path_centroids[q1][q2].ids[k]) k++;
            int aux1, aux2;
            int num_paths = short_path_centroids[q1][q2].n_elements;
            glColor3f(1, 1, 1);
            for (int i = 0; i < k; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

           /* Path: id1 to centroid of q1 */
            min_path_to_centroid(id1, nodes, q1);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                    glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

        }

        /* id1 belongs q2->q1 but id2 not belongs q2->q1 */
        else if (in_path(id1, q2, q1) == true && in_path(id2, q2, q1) == false) {
            int m = 0;
            while(id1 != short_path_centroids[q2][q1].ids[m]) m++;
            int aux1, aux2;
            int num_paths = short_path_centroids[q2][q1].n_elements;
            glColor3f(1, 1, 1);
            for (int i = 0; i < m; i++){
                aux1 = short_path_centroids[q2][q1].ids[i];
                aux2 = short_path_centroids[q2][q1].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                    glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

           /* Path: id2 to centroid of q2 */
            min_path_to_centroid(id2, nodes, q2);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);
                glBegin(GL_LINES);
                    glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                    glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

        }

        /* id1 not belongs {q1->q2, q2->q1} and id2 not belongs {q1->q2, q2->q1}*/
        else {
            /* Path: id2 to centroid of q2 */
            min_path_to_centroid(id2, nodes, q2);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);                          // OTHER COLOR
                glBegin(GL_LINES);
                glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

            /* Path: id1 to centroid of q1 */
            min_path_to_centroid(id1, nodes, q1);
            glColor3f(0.953, 0.925, 0.153);
            for (int i = 0; i < short_path_id.size() -1; i++){
                glLineWidth((GLfloat)AUX_EDGE);                          // OTHER COLOR
                glBegin(GL_LINES);
                glVertex2f(nodes->at(short_path_id[i]).m_data[0], nodes->at(short_path_id[i]).m_data[1]);
                glVertex2f(nodes->at(short_path_id[i+1]).m_data[0], nodes->at(short_path_id[i+1]).m_data[1]);
                glEnd();
            }
            glLineWidth(1.0f);

            /* Path: from centroid of q1 to centroid of q2 */
            int num_paths = short_path_centroids[q1][q2].n_elements;
            int aux1, aux2;
            glColor3f(1, 1, 1);
            for (int i = 0; i < num_paths; i++){
                aux1 = short_path_centroids[q1][q2].ids[i];
                aux2 = short_path_centroids[q1][q2].ids[i+1];
                glLineWidth((GLfloat)MAIN_EDGE);
                glBegin(GL_LINES);
                glVertex2f(nodes->at(aux1).m_data[0], nodes->at(aux1).m_data[1]);
                glVertex2f(nodes->at(aux2).m_data[0], nodes->at(aux2).m_data[1]);
                glEnd();

            }
            glLineWidth(1.0f);
        }
    }
}

void screen() {
    /* Draw Quadrants, Edges (lines) , Vertex (circles) and Ids (start - end) */
    clock_t start, end;
    start = clock();
    if (Nnodes != 6 && Nnodes != 7 && Nnodes != 8) draw_edges();
    draw_vertex();
    draw_quadrant();
    draw_centroids();
    draw_ids();
    cout << "\nDraw Time: " << 1000.0 * (double)(clock() - start)/(double)CLOCKS_PER_SEC << " ms.\n";

    /* Draw the shortest path between two any ids - and calculate the time*/
    start = clock();
    if (type == 0) {
        draw_shortest_path(ID_1, ID_2);
        cout << "Find the path Serial Time: " << 1000.0 * (double)(clock() - start)/(double)CLOCKS_PER_SEC << " ms.\n";
    }
    else if (type = 1 && Nnodes != 7 && Nnodes != 6) {
        vector<int> nodes_1(PARAL_NOD);
        vector<int> nodes_2(PARAL_NOD);

        draw_circles_to_parallel();
        draw_nodes_to_parallel(nodes_1, nodes_2);

        for (int i = 0; i < PARAL_NOD; i++) {
            draw_shortest_path(nodes_1[i], nodes_2[i]);
        }
        cout << "Find the path parallel Time: " << 1000.0 * (double)(clock() - start)/(double)CLOCKS_PER_SEC << " ms.\n";
    }
    glFlush();

}

 void initializeOpenGL(){
    glutInitWindowSize(1100,1000);                              // The dimension of the windows
    glutInitWindowPosition(0,0);
    glutCreateWindow("Graph");                                  // Create the windows with a tittle
    start();
    glutDisplayFunc(screen);                                    // Call the function  "screen"
    glutMainLoop();                                             // Until we close the windows
 }

#endif // MYOPENGL_H_INCLUDED
