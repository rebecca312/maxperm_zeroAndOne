#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include<iostream>
#include<fstream>
#include<math.h>
#include "maxperm_zero.h"
using namespace std;


void create_graph(igraph_t * g, string input)
{
	int i, nVertices=128, nEdges=0, a, b, wt;
	
	//scanf("%d %d",&nVertices, &nEdges);
	ifstream in(input.c_str());
	double A[128][128];
	int t = 0;

	
	for (int i = 0; i < nVertices; i++)
	{
		double num;
		if (i)
		{
			in>>num;

		}
		for (int j = 0; j < nVertices; j++)
		{
			
			in>>num;
			if(abs(num) <= 0.1)
			{
				A[i][j] = 0;
			}
			else
			{
				nEdges++;
				A[i][j] = 1;
				
			}
		}
		
	}
	
	//nEdges = nEdges / 2;
	
    //cout<<nEdges<<endl;
	igraph_vector_t v;
	igraph_vector_init (&v, 2*nEdges);//Every vector needs to be initialized before it can be used, and there are a number of initialization functions
                                          //or otherwise called constructors. This function constructs a vector of the given size and initializes each entry to 0
	

	/*for(i=0;i<2*nEdges;i+=2)
	{
		//scanf("%d %d",&a,&b);
		if(a>=nVertices || b>=nVertices)
		{
			printf("vertex id overflow : %d-%d\n",a,b);
			break;
		}

		VECTOR(v)[i]=a;
		VECTOR(v)[i+1]=b;
	}*/
	for (int i = 0; i < nVertices; i++)
	{
		for (int j = 0; j < nVertices; j++)
		{
			if (A[i][j])
			{
				VECTOR(v)[t]=i;
				VECTOR(v)[t+1]=j;
				t += 2;
			}
			
		}
	}
	//cout<<t<<endl;
	

	igraph_create(g, &v, 0, 0);         //创建一个graph
	igraph_simplify(g, 1, 0, 0);  //移除multiple edges
}

/*void create_graph(igraph_t * g, string input)
{
	int i,  nEdges=0, a, b, wt;
	const int nVertices=115;
	//scanf("%d %d",&nVertices, &nEdges);
	ifstream in(input.c_str());
//	double A[nVertices][nVertices];
	int t = 0;

	//memset(A,0,sizeof(A));
	nEdges = 1226;
	int be;
	igraph_vector_t v;
	igraph_vector_init (&v, 2*nEdges);//Every vector needs to be initialized before it can be used, and there are a number of initialization functions
                                          //or otherwise called constructors. This function constructs a vector of the given size and initializes each entry to 0
	

	while(in >>be && be != EOF)
	{
		int en;
		in>>en;
	//	cout<<be<<" "<<en<<endl;
		//A[be][en] = 1;
		//nEdges++;
		
		VECTOR(v)[t]=be;
		VECTOR(v)[t+1]=en;
		t += 2;
		
	}
	
	
    //cout<<nEdges<<endl;
	
	/*for(i=0;i<2*nEdges;i+=2)
	{
		//scanf("%d %d",&a,&b);
		if(a>=nVertices || b>=nVertices)
		{
			printf("vertex id overflow : %d-%d\n",a,b);
			break;
		}

		VECTOR(v)[i]=a;
		VECTOR(v)[i+1]=b;
	}
/*	for (int i = 0; i < nVertices; i++)
	{
		for (int j = 0; j < nVertices; j++)
		{
			if (A[i][j])
			{
				VECTOR(v)[t]=i;
				VECTOR(v)[t+1]=j;
				t += 2;
			}
			
		}
	}
	//cout<<t<<endl;
	

	igraph_create(g, &v, 0, 0);         //创建一个graph
	igraph_simplify(g, 1, 0, 0);  //移除multiple edges
}*/


void getMembershipFromFile(char filename[], igraph_vector_t * membership)
{
	char line[10000];
	FILE * fp;
	fp = fopen(filename, "r");
	VECTOR(*membership)[0] = 0;
	while(fgets(line, 10000, fp))
	{
		char * pch = strtok(line, " \t\r\n");
		int vertex = atoi(pch);
		pch = strtok(NULL, " \t\r\n");
		int comm = atoi(pch);
		VECTOR(*membership)[vertex] = comm;
	}
	fclose(fp);
}

int main(int argc, char * argv[])
{
	//string dirname=argv[1];
//	string filename=argv[2];

	int i, nVertices, maxIt;
	double Netw_perm, result;
	char line[10000];
	FILE * fp, * out;
	igraph_t g;

//	string output="E:\\VS\\Projects\\maxperm\\maxperm\\output\\" + dirname + "\\" + filename;
//	out = fopen(output.c_str(), "w");

	igraph_vector_t membership1, membership2, membership3;
	
//	string input="F:\\matlab_workspace\\pro\\output\\"+dirname+"\\"+filename;
	string input = "E:\\VS\\Projects\\maxperm\\maxperm\\football_network.txt";
	create_graph(&g, input);
	nVertices = igraph_vcount(&g);   //The number of vertices in a graph
	
	igraph_vector_init(&membership1, nVertices);
	
	maxIt = 10;//Max Iteration
	clock_t t = clock();
	Netw_perm=igraph_community_MaxPerm(&g, &membership1, maxIt);
	t = clock() - t;
	printf("Our MaxPerm Implementation took %f seconds\n",((float)t)/CLOCKS_PER_SEC);
	//fprintf( out,"%lf\n",Netw_perm );
	//fprintf(out, "Our MaxPerm Implementation took %f seconds\n",((float)t)/CLOCKS_PER_SEC);
	//fprintf(out, "Network Permanence is %lf\n",Netw_perm );	

	printf( "Network Permanence is %lf\n",Netw_perm );
	//for(i=0;i<nVertices;++i)
	//{
	//	fprintf(out, "%d\t%ld\n",i,(long)VECTOR(membership1)[i]);
	//}
	
	
	/*
	To find the NMI with ground truth file:community.dat

	//Compare with ground truth of LFR
	igraph_vector_init(&membership2, nVertices);
	getMembershipFromFile("community.dat", &membership2);	
	igraph_compare_communities(&membership1, &membership2, &result, IGRAPH_COMMCMP_NMI);
	printf("NMI of our MaxPerm Implementation with Ground Truth of LFR is %lf\n", result);

	*/
	
	//Free allocated resources
	igraph_vector_destroy(&membership1);
	//igraph_vector_destroy(&membership2);
	
	igraph_destroy(&g);
	while (1);
	
	return 0;

}