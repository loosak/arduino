/* Nastaveni ESP modulu, ktere pracuje takto:
 * 1. Pokud je forceConfigure ==0, tak se aktivuje WiFi v rezimu AP a cele ESP je mozne nastavit webovym rozhranim (pocitam s prenosem hodnoty nouzoveho tlacitka pri startu).
 * 2. Neni-li tlacitko stisknute, tak se vezme rezim prace a AP se nastavi dle nej (WIFI_STA a WIFI_AP)
 * 3. Pokud byl rezim prace WIFI_STA a ESP se nepripoji k zadne siti do casu WIFI_STA_CONNECT_TIMEOUT, tak se pokracuje jako kdyby bylo stisknute rekonfiguracni tlacitko
 */

#ifndef __WiFiConfig_h__
#define __WiFiConfig_h__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

#define elementSize(type, element) sizeof(((type *)0)->element) 

// Struktura konfigurace, ulozena v EEPROM
typedef struct
{
	uint8_t mode; // rezim prace AP/STA
	uint8_t ip; // konfigurace ip (staticka/DHCP)
	char ssid[32]; // SSID site
	char pass[64]; // heslo
	char devname[32]; // jmeno zarizeni (pro NBNS plati jen 16 znaku)
	uint32_t ipaddr; // ip adresa v pripade staticke konfigurace
	uint32_t netmask; // sitova maska v pripade staticke konfigurace
	uint32_t gateway; // sitova brana v pripade staticke konfigurace
	uint32_t dns; // ip adresa DNS serveru v pripade staticke konfigurace
} wificonfigarea_t;

// Parametr, predany uzivatelske callback funkci, urceny pro aplikacni vizualizaci stavu konfigurace a pripojeni
typedef enum
{
	WCS_CONNECTSTART = 0, // zacatek pokusu o pripojeni k ulozene konfiguraci
	WCS_CONNECTING = 1, // probiha pokus o pripojeni
	WCS_CONNECTED = 2, // pripojeni bylo uspesne
	WCS_CONFIGSTART = 3, // zacatek startu konfiguracniho AP
	WCS_CONFIGWAIT = 4, // cekame na nastaveni konfigurace pres web
}wificonfigstate_t;

typedef void (*wificonfig_cb)(wificonfigstate_t state); // definice callbacku

typedef void (*storeparam_cb)(const char *newvalue); // callback pro ulozeni uzivatelskeho parametru

#ifndef WIFI_STA_CONNECT_TIMEOUT
 #define WIFI_STA_CONNECT_TIMEOUT 10000UL // delka cekani na pripojeni k AP [ms]
#endif

#ifndef SETUP_SSID
 #define SETUP_SSID "ESPPBSetup"
#endif

/**
 *  \brief Ziskani retezce z EEPROM ze zadaneho offsetu
 *  
 *  \param [in] start Ofset zacatku retezce
 *  \param [in] len Delka retezce
 *  \return Vycteny retezec
 */
String getEEPROMString(int start, int len);

/**
 *  \brief Ulozeni retezce do EEPROM
 *  
 *  \param [in] start Ofset zacatku ukladani
 *  \param [in] len Maximalni delka ulozeneho retezce
 *  \param [in] string Ukladany retezec
 */
void setEEPROMString(int start, int len, String string);

/**
 *  \brief Ziskani nasi ip adresy
 *  
 *  \return Nase ip adresa, ziskana dle rezimu prace modulu
 */
IPAddress getOurIP(void);

/**
 *  \brief Ziskani nasi MAC adresy
 *  
 *  \param [in] mac kam ma byt MAC adresa ulozena
 *  \return Ukazatel na ulozenou MAC adresu (vraci parametr mac)
 *  
 *  \details Details
 */
uint8_t * getOurMAC(uint8_t *mac);

uint32_t getEEPROMuint32(int start);

void setEEPROMuint32(int start, uint32_t val);

extern char WiFiDeviceName[]; // jmeno zarizeni, pouzivane i pro DHCP

class WiFiConfigUsrParameter
{
  public:
    WiFiConfigUsrParameter(const char *id, const char *label, const char *defaultValue, int length, storeparam_cb cb);

    const char *getID();
    const char *getValue();
    const char *getLabel();
    int         getValueLength();
	void		setNext(WiFiConfigUsrParameter *n);
	WiFiConfigUsrParameter *getNext();
	void		setNewValue(const char *newval);
  private:
    const char *_id;
    const char *_label;
    char       *_value;
    int         _length;

	storeparam_cb _cb;
	WiFiConfigUsrParameter *_next;

    friend class WiFiConfig;
};

class WiFiConfig
{
public:
	WiFiConfig(): _params(NULL), _timeout(0xFFFFFFFF) {}
	void begin(int configarea, uint8_t forceConfigure, wificonfig_cb cb);
	void begin(int configarea, uint8_t forceConfigure, uint32_t timeout, wificonfig_cb cb);
    void addParameter(WiFiConfigUsrParameter *p);
private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;
	void setupAP(wificonfig_cb cb);
	void handleDisplayAP(void);
	void handleSetAP(void);
	void handle204(); // android/Chrome OS Captive portal handler
	void handleNotFound(); // notFound handler - jen jednoducha stranka kvuli CaptivePortalu umoznuje prejit na spravnou stranku (ale nedela to...)
	WiFiConfigUsrParameter *searchUsrParameter(const char *name);

	WiFiConfigUsrParameter *_params; // ukazatel na posledni zadany uzivatelsky parametr
	uint32_t _timeout; // timeout pri cekani na konfiguraci
};
#endif
