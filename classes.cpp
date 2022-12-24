#include <string>
#include <iostream>

class Elevator
{
	protected:
		std::string modelName;
		double workTime[2];
		int currentFloor;

	public: 
		Elevator(std::string modelName, double workTime)
		{
			this->currentFloor = 0;
			this->modelName = modelName;			
			if (workTime == 0)
				this->workTime[1] = 1;
			else this->workTime[1] = workTime;
			this->workTime[0] = 0;
		}

		void Call(int floor)
		{
			while (floor != currentFloor && workTime[1] > workTime[0])
			{
				if (currentFloor > floor)
					currentFloor--;
				else currentFloor++;
				workTime[0]+= 1;
			}
		}

		void ShowInfo()
		{
			std::cout << "Model: " << modelName <<" Floor:" << currentFloor << " WorkingTime: " << workTime[0] << "\\" << workTime[1];
		}

		friend void Repair(Elevator& elevator);
		friend bool operator==(Elevator& e1, Elevator& e2);
		friend bool operator>(Elevator& e1, Elevator& e2);
		friend bool operator<(Elevator& e1, Elevator& e2);
};

class PublicElevator : public Elevator
{
	private:
		unsigned int maxPeople;
		unsigned int curPeople;
	public:
		PublicElevator(std::string modelName, double workTime, unsigned int maxPeople) : Elevator(modelName, workTime)
		{
			this->maxPeople = maxPeople;
			this->curPeople = 0;
		}

		void Call(int floor, unsigned int people)
		{
			if (curPeople == 0)
			{
				Elevator::Call(floor);
				curPeople = people;
			}
			else std::cout << modelName << " is Occupied!\n";
		}

		void SetCourse(int floor)
		{
			if (curPeople != 0)
			{
				while (floor != currentFloor && workTime[1] > workTime[0])
				{
					if (currentFloor > floor)
						currentFloor--;
					else currentFloor++;
					if (curPeople > maxPeople)
						workTime[0] += 1 + 0.1 * (curPeople - maxPeople);
					else workTime[0] += 1;
				}
			}
			else std::cout << modelName << " is Empty!\n";
		}

		void ShowInfo()
		{
			Elevator::ShowInfo();
			std::cout << " Load: " << (double) curPeople / (double) maxPeople * 100.0 << "%\n";
		}

};

class CargoElevator : public Elevator
{
	private:
		unsigned int maxWeight;
		double curWeight;
	public:
		CargoElevator(std::string modelName, double workTime, unsigned int maxWeight) : Elevator(modelName, workTime)
		{
			this->maxWeight = maxWeight;
			this->curWeight = 0;
		}

		void Call(int floor, double load)
		{
			if (curWeight == 0)
			{
				Elevator::Call(floor);
				curWeight = load;
			}
			else std::cout << modelName << " is Occupied!\n";
		}

		void SetCourse(int floor)
		{
			if (curWeight != 0)
			{
				while (floor != currentFloor && workTime[1] > workTime[0])
				{
					if (currentFloor > floor)
						currentFloor--;
					else currentFloor++;
					if (curWeight > maxWeight)
						workTime[0] += 1 + 0.5 * (curWeight - maxWeight);
					else workTime[0] += 1;
				}
				curWeight = 0;
			}
			else std::cout << modelName << " is Empty!\n";
		}

		void ShowInfo()
		{
			Elevator::ShowInfo();
			std::cout << " Load: " << curWeight / maxWeight * 100.0 << "%\n";
		}
};

bool operator==(Elevator& e1, Elevator& e2)
{
	return (e1.currentFloor == e2.currentFloor);
}

bool operator>(Elevator& e1, Elevator& e2)
{
	return (e1.currentFloor > e2.currentFloor);
}

bool operator<(Elevator& e1, Elevator& e2)
{
	return (e1.currentFloor < e2.currentFloor);
}

void Repair(Elevator& elevator)
{
	elevator.workTime[0] = 0;
}

int main()
{
	PublicElevator ep1("SocialElevator", 200, 20);
	CargoElevator ce1("Kone Corp. 233", 300, 120.50);
	ep1.Call(4, 25);
	ep1.SetCourse(100);
	ep1.Call(1, 50);
	ep1.SetCourse(-90);
	ep1.ShowInfo();
	Repair(ep1);
	ep1.SetCourse(-20);
	ep1.ShowInfo();
	std::cout << (ep1 < ce1) << std::endl;
	ce1.Call(10, 90);
	ce1.ShowInfo();
	ce1.SetCourse(-200);
	ce1.ShowInfo();
	return 0;
}
