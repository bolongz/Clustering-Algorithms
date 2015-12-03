#include<iostream>
#include<stdio.h>
#include<vector>
#include<cassert>
#include<cassert>
using namespace std;

int main(){

	FILE *out1 = fopen("out_complete.txt","r");
	FILE *out2 = fopen("out_llord.txt","r");
	assert(out1);
	assert(out2);
	int c1, c2, hamming = 0;
	vector<int> a;
	vector<int> b;
	while(fscanf(out1, "%d", &c1) ==1 && fscanf(out2, "%d", &c2) == 1){
		a.push_back(c1);
		b.push_back(c2);
	}
	assert(a.size() == b.size());
	for(int i = 0; i < a.size()-1 ; i++){
		for(int j = i ; j < a.size(); j++){
			if(a[i] == a[j]){
				if(b[i] != b[j]){
					hamming++;
				}
			}
		}
	}
	for(int i = 0; i < b.size()-1 ; i++){
		for(int j = i ; j < b.size(); j++){
			if(b[i] == b[j]){
				if(a[i] != a[j]){
					hamming++;
					}
			}
		}
	
	}
	cout <<"hamming distance is: " <<  double(hamming)/double(a.size() * (a.size() -1)) *  2 << endl;
	fclose(out1);
	fclose(out2);
	return 0;
}
