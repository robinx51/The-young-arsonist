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
	cout << "�����: ";
	for (int i = 0; i < coordsArr.size(); i++) {
		cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << " ";
	}
	cout << endl << endl << "������:" << endl;
	for (int i = 0; i < stickArr.size(); i++) {
		stickArr.at(i).get()->Print();
	}
	cout << endl << "������ � ������������ �������� ������� (" << maxTime << "): " << stickMaxTime.get()->GetNum() << endl;

}
void Figure::PrintMatrix()
{
	cout << endl << "������� ���������:" << endl << "\t";
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

void Figure::GetTime()
{
	float totalTime = 0.0f;
	deque<shared_ptr<Stick>> history;
	vector <shared_ptr<Stick>> currentSticks;

	array<int, 2> coordsMax = stickMaxTime.get()->GetCoords1();
	array<int, 2> coordsMin = stickMaxTime.get()->GetCoords1();
	shared_ptr<Stick> stickMaxTime;
	float localMaxTime = 0;
	
	// ���������� ������, ����������� � ������ �����
	for (vector<shared_ptr<Stick>> item : matrix) {
		shared_ptr <Stick> stick = item.at(GetNumCoord(coordsMax));
		if (stick != nullptr && stick.get()->GetStatus() != 3)
			currentSticks.push_back(item.at(GetNumCoord(coordsMax)));
	}

	while (true) {
		if (currentSticks.size() == 2) {
			/*vector<array<int, 2>> newCoords;
			for (shared_ptr <Stick> stick : currentSticks) {
				switch (stick.get()->GetStatus()) {
				case 3: {
					BurnedStick(currentSticks, history, stick, stick.get()->GetCoords2(), coordsMin);
					break;
				} case 4: {
					BurnedStick(currentSticks, history, stick, stick.get()->GetCoords1(), coordsMin);
					break;
				} default: {
					break;
				}
				}
			}*/

			float localMinTime = 1000;
			shared_ptr<Stick> stickMinTime;

			for (shared_ptr<Stick> stick : currentSticks) {
				if (stick.get()->GetStatus() == 0) {
					const float localTime = stick.get()->GetTime();
					/*localMaxTime = localTime > localMaxTime ? localTime : localMaxTime;
					localMinTime = localTime < localMinTime ? localTime : localMinTime;*/
					if (localTime > localMaxTime) {
						localMaxTime = localTime;
						stickMaxTime = stick;
					}
					else {
						localMinTime = localTime;
						stickMinTime = stick;
					}
					// ��������� ����� � ���������� ������
					//stick.get()->SetStatus(1);
				}
			}

			// ������ � ����������� �������� �������, � � ������������ ������||���������� ������
			localMaxTime -= localMinTime;
			totalTime += localMinTime;

			array<int, 2> tempCoords1 = stickMinTime.get()->GetCoords1();

			stickMinTime.get()->SetStatus(coordsMin == tempCoords1 ? 3 : 4);

			BurnedStick(currentSticks, history, stickMinTime, (coordsMin == tempCoords1 ? stickMinTime.get()->GetCoords2() : tempCoords1), coordsMin);

			tempCoords1 = stickMaxTime.get()->GetCoords1();
			if (localMaxTime == 0) 
				BurnedStick(currentSticks, history, stickMaxTime, (coordsMin == tempCoords1 ? stickMaxTime.get()->GetCoords2() : tempCoords1), coordsMax);
			else
				stickMaxTime.get()->SetStatus(coordsMax == tempCoords1 ? 1 : 2);

		}
		else if (currentSticks.size() == 1) {
			if (coordsMin == stickMaxTime.get()->GetCoords1() || coordsMin == stickMaxTime.get()->GetCoords2()) {
				totalTime += localMaxTime / 2;
				break;
			}
			else {
				totalTime += localMaxTime;
				break;
			}
		}
		else if (history.size() == stickArr.size())
			break;
		else {
			stringstream ss;
			ss << "���� �� ���������� ����� ���������� ������ �� ����� �����: " << currentSticks.size();

			const string message = ss.str();
			throw message;
		}

	}
	cout << "����������� ����� ������� ������: " << totalTime << endl;
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
			for (short num = 1; num <= amount; num++) {
				getline(fin, line);
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

				const shared_ptr<Stick> stick = make_shared<Stick>(coords1, coords2, time, num);

				// ���������� �������, ����������� ��������� �� ������
				stickArr.push_back(stick);			

				if (maxTime < time) {
					maxTime = time;
					stickMaxTime = stick;
				}

			}
			FillMatrix();
		}
		else
			throw "������������ ���������� ������!";
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
		// ��������� ������ ��������� ��� ������ � ������� ���������
		const int num1 = GetNumCoord(tempStick.get()->GetCoords1() );
		const int num2 = GetNumCoord(tempStick.get()->GetCoords2() );

		// ���������� ������� ��������� �������� ������
		matrixInt.at(num1).at(num2) = tempStick.get()->GetNum();
		matrixInt.at(num2).at(num1) = tempStick.get()->GetNum();

		// ���������� ������� ��������� ����������� �� ������
		matrix.at(num1).at(num2) = tempStick;
		matrix.at(num2).at(num1) = tempStick;
	}
	cout << endl;
}

void Figure::BurnedStick(vector <shared_ptr<Stick>>& currentSticks, deque<shared_ptr<Figure::Stick>>& history, shared_ptr<Stick> stick, array<int, 2> coords, array<int, 2>& coordsMin)
{
	currentSticks.erase(remove(currentSticks.begin(), currentSticks.end(), stick), currentSticks.end());
	history.push_back(stick);
	//newCoords.push_back(coords);
	coordsMin = coords;

	for (vector<shared_ptr<Stick>> item : matrix) {
		shared_ptr <Stick> TempStick = item.at(GetNumCoord(coords));
		if (TempStick != nullptr && TempStick.get()->GetStatus() == 0) {
			currentSticks.push_back(item.at(GetNumCoord(coords)));
		}
	}
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
	status = 0;
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
int Figure::Stick::GetStatus() const noexcept
{					// 0 - ������ �����; 1 - ����� �� ������ ����������; 2 - ����� �� ������ ����������; 
	return status;	// 3 - ������� �� ������ ����������; 4 - ������� �� ������ ����������;
}

void Figure::Stick::SetStatus(int status) noexcept
{
	this->status = status;
}

array<int, 2> Figure::Stick::GetCoords1() noexcept
{
	return coords1;
}
array<int, 2> Figure::Stick::GetCoords2() noexcept
{
	return coords2;
}