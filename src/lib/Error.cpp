#include "Error.h"

// initially defined to be zero for the program to be built to be non-daemon based
// In case of daemon process, logging will go through syslog (/var/log/system)
int unp::daemonProc = 0;
