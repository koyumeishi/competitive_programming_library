#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;
#define INF 1e9

typedef struct{
	int to;
	int cap;
	int cost;
	int rev;
}edge;

void add_edge(vector<vector<edge> > &G, int from, int to, int cap, int cost){
	G[from].push_back((edge){to, cap, cost, (int)G[to].size()});
	G[to].push_back((edge){from, 0, -cost, (int)G[from].size()-1});
}


//min cost : s->t (flow:f)
int min_cost_flow(vector<vector<edge> > &G, int s, int t, int f){
	const int N = G.size();
	int cost = 0;
	vector<int> prev_v(N,-1);
	vector<int> prev_e(N,-1);
	
	while(f>0){
		//min distance(cost based) search with SPFA
		vector<int> dist(N, INF);

		vector<int> cnt(dist.size(), 0);

		dist[s] = 0;
		prev_v[s] = s;
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
			for(int i=0; i<G[pos].size(); i++){
				edge& E = G[pos][i];
				if(dist[E.to] > dist[pos] + E.cost && E.cap > 0){
					dist[E.to] = dist[pos] + E.cost;
					prev_v[ E.to ] = pos;
					prev_e[ E.to ] = i;

					if(cnt[ E.to ] == 0){
						my_push( E.to );
					}
				}
			}
		}


		//cannot achieved to "t" return -1
		if(dist[t]>=INF) return -1;

		
		//add cost of s->t with flow=d
		int pos=t;
		int d=INF;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			pos = i;
			d = min(d, G[i][j].cap);
		}
		
		pos = t;
		//cout << t ;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			G[i][j].cap -= d;
			G[ G[i][j].to ][ G[i][j].rev ].cap += d;
			//cost += G[i][j].cost * d;
			pos = i;
			//cout << " <- " << pos;
		}
		//cout << endl;
		cost += d * dist[t];
		f -= d;

		//f==0 then end
	}
	return cost;
}


//min cost : s->t (flow:f)
//SPFA_fast version using SLF and LLL
int min_cost_flow(vector<vector<edge> > &G, int s, int t, int f){
	const int N = G.size();
	int cost = 0;
	vector<int> prev_v(N,-1);
	vector<int> prev_e(N,-1);
	
	while(f>0){
		//min distance(cost based) search with SPFA
		vector<int> dist(N, INF);

		long long sum = 0;
		vector<int> cnt(dist.size(), 0);

		dist[s] = 0;
		prev_v[s] = s;
		deque<int> Q;
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
			for(int i=0; i<G[pos].size(); i++){
				edge& E = G[pos][i];
				if(dist[E.to] > dist[pos] + E.cost && E.cap > 0){

					if(cnt[E.to] != 0){
						sum -= dist[ E.to ];
					}

					dist[E.to] = dist[pos] + E.cost;
					prev_v[ E.to ] = pos;
					prev_e[ E.to ] = i;

					sum += dist[E.to];

					if(cnt[ E.to ] == 0){
						my_push( E.to );
						adjust();
					}
				}
			}
		}


		//cannot achieved to "t" return -1
		if(dist[t]>=INF) return -1;

		
		//add cost of s->t with flow=d
		int pos=t;
		int d=INF;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			pos = i;
			d = min(d, G[i][j].cap);
		}
		
		pos = t;
		//cout << t ;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			G[i][j].cap -= d;
			G[ G[i][j].to ][ G[i][j].rev ].cap += d;
			//cost += G[i][j].cost * d;
			pos = i;
			//cout << " <- " << pos;
		}
		//cout << endl;
		cost += d * dist[t];
		f -= d;

		//f==0 then end
	}
	return cost;
}




