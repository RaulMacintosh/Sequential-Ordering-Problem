#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

int n;
int initialPopulation;
int bestCost;
vector<int> bestSolution;
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
	for (int i = 0; i < n; i++){
		for (int j = i+1; j < n; j++){
			if (chromossome[i] == chromossome[j]){
				return false;
			}
		}
	}

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
	int flag = true;

	for (int i = 1; i <= n; i++){
		chromossome.push_back(i);
	}

	if(isValid(chromossome)){
		valids.push_back(chromossome);
		flag = false;
		bestSolution = chromossome;
		bestCost = getCost(chromossome);
	}else{
		nonValids.push_back(chromossome);
	}

	int i = 0;
	while(i < initialPopulation){
		randomizeChoromossome(chromossome);
		
		if(isValid(chromossome)){
			valids.push_back(chromossome);
			i++;

			if(flag){
				flag = false;
				bestSolution = chromossome;
				bestCost = getCost(chromossome);
			}
		}else{
			nonValids.push_back(chromossome);
		}
	}
}

void getBestSolution(){
	for (int i = 1; i < valids.size(); i++){
		int aux = getCost(valids[i]);
		if(aux < bestCost){
			bestCost = aux;
			bestSolution = valids[i];
		}
	}
}

void mutation(vector<int> &chromossome){
	int gene1 = rand()%(n-1) + 1;
	int gene2 = rand()%(n-1) + 1;

	int aux = chromossome[gene1];
	chromossome[gene1] = chromossome[gene2];
	chromossome[gene2] = aux;
}

vector<int> crossOver(int y, int x){
	vector<int> father = valids[y];
	vector<int> mother = valids[x];
	vector<int> soon;

	for(int i = 0; i < n; i++){
		if (i % 2 == 0){
			soon.push_back(father[i]);
		}else{
			soon.push_back(mother[i]);
		}
	}

	if(!isValid(soon)){
		mutation(soon);
	}

	return soon;
}

vector<int> crossOver2(int y, int x){
	vector<int> father = nonValids[y];
	vector<int> mother = valids[x];
	vector<int> soon;

	for(int i = 0; i < n; i++){
		if (i % 2 == 0){
			soon.push_back(father[i]);
		}else{
			soon.push_back(mother[i]);
		}
	}

	if(!isValid(soon)){
		mutation(soon);
	}

	return soon;
}

vector<int> crossOver3(int y, int x){
	vector<int> father = valids[y];
	vector<int> mother = nonValids[x];
	vector<int> soon;

	for(int i = 0; i < n; i++){
		if (i % 2 == 0){
			soon.push_back(father[i]);
		}else{
			soon.push_back(mother[i]);
		}
	}

	if(!isValid(soon)){
		mutation(soon);
	}

	return soon;
}

void perturbation(){
	for (int i = 0; i < initialPopulation; i++){
		int father = rand()%(nonValids.size());
		int mother = rand()%(initialPopulation);

		if(father == mother){
			i -= 1;
			continue;
		}

		vector<int> soon = crossOver2(father, mother);

		if(isValid(soon)){
			valids[mother] = soon;
		}else{
			nonValids[father] = soon;
		}
	}	
}

void perturbation2(){
	for (int i = 0; i < initialPopulation; i++){
		int father = rand()%(initialPopulation);
		int mother = rand()%(nonValids.size());

		if(father == mother){
			i -= 1;
			continue;
		}

		vector<int> soon = crossOver3(father, mother);

		if(isValid(soon)){
			valids[father] = soon;
		}else{
			nonValids[mother] = soon;
		}
	}	
}

void procreation(){
	for (int i = 0; i < initialPopulation; i++){
		int father = rand()%(initialPopulation);
		int mother = rand()%(initialPopulation);

		if(father == mother){
			i -= 1;
			continue;
		}

		vector<int> soon = crossOver(father, mother);

		if(isValid(soon)){
			valids[father] = soon;
		}else{
			nonValids[mother] = soon;
		}
	}
}

int main(int argc, char* argv[]){
	initialPopulation = atoi(argv[1]);
	readInstance(argv[2]);

	srand(time(NULL));

	generatePopulation();
	getBestSolution();

	for (int i = 0; i < n; i++){ // N perturbações
		for (int j = 0; j < n; j++){ // N gerações
			procreation();
			getBestSolution();
		}

		if (i % 2 == 0){
			perturbation();
		}else{
			perturbation2();
		}
	}

	printChormossome(bestSolution);
	printf("Cost: %i\n", bestCost);

	return 0;
}