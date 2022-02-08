#include "tinyfsm.hpp"
#include <iostream>

// ----------------------------------------------------------------------------
// 1. Event Declarations
//
struct InitOkEvent : tinyfsm::Event {};
struct EnableEvent : tinyfsm::Event {};
struct DisableEvent : tinyfsm::Event {};
struct BillInEvent : tinyfsm::Event {};
struct AcceptEvent : tinyfsm::Event {};
struct RejectEvent : tinyfsm::Event {};
struct StackedEvent : tinyfsm::Event {};

// ----------------------------------------------------------------------------
// 2. State Machine Base Class Declaration
//
struct BillAcceptor : tinyfsm::MealyMachine<BillAcceptor> {
    /* pure virtual reaction (override required in all states) */
    virtual void react(InitOkEvent const &) = 0;
    virtual void react(EnableEvent const &) = 0;
    virtual void react(DisableEvent const &) = 0;
    virtual void react(BillInEvent const &) = 0;
    virtual void react(AcceptEvent const &) = 0;
    virtual void react(RejectEvent const &) = 0;
    virtual void react(StackedEvent const &) = 0;

    /* transition actions */
    static void EnableDevice() {
        std::cout << "* Enabling device..." << std::endl;
    }
    static void DisableDevice()  {
        std::cout << "* Disabling device..."  << std::endl;
    }
    static void BillIsIn() {
        std::cout << "Validating bill..." << std::endl;
        std::cout << "Bill is valid with value 5€" << std::endl;
        std::cout << "* You should accept or reject the bill inside" << std::endl;
    }
    static void AcceptBill () {
        std::cout << "Accepting bill ... " << std::endl;
    }
    static void RejectBill () {
        std::cout << "Rejecting bill ... " << std::endl;
    }
    static void StackedBill () {
        std::cout << "Bill stacked" << std::endl;
    }
};


// ----------------------------------------------------------------------------
// 3. State Declarations
//

struct Disable; //forward declaration
struct Enable;
struct InEscrow;
struct Stacking;

struct Setup : BillAcceptor {
    void react (InitOkEvent const &) override {
        transit<Disable>(DisableDevice);
    }

    void react (EnableEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }

    void react (DisableEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    };

    void react (BillInEvent const &) override {
        std::cout << "Reject the note inside" << std::endl;
    }

    void react (AcceptEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }

    void react (RejectEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }

    void react(StackedEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }
};

struct Enable : BillAcceptor {

    void react (InitOkEvent const &) override {
        std::cout << "Cannot perform it at this moment" << std::endl;
    }

    void react(EnableEvent const &) override {
        std::cout << "The device is already enabled" << std::endl;
    }

    void react(DisableEvent const &) override {
        transit<Disable>(DisableDevice);
    };

    void react (BillInEvent const &) override {
        transit<InEscrow>(BillIsIn);
    }

    void react (AcceptEvent const &) override {
        std::cout << "Cannot perform it at the moment" << std::endl;
    }

    void react (RejectEvent const &) override {
        std::cout << "Cannot perform it at the moment" << std::endl;
    }

    void react(StackedEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }
};

struct Disable : BillAcceptor {

    void react (InitOkEvent const &) override {
        std::cout << "Cannot perform it at this moment" << std::endl;
    }

    void react(EnableEvent const &) override {
        transit<Enable>(EnableDevice);
    };

    void react (DisableEvent const &) override {
        std::cout << "The device is already disabled" << std::endl;
    };

    void react (BillInEvent const &) override {
        std::cout << "Cannot perform it at this moment" << std::endl;
    }

    void react (AcceptEvent const &) override {
        std::cout << "Cannot perform it at the moment" << std::endl;
    }

    void react (RejectEvent const &) override {
        std::cout << "Cannot perform it at the moment" << std::endl;
    }

    void react(StackedEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }
};

struct InEscrow : BillAcceptor {
    void react (InitOkEvent const &) override {
        std::cout << "Cannot perform it at this moment" << std::endl;
    }

    void react(EnableEvent const &) override {
        std::cout << "The device is already enable and has a bill in escrow" << std::endl;
    };

    void react (DisableEvent const &) override {
        std::cout << "Reject note" << std::endl;
        transit<Disable>(DisableDevice);
    };

    void react (BillInEvent const &) override {
        std::cout << "Cannot perform it at this moment. Already has a bill in escrow" << std::endl;
    }

    void react (AcceptEvent const &) override {
        transit<Stacking>(AcceptBill);
    }

    void react (RejectEvent const &) override {
        transit<Enable>(RejectBill);
    }

    void react(StackedEvent const &) override {
        std::cout << "The device is initializing, cannot perform it at the moment" << std::endl;
    }
};

struct Stacking : BillAcceptor {
    void react (InitOkEvent const &) override {
        std::cout << "Cannot perform it at the moment" << std::endl;
    }

    void react(EnableEvent const &) override {
        std::cout << "Cannot perform this at the moment" << std::endl;
    };

    void react (DisableEvent const &) override {
        std::cout << "Cannot perform this at the moment" << std::endl;
    };

    void react (BillInEvent const &) override {
        std::cout << "Cannot perform this at the moment" << std::endl;
    }

    void react (AcceptEvent const &) override {
        std::cout << "Cannot perform this at the moment" << std::endl;
    }

    void react (RejectEvent const &) override {
        std::cout << "Cannot perform this at the moment" << std::endl;
    }

    void react(StackedEvent const &) override {
        transit<Enable>(StackedBill);
    }
};


FSM_INITIAL_STATE(BillAcceptor, Setup)


// ----------------------------------------------------------------------------
// Main
//
int main()
{
    BillAcceptor::start();

    std::cout << "> Welcome to Bill Acceptor" << std::endl;
    while(1)
    {
        char c;
        std::cout << std::endl << "i = Init Ok" << std::endl <<
                                "e = Enable" << std::endl <<
                                "d = Disable" << std::endl <<
                                "n = Note is In" << std::endl <<
                                "a = Accept bill" << std::endl <<
                                "r = Reject bill" << std::endl <<
                                "s = Stacked" << std::endl <<
                                "q = Quit" << std::endl;
        std::cin >> c;
        switch(c) {
            case 'i':
                std::cout << "Init ok" << std::endl;
                BillAcceptor::dispatch(InitOkEvent());
                break;
            case 'e':
                std::cout << "Enabling the bill acceptor" << std::endl;
                BillAcceptor::dispatch (EnableEvent());
                break;
            case 'd':
                std::cout << "Disabling the bill acceptor" << std::endl;
                BillAcceptor::dispatch (DisableEvent());
                break;
            case 'n':
                std::cout << "Note is in" << std::endl;
                BillAcceptor::dispatch (BillInEvent());
                break;
            case 'a':
                std::cout << "Accepting bill" << std::endl;
                BillAcceptor::dispatch(AcceptEvent());
                break;
            case 'r':
                std::cout << "Rejecting bill" << std::endl;
                BillAcceptor::dispatch(RejectEvent());
                break;
            case 's':
                std::cout << "Stack item" << std::endl;
                BillAcceptor::dispatch(StackedEvent());
            case 'q':
                return 0;
            default:
                std::cout << "> Invalid input" << std::endl;
        };
    }
}