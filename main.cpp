#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

int n;
int initialPopulation;
vector< vector<int> > graph;
vector< vector<int> > valids;
vector< vector<int> > nonValids;

void readInstance(char* fileName){
	ifstream fileData;
	fileData.open(fileName);
	if(fileData.is_open()){
		while(!fileData.eof()){
			fileData >> n;
			
			for(int i = 0; i < n; i++){
				vector<int> line;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
				}

				graph.push_back(line);
			}
			
			for(int i = 0; i < n; i++){
				vector<int> line;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
				}

				graph.push_back(line);
			}
		}
	}
}

void printInstance(){
	printf("Size: %i\n", n);
	printf("________________________________________________\n");
	printf("\n");

	printf("Graph:\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%i ", graph[i][j]);
		}
		printf("\n");
	}
}

void printChormossome(vector<int> chromossome){
	printf("[ ");

	for (int i = 0; i < n; i++){
		printf("%i ", chromossome[i]);
	}

	printf("]\n");
}

bool isValid(vector<int> chromossome){
	for (int i = 0; i < n-1; i++){
		if(graph[chromossome[i]][chromossome[i+1]] == -1){
			return false;
		}
	}

	return true;
}

int getCost(vector<int> chromossome){
	int cost = 0;
	
	for (int i = 0; i < n-1; i++){
		cost += graph[chromossome[i]][chromossome[i+1]];
	}

	return cost;
}

void randomizeChoromossome(vector<int> &chromossome){
	for(int i = 1; i < n-1; i++){
		int randomIndex = i + rand()%((n-1)-i);
		int aux = chromossome[i];

		if(graph[chromossome[randomIndex]][aux] == -1){
			i -= 1;
		}else{
			chromossome[i] = chromossome[randomIndex];
			chromossome[randomIndex] = aux;
		}
	}
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
		randomizeChoromossome(chromossome);
		
		if(isValid(chromossome)){
			valids.push_back(chromossome);
		}else{
			nonValids.push_back(chromossome);
		}
	}
}

int main(int argc, char* argv[]){
	initialPopulation = atoi(argv[1]);
	readInstance(argv[2]);
	//printInstance();

	srand(time(NULL));

	generatePopulation();

	return 0;
}