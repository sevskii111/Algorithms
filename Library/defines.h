#pragma once
#include "string"

typedef int Mark;

struct Edge {
	size_t tail;
	Mark mark;
};

typedef Edge* Data;
//typedef int Data;