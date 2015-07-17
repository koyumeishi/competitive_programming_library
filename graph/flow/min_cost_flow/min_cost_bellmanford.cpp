#include <iostream>
#include <vector>
#include <queue>

using namespace std;

//class template
//using bellman-ford & dijkstra
//verified 2015/5/21 https://icpc.kattis.com/problems/catering
template<class T>
class Min_Cost_Flow{
public:

	struct edge{
		int to;
		int cap;
		T cost;
		int rev;
	};

	const T INF;
	vector<vector<edge>> G;

	Min_Cost_Flow(int n, T inf) : G(n), INF(inf){

	}

	void add_edge(int from, int to, int cap, T cost){
		G[from].push_back((edge){to, cap, cost, (int)G[to].size()});
		G[to].push_back((edge){from, 0, -cost, (int)G[from].size()-1});
	}


	//min cost : s->t (flow:f)
	T min_cost_flow(int s, int t, int f){
		T cost = 0;
		vector<int> prev_v(G.size(),-1);
		vector<int> prev_e(G.size(),-1);
		vector<T> potantial(G.size(), 0);
		vector<T> dist(G.size(), INF);

		dist[s] = 0;

		/*bellman_ford*/{
			bool update = true;
			for(int cnt = 0; update; cnt++){
				update = false;
				for(int i=0; i<G.size(); i++){
					if(dist[i] >= INF) continue;
					for(int j=0; j<G[i].size(); j++){
						if(G[i][j].cap > 0 && dist[G[i][j].to] > dist[i] + G[i][j].cost){
							dist[G[i][j].to] = dist[i] + G[i][j].cost;

							update = true;
						}
					}
				}

				if(update && cnt >= G.size()){
					cerr << " there is a negative cycle " << endl;
					return -1;
				}
				cnt++;
			}

			for(int i=0; i<G.size(); i++){
				potantial[i] = dist[i];
			}
		}

		while(f>0){
			fill(dist.begin(), dist.end(), INF);
			dist[s] = 0;
			prev_v[s] = s;

			/*dijkstra*/{
				priority_queue<pair<T,int>, vector<pair<T,int>>, greater<pair<T,int>>> pq;
				pq.push({0, s});
				while(pq.size()){
					pair<T, int> p = pq.top(); pq.pop();
					if(dist[p.second] < p.first) continue;
					dist[p.second] = p.first;
					for(int i=0; i<G[p.second].size(); i++){
						edge& e = G[p.second][i];
						T new_dist = dist[p.second] + e.cost + potantial[p.second] - potantial[e.to];
						if(e.cap > 0 && dist[e.to] > new_dist){
							dist[e.to] = new_dist;
							prev_v[e.to] = p.second;
							prev_e[e.to] = i;
							pq.push({dist[e.to], e.to});
						}
					}
				}
			}

			if(potantial[t] + dist[t] == INF){
				cerr << "couldn't insert flow f : " << f << endl;
				return -1;
			}

			for(int i=0; i<G.size(); i++){
				potantial[i] += dist[i];
			}

			int d = f;
			int pos = t;
			while(pos != s){
				int v = prev_v[pos];
				int e = prev_e[pos];
				d = min(d, G[v][e].cap);
				pos = prev_v[pos];
			}

			pos = t;
			while(pos != s){
				int v = prev_v[pos];
				int e = prev_e[pos];
				G[v][e].cap -= d;
				G[G[v][e].to][G[v][e].rev].cap += d;
				pos = v;
			}
			cost += d * potantial[t];	//potential is new_dist
			f -= d;

			//f==0 then end
		}
		return cost;
	}

};


#define INF 1e9

typedef struct{
	int to;
	int cap;
	int cost;
	int rev;
}edge;

//min cost : s->t (flow:f)
int min_cost_flow(vector<vector<edge> > &G, int s, int t, int f){
	int cost = 0;
	vector<int> prev_v(G.size(),-1);
	vector<int> prev_e(G.size(),-1);
	
	while(f>0){
		//min distance(cost based) search with Belman-Ford
		vector<int> dist(G.size(), INF);
		dist[s] = 0;
		prev_v[s] = s;
		bool update=true;
		while(update){
			update = false;
			for(int i=0; i<G.size(); i++){
				if(dist[i] >= INF) continue;
				
				vector<edge> &e = G[i];
				for(int j=0; j<e.size(); j++){
					if(dist[e[j].to] > dist[i] + e[j].cost && e[j].cap>0){
						dist[e[j].to] = dist[i] + e[j].cost;
						update = true;
						prev_v[ e[j].to ] = i;
						prev_e[ e[j].to ] = j;
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


void add_edge(vector<vector<edge> > &G, int from, int to, int cap, int cost){
	G[from].push_back((edge){to, cap, cost, G[to].size()});
	G[to].push_back((edge){from, 0, -cost, (int)G[from].size()-1});
}

