#include "Figure.h"
#include <sstream>
#include <iomanip>

static bool valueExist(const vector<array<float, 2>>& data, array<float, 2> value) {
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
	std::cout << "�����: ";
	for (int i = 0; i < coordsArr.size(); i++) {
		std::cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << " ";
	}
	std::cout << endl << endl << "������:" << endl;
	for (int i = 0; i < stickArr.size(); i++) {
		stickArr.at(i).get()->Print();
	}
	std::cout << endl << "������� ���������:" << endl << "\t";
	for (int i = 0; i < coordsArr.size(); i++) {
		std::cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << "\t";
	}
	std::cout << endl;
	for (int i = 0; i < matrixInt.size(); i++) {
		std::cout << coordsArr.at(i).at(0) << coordsArr.at(i).at(1) << "\t";
		for (int j = 0; j < matrixInt.size(); j++) {
			std::cout << matrixInt.at(i).at(j) << "\t";
		}
		std::cout << endl;
	}
	std::cout << endl;
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
			std::cout << "���������� ������: " << amount << endl;
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

				array<float, 2> coords1 = { coords.at(0), coords.at(1) };
				array<float, 2> coords2 = { coords.at(2), coords.at(3) };

				// ���������� ������� ��������� ����������� ����������
				if (!valueExist(coordsArr, coords1))
					coordsArr.push_back(coords1); 
				if (!valueExist(coordsArr, coords2))
					coordsArr.push_back(coords2);

				shared_ptr<Stick> stick = make_shared<Stick>(coords1, coords2, time, num);

				// ���������� �������, ����������� ��������� �� ������
				stickArr.push_back(stick);

				// ���������� ������� ���������
				{
					if (coordsMap.find(coords1) != coordsMap.end()) {
						vector<shared_ptr<Stick>> tempVec1 = coordsMap[coords1];
						tempVec1.push_back(stick);
						coordsMap[coords1] = tempVec1;
					}
					else {
						vector<shared_ptr<Stick>> tempVec = { stick };
						coordsMap[coords1] = tempVec;
					}
					if (coordsMap.find(coords2) != coordsMap.end()) {
						vector<shared_ptr<Stick>> tempVec2 = coordsMap[coords2];
						tempVec2.push_back(stick);
						coordsMap[coords2] = tempVec2;
					}
					else {
						vector<shared_ptr<Stick>> tempVec = { stick };
						coordsMap[coords2] = tempVec;
					}
				}
			}
			FillMatrix();
			InitCross();
			for (const auto& pair : coordsMap) {
				if (pair.second.size() < 2)
					throw "���������� ��������� ������!";
			}
		}
		else
			throw "������������ ���������� ������!";
	}
	fin.close();
}

// ������ ���������� ���������� � ���� f.out.txt � ����� ��� ����������� � �������
void Figure::PrintResult(float time, array<float, 2> coords)
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
		shared_ptr<Stick> tempStick = stickArr.at(stick);
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
	std::cout << endl;
}

