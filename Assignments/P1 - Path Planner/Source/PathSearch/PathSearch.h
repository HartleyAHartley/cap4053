#pragma once
#ifdef max
#undef max // I want to use std::max
#define resetmax
#endif

#include <cstdint>
#include <vector>
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
			uint32_t weight = 0;
			double x = 0;
			double y = 0;
			const Tile* tile = nullptr;
		};
		vertex* map = nullptr;
		int stride = 0;
		int depth = 0;

		int startRow = 0;
		int startCol = 0;
		int goalRow = 0;
		int goalCol = 0;
		double goalX = 0.0;
		double goalY = 0.0;

		std::vector<uint32_t> openSet{};
		bool* inOpenSet = nullptr;
		uint32_t* cameFrom = nullptr;
		double* gScore = nullptr;
		double* fScore = nullptr;
		bool complete = false;

		const int oddColOffsets[6]  = { 0, 1, 1, 1, 0,-1};
		const int oddRowOffsets[6]  = {-1,-1, 0, 1, 1, 0};
		const int evenColOffsets[6] = {-1, 0, 1, 0,-1,-1};
		const int evenRowOffsets[6] = {-1,-1, 0, 1, 1, 0};

		auto heuristic(double x, double y) const -> double;
		static auto timelimit(long timeslice) -> void;
	};
}

#ifdef resetmax
#define max(a ,b) (((a) > (b)) ? (a) : (b)) // put it back for safety
#undef resetmax
#endif
