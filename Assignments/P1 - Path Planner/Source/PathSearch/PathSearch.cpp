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
			stride = _tileMap->getColumnCount()+2;
			depth = _tileMap->getRowCount()+2;
			map = new vertex[(stride)*(depth)]();

			for (int row = 0; row < depth; row++) {
				for (int col = 0; col < stride; col++) {
					int position = row * stride + col;
					if (row == 0 || row == (depth-1) || col == 0 || col == (stride-1)) {
						continue;
					}
					map[position].tile = _tileMap->getTile(row-1, col-1);
					map[position].weight = map[position].tile->getWeight();
					map[position].x = col - ((row + (row&1))>>1);
					map[position].z = row;
					map[position].y	= -map[position].x-map[position].z;
				}
			}
			dist = _tileMap->getTile(0,1)->getXCoordinate() - _tileMap->getTile(0,0)->getXCoordinate();
		}

		auto PathSearch::initialize(int _startRow, int _startCol, int _goalRow, int _goalCol) -> void{
			startRow = _startRow+1;
			startCol = _startCol+1;
			goalRow = _goalRow+1;
			goalCol = _goalCol+1;
			complete = false;

			int size = stride*depth;
			openSet.reserve(size);
			inOpenSet = new bool[size];
			std::fill_n(inOpenSet,size,false);

			uint32_t startPos = startRow * stride + startCol;
			goalX = goalCol - ((goalRow + (goalRow&1))>>1);
			goalZ = goalRow;
			goalY = -goalX-goalZ;

			openSet.push_back(startPos);
			
			cameFrom = new uint32_t[size];
			std::fill_n(cameFrom,size,static_cast<uint32_t>(-1));

			gScore = new int[size];
			std::fill_n(gScore,size,std::numeric_limits<int>::max());
			gScore[startPos] = 0;

			fScore = new int[stride*depth];
			std::fill_n(fScore,size,std::numeric_limits<int>::max());
			fScore[startPos] = heuristic(map[startPos].x,map[startPos].y,map[startPos].z);
		}

		auto PathSearch::update(long timeslice) -> void{
			typedef std::chrono::high_resolution_clock Time;
			typedef std::chrono::milliseconds ms;
			typedef std::chrono::duration<float> fsec;
			auto t0 = Time::now();
			auto t1 = Time::now();
			ms d;
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
				// map[current].tile->setFill(0xffad6b9f);
				for(int i = 0; i < 6; i++){
					uint32_t neighbor;
					int neighborX = map[current].x + cubeDirs[i].x;
					int neighborZ = map[current].z + cubeDirs[i].z;
					int neighborCol = neighborX + (neighborZ + (neighborZ&1) >>1);
					int neighborRow = neighborZ;
					neighbor = neighborRow * stride + neighborCol;
					if(map[neighbor].weight == 0){
						continue;
					}

					
					double tentativegScore = gScore[current] + map[neighbor].weight;
					if(tentativegScore >= gScore[neighbor]){
						continue;
					}

					cameFrom[neighbor] = current;
					gScore[neighbor] = tentativegScore;
					fScore[neighbor] = gScore[neighbor] + heuristic(map[neighbor].x,map[neighbor].y, map[neighbor].z);

					if(inOpenSet[neighbor]){
						continue;
					}

					// map[neighbor].tile->setFill(0xff6430c7);
					openSet.push_back(neighbor);
					inOpenSet[neighbor] = true;
				}
				t1 = Time::now();
			}while(!complete && std::chrono::duration_cast<ms>(t1-t0).count() < timeslice && !openSet.empty());

			if(!complete && openSet.empty()){
				std::cerr << "err" << std::endl;
				complete = true;
			}

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
			// float cost = map[current].weight * dist;
			while(cameFrom[current] != static_cast<uint32_t>(-1)){
				current = cameFrom[current];
				path.push_back(map[current].tile);
				// cost += map[current].weight * dist;
			}
			// cost -= map[current].weight * dist;
			// std::cout << cost << std::endl;
			return path;
		}
}
