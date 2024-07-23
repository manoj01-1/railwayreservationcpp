# Railway Ticket Reservation System

## Overview
The Railway Ticket Reservation System is a console-based application developed in C++ using Object-Oriented Programming (OOP) principles. It simulates the booking and cancellation of railway tickets, managing different types of tickets including Berth and Reservation Against Cancellation (RaC), and maintaining a waiting list.

## Features
- **Ticket Booking:** Book Berth tickets with a preferred berth type (Lower, Upper, Middle). If berths are full, passengers are placed in RaC or added to a waiting list.
- **Ticket Cancellation:** Cancel tickets and reallocate seats from RaC and the waiting list accordingly.
- **View Tickets:** Display all booked tickets, RaC list, and waiting list.
- **OOP Design:** Utilizes OOP concepts such as inheritance, polymorphism, encapsulation, and composition for modularity and maintainability.

## Project Structure
- **Main Application:** Contains the primary interface for interacting with the system.
- **Booking Management:** Handles booking and cancellation logic for the RAC and Waiting list.
- **Passenger and Ticket Classes:** Represent passengers and their tickets, encapsulating relevant data and operations.

## How to Use
1. Clone the repository.
    ```sh
    git clone https://github.com/yourusername/railway-reservation-system.git
    ```
2. Navigate to the project directory.
    ```sh
    cd railway-reservation-system
    ```
3. Compile the code using a C++ compiler.
    ```sh
    g++ -o reservation_system main.cpp Login.cpp
    ```
4. Run the executable.
    ```sh
    ./reservation_system
    ```
5. Follow the menu options to book, cancel, or view tickets.

## Future Enhancements
- add a user login page for the individual.
- Implement a database for persistent storage of user and ticket data.
- Enhance security features for user authentication.


