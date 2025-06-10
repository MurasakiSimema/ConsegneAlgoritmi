#include <iostream>
#include <fstream>
#include <string>


std::string* file1;
int size1;
std::string* file2;
int size2;

void calculateGraph(int* graph, int& g); {
  int g1 = 0;
  int g2 = 0;
  int* graph1 = new int[size1 / 2];
  int* graph2 = new int[size2 / 2];
  findGraph(graph1, graph2, g1, g2);

  int* graph = new int[g2];

  int i;
  for (i = 0; i < g1 && i < g2; ++i) {
    if (graph1[i] != graph2[i]) {

    }
  }
}

void findGraph(int* graph1, int* graph2, int& g1, int& g2) {
  int* open = new int[size1 / 2];
  int openGraph = 0;


  for (int i = 0; i < size1; ++i) {
    if (file1[i].find("{")) {
      ++openGraph;
      open[openGraph - 1] = i;
    }
    else if (file1[i].find("}")) {
      if (openGraph > 0) {
        --openGraph;
        graph1[g1++] = open[openGraph] - i;
      }
    }
  }

  for (int i = 0; i < size2; ++i) {
    if (file2[i].find("{")) {
      ++openGraph;
      open[openGraph - 1] = i;
    }
    else if (file2[i].find("}")) {
      if (openGraph > 0) {
        --openGraph;
        graph2[g2++] = open[openGraph] - i;
      }
    }
  }
}