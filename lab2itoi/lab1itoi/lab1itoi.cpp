// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	System::Console::SetWindowSize(System::Console::LargestWindowWidth, System::Console::LargestWindowHeight);
	std::unique_ptr<image> img = std::make_unique<image>();
	img->load(L"input.bmp");
	img->setKE(image::MIRROR);

	std::unique_ptr<pyramid> p = std::make_unique<pyramid>();

	p->Gen(img,5,1.6);

	int count = p->images.size();

	int i = 0;
	while (true) {
		Console::Write("\r                                                                                                     \r");
		p->images[i]->draw();
		Console::Write("octave: "+(p->inform[i]->octave)+", sigma: "+(p->inform[i]->sigma));
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
