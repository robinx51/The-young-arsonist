#include "Figure.h"
#include <sstream>

static bool valueExist(const vector<array<int, 2>>& data, array<int, 2> value) {
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

void Figure::Print()
{
	cout << "Точки: ";
	for (int i = 0; i < stickArr.size(); i++) {
		cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << " ";
	}
	cout << endl << "Спички:" << endl;
	for (int i = 0; i < stickArr.size(); i++) {
		stickArr.at(i).get()->Print();
	}
	cout << "Спичка с максимальным временем горения (" << maxTime << "): " << stickMaxTime.get()->GetNum();

}


void Figure::HandleFile()
{
	ifstream fin;
	fin.open("Code/Files/f.in.txt");
	if (!fin.is_open())
		throw "Ошибка чтения файла!";
	else {
		string line;
		getline(fin, line);
		const int amount = stoi(line);
		if (amount >= 1 && amount <= 40) {
			cout << "Количество спичек: " << amount << endl;
			for (short num = 1; num <= amount; num++) {
				getline(fin, line);
				array<int, 4> coords = {};
				float time;

				// Считывание данных из файла
				{
					stringstream ss(line);
					string word;
					for (int i = 0; i <= 4; i++) {
						// Чтение символов до следующего пробела
						ss >> word;
						if (i == 4) {
							time = stof(word);
						}
						else {
							coords.at(i) = stoi(word);
						}
					}
				}
				//shared_ptr <array<int, 2>> coords1 = make_shared<array<int, 2>>(); // (coords.at(0), coords.at(1));
				//coords1.get()->at(0) = coords.at(0); coords1.get()->at(1) = coords.at(1);
				//shared_ptr <array<int, 2>> coords2 = make_shared<array<int, 2>>();// (coords.at(2), coords.at(3));
				//coords2.get()->at(0) = coords.at(2); coords2.get()->at(1) = coords.at(3);

				array<int, 2> coords1 = { coords.at(0), coords.at(1) };
				array<int, 2> coords2 = { coords.at(2), coords.at(3) };

				// Заполнение вектора, содержащего координаты точек
				if (!valueExist(coordsArr, coords1))
					coordsArr.push_back(coords1); 
				if (!valueExist(coordsArr, coords2))
					coordsArr.push_back(coords2);

				const shared_ptr<Stick> stick = make_shared<Stick>(coords1, coords2, time, num);

				// Заполнение вектора, содержащего указатели на спички
				stickArr.push_back(stick);			

				if (maxTime < time) {
					maxTime = time;
					stickMaxTime = stick;
				}

			}
			//AddCommons();
		}
		else
			throw "Недопустимое количество спичек!";
	}
	fin.close();
}

void Figure::FillMatrix()
{
	InitNatrix();
	for (int stick = 0; stick < stickArr.size(); stick++) {
		
	}
	cout << endl;
}

void Figure::InitNatrix()
{
	const size_t size = coordsArr.size();
	for (int i = 0; i < size; i++) {
		matrix.push_back(vector<shared_ptr<Stick> >(size));
	}
}

Figure::Stick::Stick(array<int, 2> coords1, array<int, 2> coords2, float time, short num)
{
	this->coords1 = coords1;
	this->coords2 = coords2;
	this->time = time;
	this->num = num;
}

void Figure::Stick::Print()
{
	cout << num << ": " << coords1.at(0) << coords1.at(1) << " ";
	cout << coords2.at(0) << coords2.at(1) << " " << time << endl;
}

short Figure::Stick::GetNum()
{
	return num;
}
//void Figure::Stick::AddCommon(array<int, 2> arr, shared_ptr<Stick> stick)
//{
//	commonsMap[arr] = stick;
//}
//
//map<array<int, 2>, shared_ptr<Figure::Stick>> Figure::Stick::GetCommons()
//{
//	return commonsMap;
//}