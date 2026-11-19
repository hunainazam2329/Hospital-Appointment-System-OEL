
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "PatientLinkedList.h"
#include "AppointmentQueue.h"

class FileManager
{
public:
    static void savePatients(PatientList& patients);

    static void loadPatients(PatientList& patients);

    static void saveAppointments(AppointmentQueue& appointments);

    static void loadAppointments(
        AppointmentQueue& appointments,
        PatientList& patients);
};

#endif
