// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "../../share/interest_points.h"
#include "descriptor.h"
#include "hough.h"

#include <map>

using namespace System;


int main(array<System::String ^> ^args)
{
	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img1 = std::make_unique<image>();
	img1->load(L"input1.bmp");
	img1->setKE(image::REPT);
	std::unique_ptr<image> img2 = std::make_unique<image>();
	img2->load(L"input2.bmp");
	img2->setKE(image::REPT);

	System::IntPtr handle = System::Diagnostics::Process::GetCurrentProcess()->MainWindowHandle;
	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromHwnd(handle);
	g->Clear(System::Drawing::Color::Black);

	int N = 500;

	interest_points IP;
	IP.SIFT(*img1, N, 2, 0);

	img1->draw(g);

	interest_points IP2;
	IP2.SIFT(*img2, N, 2, 0);
	img2->draw(g, img1->getWidth(), 0);

	double x0 = img1->getWidth() * 0.5;
	double y0 = img1->getHeight() * 0.5;


	auto D1 = descriptor::GetDescriptors(IP, N, 10);

	for (int i = 0;i < N;i++) {
		D1[i]->centerDistance = sqrt((IP.getPoint(i).x - x0)*(IP.getPoint(i).x - x0) + (IP.getPoint(i).y - y0)*(IP.getPoint(i).y - y0));
		D1[i]->centerAngle = atan2(IP.getPoint(i).y - y0, IP.getPoint(i).x - x0);
	}

	auto D2 = descriptor::GetDescriptors(IP2, N, 10);

	auto result = descriptor::Connect(IP, IP2, D1, D2, N, 0.7);

	g->ResetClip();


	System::Drawing::Pen ^pen = gcnew System::Drawing::Pen(System::Drawing::Color::Red);

	Random ^rand = gcnew Random();
	for (int i = 0;i < result->size();i++) {

		interest_points::point a = (*result)[i].ptA;
		interest_points::point b = (*result)[i].ptB;
		g->DrawLine(pen, a.x, a.y, b.x + img1->getWidth(), b.y);

	g->SetClip(System::Drawing::Rectangle(0, 0, img1->getWidth(), img1->getHeight()));
		float scale1 = 1.0 / IP.P.getScale((*result)[i].ptA.z);
		g->DrawEllipse(pen, (*result)[i].ptA.x - scale1, (*result)[i].ptA.y - scale1, (float)3 * scale1, (float)3 * scale1);
		g->ResetClip();
	g->SetClip(System::Drawing::Rectangle(img1->getWidth(), 0, img2->getWidth(), img2->getHeight()));
		float scale2 = 1.0 / IP.P.getScale((*result)[i].ptB.z);
		g->DrawEllipse(pen, img1->getWidth()+(*result)[i].ptB.x - scale1, (*result)[i].ptB.y - scale1, (float)3 * scale1, (float)3 * scale1);
	g->ResetClip();
	}

	auto param = hough::Find(*result, img1->getWidth(), img1->getHeight(), 0);

	array<System::Drawing::Point> ^points = gcnew array<System::Drawing::Point>(4);
	points[0] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 0.00) - 0.5*param->height * sin(param->angle + 0.00) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 0.00) + 0.5*param->height * cos(param->angle + 0.00)));
	points[1] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 1.57) - 0.5*param->height * sin(param->angle + 1.57) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 1.57) + 0.5*param->height * cos(param->angle + 1.57)));
	points[2] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 3.14) - 0.5*param->height * sin(param->angle + 3.14) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 3.14) + 0.5*param->height * cos(param->angle + 3.14)));
	points[3] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 4.72) - 0.5*param->height * sin(param->angle + 4.72) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 4.72) + 0.5*param->height * cos(param->angle + 4.72)));

	g->DrawPolygon(pen, points);

	Console::ReadKey();
	
	img1.release();

    return 0;
}
