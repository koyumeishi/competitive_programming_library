//Shortest Path Faster Algorithm
// http://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm

#include <vector>
#include <algorithm>
#include <queue>
#include <deque>
 
typedef struct{
	int from;
	int to;
	int cost;
}edge;

//SPFA
//O(VE)
//faster than bellman_ford algorithm
void SPFA(vector<vector<edge>> &G, vector<int> &dist, int s){
	const int INF = 1e8;
	fill(dist.begin(), dist.end(), INF);

	vector<int> cnt(dist.size(), 0);

	dist[s] = 0;
	queue<int> Q;

	auto my_push = [&](int node){
		Q.push(node);
		cnt[node]++;
	};
	auto my_pop = [&]() -> int{
		int ret = Q.front();
		cnt[ret]--;
		Q.pop();
		return ret;
	};
	
	my_push(s);

	while(!Q.empty()){
		int pos = my_pop();

		for(edge& E : G[pos]){
			if(dist[ E.to ] > dist[ pos ] + E.cost ){
				dist[ E.to ] = dist[ pos ] + E.cost;

				if(cnt[ E.to ] == 0){
					my_push( E.to );
				}
			}

		}
		
	}

}


//using some techniques writen in wikipedia, SLF and LLL
void SPFA__(vector<vector<edge>> &G, vector<int> &dist, int s){
	const int INF = 1e8;
	fill(dist.begin(), dist.end(), INF);

	vector<int> cnt(dist.size(), 0);

	dist[s] = 0;
	deque<int> Q;

	long long sum = 0;

	auto my_push = [&](int node){
		//SLF
		if(Q.size() == 0 || dist[Q.front()] > dist[node]){
			Q.push_front(node);
		}else{
			Q.push_back(node);
		}
		cnt[node]++;
	};
	auto my_pop = [&]() -> int{
		int ret = Q.front();
		cnt[ret]--;
		Q.pop_front();
		return ret;
	};

	auto adjust = [&](){
		//LLL
		double ave = 1.0*sum / Q.size();
		if(Q.size() <= 1) return;
		while( dist[Q.front()] > ave ){
			Q.push_back( Q.front() );
			Q.pop_front();
		}
	};
	
	my_push(s);

	while(!Q.empty()){
		int pos = my_pop();
		sum -= dist[pos];

		for(edge& E : G[pos]){
			if(dist[ E.to ] > dist[ pos ] + E.cost ){

				if(cnt[E.to] != 0){
					sum -= dist[ E.to ];
				}

				dist[ E.to ] = dist[ pos ] + E.cost;

				sum += dist[E.to];

				if(cnt[ E.to ] == 0){
					my_push( E.to );

					adjust();
				}
			}

		}
		
	}

}