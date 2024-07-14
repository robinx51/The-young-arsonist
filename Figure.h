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
	float GetTime();
	
private:
	class Stick {
	public:
		Stick(array<int, 2> coords1, array<int, 2> coords2, float time, short num);
		//void AddCommon(array<int, 2> arr, shared_ptr<Stick> stick);
		//map<array<int, 2>, shared_ptr<Stick>> GetCommons();
		void Print();
		short GetNum();

	private:
		short num;
		float time = 0;
		array<int, 2> coords1 = {};
		array<int, 2> coords2 = {};
		//map<array<int, 2>, shared_ptr<Stick>> commonsMap;
	};

	float maxTime = 0;
	shared_ptr<Stick> stickMaxTime;

	vector<vector<shared_ptr<Stick>>> matrix;

	vector<shared_ptr<Stick>> stickArr;
	vector<array<int, 2>> coordsArr;

	void HandleFile();
	void FillMatrix();
	void InitNatrix();
};