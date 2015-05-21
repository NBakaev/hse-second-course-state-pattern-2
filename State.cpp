#include <algorithm>
#include <iostream>

#include "State.h"

void IState::AddPaper(int count) {
	_printer->AddPaperCount(count);
	std::cout << "Бумага добавлена" << std::endl;
}

void PowerOffState::On() {
	_printer->SetState(new WaitingState(_printer));
	std::cout << "Принтер включен" << std::endl;
	delete this;
}	

void PowerOffState::Off() {
	std::cout << "Принтер уже выключен" << std::endl;
}

void PowerOffState::Print() {
	std::cout << "Принтер отключен. Печать невозможна" << std::endl;
}


void WaitingState::On() {
	std::cout << "Принтер уже и так включен" << std::endl;
}

void WaitingState::Off() {
	_printer->SetState(new PowerOffState(_printer));
	std::cout << "Принтер выключен" << std::endl;
	delete this;
}

void WaitingState::Print() {
	if (_printer->GetPaperCount() > 0) {
		std::cout << "Печатаем..." << std::endl;
		_printer->AddPaper(-1);
		_printer->SetState(new PrintState(_printer));
		_printer->EnqueueDocument();
		delete this;
	} 
	else {
		_printer->SetState(new PaperOffState(_printer));
		_printer->Print();
		delete this;
	}
}

void PaperOffState::On() {
	std::cout << "Принтер уже и так включен" << std::endl;
}

void PaperOffState::Off() {
	if(_printer->UpdateQueue()) {
		_printer->SetState(new PowerOffState(_printer));
		std::cout << "Принтер выключен" << std::endl;
	} 
	else {
		std::cout << "Принтер ещё печатает. Невозможно прервать печать" << std::endl;
	}
	delete this;
}

void PaperOffState::AddPaper(int count) {
	std::cout << "Добовляем бумагу" << std::endl;
	_printer->AddPaperCount(count);
	if (_printer->GetPaperCount() > 0) {
		if (_printer->UpdateQueue()) {
			_printer->SetState(new WaitingState(_printer));
		} 
		else {
			_printer->SetState(new PrintState(_printer));
		}
		delete this;
	}
}

void PaperOffState::Print() {
	std::cout << "Нет бумаги" << std::endl;
}

void PrintState::On() {
	std::cout << "Принтер уже и так включен" << std::endl;
}

void PrintState::Off() {
	if(_printer->UpdateQueue()) {
		_printer->SetState(new PowerOffState(_printer));
		std::cout << "Принтер выключен" << std::endl;
	} 
	else {
		std::cout << "Принтер ещё печатает. Невозможно прервать печать" << std::endl;
	}
	delete this;
}

void PrintState::Print() {
	if(_printer->UpdateQueue()) {
		_printer->SetState(new WaitingState(_printer));
		_printer->Print();
		delete this;
	}
	else if (_printer->GetPaperCount() < 1) {
		 _printer->SetState(new PaperOffState(_printer));
		 _printer->Print();
		 delete this;
	}
	else {
		_printer->EnqueueDocument();
		_printer->AddPaper(-1); 
		std::cout << "Принтер ещё печатает. Документ добавлен в очередь" << std::endl;
	}
}

void Printer::On() {
	_state->On();
}

void Printer::Off() {
	_state->Off();
}

void Printer::Print() {
	_state->Print();
}

void Printer::AddPaper(int count) {
	if (count > 0) {
		_state->AddPaper(count);
	} else {
		AddPaperCount(count);
	}
}

void Printer::AddPaperCount(int count) {
	_paper_count += count;
}

void Printer::SetState(IState *state) {
	_state = state;
}

void Printer::EnqueueDocument() {
	_documents_enqueued.push_back(system_clock::now());
}

bool Printer::UpdateQueue() {
	timepoint now = system_clock::now();

	_documents_enqueued.erase(std::remove_if(_documents_enqueued.begin(), _documents_enqueued.end(), 
		[&now](timepoint &t) -> bool {
		return (now - t - std::chrono::seconds(3)).count() > 0;
	}), _documents_enqueued.end());

	return _documents_enqueued.empty();
}

int Printer::GetPaperCount() {
	return _paper_count;
}


Printer::Printer() {
	_paper_count = 10;
	_state = new PowerOffState(this);
}

Printer::~Printer() {
	delete _state;
}