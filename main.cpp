#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

int n;
int initialPopulation;
vector< vector<int> > graph;
vector< vector<int> > valids;
vector< vector<int> > nonValids;

void readInstance(char* fileName){

}

bool isValid(vector<int> chromossome){
	for (int i = 0; i < n-1; i++){
		if(graph[chromossome[i]][chromossome[i+1]] == -1){
			return false;
		}
	}

	return true;
}

vector<int> randomizeChoromossome(vector<int> chromossome){
	vector<int> newChromossome;
	newChromossome.push_back(chromossome[0]);
	for(int i = 1; i < n-1; i++){
		int randomIndex = i + rand()%((n-1)-i);
		newChromossome.push_back(chromossome[i]);
	}
	newChromossome.push_back(chromossome[n-1]);

	return newChromossome;
}

void generatePopulation(){
	vector<int> chromossome;

	for (int i = 1; i <= n; i++){
		chromossome.push_back(i);
	}

	if(isValid(chromossome)){
		valids.push_back(chromossome);
	}else{
		nonValids.push_back(chromossome);
	}

	for(int i = 0; i < initialPopulation; i++){
		vector<int> newChromossome = randomizeChoromossome(chromossome);
		
		if(isValid(newChromossome)){
			valids.push_back(newChromossome);
		}else{
			nonValids.push_back(newChromossome);
		}
	}
}

int main(int argc, char* argv[]){
	initialPopulation = atoi(argv[1]);
	readInstance(argv[2]);

	srand (time(NULL));

	generatePopulation();

	return 0;
}