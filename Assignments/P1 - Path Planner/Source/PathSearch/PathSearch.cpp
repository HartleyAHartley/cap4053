#include "PathSearch.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>
#undef max


namespace ufl_cap4053::searches{

		PathSearch::PathSearch(){

		}

		PathSearch::~PathSearch(){
			if(map){
				delete[] map;
			}
			if(gScore){
				delete[] gScore;	
			}
			if(fScore){
				delete[] fScore;	
			}
			if(cameFrom){
				delete[] cameFrom;
			}
			if(inOpenSet){
				delete[] inOpenSet;
			}
		}

		auto PathSearch::load(TileMap* _tileMap) -> void{
			stride = _tileMap->getColumnCount();
			depth = _tileMap->getRowCount();
			map = new vertex[stride*depth]();
			std::cout << stride*depth << std::endl;

			for (int row = 0; row < depth; row++) {
				for (int column = 0; column < stride; column++) {
					int position = row * stride + column;
					map[position].tile = _tileMap->getTile(row, column);
					map[position].weight = map[position].tile->getWeight();
					map[position].x = map[position].tile->getXCoordinate();
					map[position].y = map[position].tile->getYCoordinate();
				}
			}
		}

		auto PathSearch::initialize(int _startRow, int _startCol, int _goalRow, int _goalCol) -> void{
			startRow = _startRow;
			startCol = _startCol;
			goalRow = _goalRow;
			goalCol = _goalCol;

			int size = stride*depth;
			openSet.reserve(size);
			inOpenSet = new bool[size];
			std::fill_n(inOpenSet,size,false);

			uint32_t startPos = startRow * stride + startCol;
			goalX = map[startPos].tile->getXCoordinate();
			goalY = map[startPos].tile->getYCoordinate();

			openSet.push_back(startPos);
			
			cameFrom = new uint32_t[size];
			std::fill_n(cameFrom,size,static_cast<uint32_t>(-1));

			gScore = new double[size];
			std::fill_n(gScore,size,std::numeric_limits<double>::infinity());
			gScore[startPos] = 0;

			fScore = new double[stride*depth];
			std::fill_n(fScore,size,std::numeric_limits<double>::infinity());
			fScore[startPos] = heuristic(map[startPos].x,map[startPos].y);
		}

		auto PathSearch::update(long timeslice) -> void{
			std::thread t1(timelimit, timeslice);
			do{
				uint32_t current = openSet.back();
				if(current == (goalRow * stride + goalCol)){
					complete = true;
					break;
				}
				std::pop_heap(openSet.begin(),openSet.end(),[this](const auto& lhs, const auto& rhs){
					return fScore[lhs] < fScore[rhs];
				});
				inOpenSet[current] = false;
				for(int i = 0; i < 6; i++){
					if(map[current].weight == 0){
						continue;
					}
					uint32_t neighbor;
					int row = (current-(current%stride))/stride;
					int col = current%stride;
					if(((current-(current%stride))/stride) % 2 == 0){
						//even
					 	neighbor = (row+evenRowOffsets[i]) * stride + (col+evenColOffsets[i]);
					}else{
						//odd
						neighbor = (row+oddRowOffsets[i]) * stride + (col+oddColOffsets[i]);
					}

					double tentativegScore = gScore[current] + map[neighbor].weight;
					if(tentativegScore < gScore[neighbor]){
						cameFrom[neighbor] = current;
						gScore[neighbor] = tentativegScore;
						fScore[neighbor] = gScore[neighbor] + heuristic(map[neighbor].x,map[neighbor].y);
						if(!inOpenSet[neighbor]){
							openSet.push_back(neighbor);
							inOpenSet[neighbor] = true;
							std::push_heap(openSet.begin(),openSet.end(),[this](const auto& lhs, const auto& rhs){
								return fScore[lhs] < fScore[rhs];
							});
						}
					}
				}
			}while(!openSet.empty() && !t1.joinable());

			if(complete){
				t1.detach();
			}else{
				if(openSet.empty()){
					std::cerr << "err" << std::endl;
				}
				complete = true;
			}

		}

		auto PathSearch::timelimit(long timeslice) -> void{
			std::this_thread::sleep_for(std::chrono::milliseconds(timeslice));
		}

		auto PathSearch::shutdown() -> void{
			if(gScore){
				delete[] gScore;
				gScore = nullptr;	
			}
			if(fScore){
				delete[] fScore;
				fScore = nullptr;	
			}
			if(cameFrom){
				delete[] cameFrom;
				cameFrom = nullptr;
			}
			if(inOpenSet){
				delete[] inOpenSet;
				inOpenSet = nullptr;
			}
			openSet.clear();
			startRow = 0;
			startCol = 0;
			goalRow = 0;
			goalCol = 0;
			complete = false;
		}

		auto PathSearch::unload() -> void{
			shutdown();
			if(map){
				delete[] map;
				map = nullptr;
				stride = 0;
				depth = 0;
			}
		}

		auto PathSearch::isDone() const -> bool{
			return complete;
		}

		auto  PathSearch::getSolution() const -> std::vector<Tile const*> const {
			std::vector<Tile const*> path;
			int current = goalRow * stride + goalCol;
			path.push_back(map[current].tile);
			while(cameFrom[current] != static_cast<uint32_t>(-1)){
				current = cameFrom[current];
				path.insert(path.begin(),map[current].tile);
			}
			return {};
		}

		auto PathSearch::heuristic(double x, double y) const -> double {
			return std::sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));
		}
}
