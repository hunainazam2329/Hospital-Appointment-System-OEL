
#ifndef PATIENT_LINKED_LIST_H
#define PATIENT_LINKED_LIST_H

#include <iostream>
#include <string>
using namespace std;

struct Patient
{
    int id;
    string name;
    int age;
    string disease;
    Patient* next;

    // Constructor using initialization list
    Patient(int pId, string pName, int pAge, string pDisease)
        : id(pId), name(pName), age(pAge),
          disease(pDisease), next(nullptr)
    {
    }
};

class PatientList
{
private:
    Patient* head;

public:
    PatientList();
    ~PatientList();

    bool isEmpty();

    bool addPatient(int id, string name, int age, string disease);

    bool updatePatient(int id, string name, int age, string disease);

    bool deletePatient(int id);

    Patient* searchById(int id);

    void displayAll();

    Patient* getHead();
};

#endif
