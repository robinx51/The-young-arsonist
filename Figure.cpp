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
	cout << endl << endl << "Спички:" << endl;
	for (int i = 0; i < stickArr.size(); i++) {
		stickArr.at(i).get()->Print();
	}
	cout << endl << "Спичка с максимальным временем горения (" << maxTime << "): " << stickMaxTime.get()->GetNum() << endl;

}
void Figure::PrintMatrix()
{
	cout << endl << "Матрица смежности:" << endl << "\t";
	for (int i = 0; i < coordsArr.size(); i++) {
		cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << "\t";
	}
	cout << endl;
	for (int i = 0; i < matrixInt.size(); i++) {
		cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << "\t";
		for (int j = 0; j < matrixInt.size(); j++) {
			cout << matrixInt.at(i).at(j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

float Figure::GetTime()
{

	return 0.0f;
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
			FillMatrix();
		}
		else
			throw "Недопустимое количество спичек!";
	}
	fin.close();
}

void Figure::InitNatrix()
{
	const size_t size = coordsArr.size();
	for (int i = 0; i < size; i++) {
		matrixInt.push_back(vector<int>(size));
		matrix.push_back(vector<shared_ptr<Stick>>(size));
	}
}
void Figure::FillMatrix()
{
	InitNatrix();
	for (int stick = 0; stick < stickArr.size(); stick++) {
		shared_ptr<Stick> tempStick = GetStick(stick);
		// Получение номера соординат для поиска в векторе координат
		const int num1 = GetNumCoord(tempStick.get()->GetCoords1() );
		const int num2 = GetNumCoord(tempStick.get()->GetCoords2() );

		// Заполнение матрицы смежности номерами спичек
		matrixInt.at(num1).at(num2) = tempStick.get()->GetNum();
		matrixInt.at(num2).at(num1) = tempStick.get()->GetNum();

		// Заполнение матрицы смежности указателями на спички
		matrix.at(num1).at(num2) = tempStick;
		matrix.at(num2).at(num1) = tempStick;
	}
	cout << endl;
}

shared_ptr<Figure::Stick> Figure::GetStick(int num)
{
	return stickArr.at(num);
}
int Figure::GetNumCoord(array<int, 2> coords)
{
	auto result{ find(begin(coordsArr), end(coordsArr), coords) };
	if (result == end(coordsArr))
		return NULL;
	else
		return (int)(result - coordsArr.begin());
}



Figure::Stick::Stick(array<int, 2> coords1, array<int, 2> coords2, float time, int num) noexcept
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

int Figure::Stick::GetNum() const noexcept
{
	return num;
}
float Figure::Stick::GetTime() const noexcept
{
	return time;
}
string Figure::Stick::GetStatus()
{
	return status;
}

array<int, 2> Figure::Stick::GetCoords1() noexcept
{
	return coords1;
}
array<int, 2> Figure::Stick::GetCoords2() noexcept
{
	return coords2;
}