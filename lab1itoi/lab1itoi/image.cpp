#include "stdafx.h"
#include "image.h"
#include <Windows.h>


image::image()
{
	width = 0;
	height = 0;
	KE = BLACK;
}

image::image(const int width, const int height)
{
	this->width = width;
	this->height = height;
	KE = BLACK;
	V = new double[width*height];
	for (int i = 0;i < width*height;i++) {
		V[i] = 0;
	}
}

image::image(const image *a)
{
	width = a->width;
	height = a->height;
	V = new double[width*height];
	for (int i = 0;i < width*height;i++) {
		V[i] = a->V[i];
	}
	KE = a->KE;
}

void image::load(System::String ^filename)
{
	System::Drawing::Image ^img = System::Drawing::Image::FromFile(filename);
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(img);
	width = bmp->Width;
	height = bmp->Height;
	//delete V;
	V = new double[width*height];
	for (int y = 0;y < bmp->Height;y++) {
		for (int x = 0;x < bmp->Width;x++) {
			float c = bmp->GetPixel(x, y).GetBrightness();
			V[x + y*width] = c;
		}
	}
}

void image::save(System::String ^filename)
{
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(width, height);
	for (int y = 0;y < bmp->Height;y++) {
		for (int x = 0;x < bmp->Width;x++) {
			double c = V[x + y*width];
			if (c < 0) c = 0;
			if (c > 1) c = 1;
			bmp->SetPixel(x, y, System::Drawing::Color::FromArgb((int)255 * c, (int)255 * c, (int)255 * c));
		}
	}
	System::Drawing::Image ^img = bmp;
	img->Save(filename,System::Drawing::Imaging::ImageFormat::Bmp);
}

void image::draw() {
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(width, height);
	for (int y = 0;y < height;y++) {
		for (int x = 0;x < width;x++) {
			double c = V[x + y*width];
			if (c < 0) c = 0;
			if (c > 1) c = 1;
			bmp->SetPixel(x, y, System::Drawing::Color::FromArgb((int)255*c, (int)255*c, (int)255*c));
		}
	}
	int x = System::Console::WindowLeft;
	int y = System::Console::WindowTop;

	System::IntPtr handle = System::Diagnostics::Process::GetCurrentProcess()->MainWindowHandle;

	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromHwnd(handle);
	g->DrawImage(bmp, 0, 0, width, height);
}

void image::Normalize() {
	double min, max;
	min = max = V[0];
	for (int i = 1;i < width*height;i++) {
		if (V[i] < min) min = V[i];
		if (V[i] > max) max = V[i];
	}
	double dist = max - min;
	for (int i = 0;i < width*height;i++) {
		V[i] = (V[i] - min)/dist;
	}
}

void image::setKE(const effect ke) {
	KE = ke;
}

double image::getPixel(const int x, const int y) {
	if (width == 0 || height == 0) return 0;
	if (x >= 0 && x < width && y >= 0 && y < height) return V[x + y*width];
	if (x < 0) {
		if (y < 0) {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[0];
				break;
			case MIRROR:
				return getPixel(-x,-y);
				break;
			}
		}
		else if (y < height) {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[0+y*width];
				break;
			case MIRROR:
				return getPixel(-x, y);
				break;
			}
		}
		else {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[width*(height-1)];
				break;
			case MIRROR:
				return getPixel(-x, 2*height-y-2);
				break;
			}
		}
	}
	else if (x < width) {
		if (y < 0) {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[x];
				break;
			case MIRROR:
				return getPixel(x, -y);
				break;
			}
		}
		/*else if (y < height) {
			return V[x + y*width];
		}*/
		else {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[x+width*(height - 1)];
				break;
			case MIRROR:
				return getPixel(x, 2*height-y-2);
				break;
			}
		}
	}
	else {
		if (y < 0) {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[(width-1)];
				break;
			case MIRROR:
				return getPixel(2*width-x-2, -y);
				break;
			}
		}
		else if (y < height) {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[(width-1) + y*width];
				break;
			case MIRROR:
				return getPixel(2*width-x-2, y);
				break;
			}
		}
		else {
			switch (KE) {
			case BLACK:
				return 0;
				break;
			case REPT:
				return V[(width - 1) + width*(height - 1)];
				break;
			case MIRROR:
				return getPixel(2*width-x-2, 2*height-y-2);
				break;
			}
		}
	}
}

void image::setPixel(const int x, const int y, const double c) {
	if (x < 0) return;
	if (x > width) return;
	if (y < 0) return;
	if (y > height) return;
	V[x + y*width] = c;
}

std::unique_ptr<image> image::convolution(const kernel &k) {
	std::unique_ptr<image> out = std::unique_ptr<image>(new image(this));

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double Sum = 0;

			for (int i = 0; i < k.width; i++)
			{
				for (int j = 0; j < k.height; j++)
				{
					int pixelPosX = x + (i - (k.width / 2));
					int pixelPosY = y + (j - (k.height / 2));

					double c = getPixel(pixelPosX, pixelPosY);

					double kernelVal = k.K[i + j*k.width];

					Sum += c * kernelVal;
				}
			}

			out->setPixel(x, y, Sum);
		}
	}

	return out;
}

std::unique_ptr<image> image::Sobel(const kernel Ky, const kernel Kx) {
	std::unique_ptr<image> Gy = this->convolution(Ky);
	std::unique_ptr<image> Gx = this->convolution(Kx);
	std::unique_ptr<image> out = std::unique_ptr<image>(new image(this));
	for (int i = 0;i < width*height;i++) {
		double x = Gx->V[i];
		double y = Gy->V[i];

		int r = sqrt(x*x + y*y);

		out->V[i] = r;
	}
	return out;
}

image::~image()
{
	delete V;
}