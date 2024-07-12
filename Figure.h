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
	float GetTime();
	
private:
	class Stick {
	public:
		Stick(array<int, 2> coords1, array<int, 2> coords2, float time);
		void AddCommon(array<int, 2> arr, shared_ptr<Stick> stick);

	private:
		float time = 0;
		array<int, 2> coords1 = {};
		array<int, 2> coords2 = {};
		map<array<int, 2>, shared_ptr<Stick>> commonsMap;
	};

	float maxTime = 0;
	shared_ptr<Stick> stickMaxTime;

	vector<shared_ptr<Stick>> stickArr;
	vector<array<int, 2>> coordsArr;

	void HandleFile();
};