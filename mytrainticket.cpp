#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

class Passenger {
private:
    static int idCounter; // Static counter for generating unique IDs
    int id;
    string name;
    int age;

public:
    Passenger(string n, int a) : name(n), age(a) {
        id = ++idCounter; // Assigning unique ID
    }

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }

    void display() const {
        cout << "Passenger ID: " << id << ", Name: " << name << ", Age: " << age << endl;
    }
};

int Passenger::idCounter = 0;

class Ticket {
private:
    Passenger passenger;
    string ticketType;
    string seatNumber; // Unique seat number for each berth

public:
    Ticket(const Passenger& p, string type, string seat) : passenger(p), ticketType(type), seatNumber(seat) {}

    Passenger getPassenger() const { return passenger; }
    string getTicketType() const { return ticketType; }
    string getSeatNumber() const { return seatNumber; }
    void setTicketType(string type) { ticketType = type; }
    void setSeatNumber(string seat) { seatNumber = seat; }
};

class RailwayReservationSystem {
private:
    vector<Ticket> tickets;
    vector<Passenger> waitingList;
    vector<Passenger> racList;
    int availableBerths[3]; // Array to track available berths (L, U, M)
    int racAvailable; // To track the availability of RAC seats

public:
    RailwayReservationSystem() {
        availableBerths[0] = 1; // 21 berths for L
        availableBerths[1] = 1; // 21 berths for U
        availableBerths[2] = 1; // 21 berths for M
        racAvailable = 1; // 1 RAC seat available
    }

    // Function to book a ticket
    void bookTicket(const Passenger& p, string ticketType, char preferredBerth = '\0') {
        if (ticketType == "Berth") {
            int berthIndex;
            if (preferredBerth == 'L') {
                berthIndex = 0;
            } else if (preferredBerth == 'U') {
                berthIndex = 1;
            } else if (preferredBerth == 'M') {
                berthIndex = 2;
            } else {
                // Ask user for preferred berth if not specified
                cout << "Enter preferred berth (L/U/M): ";
                cin >> preferredBerth;
                berthIndex = (preferredBerth == 'L') ? 0 : (preferredBerth == 'U') ? 1 : 2;
            }

            // Check if preferred berth is available
            if (availableBerths[berthIndex] > 0) {
                string seatNumber = generateSeatNumber(preferredBerth);
                Ticket ticket(p, "Berth " + string(1, "LUM"[berthIndex]), seatNumber);
                tickets.push_back(ticket);
                availableBerths[berthIndex]--;
                cout << "Berth ticket booked successfully for ";
                p.display();
                cout << "Seat Number: " << seatNumber << endl;
            } else {
                // Look for any available berth
                bool booked = false;
                for (int i = 0; i < 3; ++i) {
                    if (availableBerths[i] > 0) {
                        string seatNumber = generateSeatNumber("LUM"[i]);
                        Ticket ticket(p, "Berth " + string(1, "LUM"[i]), seatNumber);
                        tickets.push_back(ticket);
                        availableBerths[i]--;
                        cout << "Berth ticket booked successfully for ";
                        p.display();
                        cout << "Seat Number: " << seatNumber << endl;
                        booked = true;
                        break;
                    }
                }

                // If all berths are filled, push to RaC or Waiting List
                if (!booked) {
                    bookRaCTicket(p);
                }
            }
        } else {
            cout << "Invalid ticket type!" << endl;
        }
    }

    // Function to book an RaC ticket
    void bookRaCTicket(const Passenger& p) {
        if (racAvailable > 0) {
            stringstream ss;
            ss << "RaC " << 2 - racAvailable;
            string seatNumber = ss.str();
            Ticket ticket(p, "RaC", seatNumber);
            tickets.push_back(ticket);
            racList.push_back(p);
            racAvailable--;
            cout << "RaC ticket booked successfully for ";
            p.display();
            cout << "Seat Number: " << seatNumber << endl;
        } else {
            // If RaC list is full, add to Waiting List
            waitingList.push_back(p);
            cout << "RaC list full. Added to Waiting List for ";
            p.display();
        }
    }

