// lab1itoi.cpp: главный файл проекта.

#include "stdafx.h"
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "../../share/interest_points.h"
#include "descriptor.h"
#include "hough.h"
#include "Engine.h"

#include <Windows.h>

#include <map>

using namespace System;

[System::STAThread]
int main(array<System::String ^> ^args)
{
	FreeConsole();

	course_itoi::Form1 ^F1 = gcnew course_itoi::Form1();

	while (true) {

		System::Windows::Forms::DialogResult dr = F1->ShowDialog();

		if (dr != System::Windows::Forms::DialogResult::OK) return 0;

		int N = F1->N;
		double T = 0.01*F1->T;
		double T1 = 0.01*F1->T1;
		System::String ^input1 = F1->input1;
		System::String ^input2dir = F1->input2;

		course_itoi::Form3 ^F3= gcnew course_itoi::Form3();

		F3->Show();

		System::Collections::Generic::List<Engine::Result^> ^res = Engine::FindContaining(input1, input2dir, N, T, T1);

		F3->Close();

		course_itoi::Form2 ^F2 = gcnew course_itoi::Form2();
		F2->Results = gcnew System::Collections::Generic::List<course_itoi::Form2::Result>();
		for (int i = 0;i < res->Count;i++) {
			F2->Results->Add(course_itoi::Form2::Result(res[i]->fileName,res[i]->x,res[i]->y,res[i]->size,res[i]->angle,res[i]->width,res[i]->height,res[i]->weight));
		}
		F2->ShowDialog();
	}

	//Console::ReadKey();

    return 0;
}
