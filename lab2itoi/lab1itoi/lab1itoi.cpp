// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "image.h"
#include "pyramid.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img = std::unique_ptr<image>(new image());
	img->load(L"input.bmp");
	img->setKE(image::MIRROR);

	std::unique_ptr<pyramid> p = std::unique_ptr<pyramid>(new pyramid());

	p->Gen(img,4);

	int count = p->images.size();

	int i = 0;
	while (true) {
		Console::Write("\r                                                                                                     \r");
		p->images[i]->draw();
		Console::Write("oktava: "+(p->inform[i]->oktava)+", sigma: "+(p->inform[i]->sigma));
		ConsoleKeyInfo cki = Console::ReadKey();
		if (cki.Key == ConsoleKey::RightArrow) {
			i++;
			if (i >= count) i = count - 1;
		}
		if (cki.Key == ConsoleKey::LeftArrow) {
			i--;
			if (i < 0) i = 0;
		}
		if (cki.Key == ConsoleKey::Escape) break;
	}

	img.release();

    return 0;
}
