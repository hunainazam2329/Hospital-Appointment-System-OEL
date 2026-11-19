
#include "PatientAlgorithms.h"
#include <utility>

// Bubble Sort by patient ID
void PatientAlgorithms::sortById(PatientList& patients)
{
    Patient* head = patients.getHead();

    if (head == nullptr || head->next == nullptr)
    {
        cout << "\nNot enough records to sort.\n";
        return;
    }

    bool swapped;

    do
    {
        swapped = false;
        Patient* current = head;

        while (current->next != nullptr)
        {
            Patient* nextNode = current->next;

            if (current->id > nextNode->id)
            {
                // Swap complete patient information
                swap(current->id, nextNode->id);
                swap(current->name, nextNode->name);
                swap(current->age, nextNode->age);
                swap(current->disease, nextNode->disease);

                swapped = true;
            }

            current = current->next;
        }

    } while (swapped);

    cout << "\nPatients sorted by ID successfully!\n";
}

// Binary Search using temporary pointer array
void PatientAlgorithms::binarySearchId(
    PatientList& patients, int id)
{
    Patient* temp = patients.getHead();

    if (temp == nullptr)
    {
        cout << "\nPatient list is empty!\n";
        return;
    }

    // Sort records before binary search
    sortById(patients);

    // Count patients
    int count = 0;

    temp = patients.getHead();

    while (temp != nullptr)
    {
        count++;
        temp = temp->next;
    }

    // Create temporary array of pointers
    Patient** arr = new Patient*[count];

    temp = patients.getHead();

    for (int i = 0; i < count; i++)
    {
        arr[i] = temp;
        temp = temp->next;
    }

    int low = 0;
    int high = count - 1;
    bool found = false;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid]->id == id)
        {
            cout << "\nPatient Found!\n";
            cout << "ID: " << arr[mid]->id << endl;
            cout << "Name: " << arr[mid]->name << endl;
            cout << "Age: " << arr[mid]->age << endl;
            cout << "Disease: " << arr[mid]->disease << endl;

            found = true;
            break;
        }
        else if (arr[mid]->id < id)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    if (!found)
        cout << "\nPatient ID not found!\n";

    delete[] arr;
}
