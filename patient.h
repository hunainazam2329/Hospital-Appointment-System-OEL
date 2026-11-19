#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    string disease;
    Patient* next;

    // Constructor initializes data and next pointer
    Patient(int pId, string pName, int pAge, string pDisease, Patient* nxt = nullptr) {
        id = pId;
        name = pName;
        age = pAge;
        disease = pDisease;
        next = nxt;
    }
};

class PatientList {
private:
    Patient* head;

public:
    PatientList() {
        head = nullptr;
    }

    ~PatientList() {
        Patient* temp;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool isEmpty() {
        return head == nullptr;
    }

    // Linear Search by ID
    Patient* searchById(int id) {
        Patient* curr = head;
        while (curr != nullptr) {
            if (curr->id == id) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    // Add patient at end (checks duplicate ID)
    void addPatient(int id, string name, int age, string disease) {
        if (searchById(id) != nullptr) {
            cout << "Error: Duplicate Patient ID " << id << "!\n";
            return;
        }

        Patient* newNode = new Patient(id, name, age, disease, nullptr);
        if (head == nullptr) {
            head = newNode;
        } else {
            Patient* curr = head;
            while (curr->next != nullptr) {
                curr = curr->next;
            }
            curr->next = newNode;
        }
        cout << "Patient registered successfully.\n";
    }

    void updatePatient(int id, string name, int age, string disease) {
        if (isEmpty()) {
            cout << "No patients in list.\n";
            return;
        }
        Patient* p = searchById(id);
        if (p == nullptr) {
            cout << "Patient ID not found.\n";
            return;
        }
        p->name = name;
        p->age = age;
        p->disease = disease;
        cout << "Patient record updated.\n";
    }

    void deletePatient(int id) {
        if (isEmpty()) {
            cout << "No patients in list.\n";
            return;
        }

        Patient* curr = head;
        Patient* prev = nullptr;

        while (curr != nullptr && curr->id != id) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            cout << "Patient ID not found.\n";
            return;
        }

        if (prev == nullptr) {
            head = curr->next;
        } else {
            prev->next = curr->next;
        }
        delete curr;
        cout << "Patient deleted.\n";
    }

    // Bubble Sort by Patient ID
    void sortById() {
        if (head == nullptr || head->next == nullptr) {
            cout << "Not enough records to sort.\n";
            return;
        }
        bool swapped;
        do {
            swapped = false;
            Patient* curr = head;
            while (curr->next != nullptr) {
                if (curr->id > curr->next->id) {
                    swap(curr->id, curr->next->id);
                    swap(curr->name, curr->next->name);
                    swap(curr->age, curr->next->age);
                    swap(curr->disease, curr->next->disease);
                    swapped = true;
                }
                curr = curr->next;
            }
        } while (swapped);
        cout << "Patients sorted by ID.\n";
    }

    // Binary Search on a sorted copy of IDs
    void binarySearchId(int targetId) {
        if (isEmpty()) {
            cout << "No patients to search.\n";
            return;
        }
        sortById();

        int count = 0;
        Patient* curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }

        int* ids = new int[count];
        curr = head;
        for (int i = 0; i < count; i++) {
            ids[i] = curr->id;
            curr = curr->next;
        }

        int left = 0, right = count - 1;
        bool found = false;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (ids[mid] == targetId) {
                found = true;
                break;
            } else if (ids[mid] < targetId) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        delete[] ids;

        if (found) {
            Patient* p = searchById(targetId);
            cout << "Found -> ID: " << p->id << ", Name: " << p->name
                 << ", Age: " << p->age << ", Disease: " << p->disease << "\n";
        } else {
            cout << "Patient ID not found.\n";
        }
    }

    void displayAll() {
        if (isEmpty()) {
            cout << "No patient records found.\n";
            return;
        }
        cout << "\n--- Patient List ---\n";
        Patient* curr = head;
        while (curr != nullptr) {
            cout << "ID: " << curr->id << " | Name: " << curr->name
                 << " | Age: " << curr->age << " | Disease: " << curr->disease << "\n";
            curr = curr->next;
        }
    }

    void saveToFile(string filename) {
        ofstream out(filename);
        if (!out) return;
        Patient* curr = head;
        while (curr != nullptr) {
            out << curr->id << "|" << curr->name << "|" << curr->age << "|" << curr->disease << "\n";
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

            int id = stoi(line.substr(0, p1));
            string name = line.substr(p1 + 1, p2 - p1 - 1);
            int age = stoi(line.substr(p2 + 1, p3 - p2 - 1));
            string disease = line.substr(p3 + 1);

            if (searchById(id) == nullptr) {
                Patient* newNode = new Patient(id, name, age, disease, nullptr);
                if (head == nullptr) {
                    head = newNode;
                } else {
                    Patient* curr = head;
                    while (curr->next != nullptr) curr = curr->next;
                    curr->next = newNode;
                }
            }
        }
        in.close();
    }
};

#endif