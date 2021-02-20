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

			tileDist = _tileMap->getTile(0, 1)->getXCoordinate() - _tileMap->getTile(0, 0)->getXCoordinate();

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
			complete = false;

			int size = stride*depth;
			openSet.reserve(size);
			inOpenSet = new bool[size];
			std::fill_n(inOpenSet,size,false);

			uint32_t startPos = startRow * stride + startCol;
			goalX = map[goalRow * stride + goalCol].tile->getXCoordinate();
			goalY = map[goalRow * stride + goalCol].tile->getYCoordinate();

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
			halt = false;
			std::thread t1(&PathSearch::timelimit, this, timeslice);
			do{

				std::pop_heap(openSet.begin(), openSet.end(), [this](const auto& lhs, const auto& rhs) {
					return fScore[lhs] > fScore[rhs];
				});
				uint32_t current = openSet.back();
				openSet.pop_back();

				if(current == (goalRow * stride + goalCol)){
					complete = true;
					break;
				}

				inOpenSet[current] = false;
				map[current].tile->setFill(0xffad6b9f);
				for(int i = 0; i < 6; i++){
					if(map[current].weight == 0){
						continue;
					}
					uint32_t neighbor;
					int row = (current-(current%stride))/stride;
					int col = current%stride;
					if(((current-(current%stride))/stride) % 2 == 0){ //even
						if (row + evenRowOffsets[i] < 0 || col + evenColOffsets[i] < 0 || row + evenRowOffsets[i] >= depth || col + evenColOffsets[i] >= stride) {
							continue;
						}
					 	neighbor = (row+evenRowOffsets[i]) * stride + (col+evenColOffsets[i]);
					}else{ //odd
						if (row + oddRowOffsets[i] < 0 || col + oddColOffsets[i] < 0 || row + oddRowOffsets[i] >= depth || col + oddColOffsets[i] >= stride) {
							continue;
						}
						neighbor = (row+oddRowOffsets[i]) * stride + (col+oddColOffsets[i]);
					}
					if (neighbor >= stride * depth || map[neighbor].weight == 0) {
						continue;
					}

					
					double tentativegScore = gScore[current] + tileDist*(map[current].weight + map[neighbor].weight)/2.;
					if(tentativegScore < gScore[neighbor]){
						cameFrom[neighbor] = current;
						gScore[neighbor] = tentativegScore;
						fScore[neighbor] = gScore[neighbor] + 1.20 * heuristic(map[neighbor].x,map[neighbor].y);
						if(!inOpenSet[neighbor]){
							map[neighbor].tile->setFill(0xff6430c7);
							openSet.push_back(neighbor);
							std::push_heap(openSet.begin(), openSet.end(), [this](const auto& lhs, const auto& rhs) {
								return fScore[lhs] > fScore[rhs];
							});
							inOpenSet[neighbor] = true;
						}
					}
				}
			}while(!openSet.empty() && !halt && timeslice != 0);

			if(complete || timeslice == 0){
				t1.detach();
			}else{
				if(openSet.empty()){
					std::cerr << "err" << std::endl;
					complete = true;
				}
				t1.join();
			}

		}

		auto PathSearch::timelimit(long timeslice) -> void{
			std::this_thread::sleep_for(std::chrono::milliseconds(timeslice));
			halt = true;
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
		}

		auto PathSearch::unload() -> void{
			shutdown();
			if(map){
				delete[] map;
				map = nullptr;
			}
		}

		auto PathSearch::isDone() const -> bool{
			return complete;
		}

		auto  PathSearch::getSolution() const -> std::vector<Tile const*> const {
			std::vector<Tile const*> path;
			int current = goalRow * stride + goalCol;
			path.push_back(map[current].tile);
			map[current].tile->setFill(0xffff0000);
			while(cameFrom[current] != static_cast<uint32_t>(-1)){
				map[current].tile->addLineTo(map[cameFrom[current]].tile, 0x00);
				current = cameFrom[current];
				path.push_back(map[current].tile);
			}
			map[current].tile->setFill(0xff008000);
			return path;
		}

		auto PathSearch::heuristic(double x, double y) const -> double {
			return std::sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));
		}
}
