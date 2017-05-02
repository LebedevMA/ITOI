#include "stdafx.h"
#include "Engine.h"

	Engine::Engine()
	{
	}

	System::Collections::Generic::List<Engine::Result^> ^Engine::FindContaining(System::String ^ inputImage, System::String ^ inputDirectory, int N, double T, double T1)
	{
		System::Collections::Generic::List<Result^> ^res = gcnew System::Collections::Generic::List<Result^>();

		System::String ^input1 = inputImage;
		System::String ^input2dir = inputDirectory;

		std::unique_ptr<image> img1 = std::make_unique<image>();
		img1->load(input1);
		img1->setKE(image::REPT);

		interest_points IP;
		IP.SIFT(*img1, N, 2, 0);

		double x0 = img1->getWidth() * 0.5;
		double y0 = img1->getHeight() * 0.5;

		auto D1 = descriptor::GetDescriptors(IP, N, 10);

		for (int i = 0;i < N;i++) {
			D1[i]->centerDistance = sqrt((IP.getPoint(i).x - x0)*(IP.getPoint(i).x - x0) + (IP.getPoint(i).y - y0)*(IP.getPoint(i).y - y0));
			D1[i]->centerAngle = atan2(IP.getPoint(i).y - y0, IP.getPoint(i).x - x0);
		}

		array<System::String ^> ^files = System::IO::Directory::GetFiles(input2dir);

		for (int i = 0;i < files->Length;i++)
		{
			System::String ^input2 = files[i];

			std::unique_ptr<image> img2 = std::make_unique<image>();
			img2->load(input2);
			img2->setKE(image::REPT);


			interest_points IP2;
			IP2.SIFT(*img2, N, 2, 0);

			auto D2 = descriptor::GetDescriptors(IP2, N, 10);

			auto result = descriptor::Connect(IP, IP2, D1, D2, N, T);

			auto param = hough::Find(*result, img1->getWidth(), img1->getHeight(), 0);
			/*
			array<System::Drawing::Point> ^points = gcnew array<System::Drawing::Point>(4);
			points[0] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 0.00) - 0.5*param->height * sin(param->angle + 0.00) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 0.00) + 0.5*param->height * cos(param->angle + 0.00)));
			points[1] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 1.57) - 0.5*param->height * sin(param->angle + 1.57) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 1.57) + 0.5*param->height * cos(param->angle + 1.57)));
			points[2] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 3.14) - 0.5*param->height * sin(param->angle + 3.14) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 3.14) + 0.5*param->height * cos(param->angle + 3.14)));
			points[3] = System::Drawing::Point((float)(param->x + 0.5*param->width * cos(param->angle + 4.72) - 0.5*param->height * sin(param->angle + 4.72) + img1->getWidth()), (float)(param->y + 0.5*param->width * sin(param->angle + 4.72) + 0.5*param->height * cos(param->angle + 4.72)));

			g->DrawPolygon(pen, points);
			*/

			if (param->weight >= T1) {
				res->Add(gcnew Result(input2, param->x, param->y, param->size, param->angle, param->width, param->height, param->weight));
			}

			img2.release();

		}

		img1.release();

		return res;
	}
