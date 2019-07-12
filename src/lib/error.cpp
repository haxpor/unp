#include "error.h"

// initially defined to be non-zero for the program to be built to be daemon based
// thus logging will go through syslog (/var/log/system can be checked for error message)
int unp::daemonProc = 1;
