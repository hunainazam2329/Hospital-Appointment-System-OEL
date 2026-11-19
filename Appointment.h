#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include "patient.h"
using namespace std;

// 1 = Scheduled, 2 = Completed, 3 = Cancelled
struct Appointment {
    int appId;
    int patientId;
    string date;
    int status;
    Appointment* next;

    // Constructor initializes data and next pointer
    Appointment(int aId, int pId, string appDate, int st = 1, Appointment* nxt = nullptr) {
        appId = aId;
        patientId = pId;
        date = appDate;
        status = st;
        next = nxt;
    }
};

class AppointmentQueue {
private:
    Appointment* front;
    Appointment* rear;

    string getStatusText(int st) {
        if (st == 1) return "Scheduled";
        if (st == 2) return "Completed";
        if (st == 3) return "Cancelled";
        return "Unknown";
    }

public:
    AppointmentQueue() {
        front = nullptr;
        rear = nullptr;
    }

    ~AppointmentQueue() {
        Appointment* temp;
        while (front != nullptr) {
            temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    Appointment* findById(int appId) {
        Appointment* curr = front;
        while (curr != nullptr) {
            if (curr->appId == appId) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    // Add appointment at rear (arrival order)
    void addAppointment(int appId, int patientId, string date, PatientList& patients) {
        if (findById(appId) != nullptr) {
            cout << "Error: Duplicate Appointment ID!\n";
            return;
        }
        if (patients.searchById(patientId) == nullptr) {
            cout << "Error: Patient ID not found!\n";
            return;
        }

        Appointment* newNode = new Appointment(appId, patientId, date, 1, nullptr);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Appointment scheduled.\n";
    }

    // Complete next scheduled appointment in arrival order
    void completeNext() {
        if (isEmpty()) {
            cout << "Appointment queue is empty!\n";
            return;
        }
        Appointment* curr = front;
        while (curr != nullptr && curr->status != 1) {
            curr = curr->next;
        }
        if (curr == nullptr) {
            cout << "No scheduled appointments left.\n";
            return;
        }
        curr->status = 2;
        cout << "Appointment #" << curr->appId << " marked as Completed.\n";
    }

    // Cancel appointment by ID
    void cancelAppointment(int appId) {
        if (isEmpty()) {
            cout << "Appointment queue is empty!\n";
            return;
        }
        Appointment* app = findById(appId);
        if (app == nullptr) {
            cout << "Appointment not found!\n";
            return;
        }
        if (app->status == 3) {
            cout << "Appointment is already Cancelled.\n";
            return;
        }
        if (app->status == 2) {
            cout << "Completed appointment cannot be cancelled.\n";
            return;
        }
        app->status = 3;
        cout << "Appointment #" << appId << " Cancelled.\n";
    }

    // Bubble Sort appointments by Appointment ID
    void sortById() {
        if (isEmpty() || front->next == nullptr) {
            cout << "Not enough appointments to sort.\n";
            return;
        }
        bool swapped;
        do {
            swapped = false;
            Appointment* curr = front;
            while (curr->next != nullptr) {
                if (curr->appId > curr->next->appId) {
                    swap(curr->appId, curr->next->appId);
                    swap(curr->patientId, curr->next->patientId);
                    swap(curr->date, curr->next->date);
                    swap(curr->status, curr->next->status);
                    swapped = true;
                }
                curr = curr->next;
            }
        } while (swapped);
        cout << "Appointments sorted by ID.\n";
    }

    void displayAll() {
        if (isEmpty()) {
            cout << "No appointments in queue.\n";
            return;
        }
        cout << "\n--- Appointments (Arrival Order) ---\n";
        Appointment* curr = front;
        while (curr != nullptr) {
            cout << "App ID: " << curr->appId
                 << " | Patient ID: " << curr->patientId
                 << " | Date: " << curr->date
                 << " | Status: " << getStatusText(curr->status) << "\n";
            curr = curr->next;
        }
    }

    void saveToFile(string filename) {
        ofstream out(filename);
        if (!out) return;
        Appointment* curr = front;
        while (curr != nullptr) {
            out << curr->appId << "|" << curr->patientId << "|" << curr->date << "|" << curr->status << "\n";
            curr = curr->next;
        }
        out.close();
    }

    void loadFromFile(string filename) {
        ifstream in(filename);
        if (!in) return;
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);
            if (p3 == string::npos) continue;

            int appId = stoi(line.substr(0, p1));
            int pId = stoi(line.substr(p1 + 1, p2 - p1 - 1));
            string date = line.substr(p2 + 1, p3 - p2 - 1);
            int st = stoi(line.substr(p3 + 1));

            if (findById(appId) == nullptr) {
                Appointment* newNode = new Appointment(appId, pId, date, st, nullptr);
                if (isEmpty()) {
                    front = rear = newNode;
                } else {
                    rear->next = newNode;
                    rear = newNode;
                }
            }
        }
        in.close();
    }
};

#endif