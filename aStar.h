#pragma once

#include <vector>

struct Node {
	int x = 0, y = 0;
	double cost = 0;

	int parentNode[2] = { -1, -1 };

	Node() {}

	Node(int x, int y, int pX, int pY) {
		this->x = x;
		this->y = y;
		this->parentNode[0] = pX;
		this->parentNode[1] = pY;
	}


};

struct a_star_search {
	std::vector<std::string> map; 
	Node startNode; 
	Node finishNode; 

	std::vector<Node> explored;
	std::vector<Node> open;
	std::vector<Node> path;

	a_star_search(std::vector<std::string> map) {
		this->map = map; 

		find_char_in_map('s', startNode); 
		find_char_in_map('f', finishNode); 
	}


	int search_path() {
		explored.push_back(startNode);


		checkSides(startNode);


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
				traceBack(current);
				return 1;
			}
			else {
				checkSides(current);

			}

		}

		std::cout << "no path foun" << std::endl;
		return -1;

	}


	void traceBack(Node currentNode) {


		int x = currentNode.x;
		int y = currentNode.y;

		while (x != startNode.x || y != startNode.y) {
			path.push_back(currentNode); 


			Node parent = find_parent_node(currentNode);
			currentNode = parent;
			x = parent.x;
			y = parent.y;
		}

		//std::cout << y << " " << x << std::endl;
		path.push_back(currentNode); 

		std::reverse(path.begin(), path.end()); 



	}

	Node find_parent_node(Node& currentNode) {

		for (int i = 0; i < explored.size(); i++) {
			if (explored[i].x == currentNode.parentNode[0] && explored[i].y == currentNode.parentNode[1]) {
				return explored[i];
			}
		}

		throw std::runtime_error("Parent Element in vec explored missing");

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

	void checkSides(Node& currentNode) {
		if (currentNode.y > 0) {
			Node top(currentNode.x, currentNode.y - 1, currentNode.x, currentNode.y);
			top.cost = calculate_total_cost(top);
			checkSide(top);

		}

		if (currentNode.y < map[0].size() - 1) {
			Node bottom(currentNode.x, currentNode.y + 1, currentNode.x, currentNode.y);
			bottom.cost = calculate_total_cost(bottom);
			checkSide(bottom);
		}

		if (currentNode.x > 0) {
			Node left(currentNode.x - 1, currentNode.y, currentNode.x, currentNode.y);
			left.cost = calculate_total_cost(left);

			checkSide(left);
		}

		if (currentNode.x < map.size() - 1) {
			Node right(currentNode.x + 1, currentNode.y, currentNode.x, currentNode.y);
			right.cost = calculate_total_cost(right);

			checkSide(right);
		}
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

	void find_char_in_map(char toFind, Node& node) { // used to find start and finish in map
		for (int i = 0; i < map.size(); i++) {
			for (int i2 = 0; i2 < map[i].size(); i2++) {
				if (map[i][i2] == toFind) {
					node.x = i;
					node.y = i2;
				}
			}
		}
	}

	double calculate_total_cost(Node currentNode) {


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
};


