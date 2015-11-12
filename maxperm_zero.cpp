#include "maxperm_zero.h"

double Perm(igraph_t * g, igraph_vector_t *neig_v, int * vertex_community, int v)
{
	int i,j,neighbor,neighbor_2;
	int *E_v,*edge_temp;
	int numerator=0,denominator=0;
	int Etemp_v;

	
	E_v=(int *) calloc(igraph_vcount(g), sizeof(int));

	edge_temp=(int*) malloc(sizeof(int));

	int I_v=0,Emax_v=0,D_v=0;
	double cin_v=0.0;
	for (i = 0; i < igraph_vector_size (neig_v); ++i)
	{
		neighbor=VECTOR(*neig_v)[i];
		if(vertex_community[neighbor]==vertex_community[v])
		{
			I_v++;
		}
		else
		{
			E_v[vertex_community[neighbor]]++;
			Etemp_v = E_v[vertex_community[neighbor]];
			if(Etemp_v>Emax_v)
				Emax_v=Etemp_v;
		}
		if(vertex_community[neighbor]!=vertex_community[v])
			continue;
		
		for (j = i+1; j < igraph_vector_size (neig_v); ++j)
		{
			neighbor_2=VECTOR(*neig_v)[j];
			if(vertex_community[neighbor_2]!=vertex_community[v])
				continue;
			denominator++;
			igraph_get_eid(g, edge_temp, neighbor, neighbor_2, 0, 0);//Get the edge id from the end points of an edge. 
			                                                       //edge_temp: Pointer to an integer, the edge id will be stored here.
			if((*edge_temp)!=-1)
			{
				numerator++;
			}
		}

	}
	D_v= i>0?i:1;
	if(Emax_v==0)
		Emax_v=1;
	
	if(denominator==0)
		cin_v=0.0;
	else
		cin_v=(1.0*numerator)/(denominator);

	free(E_v);
	free (edge_temp);
	//cout<<I_v<<" "<<Emax_v<<" "<<D_v<<" "<<cin_v<<endl;
	return ((1.0*I_v)/(Emax_v*D_v))-1+cin_v;	
}

double igraph_community_MaxPerm(igraph_t * g, igraph_vector_t * membership, int maxIt)
{
	int i,j,u,v,vertices,Itern,count,prev;
	double Sum,Old_Sum,cur_p,n_p,n_p_neig,Netw_perm;
	double cur_p_neig;
	int *Comm_v;
	int C,Original_C;
	igraph_vector_t *neig;
	vertices = igraph_vcount(g);
	
	//cout<<vertices<<endl;
	neig = (igraph_vector_t *) malloc(vertices * sizeof(igraph_vector_t));

	for (i = 0; i < vertices; ++i)
	{

		igraph_vector_init(&neig[i], 1);
		igraph_neighbors(g, &neig[i], i, IGRAPH_ALL); //Adjacent vertices to a vertex
		
	}	
	
	int *vertex_community;
	vertex_community = (int *) malloc(vertices*sizeof(int));	//stores which community a node belongs
	Comm_v = (int *) malloc(vertices*sizeof(int));	//stores which community a node belongs
	for (i = 0; i < vertices; ++i)
	{
		vertex_community[i]=i;
	}
	
	Old_Sum=-INT_MAX;
	Sum = Old_Sum + 1;
	printf("%lf\n", Old_Sum);
	Itern=0;
	while(Sum>Old_Sum && Itern<maxIt)
	{
		Itern++;
		Old_Sum=Sum;
		Sum=0;
		for (v = 0; v < vertices; ++v)
		{
			// if(v % 1000 == 0)
			// 	printf("vertex = %d\n", v);
			cur_p=Perm(g, &(neig[v]), vertex_community, v);     //得到当前community的这个点的permanence
			
			if(cur_p==1)
			{
				
				Sum+=cur_p;
				continue;
			}
			cur_p_neig=0;
			int * isPresent = (int *)malloc(vertices*sizeof(int));
		    
		//	memset(isPresent,0,vertices*sizeof(int));
		
			
			for (int k = 0; k < vertices; k++)
			{
				isPresent[k] = 0;
		
			}
			
			int numComV = 0;
			
			for (j = 0; j < igraph_vector_size (&neig[v]); ++j)  //得到v的邻居节点的总的permanence
			{
				u=VECTOR(neig[v])[j];                            //邻居节点 
				int uCommunity = vertex_community[u];              //邻居节点所在的community
				
				cur_p_neig+=Perm(g, &(neig[u]), vertex_community, u);        
				
				if(!isPresent[uCommunity])                          //为了得到v点的邻居community
				{
					
					isPresent[uCommunity] = 1;
					Comm_v[numComV++] = uCommunity;                      //存储邻居community
				}
			}
			free(isPresent);
			
			j=0;
			
			while(j < numComV)
			{
				C=Comm_v[j];                                        //其中一个邻居community
				j++;
				Original_C=vertex_community[v];

				vertex_community[v]=C;                          //移动到邻居comunity  C
				n_p=Perm(g, &(neig[v]), vertex_community, v);       //移动之后的permanence

				n_p_neig=0;

				for (i = 0; i < igraph_vector_size (&neig[v]); ++i)  //计算移动之后的邻居节点的permanence
				{
					u=VECTOR(neig[v])[i];
					
					n_p_neig+=Perm(g, &(neig[u]), vertex_community, u);

				}
				if (cur_p + cur_p_neig < n_p + n_p_neig)// Many different kinds of conditions can be put here，可以自己定选择
				//if(cur_p < n_p)
				{   
				
					cur_p=n_p;
					cur_p_neig=n_p_neig;	//This line not in original algo
				}
				else
				{
					vertex_community[v]=Original_C;               //否则就不移动
				}							

			}
			
			Sum+=cur_p;                         //得到总的permanence
			//cout<<Sum<<endl;
		}	
		printf("SumQ %lf :: iter %d\n", Sum, Itern);
	}
	Netw_perm=Sum/vertices;                                         //归一化
	
	for (i = 0; i < vertices; ++i)
	{
		igraph_vector_destroy(&neig[i]);
	}

	free(neig);

	for (i = 0; i < vertices; ++i)
	{
		VECTOR(*membership)[i] = vertex_community[i];
	}

	free(vertex_community);
	return Netw_perm;
}