// lab1itoi.cpp: ������� ���� �������.

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
	img1->setKE(image::MIRROR);

	std::unique_ptr<image> grad1 = img1->Sobel();

	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img2 = std::make_unique<image>();
	img2->load(L"input2.bmp");
	img2->setKE(image::MIRROR);

	std::unique_ptr<image> grad2 = img2->Sobel();

	System::IntPtr handle = System::Diagnostics::Process::GetCurrentProcess()->MainWindowHandle;
	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromHwnd(handle);
	g->Clear(System::Drawing::Color::Black);

	int N = 100;

	interest_points IP;
	IP.Harris(*img1, N, 4, 2);
	System::Drawing::Bitmap ^bmp1 = gcnew System::Drawing::Bitmap(img1->getWidth(), img1->getHeight());
	System::Drawing::Graphics ^g1 = System::Drawing::Graphics::FromImage(bmp1);
	img1->draw(g1);
	g->DrawImage(bmp1, 0, 0);

	interest_points IP2;
	IP2.Harris(*img2, N, 4, 2);
	System::Drawing::Bitmap ^bmp2 = gcnew System::Drawing::Bitmap(img2->getWidth(), img2->getHeight());
	System::Drawing::Graphics ^g2 = System::Drawing::Graphics::FromImage(bmp2);
	img2->draw(g2);
	g->DrawImage(bmp2, bmp1->Width, 0);

	auto D1 = descriptor::GetDescriptors(IP, *img1, N, 16);
	auto D2 = descriptor::GetDescriptors(IP2, *img2, N, 16);

	std::unique_ptr < std::vector < descriptor::line >> result = descriptor::Connect(IP, IP2, D1, D2, N, 0.8);

System::Drawing::Pen ^pen1 = gcnew System::Drawing::Pen(System::Drawing::Color::Red);
System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);


	for (int i = 0;i < result->size();i++) {
			interest_points::point a = (*result)[i].ptA;
			interest_points::point b = (*result)[i].ptB;
			g->DrawLine(pen1, a.x, a.y, b.x + bmp1->Width, b.y);

			g->FillRectangle(brush, a.x, a.y, 4, 4);
			g->FillRectangle(brush, b.x + bmp1->Width, b.y, 4, 4);
	}

	Console::ReadKey();

	img1.release();
	img2.release();

    return 0;
}
