#pragma once
#include <SFML/Graphics.hpp>
#include "SourceFile.h"
#include <cmath>
#include <stdio.h>
#include <iomanip>
#include <sstream>

using namespace sf;

struct Group
{
	int i, ni;
	double begin, end, mid, nin, ninhx;
};

struct GroupVar
{
	double ui, uini, ui2ni;
};

struct Xi2
{
	double zi, phizi, pi, npi, ninpi2npi;
};

class Math
{
public:
	Math()
	{
		table1Image.loadFromFile("FILES/TABLE1.png");
		table1Texture.loadFromImage(table1Image);
		table1Sprite.setTexture(table1Texture);

		table2Image.loadFromFile("FILES/TABLE2.png");
		table2Texture.loadFromImage(table2Image);
		table2Sprite.setTexture(table2Texture);

		table3Image.loadFromFile("FILES/TABLE3.png");
		table3Texture.loadFromImage(table3Image);
		table3Sprite.setTexture(table3Texture);

		table4Image.loadFromFile("FILES/TABLE4.png");
		table4Texture.loadFromImage(table4Image);
		table4Sprite.setTexture(table4Texture);

		table5Image.loadFromFile("FILES/TABLE5.png");
		table5Texture.loadFromImage(table5Image);
		table5Sprite.setTexture(table5Texture);

		table6Image.loadFromFile("FILES/TABLE6.png");
		table6Texture.loadFromImage(table6Image);
		table6Sprite.setTexture(table6Texture);

		if (!font.loadFromFile("FILES/TIMES_RUS.ttf"))
		{
			cout << "FAILED TO LOAD FONT" << endl;
		}

		if (!fontReg.loadFromFile("FILES/TIMES_RUS_REG.ttf"))
		{
			cout << "FAILED TO LOAD FONT" << endl;
		}

		//Скопируем значения из массивов в files в x и y соответственно
		for (int i = 0; i < n; i++)
		{
			x[i] = files.getX(i);
			y[i] = files.getY(i);
		}
		//Установим минимальные и максимальные значения x и y первым значениям для дальнейшего сравнения
		maxX = x[1];
		minX = x[1];
		maxY = y[1];
		minY = y[1];
		//Найдем минимальные и максимальные значения x и y
		for (int i = 0; i < n; i++)
		{
			if (x[i] > maxX)
				maxX = x[i];
			if (y[i] > maxY)
				maxY = y[i];
			if (x[i] < minX)
				minX = x[i];
			if (y[i] < minY)
				minY = y[i];
		}
		cout << "maxX = " << maxX << " maxY = " << maxY << endl;
		cout << "minX = " << minX << " minY = " << minY << endl;
		//Найдем размах
		Rx = maxX - minX;
		Ry = maxY - minY;
		cout << "Rx = " << Rx << " Ry = " << Ry << endl;
		//Вычислим шаг разбиения
		hx = ceil(Rx / r);
		hy = ceil(Ry / r);
		cout << "hx = " << hx << " hy = " << hy << endl;
		//Найдем расширение промежутка разбиения
		increaceX = (hx - (Rx / r)) * r;
		increaceY = (hy - (Ry / r)) * r;
		cout << "increaceX = " << increaceX << " increaceY = " << increaceY << endl;
		//Найдем a0 и b0
		a0 = floor(minX) - floor(increaceX / 2 + 0.001);
		b0 = floor(minY) - floor(increaceY / 2 + 0.001);
		cout << "a0 = " << a0 << " b0 = " << b0 << endl;
		//Найдем значения для группированного ряда x
		for (int k = 0; k < r; k++)
		{
			groupX[k].i = k + 1;
			groupX[k].begin = a0 + hx * k;
			groupX[k].end = a0 + hx * k + hx;
			groupX[k].mid = (groupX[k].begin + groupX[k].end) / 2;
			groupX[k].ni = 0;
			for (int j = 0; j < n; j++)
				if (x[j] >= groupX[k].begin && x[j] < groupX[k].end)
					groupX[k].ni++;
			groupX[k].nin = groupX[k].ni / double(n);
			groupX[k].ninhx = groupX[k].nin / hx;
		}
		//Найдем значения для группированного ряда y
		for (int k = 0; k < r; k++)
		{
			groupY[k].i = k + 1;
			groupY[k].begin = b0 + hy * k;
			groupY[k].end = b0 + hy * k + hy;
			groupY[k].mid = (groupY[k].begin + groupY[k].end) / 2;
			groupY[k].ni = 0;
			for (int j = 0; j < n; j++)
				if (y[j] >= groupY[k].begin && y[j] < groupY[k].end)
					groupY[k].ni++;
			groupY[k].nin = groupY[k].ni / double(n);
			groupY[k].ninhx = groupY[k].nin / hy;
		}
		//Найдем значения группированных данных для условных вариант
		for (int i = 0; i < r; i++)
		{
			groupVarX[i].ui = (groupX[i].mid - groupX[3].mid) / hx;
			groupVarY[i].ui = (groupY[i].mid - groupY[3].mid) / hy;
			groupVarX[i].uini = groupVarX[i].ui * groupX[i].ni;
			groupVarY[i].uini = groupVarY[i].ui * groupY[i].ni;
			groupVarX[i].ui2ni = groupVarX[i].ui * groupVarX[i].uini;
			groupVarY[i].ui2ni = groupVarY[i].ui * groupVarY[i].uini;
		}
		sumni = 0;
		sumuini = 0;
		sumui2ni = 0;
		summi = 0;
		sumvimi = 0;
		sumvi2mi = 0;
		for (int i = 0; i < r; i++)
		{
			sumni += groupX[i].ni;
			sumuini += groupVarX[i].uini;
			sumui2ni += groupVarX[i].ui2ni;
			summi += groupY[i].ni;
			sumvimi += groupVarY[i].uini;
			sumvi2mi += groupVarY[i].ui2ni;
		}
		//Рассчет точечных оценок
		ushtrih = sumuini / double(n);
		vshtrih = sumvimi / double(n);
		u2shtrih = sumui2ni / double(n);
		v2shtrih = sumvi2mi / double(n);
		s2u = double(n) / double(n - 1) * (u2shtrih - ushtrih * ushtrih);
		s2v = double(n) / double(n - 1) * (v2shtrih - vshtrih * vshtrih);
		su = sqrt(s2u);
		sv = sqrt(s2v);
		xshtrih = hx * ushtrih + groupX[3].mid;
		yshtrih = hy * vshtrih + groupY[3].mid;
		s2x = hx * hx * s2u;
		s2y = hy * hy * s2v;
		sx = sqrt(s2x);
		sy = sqrt(s2y);
		//Расчет Xi2
		for (int i = 0; i < r; i++)
		{
			if (i != r - 1)
			{
				xi2X[i].zi = (groupX[i].end - xshtrih) / sx;
				xi2Y[i].zi = (groupY[i].end - yshtrih) / sy;
			}
			else
			{
				xi2X[i].zi = 5;
				xi2Y[i].zi = 5;
			}
			xi2X[i].phizi = files.getPhi(xi2X[i].zi);
			xi2Y[i].phizi = files.getPhi(xi2Y[i].zi);
			//cout << "phizi = " << xi2X[i].phizi << " i = " << groupX[i].i << " zi = " << xi2X[i].zi << endl;
			if (i != 0)
			{
				xi2X[i].pi = files.getPhi(xi2X[i].zi) - files.getPhi(xi2X[i - 1].zi);
				xi2Y[i].pi = files.getPhi(xi2Y[i].zi) - files.getPhi(xi2Y[i - 1].zi);
			}
			else
			{
				xi2X[i].pi = files.getPhi(xi2X[i].zi) + 0.5f;
				xi2Y[i].pi = files.getPhi(xi2Y[i].zi) + 0.5f;
			}
			sumpix += xi2X[i].pi;
			sumpiy += xi2Y[i].pi;
			//cout << "pi = " << xi2X[i].pi << endl;
			xi2X[i].npi = n * xi2X[i].pi;
			xi2Y[i].npi = n * xi2Y[i].pi;
			sumnpix += xi2X[i].npi;
			sumnpiy += xi2Y[i].npi;
		}
		for (int i = 0; i < r; i++)
		{
			nix[i] = groupX[i].ni;
			niy[i] = groupY[i].ni;
			npix[i] = xi2X[i].npi;
			npiy[i] = xi2Y[i].npi;
		}
		for (int i = 0; i < r - 1; i++)
		{
			if (xi2X[i].npi < 5)
			{
				nix[i] = 0;
				npix[i] = 0;
				for (int j = 0; j < r; j++)
				{
					if (xi2X[j].npi >= 5)
					{
						npix[j] += xi2X[i].npi;
						nix[j] += groupX[i].ni;
						break;
					}
				}
			}
			if (xi2X[i].npi >= 5)
				break;
		}
		for (int i = r - 1; i > -1; i--)
		{
			if (xi2X[i].npi < 5)
			{
				nix[i] = 0;
				npix[i] = 0;
				for (int j = r - 1; j > -1; j--)
				{
					if (xi2X[j].npi >= 5)
					{
						npix[j] += xi2X[i].npi;
						nix[j] += groupX[i].ni;
						break;
					}
				}
			}
			if (xi2X[i].npi >= 5)
				break;
		}
		//xi2
		for (int i = 0; i < r - 1; i++)
		{
			if (xi2Y[i].npi < 5)
			{
				niy[i] = 0;
				npiy[i] = 0;
				for (int j = 0; j < r; j++)
				{
					if (xi2Y[j].npi >= 5)
					{
						npiy[j] += xi2Y[i].npi;
						niy[j] += groupY[i].ni;
						break;
					}
				}
			}
			if (xi2Y[i].npi >= 5)
				break;
		}
		for (int i = r - 1; i > -1; i--)
		{
			if (xi2Y[i].npi < 5)
			{
				niy[i] = 0;
				npiy[i] = 0;
				for (int j = r - 1; j > -1; j--)
				{
					if (xi2Y[j].npi >= 5)
					{
						npiy[j] += xi2Y[i].npi;
						niy[j] += groupY[i].ni;
						break;
					}
				}
			}
			if (xi2Y[i].npi >= 5)
				break;
		}
		for (int i = 0; i < r; i++)
		{
			npix2[i] = (nix[i] - npix[i]) * (nix[i] - npix[i]) / npix[i];
			npiy2[i] = (niy[i] - npiy[i]) * (niy[i] - npiy[i]) / npiy[i];
			if (npix[i] != 0)
			{
				sumxix += npix2[i];
				stepx++;
			}
			if (npiy[i] != 0)
			{
				sumxiy += npiy2[i];
				stepy++;
			}
			//cout << "nix = " << nix[i] << " npix = " << npix[i] << endl;
			//cout << "niy = " << niy[i] << " npiy = " << npiy[i] << endl;
		}
		dovlowMX = xshtrih - sx / sqrt(n) * student097549;
		dovlowMY = yshtrih - sy / sqrt(n) * student097549;
		dovhighMX = xshtrih + sx / sqrt(n) * student097549;
		dovhighMY = yshtrih + sy / sqrt(n) * student097549;
		dovlowDX = (n - 1) * s2x / xi0975249;
		dovlowDY = (n - 1) * s2y / xi0975249;
		dovhighDX = (n - 1) * s2x / xi0025249;
		dovhighDY = (n - 1) * s2y / xi0025249;
		for (int i = 0; i < r; i++)
			for (int j = 0; j < r; j++)
				xy[i][j] = 0;
		for (int k = 0; k < n; k++)
			for (int i = 0; i < r; i++)
				for (int j = 0; j < r; j++)
					if (groupX[i].begin <= x[k] && x[k] < groupX[i].end && groupY[j].begin <= y[k] && y[k] < groupY[j].end)
						xy[i][j]++;

		for (int i = 0; i < r; i++)
			for (int j = 0; j < r; j++)
				uvshtrih += groupVarX[i].ui * groupVarY[j].ui * xy[i][j];
		cout << "uvshtrih = " << uvshtrih << endl;
		uvshtrih /= n;
		cout << "uvshtrih = " << uvshtrih << endl;
		correl = n / (n - 1) * (uvshtrih - ushtrih * vshtrih) / su / sv;
	}

