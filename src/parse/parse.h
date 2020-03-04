#pragma once
#include "core.h"
#include <list>
#include <string>
#include <vector>
void read_file(std::string name, std::vector<Vertex> &vertex,
               std::list<Edge> &edge, std::list<Face> &face);

void write_file(std::string name, std::vector<Vertex> &vertex,
               std::list<Edge> &edge, std::list<Face> &face);
