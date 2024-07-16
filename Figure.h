#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <deque>
#include "vector"
using namespace std;

class Figure
{
public:
	void OpenFile();
	void Print();
	void PrintMatrix();
	void GetTime();
	
private:
	class Stick {
	public:
		Stick(array<int, 2> coords1, array<int, 2> coords2, float time, int num) noexcept;

		void Print();

		void SetStatus(int status) noexcept;
		void SetFire(array<int, 2> coords);
		void SetBurned() noexcept;
		void SetBurned(array<int, 2> coords);


		int GetNum() const noexcept;
		float GetTime() const noexcept;
		int GetStatus() const noexcept;

		array<int, 2> GetCoords1() noexcept;
		array<int, 2> GetCoords2() noexcept;
		array<int, 2> GetOtherCoords(array<int, 2> coords);
		array<int, 2> GetBurnedCoords() noexcept;

	private:
		int num;
		float time = 0;
		int status;

		array<int, 2> coords1 = {};
		array<int, 2> coords2 = {};
	};

	vector<vector<shared_ptr<Stick>>> matrix;
	vector<vector<int>> matrixInt;

	vector<shared_ptr<Stick>> stickArr;
	vector<array<int, 2>> coordsArr;

	void HandleFile();
	void PrintResult(float time, array<int, 2> coords);
	void InitNatrix();
	void FillMatrix();
	void BurnedStick(map <shared_ptr<Stick>, float>& currentSticks, deque<shared_ptr<Figure::Stick>>& history, shared_ptr<Stick> stick);

	int GetNumCoord(array<int, 2> stick);
	shared_ptr<Stick> GetStick(int num);
};