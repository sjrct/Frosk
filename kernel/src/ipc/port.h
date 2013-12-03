//
// kernel/src/ipc/port.h
//

#ifndef IPC_PORT_H
#define IPC_PORT_H

#include <util.h>

handle_t poll_port(void);
void refuse_port(handle_t);
uint read_port (void *, uint, handle_t);
uint write_port(void *, uint, handle_t);

#endif
