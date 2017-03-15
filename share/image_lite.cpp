#include "stdafx.h"
#include "image_lite.h"


image_lite::image_lite()
{
	width = 0;
	height = 0;
	V = nullptr;
}

image_lite::image_lite(const std::unique_ptr<image> &a)
{
	width = a->getWidth();
	height = a->getHeight();
	V = new unsigned char[width*height];
	for (int i = 0;i < width*height;i++) {
		V[i] = (unsigned char)(255 * a->getElement(i));
	}
}

void image_lite::draw() {
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(width, height);
	for (int y = 0;y < height;y++) {
		for (int x = 0;x < width;x++) {
			unsigned char c = V[x + y*width];
			if (c < 0) c = 0;
			if (c > 255) c = 255;
			bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(c, c, c));
		}
	}
	int x = System::Console::WindowLeft;
	int y = System::Console::WindowTop;

	System::IntPtr handle = System::Diagnostics::Process::GetCurrentProcess()->MainWindowHandle;

	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromHwnd(handle);
	g->Clear(System::Drawing::Color::Black);
	g->DrawImage(bmp, 0, 0, width, height);
}

image_lite::~image_lite()
{
	//delete V;
}
