#pragma once
#ifdef max
#undef max // I want to use std::max
#define resetmax
#endif

#include <cstdint>
#include <vector>
#include <cmath>
#include "../platform.h"
#include "../Framework/TileSystem/TileMap.h"
#include "../Framework/TileSystem/Tile.h"

namespace ufl_cap4053::searches{
	class  PathSearch {
	public:
		DLLEXPORT PathSearch();
		DLLEXPORT ~PathSearch();
		DLLEXPORT auto load(TileMap* _tileMap) -> void;
		DLLEXPORT auto initialize(int startRow, int startCol, int goalRow, int goalCol) -> void;
		DLLEXPORT auto update(long timeslice) -> void;
		DLLEXPORT auto shutdown() -> void;
		DLLEXPORT auto unload() -> void;
		DLLEXPORT auto isDone() const -> bool;
		DLLEXPORT auto getSolution() const -> std::vector<Tile const*> const;

	private:
		struct vertex{
			uint32_t weight = 0;
			int x = 0;
			int y = 0;
			int z = 0;
			Tile* tile = nullptr;
		};
		vertex* map = nullptr;
		int stride = 0;
		int depth = 0;

		int startRow = 0;
		int startCol = 0;
		int goalRow = 0;
		int goalCol = 0;
		int goalX = 0;
		int goalY = 0;
		int goalZ = 0;

		double dist = 0;

		std::vector<uint32_t> openSet{};
		bool* inOpenSet = nullptr;
		uint32_t* cameFrom = nullptr;
		int* gScore = nullptr;
		int* fScore = nullptr;
		bool complete = true;
		size_t openSetSize = 0;

		struct xyz {
			int x;
			int y;
			int z;
		};
		const xyz cubeDirs[6] = {
			{ 1,-1, 0}, { 1, 0,-1}, { 0, 1,-1}, 
    	{-1, 1, 0}, {-1, 0,+1}, { 0,-1, 1}
		};

		inline auto heuristic(int x, int y, int z) const -> int {
			return (abs(goalX - x) + abs(goalY - y) + abs(goalZ - z)) >> 1;
		}
	};
}

#ifdef resetmax
#define max(a ,b) (((a) > (b)) ? (a) : (b)) // put it back for safety
#undef resetmax
#endif
