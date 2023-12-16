#pragma once
#define n 50
#define student097549 2.0096f
#define xi0975249 70.2224f
#define xi0025249 32.5549f
#include <iostream>
#include <fstream>

using namespace std;

class SourceFile
{
public:
	SourceFile()
	{
		//Считываем значения x из файла
		ifstream inputX;
		inputX.open("FILES/x_numbers.txt");
		if (!inputX.is_open())
			cout << "x_numbers.txt is not open\n";
		else
		{
			double temp;
			for (int i = 0; i < n; i++)
			{
				inputX >> temp;
				x[i] = temp;
				cout << "x[" << i + 1 << "]" << x[i] << endl;
			}
		}
		inputX.close();

		//Считываем значения y из файла
		ifstream inputY;
		inputY.open("FILES/y_numbers.txt");
		if (!inputY.is_open())
			cout << "y_numbers.txt is not open\n";
		else
		{
			double temp;
			for (int i = 0; i < n; i++)
			{
				inputY >> temp;
				y[i] = temp;
				cout << "y[" << i + 1 << "] = " << y[i] << endl;
			}
		}
		inputY.close();

		//Считываем табличные значения функции лапласа
		ifstream inputL;
		inputL.open("FILES/LAPLAS.txt");
		if (!inputL.is_open())
			cout << "LAPLAS.txt is not open";
		else
		{
			double temp;
			for (int i = 0; i < 238; i++)
			{
				inputL >> temp;
				zi[i] = temp;
				inputL >> temp;
				phi[i] = temp;
			}
		}
		inputL.close();

		//Считываем табличные квантилей хи квадрат
		ifstream inputK;
		inputK.open("FILES/KVANT_XI.txt");
		if (!inputK.is_open())
			cout << "KVANT_XI.txt is not open";
		else
		{
			double temp;
			for (int i = 0; i < 5; i++)
			{
				inputK >> temp;
				kvant_xi[i] = temp;
			}
		}
		inputK.close();
		//Считываем табличные Стьюдента
		ifstream inputT;
		inputT.open("FILES/KVANT_T.txt");
		if (!inputT.is_open())
			cout << "KVANT_T.txt is not open";
		else
		{
			double temp;
			for (int i = 0; i < 5; i++)
			{
				inputT >> temp;
				kvant_t[i] = temp;
			}
		}
		inputT.close();
	}
	double getX(int position)
	{
		return x[position];
	}
	double getY(int position)
	{
		return y[position];
	}
	double getPhi(double my_zi)
	{
		bool is_neg = false;
		double local_my_zi;
		if (my_zi < 0)
		{
			is_neg = true;
			local_my_zi = -my_zi;
		}
		else
			local_my_zi = my_zi;
		double delta = abs(my_zi - zi[0]);
		double min = delta;
		int pos = 0;
		for (int i = 0; i < 238; i++)
		{
				delta = abs(local_my_zi - zi[i]);
			if (delta < min)
			{
				min = delta;
				pos = i;
			}
		}
		if (!is_neg)
			return phi[pos];
		else
			return -phi[pos];
	}
	double getKvantXi(int position)
	{
		return kvant_xi[position - 1];
	}
private:
	double x[50];
	double y[50];
	double zi[238];
	double phi[238];
	double kvant_xi[5];
	double kvant_t[49];
};

