#include "Figure.h"
#include <sstream>
#include <iomanip>

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
// ����� � ������� ������ ����� ������ ����� (��� �������)
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
	CalculateTime();
}

// ���������� � ��������� ������� ������ �� ����� f.in.txt
void Figure::HandleFile()
{
	ifstream fin;
	fin.open(inDirectory);
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

				// ���������� ������� ��������� ����������� ����������
				if (!valueExist(coordsArr, coords1))
					coordsArr.push_back(coords1); 
				if (!valueExist(coordsArr, coords2))
					coordsArr.push_back(coords2);

				// ���������� �������, ����������� ��������� �� ������
				stickArr.push_back(make_shared<Stick>(coords1, coords2, time, num));
			}
			FillMatrix();
		}
		else
			throw "������������ ���������� ������!";
	}
	fin.close();
}

// ������ ���������� ���������� � ���� f.out.txt � ����� ��� ����������� � �������
void Figure::PrintResult(float time, array<int, 2> coords)
{
	std::ofstream out;
	out.open(outDirectory);
	if (out.is_open())
	{
		out << coords.at(0) << " " << coords.at(1) << std::endl;
		out << std::fixed << std::setprecision(2) << time << std::endl;
		std::cout << "���������� ���� �������� � ����" << std::endl;
	}
	out.close();

	ifstream fin;
	fin.open("Code/Files/f.out.txt");
	if (!fin.is_open())
		throw "������ ������ �����!";
	else {
		cout << endl << "���������� ����� f.out.txt:" << endl;
		string line;
		while (getline(fin, line)) {
			std::cout << line << std::endl;
		}
	}
}

// ���������� ������ ��������� ������� ����������
void Figure::InitNatrix()
{
	const size_t size = coordsArr.size();
	for (int i = 0; i < size; i++) {
		matrixInt.push_back(vector<int>(size));
		matrix.push_back(vector<shared_ptr<Stick>>(size));
	}
}
// ���������� ������ ���������: �������� �������� ������, ������� - ����������� �� ������
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

// ���������� ������������ ������� ������� ������
void Figure::CalculateTime()
{
	float minimalTime = std::numeric_limits<float>::max();
	array<int, 2> minimalCoords;
	for (const array<int, 2> coordsStart : coordsArr) {
		float totalTime = 0.0f;

		int coutBurnedSticks = 0;
		map <shared_ptr<Stick>, float> currentSticks;

		// ��������� ������ �����
		FireCoord(coordsStart, currentSticks);

		while (true) {
			if (coutBurnedSticks != stickArr.size()) {
				float localMinTime = std::numeric_limits<int>::max();

				for (const auto& pair : currentSticks) {
					const float localTime = pair.second;

					if (localMinTime > localTime) {
						localMinTime = pair.first.get()->GetStatus() == 5 ? (localTime / 2) : localTime;
					}
				}

				// ������(�) � ����������� �������� �������(�), � ��������� ������||���������� ������
				totalTime += localMinTime;
				// �������� ���������� ������� ��� ������� �� ������������ ������ ��������� ������
				map <shared_ptr<Stick>, float> tempCurrentSticks = currentSticks;
				for (const auto& pair : tempCurrentSticks) {
					if (pair.first.get()->GetStatus() == 5) {
						currentSticks[pair.first] -= localMinTime * 2;
						tempCurrentSticks[pair.first] -= localMinTime * 2;
					}
					else {
						currentSticks[pair.first] -= localMinTime;
						tempCurrentSticks[pair.first] -= localMinTime;
					}
					if (pair.second == 0) {
						BurnedStick(currentSticks, coutBurnedSticks, pair.first);
					}
				}
			}
			else break;
		}
		if (totalTime < minimalTime) {
			minimalTime = totalTime;
			minimalCoords = coordsStart;
		}
		// ��������� ������ ������ ����� ��������� ���������
		for (shared_ptr<Stick> stick : stickArr)
			stick.get()->SetStatus(0);
	}

	cout << "����������� ����� ������� ������: " << minimalTime << endl;
	cout << "����������: " << minimalCoords.at(0) << " " << minimalCoords.at(1) << endl;
	PrintResult(minimalTime, minimalCoords);
}

// ��������� ������ ����� ��������� ������
void Figure::BurnedStick(map<shared_ptr<Stick>,float>& currentSticks, int& burnedSticks, shared_ptr<Stick> stick)
{
	const array<int, 2> coords = stick.get()->GetBurnedCoords();

	stick.get()->SetBurned();
	currentSticks.erase(stick);
	burnedSticks++;

	FireCoord(coords, currentSticks);
}

// ���������� ������, ����������� � ������ �����
void Figure::FireCoord(array<int, 2> coords, map<shared_ptr<Stick>, float>& currentSticks)
{
	for (vector<shared_ptr<Stick>> item : matrix) {
		shared_ptr <Stick> tempStick = item.at(GetNumCoord(coords));
		if (tempStick != nullptr) {
			if (tempStick.get()->GetStatus() == 0) {
				currentSticks[tempStick] = tempStick.get()->GetTime();
				tempStick.get()->SetFire(coords);
			}
			else if ((coords == tempStick.get()->GetCoords1() && tempStick.get()->GetStatus() == 2)
				|| (coords == tempStick.get()->GetCoords2() && tempStick.get()->GetStatus() == 1))
			{
				tempStick.get()->SetStatus(5);
			}
		}
	}
}

// ��������� ������ �� ������� � ������� ���� ������
shared_ptr<Figure::Stick> Figure::GetStick(int num)
{
	return stickArr.at(num);
}

// ��������� ������� ���������� ����������� � ������� ���� ���������
int Figure::GetNumCoord(array<int, 2> coords)
{
	auto result{ find(begin(coordsArr), end(coordsArr), coords) };
	if (result == end(coordsArr))
		return NULL;
	else
		return (int)(result - coordsArr.begin());
}



// ����������� ������� ������
Figure::Stick::Stick(array<int, 2> coords1, array<int, 2> coords2, float time, int num) noexcept
{
	this->coords1 = coords1;
	this->coords2 = coords2;
	this->time = time;
	this->num = num;
	status = 0;
}

// ����� � ������� ������ ������
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

array<int, 2> Figure::Stick::GetCoords1() noexcept
{
	return coords1;
}
array<int, 2> Figure::Stick::GetCoords2() noexcept
{
	return coords2;
}

void Figure::Stick::SetStatus(int status) noexcept
{
	this->status = status;
}

// ��������� ����� ���������� ������
void Figure::Stick::SetFire(array<int, 2> coords)
{
	this->SetStatus(coords == this->GetCoords1() ? 1 : 2);
}

// ��������� ����� �������� ������
void Figure::Stick::SetBurned() noexcept
{
	this->SetStatus(this->GetStatus() == 1 ? 3 : 4);
}

// ��������� ���������� ����� �������� ������
array<int, 2> Figure::Stick::GetBurnedCoords() noexcept
{
	return GetStatus() == 1 ? GetCoords2() : GetCoords1();
}