// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "interest_points.h"
#include "descriptor.h"

using namespace System;



int main(array<System::String ^> ^args)
{
struct line {
	int a, b;
	double d;
	line() {
		a = 0;
		b = 0;
		d = 0;
	}
	line(int a, int b, double d) {
		this->a = a;
		this->b = b;
		this->d = d;
	}
};
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

	int N = 12;

	interest_points IP;
	IP.Moravek(*img1, N, 4, 8);
	System::Drawing::Bitmap ^bmp1 = gcnew System::Drawing::Bitmap(img1->getWidth(), img1->getHeight());
	System::Drawing::Graphics ^g1 = System::Drawing::Graphics::FromImage(bmp1);
	img1->draw(g1);
	for (int i = 0;i < IP.getCount();i++) {
		System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);
		g1->FillRectangle(brush, IP.getPoint(i).x, IP.getPoint(i).y, 4, 4);
	}
	//bmp1->Save("img1.png");
	g->DrawImage(bmp1, 0, 0);

	interest_points IP2;
	IP2.Moravek(*img2, N, 4, 8);
	System::Drawing::Bitmap ^bmp2 = gcnew System::Drawing::Bitmap(img2->getWidth(), img2->getHeight());
	System::Drawing::Graphics ^g2 = System::Drawing::Graphics::FromImage(bmp2);
	img2->draw(g2);
	for (int i = 0;i < IP2.getCount();i++) {
		System::Drawing::Brush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Red);
		g2->FillRectangle(brush, IP2.getPoint(i).x, IP2.getPoint(i).y, 4, 4);
	}
	//bmp2->Save("img2.png");
	g->DrawImage(bmp2, bmp1->Width, 0);

	int R = 16;

	auto D1 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	auto D2 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	
	for (int i = 0;i < N;i++) {
		D1[i] = descriptor::FromPoint(*grad1, IP.getPoint(i), R);
		D2[i] = descriptor::FromPoint(*grad2, IP2.getPoint(i), R);
	}

	auto lines = std::make_unique < line[]>(N*N);

	for (int i = 0;i < N;i++) {
		interest_points::point a = IP.getPoint(i);
		for (int j = 0;j < N;j++) {
			interest_points::point b = IP2.getPoint(j);
			double d = descriptor::Distance(D1[i], D2[j]);
			lines[i*N+j] = line(i, j, d);
		}
	}

	for (int i = 0;i < N*N-1;i++) {
		for (int j = i+1;j < N*N;j++) {
			if (lines[i].d > lines[j].d) {
				line k = lines[i];
				lines[i] = lines[j];
				lines[j] = k;
			}
		}
	}

System::Drawing::Pen ^pen1 = gcnew System::Drawing::Pen(System::Drawing::Color::Red);

auto flags = std::make_unique<double []>(2*N);
for (int i = 0;i < 2*N;i++) {
	flags[i] = true;
}

auto result = std::make_unique<line[]>(N);

	for (int i = 0;i < N*N;i++) {
		if (flags[lines[i].a] == false) continue;
		if (flags[N + lines[i].b] == false) continue;
		flags[lines[i].a] = false;
		flags[N + lines[i].b] = false;
		result[lines[i].a] = lines[i];
	}

	for (int i = 0;i < N;i++) {
		interest_points::point a = IP.getPoint(result[i].a);
		interest_points::point b = IP2.getPoint(result[i].b);
		g->DrawLine(pen1, a.x, a.y, b.x + bmp1->Width, b.y);
	}

	Console::ReadKey();

	img1.release();
	img2.release();

    return 0;
}
