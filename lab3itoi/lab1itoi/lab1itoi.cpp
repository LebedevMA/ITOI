// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "interest_points.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img = std::make_unique<image>();
	img->load(L"input.bmp");
	img->setKE(image::MIRROR);

	System::IntPtr handle = System::Diagnostics::Process::GetCurrentProcess()->MainWindowHandle;
	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromHwnd(handle);
	
	g->Clear(System::Drawing::Color::Black);
	
	interest_points IP;

	IP.Harris(*img, 100, 2, 8);


	System::Drawing::Bitmap ^img1 = gcnew System::Drawing::Bitmap(img->getWidth(),img->getHeight());
	//img->draw(g);
	System::Drawing::Graphics ^g1 = System::Drawing::Graphics::FromImage(img1);
	img->draw(g1);
	for (int i = 0;i < IP.getCount();i++) {
		System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);
		//g->FillRectangle(brush, IP.getPoint(i).x, IP.getPoint(i).y, 4, 4);
		g1->FillRectangle(brush, IP.getPoint(i).x, IP.getPoint(i).y, 4, 4);
	}
	img1->Save("img1.png");

	//Console::ReadKey();

	g->Clear(System::Drawing::Color::Black);

	interest_points IP2;

	IP2.Moravek(*img, 100, 2, 8);

	System::Drawing::Bitmap ^img2 = gcnew System::Drawing::Bitmap(img->getWidth(), img->getHeight());
	//img->draw(g);
	System::Drawing::Graphics ^g2 = System::Drawing::Graphics::FromImage(img2);
	img->draw(g2);
	for (int i = 0;i < IP2.getCount();i++) {
		System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);
		//g->FillRectangle(brush, IP2.getPoint(i).x, IP2.getPoint(i).y, 4, 4);
		g2->FillRectangle(brush, IP2.getPoint(i).x, IP2.getPoint(i).y, 4, 4);
	}
	img2->Save("img2.png");

	//Console::ReadKey();

	img.release();

    return 0;
}
