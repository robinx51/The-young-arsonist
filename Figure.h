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
	void GetTime();
	
private:
	class Stick {
	public:
		Stick(array<float, 2> coords1, array<float, 2> coords2, float time, int num) noexcept;

		void Print();

		void SetStatus(int status) noexcept;
		void SetFire(array<float, 2> coords);
		int GetFire(array<float, 2> coords);
		void SetBurned() noexcept;


		int GetNum() const noexcept;
		float GetTime() const noexcept;
		int GetStatus() const noexcept;

		array<float, 2> GetCoords1() noexcept;
		array<float, 2> GetCoords2() noexcept;
		array<float, 2> GetBurnedCoords() noexcept;

	private:
		int num;
		float time = 0;
		int status;

		array<float, 2> coords1 = {};
		array<float, 2> coords2 = {};
	};

	const string inDirectory = "Code/Files/f.in.txt";
	const string outDirectory = "Code/Files/f.out.txt";

	vector<vector<shared_ptr<Stick>>> matrix;
	vector<vector<int>> matrixInt;

	vector<shared_ptr<Stick>> stickArr;
	vector<array<float, 2>> coordsArr;
	map<array<float, 2>, vector<shared_ptr<Stick>> > coordsMap;

	void HandleFile();
	void PrintResult(float time, array<float, 2> coords);
	void InitNatrix();
	void FillMatrix();
	void InitCross();
	void CalculateTime();

	void BurnedStick(map <shared_ptr<Stick>, float>& currentSticks, map <int, int>& newStatuses, int& burnedSticks, shared_ptr<Stick> stick);
	void FireCoord(array<float, 2> coords, map <shared_ptr<Stick>, float>& currentSticks, map <int, int>& newStatuses);
	void ApplyNewStatuses(map <shared_ptr<Stick>, float>& currentSticks, map <int, int>& newStatuses);

	int GetNumCoord(array<float, 2> stick);
};