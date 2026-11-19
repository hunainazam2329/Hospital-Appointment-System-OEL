
#ifndef PATIENT_ALGORITHMS_H
#define PATIENT_ALGORITHMS_H

#include "PatientLinkedList.h"

class PatientAlgorithms
{
public:
    static void sortById(PatientList& patients);

    static void binarySearchId(PatientList& patients, int id);
};

#endif