	void drawTable1(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table1Sprite.setPosition(0, 40);
		window.draw(table1Sprite);
		for (int i = 0; i < 7; i++)
		{
			text.setString(to_string(groupX[i].i));
			text.setPosition(20, 95 + 26 * i);
			window.draw(text);
			text.setString("[" + roundAndTransformToStringPrecision1(groupX[i].begin) + ", " + roundAndTransformToStringPrecision1(groupX[i].end) + ")");
			text.setPosition(70, 95 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision1(groupX[i].mid));
			text.setPosition(210, 95 + 26 * i);
			window.draw(text);
			text.setString(to_string(groupX[i].ni));
			text.setPosition(320, 95 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(groupX[i].nin));
			text.setPosition(400, 95 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(groupX[i].ninhx));
			text.setPosition(490, 95 + 26 * i);
			window.draw(text);
		}
	}
	void drawTable2(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table2Sprite.setPosition(0, 320);
		window.draw(table2Sprite);
		for (int i = 0; i < 7; i++)
		{
			text.setString(to_string(groupY[i].i));
			text.setPosition(20, 375 + 26 * i);
			window.draw(text);
			text.setString("[" + roundAndTransformToStringPrecision1(groupY[i].begin) + ", " + roundAndTransformToStringPrecision1(groupY[i].end) + ")");
			text.setPosition(65, 375 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision1(groupY[i].mid));
			text.setPosition(210, 375 + 26 * i);
			window.draw(text);
			text.setString(to_string(groupY[i].ni));
			text.setPosition(320, 375 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(groupY[i].nin));
			text.setPosition(400, 375 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(groupY[i].ninhx));
			text.setPosition(490, 375 + 26 * i);
			window.draw(text);
		}
	}
	void drawTable3(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table3Sprite.setPosition(0, 600);
		window.draw(table3Sprite);
		for (int i = 0; i < 7; i++)
		{
			text.setString(to_string(groupX[i].i));
			text.setPosition(20, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarX[i].ui));
			text.setPosition(80, 635 + 26 * i);
			window.draw(text);
			text.setString(to_string(groupX[i].ni));
			text.setPosition(140, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarX[i].uini));
			text.setPosition(200, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarX[i].ui2ni));
			text.setPosition(265, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarY[i].ui));
			text.setPosition(325, 635 + 26 * i);
			window.draw(text);
			text.setString(to_string(groupY[i].ni));
			text.setPosition(390, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarY[i].uini));
			text.setPosition(455, 635 + 26 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(groupVarY[i].ui2ni));
			text.setPosition(520, 635 + 26 * i);
			window.draw(text);
		}
		text.setString(roundAndTransformToStringPrecision0(sumni));
		text.setPosition(140, 640 + 26 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumuini));
		text.setPosition(200, 640 + 26 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumui2ni));
		text.setPosition(265, 640 + 26 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(summi));
		text.setPosition(390, 640 + 26 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumvimi));
		text.setPosition(455, 640 + 26 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumvi2mi));
		text.setPosition(520, 640 + 26 * 7);
		window.draw(text);
	}
	void drawTable4(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table4Sprite.setPosition(0, 1020);
		window.draw(table4Sprite);
		for (int i = 0; i < r; i++)
		{
			text.setString(to_string(groupX[i].i));
			text.setPosition(10, 1090 + 28 * i);
			window.draw(text);
			if (i == 0)
			{
				text.setString(L"(-");
				text.setPosition(35, 1090 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(50, 1110 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
				text.setString(L"," + roundAndTransformToStringPrecision1(groupX[i].end) + L")");
				text.setPosition(75, 1090 + 28 * i);
				window.draw(text);
			}
			else if (i == r - 1)
			{
				text.setString(L"+    )");
				text.setPosition(85, 1090 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(95, 1110 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
				text.setString(L"[" + roundAndTransformToStringPrecision1(groupX[i].begin) + L",");
				text.setPosition(35, 1090 + 28 * i);
				window.draw(text);
			}
			else
			{
				text.setString("[" + roundAndTransformToStringPrecision1(groupX[i].begin) + ", " + roundAndTransformToStringPrecision1(groupX[i].end) + ")");
				text.setPosition(35, 1090 + 28 * i);
				window.draw(text);
			}
			text.setString(to_string(groupX[i].ni));
			text.setPosition(140, 1090 + 28 * i);
			window.draw(text);
			if (i != r - 1)
			{
				text.setString(roundAndTransformToStringPrecision3(xi2X[i].zi));
				text.setPosition(200, 1090 + 28 * i);
				window.draw(text);
			}
			else
			{
				text.setString(L"+");
				text.setPosition(200, 1090 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(220, 1110 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
			}
			text.setString(roundAndTransformToStringPrecision3(xi2X[i].phizi));
			text.setPosition(310, 1090 + 28 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(xi2X[i].pi));
			text.setPosition(450, 1090 + 28 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision2(xi2X[i].npi));
			text.setPosition(590, 1090 + 28 * i);
			window.draw(text);
			if (npix[i] != 0)
			{
				text.setString(roundAndTransformToStringPrecision2(npix2[i]));
				text.setPosition(700, 1090 + 28 * i);
				window.draw(text);
			}
		}
		text.setString(roundAndTransformToStringPrecision0(sumni));
		text.setPosition(140, 1100 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumpix));
		text.setPosition(475, 1100 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumnpix));
		text.setPosition(600, 1100 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(sumxix));
		text.setPosition(700, 1100 + 28 * 7);
		window.draw(text);
	}
	void drawTable5(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table5Sprite.setPosition(0, 1430);
		window.draw(table5Sprite);
		for (int i = 0; i < r; i++)
		{
			text.setString(to_string(groupY[i].i));
			text.setPosition(10, 1500 + 28 * i);
			window.draw(text);
			if (i == 0)
			{
				text.setString(L"(-");
				text.setPosition(35, 1500 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(50, 1520 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
				text.setString(L"," + roundAndTransformToStringPrecision0(groupY[i].end) + L")");
				text.setPosition(75, 1500 + 28 * i);
				window.draw(text);
			}
			else if (i == r - 1)
			{
				text.setString(L"+   )");
				text.setPosition(85, 1500 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(95, 1520 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
				text.setString(L"[" + roundAndTransformToStringPrecision0(groupY[i].begin) + L",");
				text.setPosition(35, 1500 + 28 * i);
				window.draw(text);
			}
			else
			{
				text.setString("[" + roundAndTransformToStringPrecision0(groupY[i].begin) + ", " + roundAndTransformToStringPrecision0(groupY[i].end) + ")");
				text.setPosition(35, 1500 + 28 * i);
				window.draw(text);
			}
			text.setString(to_string(groupY[i].ni));
			text.setPosition(140, 1500 + 28 * i);
			window.draw(text);
			if (i != r - 1)
			{
				text.setString(roundAndTransformToStringPrecision3(xi2Y[i].zi));
				text.setPosition(200, 1500 + 28 * i);
				window.draw(text);
			}
			else
			{
				text.setString(L"+");
				text.setPosition(200, 1500 + 28 * i);
				window.draw(text);
				text.setString(L"8");
				text.setPosition(220, 1520 + 28 * i);
				text.setRotation(-100);
				window.draw(text);
				text.setRotation(0);
			}
			text.setString(roundAndTransformToStringPrecision3(xi2Y[i].phizi));
			text.setPosition(310, 1500 + 28 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision3(xi2Y[i].pi));
			text.setPosition(450, 1500 + 28 * i);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision2(xi2Y[i].npi));
			text.setPosition(590, 1500 + 28 * i);
			window.draw(text);
			if (npiy[i] != 0)
			{
				text.setString(roundAndTransformToStringPrecision2(npiy2[i]));
				text.setPosition(700, 1500 + 28 * i);
				window.draw(text);
			}
		}
		text.setString(roundAndTransformToStringPrecision0(summi));
		text.setPosition(140, 1510 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumpiy));
		text.setPosition(475, 1510 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision0(sumnpiy));
		text.setPosition(600, 1510 + 28 * 7);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(sumxiy));
		text.setPosition(700, 1510 + 28 * 7);
		window.draw(text);
	}
	void drawTable6(RenderWindow& window)
	{
		Text text;
		text.setCharacterSize(18);
		text.setFillColor(Color::Black);
		text.setFont(font);
		table6Sprite.setPosition(0, 2000);
		window.draw(table6Sprite);
		for (int i = 0; i < r; i++)
		{
			text.setString(L"[" + roundAndTransformToStringPrecision0(groupX[i].begin)+L";"+roundAndTransformToStringPrecision0(groupX[i].end) + L")\nx* = " + roundAndTransformToStringPrecision1(groupX[i].mid) + L"\n<" + roundAndTransformToStringPrecision0(groupVarX[i].ui) + L">");
			text.setPosition(5, 2075 + 74 * i);
			window.draw(text);
		}
		for (int i = 0; i < r; i++)
		{
			text.setString(L"[" + roundAndTransformToStringPrecision0(groupY[i].begin) + L";" + roundAndTransformToStringPrecision0(groupY[i].end) + L")\ny* = " + roundAndTransformToStringPrecision0(groupY[i].mid) + L"\n<" + roundAndTransformToStringPrecision0(groupVarY[i].ui) + L">");
			text.setPosition(80 + 80 * i, 2010);
			window.draw(text);
		}
		for (int i = 0; i < r; i++)
			for (int j = 0; j < r; j++)
				if (xy[i][j] != 0)
				{
					text.setString(roundAndTransformToStringPrecision0(xy[i][j]));
					text.setPosition(110 + 80 * j, 2090 + 74 * i);
					window.draw(text);
				}
		double allsums = 0;
		for (int i = 0; i < r; i++)
		{
			double sumx = 0, sumy = 0;
			for (int j = 0; j < r; j++)
			{
				sumx += xy[i][j];
				sumy += xy[j][i];
			}
			allsums += sumx;
			text.setString(roundAndTransformToStringPrecision0(sumy));
			text.setPosition(110 + 80 * i, 2608);
			window.draw(text);
			text.setString(roundAndTransformToStringPrecision0(sumx));
			text.setPosition(670, 2090 + 74 * i);
			window.draw(text);
		}
		text.setString(roundAndTransformToStringPrecision0(allsums));
		text.setPosition(680, 2600);
		window.draw(text);
	}
	void draw(sf::RenderWindow& window)
	{
		Text text;
		text.setFont(font);
		text.setCharacterSize(20);
		text.setFillColor(Color::Black);
		text.setString(L"\tГруппированный ряд для X:");
		text.setPosition(10, 10);
		window.draw(text);
		drawTable1(window);
		text.setString(L"\tГруппированный ряд для Y:");
		text.setPosition(10, 290);
		window.draw(text);
		drawTable2(window);
		text.setString(L"\tГруппированные данные для условных вариант:");
		text.setPosition(10, 570);
		window.draw(text);
		drawTable3(window);
		text.setString(L"\tТочечные оценки:");
		text.setPosition(10, 875);
		window.draw(text);
		text.setString(L"Выборочное среднее x = " + roundAndTransformToStringPrecision2(xshtrih)+ L"  Выборочное среднее y = "+roundAndTransformToStringPrecision2(yshtrih));
		text.setPosition(10, 900);
		window.draw(text);
		text.setString(L"Несмещенное выборочное отклонение x = " + roundAndTransformToStringPrecision2(s2x) + L"  Несмещенное выборочное отклонение y = " + roundAndTransformToStringPrecision2(s2y));
		text.setPosition(10, 925);
		window.draw(text);
		text.setString(L"Несмещенное выборочное квадратичное отклонение x = " + roundAndTransformToStringPrecision2(sx) + L"  Несмещенное выборочное квадратичное отклонение y = " + roundAndTransformToStringPrecision2(sy));
		text.setPosition(10, 950);
		window.draw(text);
		drawTable4(window);
		drawTable5(window);
		text.setString(L"\tЧисло интервалов m = " + roundAndTransformToStringPrecision0(stepx) + L", поэтому число степеней свободы: m - k - 1 = " + roundAndTransformToStringPrecision0(stepx) + " - 2 - 1 = " + roundAndTransformToStringPrecision0(stepx - 3) + L". Квантиль = " + roundAndTransformToStringPrecision2(files.getKvantXi(stepx - 3)) + L", при уровне значимости: 0.05");
		text.setPosition(10, 1335);
		window.draw(text);
		if (files.getKvantXi(stepx - 3) > sumxix)
		{
			text.setString(L"Вывод: гипотеза Ho не противоречит выборочным данным");
			text.setPosition(10, 1360);
			window.draw(text);
		}
		else
		{
			text.setString(L"Вывод: гипотеза Ho противоречит выборочным данным");
			text.setPosition(10, 1360);
			window.draw(text);
		}
		text.setString(L"\tЧисло интервалов m = " + roundAndTransformToStringPrecision0(stepy) + L", поэтому число степеней свободы: m - k - 1 = " + roundAndTransformToStringPrecision0(stepy) + " - 2 - 1 = " + roundAndTransformToStringPrecision0(stepy - 3) + L". Квантиль = " + roundAndTransformToStringPrecision2(files.getKvantXi(stepy - 3)) + L", при уровне значимости: 0.05");
		text.setPosition(10, 1740);
		window.draw(text);
		if (files.getKvantXi(stepy - 3) > sumxiy)
		{
			text.setString(L"Вывод: гипотеза Ho не противоречит выборочным данным");
			text.setPosition(10, 1765);
			window.draw(text);
		}
		else
		{
			text.setString(L"Вывод: гипотеза Ho противоречит выборочным данным");
			text.setPosition(10, 1765);
			window.draw(text);
		}
		text.setString(L"\tДоверительные интервалы M(X) и M(Y):");
		text.setPosition(10, 1790);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(dovlowMX) + " < M(X) < " + roundAndTransformToStringPrecision2(dovhighMX));
		text.setPosition(10, 1815);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(dovlowMY) + " < M(Y) < " + roundAndTransformToStringPrecision2(dovhighMY));
		text.setPosition(10, 1840);
		window.draw(text);
		text.setString(L"\tДоверительные интервалы D(X) и D(Y):");
		text.setPosition(10, 1865);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(dovlowDX) + " < D(X) < " + roundAndTransformToStringPrecision2(dovhighDX));
		text.setPosition(10, 1890);
		window.draw(text);
		text.setString(roundAndTransformToStringPrecision2(dovlowDY) + " < D(Y) < " + roundAndTransformToStringPrecision2(dovhighDY));
		text.setPosition(10, 1915);
		window.draw(text);
		text.setString(L"\tКорреляционная таблица:");
		text.setPosition(10, 1975);
		window.draw(text);
		drawTable6(window);
		text.setString(L"\tВыборочный коеффициент корреляции:");
		text.setPosition(10, 2675);
		window.draw(text);
		text.setString(L"r = "+roundAndTransformToStringPrecision2(correl));
		text.setPosition(10, 2700);
		window.draw(text);
	}
	void printGroupX()
	{
		cout << "________________________________________________________" << endl;
		cout << "| i   | [a i-1, a i)   |  x*i  | n i | n i / n| ni/n*hx|" << endl;
		cout << "________________________________________________________" << endl;
		for (int i = 0; i < r; i++)
		{
			printf("| %-3d | [%-5.1lf, %-5.1lf) | %-5.1lf | %-3d | %-3.4lf | %-5.4lf |\n", groupX[i].i, groupX[i].begin, groupX[i].end, groupX[i].mid, groupX[i].ni, groupX[i].nin, groupX[i].ninhx);
			cout << "_______________________________________________________" << endl;
		}
	}
	void printGroupY()
	{
		cout << "________________________________________________________" << endl;
		cout << "| i   | [b i-1, b i)   |  y*i  | n i | n i / n| ni/n*hy|" << endl;
		cout << "________________________________________________________" << endl;
		for (int i = 0; i < r; i++)
		{
			printf("| %-3d | [%-5.1lf, %-5.1lf) | %-5.1lf | %-3d | %-3.4lf | %-5.4lf |\n", groupY[i].i, groupY[i].begin, groupY[i].end, groupY[i].mid, groupY[i].ni, groupY[i].nin, groupY[i].ninhx);
			cout << "_______________________________________________________" << endl;
		}
	}
	void printGroupVarX()
	{
		cout << "_____________________________________________________________" << endl;
		cout << "| i   | ui  | ni  | uini | ui2ni | vi  | mi  | vimi | vi2mi |" << endl;
		cout << "_____________________________________________________________" << endl;
		for (int i = 0; i < r; i++)
		{
			printf("| %-3d | %-3.0lf | %-3d | %-4.0lf | %-5.0lf | %-3.0lf | %-3d | %-4.0lf | %-5.0lf |\n", groupX[i].i, groupVarX[i].ui, groupX[i].ni, groupVarX[i].uini, groupVarX[i].ui2ni, groupVarY[i].ui, groupY[i].ni, groupVarY[i].uini, groupVarY[i].ui2ni);
			cout << "_____________________________________________________________" << endl;
		}
		printf("| SUM |  -  | %-3d | %-4.0lf | %-5.0lf |  -  | %-3d | %-4.0lf | %-5.0lf |\n", sumni, sumuini, sumui2ni, summi, sumvimi, sumvi2mi);
		cout << "_____________________________________________________________" << endl;
	}
	string roundAndTransformToStringPrecision0(double number)
	{
		stringstream stream;
		stream << fixed << setprecision(0) << setiosflags(ios::left);
		stream << number;
		//cout << stream.str() << endl;
		return stream.str();
	}
	string roundAndTransformToStringPrecision1(double number)
	{
		stringstream stream;
		stream << fixed << setprecision(1) << setiosflags(ios::left);
		stream << number;
		//cout << stream.str() << endl;
		return stream.str();
	}
	string roundAndTransformToStringPrecision2(double number)
	{
		stringstream stream;
		stream << fixed << setprecision(2) << setiosflags(ios::left);
		stream << number;
		//cout << stream.str() << endl;
		return stream.str();
	}
	string roundAndTransformToStringPrecision3(double number)
	{
		stringstream stream;
		stream << fixed << setprecision(3) << setw(6);
		stream << number;
		//cout << stream.str() << endl;
		return stream.str();
	}
private:
	SourceFile files;
	//Найдем по формуле Стерджеса количество интервалов
	int r = ceil(1 + 3.3 * log10(n));
	double x[50], y[50], maxX, minX, maxY, minY, Rx, Ry, hx, hy, increaceX, increaceY, a0, b0;
	double sumni, summi, sumuini, sumvimi, sumui2ni, sumvi2mi;
	double ushtrih, vshtrih, u2shtrih, v2shtrih, s2u, s2v, su, sv, xshtrih, yshtrih, s2x, s2y, sx, sy;
	double sumpix = 0, sumpiy = 0, sumnpix, sumnpiy;
	double npix[7], npiy[7], nix[7], niy[7], npix2[7], npiy2[7];
	double sumxix = 0, sumxiy = 0;
	double stepx = 0, stepy = 0;
	double xy[7][7], uvshtrih, correl;
	double dovlowMX, dovhighMX, dovlowMY, dovhighMY, dovlowDX, dovhighDX, dovlowDY, dovhighDY;
	Group* groupX = (Group*)malloc(r*sizeof(Group));
	Group* groupY = (Group*)malloc(r * sizeof(Group));
	GroupVar* groupVarX = (GroupVar*)malloc(r * sizeof(GroupVar));
	GroupVar* groupVarY = (GroupVar*)malloc(r * sizeof(GroupVar));
	Xi2* xi2X = (Xi2*)malloc(r * sizeof(Xi2));
	Xi2* xi2Y = (Xi2*)malloc(r * sizeof(Xi2));

	Image table1Image;
	Texture table1Texture;
	Sprite table1Sprite;

	Image table2Image;
	Texture table2Texture;
	Sprite table2Sprite;

	Image table3Image;
	Texture table3Texture;
	Sprite table3Sprite;

	Image table4Image;
	Texture table4Texture;
	Sprite table4Sprite;

	Image table5Image;
	Texture table5Texture;
	Sprite table5Sprite;

	Image table6Image;
	Texture table6Texture;
	Sprite table6Sprite;

	sf::Font font;
	sf::Font fontReg;
};

