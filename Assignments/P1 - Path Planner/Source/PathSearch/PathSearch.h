#pragma once
#ifdef max
#undef max // I want to use std::max
#define resetmax
#endif

#include <stdint.h>
#include "../platform.h"
#include "../Framework/TileSystem/TileMap.h"
#include "../Framework/TileSystem/Tile.h"

namespace ufl_cap4053::searches{
	class DLLEXPORT PathSearch {
	public:
		 PathSearch();
		~PathSearch();
		auto load(TileMap* _tileMap) -> void;
		auto initialize(int startRow, int startCol, int goalRow, int goalCol) -> void;
		auto update(long timeslice) -> void;
		auto shutdown() -> void;
		auto unload() -> void;
		auto isDone() const -> bool;
		auto getSolution() const -> std::vector<Tile const*> const;

	private:
		struct vertex{
			uint8_t weight = 0;
			const Tile* tile = nullptr;
		};
		vertex* map;
		int stride = 0;
		int depth = 0;
	};
}

#ifdef resetmax
#define max(a ,b) (((a) > (b)) ? (a) : (b)) // put it back for safety
#undef resetmax
#endif
