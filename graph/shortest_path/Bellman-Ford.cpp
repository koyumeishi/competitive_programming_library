#include <vector>
#include <algorithm>
 
typedef struct{
	int from;
	int to;
	int cost;
}edge;

//Bellman-Ford
//E : Edge set
//O(VE)
int Bellman_Ford(vector<edge> &E, vector<int> &dist, int s){
	const int INF = 1e8;
	fill(dist.begin(), dist.end(), INF);

	dist[s] = 0;
	
	for(int i=0; i<dist.size(); i++){
		bool update = false;

		for(int j=0; j<E.size(); j++){

			if(dist[ E[j].to ] > dist[ E[j].from ] + E[j].cost ){
				dist[ E[j].to ] = dist[ E[j].from ] + E[j].cost;

				update = true;
			}
		}
		
		if(!update) break;
		if(update && (i == dist.size()-1)){
			//there are negative cycles
			return -1;
		}
	}

	//there are NO negative cycles
	return 1;
}