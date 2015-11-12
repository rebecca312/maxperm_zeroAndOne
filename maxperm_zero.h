#pragma once
#include <igraph.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include<iostream>
#include<string.h>
using namespace std;
double Perm(igraph_t * g, igraph_vector_t *neig_v, int * vertex_community, int v);
double igraph_community_MaxPerm(igraph_t * g, igraph_vector_t * membership, int maxIt);
