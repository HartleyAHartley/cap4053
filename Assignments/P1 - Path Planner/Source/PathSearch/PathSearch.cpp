#include "PathSearch.h"
#include <vector>
#include <iostream>
#undef max

namespace ufl_cap4053::searches{

		PathSearch::PathSearch(){

		}

		PathSearch::~PathSearch(){
			delete map;
		}

		auto PathSearch::load(TileMap* _tileMap) -> void{
			stride = _tileMap->getColumnCount();
			depth = _tileMap->getRowCount();
			map = new vertex[stride*depth]();
			bool* visited = new bool[stride*depth]();
			std::cout << stride*depth << std::endl;

			std::vector<const Tile*> queue;
			queue.push_back(_tileMap->getTile(0,0));
			while(!queue.empty()){
				int position = queue.front()->getRow() * stride + queue.front()->getColumn();
				std::cout << position << std::endl;
				std::cout << queue.front()->getRow() << "," << queue.front()->getColumn() << std::endl;
				
				if(visited[position]){
					queue.pop_back();
					continue;
				}
				map[position].tile = queue.front();
				map[position].weight = queue.front()->getWeight();
				visited[position] = true;
				for(const auto& [vertex,_]: queue.front()->getLines()){
					queue.push_back(vertex);
				}
			}
		}

		auto PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol) -> void{

		}

		auto PathSearch::update(long timeslice) -> void{

		}

		auto PathSearch::shutdown() -> void{

		}

		auto PathSearch::unload() -> void{

		}

		auto PathSearch::isDone() const -> bool{
			return true;
		}

		auto  PathSearch::getSolution() const -> std::vector<Tile const*> const {
			return {};
		}
}
