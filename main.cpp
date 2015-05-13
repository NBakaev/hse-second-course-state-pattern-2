#include <iostream>

using namespace std;

class IState
{
    void On();
    void Off();
    void Print();
    void AddPaper(int count);
};

class PowerOffState : public IState
{
    private:
        Printer _printer;

    public:

    PowerOffState(Printer printer)
    {
        _printer = printer;
    }
    void On()
    {
        Console.WriteLine("Принтер включен");
        _printer.SetState(_printer.WaitingState);
    }

    void Off()
    {
        Console.WriteLine("Принтер и так выключен");
    }

    void Print()
    {
        Console.WriteLine("Принтер отключен, печать невозможна");
    }

    void AddPaper(int count)
    {
        _printer.AddPater(count);
        Console.WriteLine("Бумага добавлена");
    }
};

//////////////////////////////////////////////////////////
class WaitingState : public IState
{

    private:
    Printer _printer;


    public:

    WaitingState(Printer printer)
    {
        _printer = printer;
    }

    void On()
    {
        Console.WriteLine("Принтер уже и так включен");
    }

    void Off()
    {
        Console.WriteLine("Принтер выключен");
    }

    void Print()
    {
        if (_printer.CountPaper > 0)
        {
            Console.WriteLine("Сейчас всё распечатаем");
            _printer.AddPater(-1);
        }
        else
        {
            _printer.SetState(_printer.PaperOffState);
            _printer.PrintDocument();
        }
    }

    void AddPaper(int count)
    {
        _printer.AddPater(count);
        Console.WriteLine("Бумага добавлена");
    }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class PaperOnState : public IState
{
private:
    Printer _printer;

public:

    PaperOffState(Printer printer)
    {
        _printer = printer;
    }

    void On()
    {
        Console.WriteLine("Принтер уже и так включен");
    }

    void Off()
    {
        Console.WriteLine("Принтер выключен");
        _printer.SetState(_printer.PowerOffState);
    }

    void Print()
    {
        if (_printer.CountPaper > 0)
        {
            _printer.SetState(_printer.PrintState);
            _printer.PrintDocument();
        }
        else
        {
            Console.WriteLine("Бумаги нет, печатать не буду");
        }

    }

    void AddPaper(int count)
    {
        Console.WriteLine("Добавляем бумагу");
        _printer.AddPater(count);
        if (_printer.CountPaper > 0)
            _printer.SetState(_printer.WaitingState);
    }
};

//////////////////////////////////////////////////////////////

class PrintState : IState
{
private:
    Printer _printer;

public:
    PrintState(Printer printer)
    {
        _printer = printer;
    }
    void On()
    {
        Console.WriteLine("Принтер уже и так включен");
    }

    void Off()
    {
        Console.WriteLine("Принтер выключен");
    }

    void Print()
    {
        if (_printer.CountPaper > 0)
        {
            Console.WriteLine("Идёт печать...");
            _printer.AddPater(-1);
            _printer.SetState(_printer.WaitingState);
        }

        else
        {
            _printer.SetState(_printer.PaperOffState);
            _printer.PrintDocument();
        }

    }

    void AddPaper(int count)
    {
        _printer.AddPater(count);
        Console.WriteLine("Бумага добавлена");
    }
};

///////////////////////////////////////////////////////////


class Printer
{
private: IState _state;

public:
    int _countPaper;

    // list of all available states

    PaperOffState PaperOffState;
    PowerOnState PowerOnState;

    PrintState PrintState;
    WaitingState WaitingState;


    int CountPaper () {
        return _countPaper;
    }

    Printer()
    {
        PowerOffState = new PowerOffState(this);
        PaperOnState = new PaperOnState(this);
        PrintState = new PrintState(this);
        WaitingState = new WaitingState(this);
        _state = WaitingState;
    }

    void SetState(IState state)
    {
        _state = state;
    }

    void PrintDocument()
    {
        _state.Print();
    }

    void PowerOff()
    {
        _state.Off();
    }
    void PowerOn()
    {
        _state.On();
    }

    void AddPater(int count)
    {
        _countPaper += count;
    }
};






int main()
{
    Printer printer = new Printer();

    printer.PowerOn();
    printer.PrintDocument();
    printer.AddPater(3);
    printer.PrintDocument();
    printer.PrintDocument();
    printer.PrintDocument();
    printer.PrintDocument();
    printer.PowerOff();

    delete printer;

//    cout << "Hello World!" << endl;
    return 0;
}

