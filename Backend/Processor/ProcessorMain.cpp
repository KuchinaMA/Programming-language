#include <stdio.h>

#include "Types.h"
#include "Logfile.h"
#include "Stack.h"
#include "Protection.h"

#include "Processor.h"

int main() {

    open_log("Stackerrors.log", __TIME__);

    Processor processor = {};

    processor_ctor(&processor, "../Complier/MachineCodeF.bin");

    //printf("Enter the coefficents of the quadratic equation\n");

    cpu(&processor);

    processor_dtor(&processor);

    return 0;
}
