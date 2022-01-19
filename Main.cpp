#include <iostream>
#include <vector>
#include "aStar.h"

void find_char_in_map(char, std::vector<std::string>&, Node&);
int find_node_in_vector(std::vector<Node>&, Node&);
Node find_parent_node(Node&); 
int aStar_search(Node&, Node&); 
double calculate_cost_NtN(Node&, Node&);
double calculate_total_cost(Node, Node&, Node&);
void checkSides(Node&, Node&, Node&);
void traceBack(Node, Node&); 
void checkSide(Node&); 

/*
x : Wand
s : Start
f : ziel
_ : nichts
[SPACE] : nichts



g cost: start to current 
h cost: current to goal

*/

std::vector<std::string> map = {

	"xxxxxxxxxxxxxxx",
	"x s      x    x",
	"x xxxxxx x  x x",
	"x        x xx x",
	"x xx xxxxx    x",
	"x  x     x xx x",
	"xx x xx  x  x x",
	"x  x  x  x  x x",
	"x xxx x  x  x x",
	"x  x  x     x x",
	"xx xxxx xxx x x",
	"x  x        x x",
	"xxxx xxxxxxxx x",
	"x            fx",
	"xxxxxxxxxxxxxxx",


};

std::vector<Node> explored;
std::vector<Node> open;


int main() {

	a_star_search astar(map);
	astar.search_path(); 

	for (int i = 0; i < astar.path.size(); i++) {
		std::cout << astar.path[i].y << " " << astar.path[i].x << std::endl; 
	}

	/*
	Node startNode; 
	Node finishNode; 

	find_char_in_map('s', map, startNode);
	find_char_in_map('f', map, finishNode);



	aStar_search(startNode, finishNode); 
	*/
}

int aStar_search(Node& startNode, Node& finishNode) {

	explored.push_back(startNode);


	checkSides(startNode, startNode, finishNode); 


	while (open.size() > 0) {

		// fin cheapest node in open list
		double min = 100000000;
		int minPos = 0;

		for (int i = 0; i < open.size(); i++) {
			if (open[i].cost < min) {
				min = open[i].cost;
				minPos = i;
			}
		}

		// put min node to explored list
		explored.push_back(open[minPos]);
		Node current = open[minPos];
		open.erase(open.begin() + minPos);


		// if node is finish -> done (traceback), else check 4 neightbour nodes
		if (current.x == finishNode.x && current.y == finishNode.y) {
			std::cout << "goal found" << std::endl; 
			traceBack(current, startNode); 
			return 1; 
		}
		else {
			checkSides(current, startNode, finishNode);

		}

		// find lowest cost node and add to explored
		// check if node is finish
		// else calculate side nodes
	}

	std::cout << "no path foun" << std::endl; 
	return -1; 

}

// Utility

void find_char_in_map(char toFind, std::vector<std::string>& localMap, Node& node) { // used to find start and finish in map
	for (int i = 0; i < localMap.size(); i++) {
		for (int i2 = 0; i2 < localMap[i].size(); i2++) {
			if (localMap[i][i2] == toFind) {
				node.x = i; 
				node.y = i2; 
			}
		}
	}
}

void traceBack(Node currentNode, Node& startNode) { 


	int x = currentNode.x;
	int y = currentNode.y;

	while (x != startNode.x || y != startNode.y) {
		std::cout << y << " " << x << std::endl;


		Node parent = find_parent_node(currentNode);
		currentNode = parent; 
		x = parent.x;
		y = parent.y;
	}

	std::cout << y << " " << x << std::endl; 

}

void checkSide(Node& node) {

	if (map[node.x][node.y] != 'x') {

		int exploredPos = find_node_in_vector(explored, node);
		int openPos = find_node_in_vector(open, node);

		if (exploredPos == -1 && openPos == -1) {
			open.push_back(node);
			return; 
		}
		if (openPos == -1 && exploredPos != -1 && node.cost < explored[exploredPos].cost) {
			explored.erase(explored.begin() + exploredPos);
			explored.push_back(node);
			return; 
		}

		if (openPos != -1 && exploredPos == -1 && node.cost < open[openPos].cost) {
			open.erase(open.begin() + openPos);
			open.push_back(node);
			return; 
		}
	
	}
}

void checkSides(Node& currentNode, Node& startNode, Node& finishNode) { 
	if (currentNode.y > 0) {
		Node top(currentNode.x, currentNode.y - 1, currentNode.x, currentNode.y);
		top.cost = calculate_total_cost(top, startNode, finishNode);
		checkSide(top); 

	}

	if (currentNode.y < map[0].size() - 1) {
		Node bottom(currentNode.x, currentNode.y + 1, currentNode.x, currentNode.y);
		bottom.cost = calculate_total_cost(bottom, startNode, finishNode);
		checkSide(bottom);
	}

	if (currentNode.x > 0) {
		Node left(currentNode.x - 1, currentNode.y, currentNode.x, currentNode.y);
		left.cost = calculate_total_cost(left, startNode, finishNode);
	
		checkSide(left);
	}

	if (currentNode.x < map.size() - 1) {
		Node right(currentNode.x + 1, currentNode.y, currentNode.x, currentNode.y);
		right.cost = calculate_total_cost(right, startNode, finishNode);
	
		checkSide(right);
	}
}

double calculate_total_cost(Node currentNode, Node& startNode, Node& finishNode) {
	

	int x = currentNode.x;
	int y = currentNode.y; 

	double g_cost = 10; // start - current
	double h_cost = calculate_cost_NtN(currentNode, finishNode); // current - finish 

	while (x != startNode.x || y != startNode.y) {
		g_cost += 10.0;

		Node parent = find_parent_node(currentNode); 
		currentNode = parent;

		x = parent.x; 
		y = parent.y; 
	}
	return g_cost + h_cost; 
}

double calculate_cost_NtN(Node& currentNode, Node& desiredNode) {
	// cost from current position to finish

	double dx = currentNode.x - desiredNode.x;
	double dy = currentNode.y - desiredNode.y;

	return 10.0 * sqrt(dx * dx + dy * dy);

}

int find_node_in_vector(std::vector<Node>& vec, Node& node) {
	/*
	- check if node is already in array
		if yes return anything else than -1
	- return -1 on no find

	*/

	for (int i = 0; i < vec.size(); i++) {
		if (vec[i].x == node.x && vec[i].y == node.y) {
			return i; 
		}
	}

	return -1; 

}

Node find_parent_node(Node& currentNode) {


	for (int i = 0; i < explored.size(); i++) {
		if (explored[i].x == currentNode.parentNode[0] && explored[i].y == currentNode.parentNode[1]) {
			return explored[i];
		}
	}

	throw std::runtime_error("Parent Element in vec explored missing");

}