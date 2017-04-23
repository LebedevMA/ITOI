#include "stdafx.h"
#include "hough.h"


hough::hough()
{
}

std::unique_ptr<hough::Result>  hough::Find(const std::vector<descriptor::line> &lines, double width1, double height1, double T)
{
	std::vector<std::vector<int>> vectors;
	std::map<int, int> paramX, paramY, paramSize, paramAngle;

	for (int i = 0;i < lines.size();i++) {
		interest_points::point a = lines[i].ptA;
		interest_points::point b = lines[i].ptB;

		double angle1 = lines[i].dB->orientation - lines[i].dA->orientation;
		double angle2 = angle1 + lines[i].dA->centerAngle;
		double size = (lines[i].dB->size / lines[i].dA->size);
		double distance = lines[i].dA->centerDistance * size;

		double dx = distance * cos(angle2);
		double dy = distance * sin(angle2);

		double x1 = b.x - dx;
		double y1 = b.y - dy;

		int idxX = x1 / 50;
		int idxY = y1 / 50;
		int idxSize = log2(size);
		int idxAngle = 8 * (angle1 / 6.28);
		idxAngle += 8;
		idxAngle %= 8;

		if (paramX.find(idxX) == paramX.end()) paramX[idxX] = 0;
		paramX[idxX]++;
		if (paramY.find(idxY) == paramY.end()) paramY[idxY] = 0;
		paramY[idxY]++;
		if (paramSize.find(idxSize) == paramSize.end()) paramSize[idxSize] = 0;
		paramSize[idxSize]++;
		if (paramAngle.find(idxAngle) == paramAngle.end()) paramAngle[idxAngle] = 0;
		paramAngle[idxAngle]++;

		std::vector<int> v;
		v.push_back(idxX);
		v.push_back(idxY);
		v.push_back(idxSize);
		v.push_back(idxAngle);

		vectors.push_back(v);
	}

	double resultX = 0, resultY = 0, resultSize = 0, resultAngle = 0;
	int votesX = 0, votesY = 0, votesSize = 0, votesAngle = 0;

	for (std::map<int, int>::iterator it = paramX.begin();it != paramX.end();++it) {

		if (it->second > votesX) {
			resultX = it->first;
			votesX = it->second;
		}
	}

	for (std::map<int, int>::iterator it = paramY.begin();it != paramY.end();++it) {
		if (it->second > votesY) {
			resultY = it->first;
			votesY = it->second;
		}
	}

	for (std::map<int, int>::iterator it = paramSize.begin();it != paramSize.end();++it) {

		if (it->second > votesSize) {
			resultSize = it->first;
			votesSize = it->second;
		}
	}

	for (std::map<int, int>::iterator it = paramAngle.begin();it != paramAngle.end();++it) {
		
		if (it->second > votesAngle) {
			resultAngle = it->first;
			votesAngle = it->second;
		}
	}

	double inliners = 0;

	for (int i = 0;i < vectors.size();i++) {
		double d = 0;
		d += (resultX - vectors[i][0])*(resultX - vectors[i][0]);
		d += (resultY - vectors[i][1])*(resultY - vectors[i][1]);
		d += (resultSize - vectors[i][2])*(resultSize - vectors[i][2]);
		d += (resultAngle - vectors[i][3])*(resultAngle - vectors[i][3]);
		d = sqrt(d);
		if (d <= T) inliners++;
	}

	if (vectors.size() > 0) inliners /= vectors.size();

	std::unique_ptr<Result> R = std::make_unique<Result>();

	R->x = 50 * resultX + 25;
	R->y = 50 * resultY + 25;
	R->size = pow(0.5, resultSize);
	R->angle = 0.785 * resultAngle;
	R->width = width1 * R->size;
	R->height = height1 * R->size;
	R->inliners = inliners;
	
	return R;
}


hough::~hough()
{
}
