
#include "PatientLinkedList.h"

// Constructor
PatientList::PatientList()
{
    head = nullptr;
}

// Destructor
PatientList::~PatientList()
{
    Patient* temp;

    while (head != nullptr)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// Check whether list is empty
bool PatientList::isEmpty()
{
    return head == nullptr;
}

// Search patient by ID
Patient* PatientList::searchById(int id)
{
    Patient* temp = head;

    while (temp != nullptr)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return nullptr;
}

// Insert patient at the end
bool PatientList::addPatient(
    int id, string name, int age, string disease)
{
    if (searchById(id) != nullptr)
    {
        cout << "\nDuplicate Patient ID! Registration rejected.\n";
        return false;
    }

    Patient* newPatient =
        new Patient(id, name, age, disease);

    if (head == nullptr)
    {
        head = newPatient;
    }
    else
    {
        Patient* temp = head;

        while (temp->next != nullptr)
            temp = temp->next;

        temp->next = newPatient;
    }

    cout << "\nPatient registered successfully!\n";
    return true;
}

// Update patient information
bool PatientList::updatePatient(
    int id, string name, int age, string disease)
{
    Patient* temp = searchById(id);

    if (temp == nullptr)
    {
        cout << "\nPatient not found!\n";
        return false;
    }

    temp->name = name;
    temp->age = age;
    temp->disease = disease;

    cout << "\nPatient updated successfully!\n";
    return true;
}

// Delete patient by ID
bool PatientList::deletePatient(int id)
{
    if (head == nullptr)
    {
        cout << "\nPatient list is empty!\n";
        return false;
    }

    Patient* temp = head;
    Patient* previous = nullptr;

    while (temp != nullptr && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        cout << "\nPatient not found!\n";
        return false;
    }

    if (previous == nullptr)
        head = head->next;
    else
        previous->next = temp->next;

    delete temp;

    cout << "\nPatient deleted successfully!\n";
    return true;
}

// Display all patients
void PatientList::displayAll()
{
    if (head == nullptr)
    {
        cout << "\nNo patient records available!\n";
        return;
    }

    Patient* temp = head;

    cout << "\n========== PATIENT RECORDS ==========\n";

    while (temp != nullptr)
    {
        cout << "\nPatient ID: " << temp->id;
        cout << "\nName: " << temp->name;
        cout << "\nAge: " << temp->age;
        cout << "\nDisease: " << temp->disease;
        cout << "\n-------------------------------------\n";

        temp = temp->next;
    }
}

// Return head pointer for algorithms and file handling
Patient* PatientList::getHead()
{
    return head;
}
