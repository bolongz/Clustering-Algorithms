#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<map>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<set>
#include<time.h>
#include<map>
#include<chrono>
#include<string>
#include <cstddef>   
#include<ctime>
using namespace std;

const int MAX_N = 1 << 30;

int N, K;


int _kmeans = 1 << 30;
typedef vector<double> value;

vector<double> points; // to diposit the coordinate of points
vector<int> classes;
vector<int> _final_classes;
vector<int> _final_parents;
vector<double> _centers;
int dimension = 0; //dimension of the space
int size = 0;

/* define some operations for disjoint set*/

map<int, value> _dynamic;

vector<int> par;
vector<int> deapth;

void _init_centers(){
	srand((unsigned)time(NULL));
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	vector<int> _set;
	for(int i = 0 ; i < N ;i++) _set.push_back(i);
	shuffle(_set.begin(), _set.end(),std::default_random_engine(seed));
	double k = 0;
	vector<double>().swap(_centers);
	for(int j = 0 ; j < K ; j++){
			double x = _set[j];
			for(int i = 0 ; i < dimension ; i++){
				double _p =  points[x * dimension +i];
				_centers.push_back(_p);
				points[size + k * dimension + i] = _p; 
			}
			k++;
		}
}

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

int _find_closest(int node){
	int index = N;
	double min = _distance(node, N);
	for(int i = 1 ; i < K ;i++){
			double d = _distance(node,  N + i);
			if( min > d){
				index =  N + i;
				min = d;
			}
	}
	return index;
}
		
void _union_update(){
	map<int, value> dynamic;
	for(int i = 0; i < N; i++){
		int closest = _find_closest(i);
		_union_set(i, closest);
		int _index = _find_parent(i);
		for(int j = 0 ; j < dimension; j++){
		dynamic[_index].push_back(points[ i * dimension + j]);
		}
	}
	_dynamic.swap(dynamic);
}

void _new_centers(){
	int _size = size;
	int k = 0;
	for(auto &x : _dynamic){
		int length = x.second.size() / dimension;
		for(int i = 0; i < dimension; i++){
			double sum = 0;
			for(int j = 0 ; j < length ; j++){
				sum += x.second[ j * dimension + i];
			}
			points[_size + k * dimension + i] = sum / length;
		}
		k++;
	}
}		

double  _threshod(){
	double threshod = 0; 
	for(int i = 0; i < K ;i++){
		for(int j = 0 ; j < dimension ; j++){
			double _p1 = points[size + i * dimension + j];
			double _p2 = _centers[i * dimension + j];
			_centers[i * dimension + j] = points[size + i * dimension + j];
			threshod += (_p1 - _p2) * (_p1 - _p2);
		}
	}
	return sqrt(threshod);
}

void _update_kmeans(){
	double _s = 0;
	for(auto &x : _dynamic){
		vector<double> _tempory;
		int length = x.second.size() / dimension;
		for(int i = 0; i < dimension; i++){
			double sum = 0;
			for(int j = 0 ; j < length ; j++){
				sum += x.second[ j * dimension + i];
			}
			_tempory.push_back(sum / length);
		}
		for(int i = 0; i < length; i++){
			for(int j = 0 ; j < dimension ; j++){
				double a = x.second[ i * dimension + j];
				double b = _tempory[j];
				_s += (a -b) * (a-b);
			}
		}
	}
	if( _kmeans > _s){
		vector<int>().swap(_final_parents);
		_kmeans = _s;
		for(int i = 0 ; i < N; i++){
			_final_parents.push_back(_find_parent(i));
		}
	}
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
	time_t start, end;
	size = points.size();
	dimension = size / N;
	points.resize(size + K * dimension);
	par.resize(N + K,0);
	classes.resize(N + K, -1);
	deapth.resize(N + K,0);
	double count = 0;
	start = clock();
	while( (count++) < 100){
		double threshod = 1;
		_init_centers();
		for(int i = 0; i < classes.size(); i++)
			classes[i] = -1;
		while (threshod >= 1e-4){
			_init_disjoint(N + K);
			_union_update();
			_new_centers();
			threshod = _threshod();
		}
		_update_kmeans();
	}
	end = clock();
	ofstream output("out_llord.txt", ios::out);
	for(int i = 0 ; i < N ;i++){
		if( i ==0) cout << "[ ";
		cout << _final_parents[i] << " " ;
		if( i < N-1) cout << "," ;
		if(i == N-1) cout << "]" << endl;
		output << _final_parents[i]<< " ";
	}
	cout <<" Llord method cost: " << double(end-start)/CLOCKS_PER_SEC <<"s" << endl;
	fin.close();
	output.close();

	return 0;
}