    // Function to cancel a ticket
void cancelTicket(int passengerId) {
    cout << "Attempting to cancel ticket for passenger ID " << passengerId << endl;

    for (vector<Ticket>::iterator it = tickets.begin(); it != tickets.end(); ++it) {
        if (it->getPassenger().getId() == passengerId) {
            cout << "Ticket found for passenger ID " << passengerId << endl;

            // Debug output to verify details
            cout << "Ticket Type: " << it->getTicketType() << ", Passenger ID: " << it->getPassenger().getId() << endl;

            // Return berth if it's a berth ticket
            if (it->getTicketType().substr(0, 5) == "Berth") {
                char berthType = it->getTicketType()[6];
                int berthIndex = (berthType == 'L') ? 0 : (berthType == 'U') ? 1 : 2;
                availableBerths[berthIndex]++;
                rebookFromRaCToBerth(berthType);
            } else if (it->getTicketType() == "RaC") {
                racAvailable++;
                rebookFromWaitingToRaC();
            }

            // Confirm cancellation
            cout << "Ticket canceled successfully for passenger ID " << passengerId << endl;

            // Erase the ticket and update the iterator
            it = tickets.erase(it);

            // Display updated tickets after cancellation
            displayAllTickets();

            return;
        }
    }

    cout << "Ticket not found for passenger ID " << passengerId << ". Cancellation failed." << endl;
}




    // Function to rebook from RaC list to available berth
    void rebookFromRaCToBerth(char berthType) {
        if (!racList.empty()) {
            Passenger racPassenger = racList.front();
            racList.erase(racList.begin());
            racAvailable++;
            bookTicket(racPassenger, "Berth", berthType);
            rebookFromWaitingToRaC();
        }
    }

    // Function to rebook from waiting list to RaC list
    void rebookFromWaitingToRaC() {
        if (!waitingList.empty() && racAvailable > 0) {
            Passenger waitingPassenger = waitingList.front();
            waitingList.erase(waitingList.begin());
            bookRaCTicket(waitingPassenger);
        }
    }

    // Function to generate unique seat number for each berth type
    string generateSeatNumber(char berthType) {
        stringstream ss;
        switch (berthType) {
            case 'L':
                ss << "L" << 22 - availableBerths[0];
                break;
            case 'U':
                ss << "U" << 22 - availableBerths[1];
                break;
            case 'M':
                ss << "M" << 22 - availableBerths[2];
                break;
        }
        return ss.str();
    }

     void displayBerthTickets() const{
     	for(vector<Ticket>:: const_iterator it=tickets.begin();it!=tickets.end();it++){
     		if(it->getTicketType().substr(0,5)=="Berth"){
     			cout<<"Ticket type: "<<it->getTicketType()<< ", ";
     			it->getPassenger().display();
     			cout<<"seat Number: "<<it->getSeatNumber()<< endl;
			 }
			 
			 cout << "--------------------"<<endl;
		 }
	 }
    // Function to display all tickets
    void displayAllTickets() const {
        cout << "---- All Tickets ----" << endl;
        for (vector<Ticket>::const_iterator it = tickets.begin(); it != tickets.end(); ++it) {
            cout << "Ticket Type: " << it->getTicketType() << ", ";
            it->getPassenger().display();
            cout << "Seat Number: " << it->getSeatNumber() << endl;
        }
        cout << "---------------------" << endl;
    }

    // Function to display waiting list
    void displayWaitingList() const {
        cout << "---- Waiting List ----" << endl;
        for (vector<Passenger>::const_iterator it = waitingList.begin(); it != waitingList.end(); ++it) {
            it->display();
        }
        cout << "---------------------" << endl;
    }

    // Function to display RaC list
    void displayRaCList() const {
        cout << "---- RaC List ----" << endl;
        for (vector<Passenger>::const_iterator it = racList.begin(); it != racList.end(); ++it) {
            it->display();
        }
        cout << "-------------------" << endl;
    }
};

int main() {
    RailwayReservationSystem system;
    string name;
    int age;
    int choice;
    char preferredBerth;

    cout << "Welcome to Railway Ticket Reservation System" << endl;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Book Berth Ticket\n";
        cout << "2. Cancel Ticket\n";
        cout << "3. View All Tickets\n";
        cout << "4. View Waiting List\n";
        cout << "5. View RaC List\n";
        cout << "6. view Berth tickets\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nEnter passenger name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                getline(cin, name);
                cout << "Enter passenger age: ";
                cin >> age;
                cout << "Enter preferred berth (L/U/M): ";
 	               cin >> preferredBerth;
                system.bookTicket(Passenger(name, age), "Berth", preferredBerth);
                break;
            case 2:
                int passengerId;
                cout << "\nEnter passenger ID to cancel ticket: ";
                cin >> passengerId;
                system.cancelTicket(passengerId);
                break;
            case 3:
                system.displayAllTickets();
                break;
            case 4:
                system.displayWaitingList();
                break;
            case 5:
                system.displayRaCList();
                break;
            case 6:
            	 system.displayBerthTickets();
            	 break;
            case 7:
                cout << "Exiting... Thank you!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}

