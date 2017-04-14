// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "../../share/interest_points.h"
#include "descriptor.h"

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

	System::Drawing::Pen ^pen = gcnew System::Drawing::Pen(System::Drawing::Color::Red);
	System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);
	img1->draw(g);
	g->SetClip(System::Drawing::Rectangle(0, 0, img1->getWidth(), img1->getHeight()));
	for (int i = 0;i < IP.getCount();i++) {
		float scale = 1.0/IP.P.getScale(IP.getPoint(i).z);
		g->DrawEllipse(pen, IP.getPoint(i).x-scale, IP.getPoint(i).y-scale, (float)3*scale, (float)3*scale);
	}

	interest_points IP2;
	IP2.SIFT(*img2, N, 2, 0);

	g->ResetClip();
	img2->draw(g, img1->getWidth(), 0);
	g->SetClip(System::Drawing::Rectangle(img1->getWidth(), 0, img2->getWidth(), img2->getHeight()));
	for (int i = 0;i < IP2.getCount();i++) {
		float scale = 1.0 / IP2.P.getScale(IP2.getPoint(i).z);
		g->DrawEllipse(pen, img1->getWidth()+IP2.getPoint(i).x - scale, IP2.getPoint(i).y - scale, (float)3 * scale, (float)3 * scale);
	}

	auto D1 = descriptor::GetDescriptors(IP, N, 4);
	auto D2 = descriptor::GetDescriptors(IP2, N, 4);

	auto result = descriptor::Connect(IP, IP2, D1, D2, N, 0.2);

	g->ResetClip();

	for (int i = 0;i < result->size();i++) {
		interest_points::point a = (*result)[i].ptA;
		interest_points::point b = (*result)[i].ptB;
		g->DrawLine(pen, a.x, a.y, b.x + img1->getWidth(), b.y);
	}

	Console::ReadKey();
	
	img1.release();

    return 0;
}
