#include "stdafx.h"
#include "hough.h"


hough::hough()
{
}

std::unique_ptr<hough::Result>  hough::Find(const std::vector<descriptor::line> &lines, double width1, double height1, double T)
{
	std::map<int, std::map<int, std::map<int, std::map<int, int>>>> space;

	for (int i = 0;i < lines.size();i++) {
		interest_points::point a = lines[i].ptA;
		interest_points::point b = lines[i].ptB;

		double angle1 = lines[i].dB->orientation - lines[i].dA->orientation;
		double angle2 = angle1 + lines[i].dA->centerAngle;
		
		double size = (lines[i].dB->size / lines[i].dA->size);
		double distance = lines[i].dA->centerDistance * size;

		double dx = distance * cos(angle2);
		double dy = distance * sin(angle2);

		double x1 = b.x -  dx;
		double y1 = b.y - dy;

		int idxX = x1 / 50;
		int idxY = y1 / 50;
		int idxSize = log2(size);
		int idxAngle = 32 * (angle1 / 6.28);
		idxAngle %= 32;

		if (space.find(idxX) == space.end()
			|| space[idxX].find(idxY) == space[idxX].end()
			|| space[idxX][idxY].find(idxSize) == space[idxX][idxY].end()
			|| space[idxX][idxY][idxSize].find(idxAngle) == space[idxX][idxY][idxSize].end()) {
			space[idxX][idxY][idxSize][idxAngle] = 0;
		}
		space[idxX][idxY][idxSize][idxAngle]++;
	}

	double resultX = 0, resultY = 0, resultSize = 0, resultAngle = 0;
	int votes = 0;

	for (std::map<int, std::map<int, std::map<int, std::map<int, int>>>>::iterator it = space.begin();it != space.end();++it) {
		for (std::map<int, std::map<int, std::map<int, int>>>::iterator it1 = it->second.begin();it1 != it->second.end();++it1) {
			for (std::map<int, std::map<int, int>>::iterator it2 = it1->second.begin();it2 != it1->second.end();++it2) {
				for (std::map<int, int>::iterator it3 = it2->second.begin();it3 != it2->second.end();++it3) {
					if (it3->second > votes) {
						resultX = it->first;
						resultY = it1->first;
						resultSize = it2->first;
						resultAngle = it3->first;
						votes = it3->second;
					}
				}
			}
		}
	}

	std::unique_ptr<Result> R = std::make_unique<Result>();

	R->x = 50 * resultX + 25;
	R->y = 50 * resultY + 25;
	R->size = pow(0.5, resultSize);
	R->angle = 0.19625 * resultAngle;
	R->width = width1 * R->size;
	R->height = height1 * R->size;
	
	return R;
}


hough::~hough()
{
}