// ����������� �������������� ������ � �� ���������
void Figure::InitCross()
{
	for (const auto& pair : coordsMap) {
		for (float i = -1; i < 2; i += 2) {
			for (float j = -1; j < 2; j += 2) {
				vector<array <float, 2>> coordsVec = { pair.first };
				coordsVec.at(0).at(0) += i; 
				coordsVec.at(0).at(1) += j;

				// �������� ��������� �� ������, ����������� �� ������ �����������
				vector<shared_ptr<Stick>> sticks = { matrix.at(GetNumCoord(pair.first)).at(GetNumCoord(coordsVec.at(0))) };
				if (coordsMap.find(coordsVec.at(0)) != coordsMap.end() && sticks.at(0) != nullptr && sticks.at(0).get()->GetStatus() != 5) {
					coordsVec.push_back(coordsVec.at(0));
					coordsVec.at(1).at(0) -= i;
					coordsVec.at(0).at(1) -= j;

					// �������� ��������� �� ����������� ������, ����������� �� ������ �����������
					sticks.push_back(matrix.at(GetNumCoord(coordsVec.at(0))).at(GetNumCoord(coordsVec.at(1))) );
					if (coordsMap.find(coordsVec.at(1)) != coordsMap.end() && sticks.at(1) != nullptr && sticks.at(1).get()->GetStatus() != 5) {
						array <float, 2> coordsCentral = coordsVec.at(0);
						coordsCentral.at(0) -= i / 2;
						coordsCentral.at(1) += j / 2;

						int lastMum = stickArr.at(stickArr.size() - 1).get()->GetNum();
						for (shared_ptr<Stick> stick : sticks) {
							shared_ptr<Stick> stick1 = make_shared<Stick>(coordsCentral, stick.get()->GetCoords1(), stick.get()->GetTime()/2, ++lastMum);
							shared_ptr<Stick> stick2 = make_shared<Stick>(coordsCentral, stick.get()->GetCoords2(), stick.get()->GetTime()/2, ++lastMum);

							stickArr.push_back(stick1); stickArr.push_back(stick2);
							stickArr.erase(std::remove(stickArr.begin(), stickArr.end(), stick), stickArr.end());
							{
								array<float, 2> coords = stick.get()->GetCoords1();
								coordsMap[coords].erase(std::remove(coordsMap[coords].begin(), coordsMap[coords].end(), stick), coordsMap[coords].end());
								coordsMap[coords].push_back(stick1);

								coords = stick.get()->GetCoords2();
								coordsMap[coords].erase(std::remove(coordsMap[coords].begin(), coordsMap[coords].end(), stick), coordsMap[coords].end());
								coordsMap[coords].push_back(stick2);

								vector<shared_ptr<Stick>> tempVec = coordsMap[coordsCentral];
								tempVec.push_back(stick1); tempVec.push_back(stick2);
								coordsMap[coordsCentral] = tempVec;
							}

							// ������ 5 ��������, ��� ������ ��� ������ �������� � ���� �������
							stick.get()->SetStatus(5);
						}
					}
				}
			}
		}
	}
}

// ���������� ������������ ������� ������� ������
void Figure::CalculateTime()
{
	float minimalTime = std::numeric_limits<float>::max();
	array<float, 2> minimalCoords;
	for (const array<float, 2> coordsStart : coordsArr) {
		float totalTime = 0.0f;

		int countBurnedSticks = 0;
		map <shared_ptr<Stick>, float> currentSticks;
		map <int, int> newStatuses;

		// ��������� ������ �����
		FireCoord(coordsStart, currentSticks, newStatuses);

		while (true) {
			ApplyNewStatuses(currentSticks, newStatuses);
			if (countBurnedSticks != stickArr.size()) {
				float localMinTime = std::numeric_limits<int>::max();

				for (const auto& pair : currentSticks) {
					const float localTime = pair.first.get()->GetStatus() == 3 ? pair.second / 2 : pair.second;

					if (localMinTime > localTime) {
						localMinTime = localTime;
					}
				}

				// ������(�) � ����������� �������� �������(�), � ��������� ������||���������� ������
				totalTime += localMinTime;
				// �������� ���������� ������� ��� ������� �� ������������ ������ ��������� ������
				map <shared_ptr<Stick>, float> tempCurrentSticks = currentSticks;
				for (const auto& pair : tempCurrentSticks) {
					if (pair.first.get()->GetStatus() == 3) {
						currentSticks[pair.first] -= localMinTime * 2;
						tempCurrentSticks[pair.first] -= localMinTime * 2;
					}
					else {
						currentSticks[pair.first] -= localMinTime;
						tempCurrentSticks[pair.first] -= localMinTime;
					}
					if (pair.second == 0) {
						BurnedStick(currentSticks, newStatuses, countBurnedSticks, pair.first);
					}
				}
			}
			else break;
		}
		if (totalTime < minimalTime) {
			minimalTime = totalTime;
			minimalCoords = coordsStart;
		}
		// ��������� ������ ������ ����� ��������� ��������� ������ ����������
		for (shared_ptr<Stick> stick : stickArr)
			stick.get()->SetStatus(0);
	}

	std::cout << "����������� ����� ������� ������: " << minimalTime << endl;
	std::cout << "����������: " << minimalCoords.at(0) << " " << minimalCoords.at(1) << endl;
	PrintResult(minimalTime, minimalCoords);
}

