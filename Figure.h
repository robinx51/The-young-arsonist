#pragma once
#include <fstream>
#include "vector"
using namespace std;

class Figure
{
public:
	Figure();
	
private:
	class Stick {
	public:
		Stick(int coords[4]);
		double GetTime();
		void SetNext(Stick* next);
		void SetPrev(Stick* prev);
		Stick* GetNext();
		Stick* GetPrev();

	private:
		double time;
		int coords[4];
		Stick* next;
		Stick* prev;
	};

	vector<Stick*> arr;

	void OpenFile();
	void ReadFile(ofstream fout);
	void FillFigure(string str);	
};

