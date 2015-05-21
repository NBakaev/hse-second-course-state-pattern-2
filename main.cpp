#include <iostream>
#include "State.h"

struct Client {
	void run() {
		Printer printer;
		int action;

		while(true) {
			std::cout << "Выберите действие:" << std::endl;
			std::cout << "1. Печатать документ" << std::endl;
			std::cout << "2. Включить принтер" << std::endl;
			std::cout << "3. Выключить принтер" << std::endl;
			std::cout << "4. Добавить бумаги" << std::endl;
			std::cout << "5. Выход" << std::endl;

			std::cin >> action;

			switch(action) {
				case 1:
					printer.Print();
				break;

				case 2:
					printer.On();
				break;

				case 3:
					printer.Off();
				break;

				case 4:
					printer.AddPaper(AddPaperDialogue());
				break;

				case 5:
				return;
				
				default:
				std::cout << "Неверное действие" << std::endl;
			}
		}
	}

	int AddPaperDialogue() {
		int count;
		std::cout << "Сколько бумаги добавить?" << std::endl;
		std::cin >> count;
		return count;
	}	
};

int main(int argc, char const *argv[]) {
	Client c;
	c.run();
	return 0;
}