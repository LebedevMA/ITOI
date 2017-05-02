// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "../../share/interest_points.h"
#include "descriptor.h"
#include "ransac.h"

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

	int N = 200;

	interest_points IP;
	IP.SIFT(*img1, N, 2, 0);

	interest_points IP2;
	IP2.SIFT(*img2, N, 2, 0);

	auto D1 = descriptor::GetDescriptors(IP, N, 4);
	auto D2 = descriptor::GetDescriptors(IP2, N, 4);

	auto result = descriptor::Connect(IP, IP2, D1, D2, N, 0.5);

	g->ResetClip();


	ransac::Result res = ransac::Run(*result, 7);

	ransac::matrix PT1 = ransac::Apply(res, 0, 0); System::Drawing::Point Pt1 =  System::Drawing::Point(PT1.V[0][0], PT1.V[1][0]);
	ransac::matrix PT2 = ransac::Apply(res, img2->getWidth(), 0);System::Drawing::Point Pt2 =  System::Drawing::Point(PT2.V[0][0], PT2.V[1][0]);
	ransac::matrix PT3 = ransac::Apply(res, 0, img2->getHeight());System::Drawing::Point Pt3 =  System::Drawing::Point(PT3.V[0][0], PT3.V[1][0]);
	
	System::Object ^arr = gcnew cli::array<System::Drawing::Point>(3);

	((cli::array<System::Drawing::Point> ^)arr)[0] = Pt1;
	((cli::array<System::Drawing::Point> ^)arr)[1] = Pt2;
	((cli::array<System::Drawing::Point> ^)arr)[2] = Pt3;

	System::Drawing::Pen ^pen1 = gcnew System::Drawing::Pen(System::Drawing::Color::Red);


	g->DrawImage(img1->bitmap(), 0, 0);
	g->DrawImage(img2->bitmap(), ((cli::array<System::Drawing::Point> ^)arr));

	Console::ReadKey();
	
	img1.release();

    return 0;
}
