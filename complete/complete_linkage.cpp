#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<map>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<set>
#include<ctime>
using namespace std;

const int MAX_N = 1 << 30;

int N, K;

int _cluster_number;

typedef struct nodes{
	typedef int cluster;
	cluster a,b; // a * N + b; a, b < n;
	double dist;
}_cluster_distance;

vector<double> points; // to diposit the coordinate of points
vector<int> classes;
int dimension = 0; //dimension of the space


vector<_cluster_distance> dist;
/* define some operations for disjoint set*/

vector<_cluster_distance> _copy;

bool _compare_dist(const _cluster_distance &x, const _cluster_distance &y){
	return x.dist < y.dist;
}

vector<int> par;
vector<int> deapth;
set<int> _set;


void _init_disjoint(const int n){ //initlize the disjoint set
	for(int i = 0 ; i < n ; i++){
		par[i] = i;
		deapth[i] = 0;
	}
}

int _find_parent(int x){ //find the parent of element
	if(par[x] == x){
		return x;
	}else{
		return par[x] = _find_parent(par[x]); 
	}
}

void _union_set(int x, int y){ //union two sets
	x = _find_parent(x);
	y = _find_parent(y);
	if(x == y) return ;
	
	if(deapth[x] < deapth[y]){
		par[x] = y;
	}else{
		par[y] = x;
		if(deapth[x] == deapth[y]) deapth[x]++;
	}
}

bool _same_set(int x, int y){ // whether two elements belong to the same set.
	 return _find_parent(x) == _find_parent(y);
} 

double _distance(int i, int j){
	double sum = 0.0;	
	for(int k = 0 ; k < dimension ; k++){
		double a = *(points.begin() + i * dimension +k);
		double b = *(points.begin() + j * dimension +k);
		sum += (a-b)*(a-b);
	}
	return sqrt(sum);
}

double _find_distance(int i, int j){
	for( int k = 0 ; k < dist.size(); k++){
		if(dist[k].a == i && dist[k].b == j){
			return dist[k].dist;
		}
	}
	exit(1);
}
void _init_distance(){
	for(int i = 0; i < N - 1; i++){
		_set.insert(i);
		for(int j = i + 1; j < N; j++){
			double d = _distance(i, j);	
			_cluster_distance c{i, j, d};
			dist.push_back(c);

		}
	}
	_set.insert(N-1);
	_copy = dist;
	sort(dist.begin(),dist.end(),_compare_dist);
}
void _update_distance(int k1, int k2){ // k2 and k1 union
		int k = 0;
		if(_same_set(k1, k2)) k = _find_parent(k1);
		else exit(1);
		_set.erase(k1);_set.erase(k2);
		vector<_cluster_distance> _dist;
		double d;
		for(auto &x: _set){
				if( k1 < x && k2 < x){
					d = max(_find_distance(k1,x), _find_distance(k2, x));
				}else if( k1 <x && k2 >x){
					d = max(_find_distance(k1,x), _find_distance(x, k2));
				}else if(k1 > x && k2 > x){
					d = max(_find_distance(x, k1), _find_distance(x, k2));
				}else if(k > x && k2 <x){
					d = max(_find_distance(x, k1), _find_distance(k2,x));
				}
				if(k > x){
				_dist.push_back({x,k, d});
				}else{
				_dist.push_back({k,x, d});
				}
				for( auto &y: _set){
						int _x = _find_parent(x);
						int _y = _find_parent(y);
						if( _x < _y){
							d = _find_distance(_x, _y);
							_dist.push_back({_x, _y, d});
						}
				}

		}
		_set.insert(k);
		dist = _dist;
		sort(dist.begin(), dist.end(), _compare_dist);
}

int main(int argc, char *argv[]){
	string filename(argv[1]);
	ifstream fin(filename.c_str(), ios::in);
	fin >> N >> K;
	string line;
	while(getline(fin,line)){
		size_t pos1 = 0,pos2 = 0;
		while(true){
			pos1 = line.find_first_of("(,", pos2);
			if(pos1 == string::npos) break;
			pos2 = line.find_first_of(",)", pos1 + 1);
			if(pos2 == string::npos) break;
			string p = line.substr(pos1 + 1, pos2 - pos1 -1);
			points.push_back(atof(p.c_str()));

		}
	}
		
	par.resize(N,0);
	deapth.resize(N,0);
	classes.resize(N, -1);
	dimension = points.size() / N;
	time_t start, end;
	start = clock();
	_init_disjoint(N);
	_init_distance();
	_cluster_number = N;
	while(_cluster_number > K){
		_union_set(dist[0].a, dist[0].b);
		_update_distance(dist[0].a, dist[0].b);
		_cluster_number--;

	}
	end = clock();
	ofstream output("out_complete.txt", ios::out);
	for(int i = 0 ; i < N ;i++){
		if(i == 0){
			cout <<"[ ";
		}
		cout << _find_parent(i) << " " ;
		if( i < N-1) cout <<",";
		if(i == N-1) cout <<"]"<<endl;
		output << _find_parent(i) << " " ;
	}
	cout << "Complete linkage cost: " << double(end -start)/CLOCKS_PER_SEC<< "s" << endl;
	output.close();
	fin.close();
	return 0;
}