// ��������� ������ ����� ��������� ������
void Figure::BurnedStick(map<shared_ptr<Stick>, float>& currentSticks, map<int, int>& newStatuses, int& burnedSticks, shared_ptr<Stick> stick) {
	stick.get()->SetBurned();
	newStatuses.erase(stick.get()->GetNum());
	currentSticks.erase(stick);
	burnedSticks++;

	const array<float, 2> coords = stick.get()->GetBurnedCoords();
	FireCoord(coords, currentSticks, newStatuses);
}
// ���������� ������, ����������� � ������ �����
void Figure::FireCoord(array<float, 2> coords, map <shared_ptr<Stick>, float>& currentSticks, map <int, int>& newStatuses) {
	for (const auto& stick : coordsMap[coords]) {
		if (stick != nullptr) {
			if (stick.get()->GetStatus() == 0) {
				currentSticks[stick] = stick.get()->GetTime();						// ���������� ����� ������� ������
				newStatuses[stick.get()->GetNum()] = stick.get()->GetFire(coords);	// ���������� ������� ��� �� ������
				stick.get()->SetFire(coords);										// ��������� ������� ��� ����� ������
			}
			else if ((coords == stick.get()->GetCoords1() && stick.get()->GetStatus() == 2)
				  || (coords == stick.get()->GetCoords2() && stick.get()->GetStatus() == 1))
			{
				newStatuses[stick.get()->GetNum()] = 3;									// ���������� �� ������ �������(��������� � ����� ������ ��������)
			}
		}
	}
}

// ���������� �������� ������ ����� ��������
void Figure::ApplyNewStatuses(map<shared_ptr<Stick>, float>& currentSticks, map<int, int>& newStatuses)
{
	for (const auto& pair : currentSticks) {
		pair.first.get()->SetStatus(newStatuses[pair.first.get()->GetNum()]);
	}
}



// ��������� ������� ���������, ����������� � ������� ���� ���������
int Figure::GetNumCoord(array<float, 2> coords)
{
	auto result{ find(begin(coordsArr), end(coordsArr), coords) };
	if (result == end(coordsArr))
		return NULL;
	else
		return (int)(result - coordsArr.begin());
}



// ����������� ������� ������
Figure::Stick::Stick(array<float, 2> coords1, array<float, 2> coords2, float time, int num) noexcept
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
	std::cout << num << ": (" << coords1.at(0) << ":" << coords1.at(1) << ") ";
	std::cout << "(" << coords2.at(0) << ":" << coords2.at(1) << ") " << time << endl;
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
{					// 0 - ������ �����; 1 - ����� �� ������ ����������; 
	return status;	// 2 - ����� �� ������ ����������; 3 - ����� � ���� ������
}					// -1 - ������� � ������ ����������; -2 - ������� �� ������ ����������;

array<float, 2> Figure::Stick::GetCoords1() noexcept
{
	return coords1;
}
array<float, 2> Figure::Stick::GetCoords2() noexcept
{
	return coords2;
}

void Figure::Stick::SetStatus(int status) noexcept
{
	this->status = status;
}

// ��������� ����� ���������� ������
void Figure::Stick::SetFire(array<float, 2> coords)
{
	this->SetStatus(coords == this->GetCoords1() ? 1 : 2);
}

// ��������� ����� ���������� ������
int Figure::Stick::GetFire(array<float, 2> coords)
{
	return coords == this->GetCoords1() ? 1 : 2;
}

// ��������� ����� �������� ������
void Figure::Stick::SetBurned() noexcept
{
	this->SetStatus(this->GetStatus() == 1 ? -2 : -1);
}

// ��������� ���������� ����� �������� ������
array<float, 2> Figure::Stick::GetBurnedCoords() noexcept
{
	return GetStatus() == -1 ? GetCoords1() : GetCoords2();
}