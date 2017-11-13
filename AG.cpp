// Authors: Gilney Jr. and Raul Silveira

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

int n;
int initialPopulation;
int bestCost;
vector<int> bestSolution;
vector< vector<int> > graph;
vector< vector<int> > valids;
vector< vector<int> > nonValids;

// Função que lê o grafo do arquivo
void readInstance(char* fileName){
	ifstream fileData;
	fileData.open(fileName);
	if(fileData.is_open()){
		fileData >> n;
		graph.clear();
		for(int i = 0; i < n and !fileData.eof(); i++){
			vector<int> line;
			for(int j = 0; j < n; j++){
				int x;
				fileData >> x;
				line.push_back(x);
			}

			graph.push_back(line);
		}
	}
	fileData.close();
}

void printInstance(){
	printf("Size: %i\n", n);
	printf("________________________________________________\n");
	printf("\n");

	printf("Graph:\n");
	for(size_t i = 0; i < graph.size(); i++){
		for(size_t j = 0; j < graph.size(); j++){
			printf("%i ", graph[i][j]);
		}
		printf("\n");
	}
}

void printChormossome(const std::vector<int> &chromossome){
	std::cout << "[ ";
	for(auto &e : chromossome)
		std::cout << e << " ";
	std::cout << "]\n";
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
	for(int i = 0; i < n-1; i++)
		cost += graph[chromossome[i]][chromossome[i+1]];
	return cost;
}

void randomizeChoromossome(vector<int> &chromossome){
	for(int i = 0; i < n; i++){
		int randomIndex = rand()%n;
		int aux = chromossome[i];
		chromossome[i] = chromossome[randomIndex];
		chromossome[randomIndex] = aux;
	}
}

// Calcula a solução de menor custo
void getBestSolution()
{
	for(auto &e : valids)
	{
		int currentCost = getCost(e);
		if(currentCost < bestCost)
		{
			bestSolution = e;
			bestCost = currentCost;
		}
	}
}


void generatePopulation(){
	vector<int> chromossome;

	for (int i = 0; i < n; i++){
		chromossome.push_back(i);
	}

	int i = 0;
	while(i < initialPopulation){
		randomizeChoromossome(chromossome);
		
		if(isValid(chromossome)){
			valids.push_back(chromossome);
			i++;
		}
		else if(nonValids.size() < (size_t) initialPopulation)
			nonValids.push_back(chromossome);
	}

	bestSolution = valids[0];
	bestCost = getCost(valids[0]);
	getBestSolution();
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
	initialPopulation = atoi(argv[2]);
	int num_generations;
	if(argc == 4)
		num_generations = atoi(argv[3]);
	else
		num_generations = 100;	// Default number

	std::cout << "> Reading graph from file...\n";
	readInstance(argv[1]);
	srand(time(NULL));

	std::cout << "> Performing genetic algorithm...\n";
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	
	generatePopulation();
	for (int i = 0; i < n; i++){ // N perturbações
		for (int j = 0; j < num_generations; j++){ // Gerações
			procreation();
			getBestSolution();
		}

		if (i % 2 == 0){
			perturbation();
		}else{
			perturbation2();
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "> Best solution: \n   "; 
	printChormossome(bestSolution);
	std::cout << "> Best solution cost: " << bestCost << "\n";

	std::cout << "\t> " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s" << std::endl;
	std::cout << "\t> " << std::chrono::duration <double, std::milli> (end - start).count() << " ms" << std::endl;
	std::cout << "\t> " << std::chrono::duration <double, std::nano> (end - start).count() << " ns" << std::endl;

	return 0;
}