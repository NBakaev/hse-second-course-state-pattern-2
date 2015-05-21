#include <chrono>
#include <deque>

using system_clock = std::chrono::system_clock;
using timepoint = decltype(system_clock::now());

//Forward declaration
class Printer;

//State Interface
class IState {

protected:
	Printer *_printer;
	IState(Printer *printer) : _printer(printer) {}

public:
    virtual void On() = 0;
    virtual void Off() = 0;
    virtual void Print() = 0;
    virtual void AddPaper(int count);
};

class PowerOffState : public IState {
public:
    PowerOffState(Printer *printer) : IState(printer) {}
    void On() override;
    void Off() override;
    void Print() override;
};

class WaitingState : public IState {
public:
    WaitingState(Printer *printer) : IState(printer) {}
    void On() override;
    void Off() override;
    void Print() override;
};

class PaperOffState : public IState {
public:
    PaperOffState(Printer *printer) : IState(printer) {}
    void On() override;
    void Off() override;
    void Print() override;
    void AddPaper(int count) override;
};

class PrintState : public IState {
public:
    PrintState(Printer *printer) : IState(printer) {}
    void On() override;
    void Off() override;
    void Print() override;
};

class Printer {
	IState *_state;
	std::deque<timepoint> _documents_enqueued;
	int _paper_count;

public:
	Printer();
	~Printer();
	void On();
	void Off();
	void Print();
	void AddPaper(int);
	void AddPaperCount(int);
	bool UpdateQueue();
	void EnqueueDocument();
	void SetState(IState*);
	int GetPaperCount();
};