#include "PathSearch.h"
#undef max

namespace ufl_cap4053::searches{

		PathSearch::PathSearch(){

		}

		PathSearch::~PathSearch(){

		}

		auto PathSearch::load(TileMap* _tileMap) -> void{

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
