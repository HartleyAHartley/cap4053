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
			std::cerr << stride*depth << std::endl;

			for (int row = 0; row < depth; row++) {
				for (int column = 0; column < stride; column++) {
					int position = row * stride + column;
					map[position].tile = _tileMap->getTile(row, column);
					map[position].weight = map[position].tile->getWeight();
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
