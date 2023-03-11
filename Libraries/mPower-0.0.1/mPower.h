#include <ESP8266WiFi.h>

// enable power factor readout
#define POWER_FACTOR

#define TELNET_PORT 23

class mPower {

    public:
        // get Status fills enabled, power, powerFactor and portsAvailable 
        bool getStatus();
        // set ports updates all ports. can be set by setting the public variable enable.
        bool setPorts();
        // makes the current setting the default setting after startup
        bool setDefault();

        bool getPort(unsigned char port);
        bool setPort(unsigned char port, bool status);

        bool enabled[6];
        float power[6];
        #ifdef POWER_FACTOR
        float powerFactor[6];
        #endif
        unsigned char portsAvailable = 0;

        mPower(char *host) {
            m_host = (char *)malloc(strlen(host));
            if(m_host != NULL)
                strcpy(m_host,host);
        }

    private:

        WiFiClient  m_device;
        char* m_user = "ubnt";
        char* m_password = "ubnt";
        char* m_host;
        String m_cmdLine;

        bool login();
        bool readUntil(unsigned char wait);
};
