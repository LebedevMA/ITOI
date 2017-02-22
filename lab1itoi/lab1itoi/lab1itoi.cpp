// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "image.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img = std::unique_ptr<image>(new image());
	img->load(L"input.bmp");
	img->draw();
	img->setKE(image::MIRROR);
	std::unique_ptr<image> img1 = img->Sobel(kernel::SobelKx(), kernel::SobelKy());
	img1->Normalize();

	std::unique_ptr<image> img2 = img1->convolution(kernel::Gauss(1, 19));
	std::unique_ptr<image> img3 = img2->convolution(kernel::Gauss(1, 19).rotate());
	img3->draw();
	Console::ReadKey();
	std::unique_ptr<image> img4 = img1->convolution(kernel::Gauss(3, 19));
	std::unique_ptr<image> img5 = img4->convolution(kernel::Gauss(3, 19).rotate());
	img5->draw();
	Console::ReadKey();
    return 0;
}
