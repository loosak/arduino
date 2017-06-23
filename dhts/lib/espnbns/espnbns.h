//
#ifndef __ESPNBNS_h__
#define __ESPNBNS_h__

#include <ESP8266WiFi.h>

#define NBNS_PORT 137
/**
* @def NBNS_MAX_HOSTNAME_LEN
* @brief maximalni delka NBNS jmena zarizeni
* @remarks
* Jmeno zarizeni musi byt uvedeno VELKYMI pismenami a nesmi obsahovat mezery (whitespaces).
*/
#define NBNS_MAX_HOSTNAME_LEN 16

class NBNS
{
public:
	bool begin(const char *name);
	void poll(void);
private:
	char _name[NBNS_MAX_HOSTNAME_LEN + 1];
	void _getnbname(char *nbname, char *name, uint8_t maxlen);
	void _makenbname(char *name, char *nbname, uint8_t outlen);
};
#endif