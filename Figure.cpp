#include "Figure.h"
#include <sstream>

static bool valueExist(const vector<array<int,2> >& data, array<int, 2> value) {
	auto result{ find(begin(data), end(data), value) };
	if (result == end(data))
		return false;
	else
		return true;
}

void Figure::OpenFile()
{
	HandleFile();
}

void Figure::HandleFile()
{
	ifstream fin;
	fin.open("Code/Files/f.in.txt");
	if (!fin.is_open())
		throw "������ ������ �����!";
	else {
		string line;
		getline(fin, line);
		const int amount = stoi(line);
		if (amount >= 1 && amount <= 40) {
			cout << "���������� ������: " << amount << endl;
			while (getline(fin, line)) {
				array<int, 4> coords = {};
				float time;

				// ���������� ������ �� �����
				{
					stringstream ss(line);
					string word;
					for (int i = 0; i <= 4; i++) {
						// ������ �������� �� ���������� �������
						ss >> word;
						if (i == 4) {
							time = stof(word);
						}
						else {
							coords.at(i) = stoi(word);
						}
					}
				}

				array<int, 2> coords1 = { coords.at(0), coords.at(1) };
				array<int, 2> coords2 = { coords.at(2), coords.at(3) };
				// ���������� �������, ����������� ���������� �����
				if (!valueExist(coordsArr, coords1))
					coordsArr.push_back(coords1); 
				if (!valueExist(coordsArr, coords2))
					coordsArr.push_back(coords2);

				const shared_ptr<Stick> stick = make_shared<Stick>(coords1, coords2, time);

				// ���������� �������, ����������� ��������� �� ������
				stickArr.push_back(stick);			


				if (maxTime < time) {
					maxTime = time;
					stickMaxTime = stick;
				}

				cout << line << endl;
			}
		}
		else
			throw "������������ ���������� ������!";
	}
	fin.close();
}

Figure::Stick::Stick(array<int, 2> coords1, array<int, 2> coords2, float time)
{
	this->coords1 = coords1;
	this->coords2 = coords2;
	this->time = time;
}

void Figure::Stick::AddCommon(array<int, 2> arr, shared_ptr<Stick> stick)
{
	commonsMap[arr] = stick;
}
