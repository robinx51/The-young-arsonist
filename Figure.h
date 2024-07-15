#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include "vector"
using namespace std;

class Figure
{
public:
	void OpenFile();
	void Print();
	void PrintMatrix();
	float GetTime();
	
private:
	class Stick {
	public:
		Stick(array<int, 2> coords1, array<int, 2> coords2, float time, int num) noexcept;

		void Print();

		int GetNum() const noexcept;
		float GetTime() const noexcept;
		string GetStatus();
		
		array<int, 2> GetCoords1() noexcept;
		array<int, 2> GetCoords2() noexcept;

	private:
		int num;
		float time = 0;
		string status;

		array<int, 2> coords1 = {};
		array<int, 2> coords2 = {};
	};

	float maxTime = 0;
	shared_ptr<Stick> stickMaxTime;

	vector<vector<shared_ptr<Stick>>> matrix;
	vector<vector<int>> matrixInt;

	vector<shared_ptr<Stick>> stickArr;
	vector<array<int, 2>> coordsArr;

	void HandleFile();
	void InitNatrix();
	void FillMatrix();

	int GetNumCoord(array<int, 2> stick);
	shared_ptr<Stick> GetStick(int num);
};