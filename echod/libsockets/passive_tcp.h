
#ifndef _PASSIVE_TCP_H
#define _PASSIVE_TCP_H

unsigned short get_port_from_name(const char *service);

int passive_tcp(unsigned short port, int qlen);

#endif
