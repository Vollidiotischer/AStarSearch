#pragma once

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