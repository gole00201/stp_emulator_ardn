#include "stp.hpp"
#define STP_CNT 13
#define DEBUG_LVL 0
#define BAUD 9600

String list_of_bmk[STP_CNT] = {"009", "010", "011",
                               "012", "013", "014",
                               "015", "016", "017",
                               "018", "019", "020",
                               "021"};
Stp arr_stp[STP_CNT];
String buf = "";
const int timeToStpiter = 3000;
int randPresure = 0;
unsigned int currentTime = 0;
int currentStp = 0;

void SetUpStp(String *cfg, Stp *ar_stp)
{
    for (size_t i = 0; i < STP_CNT; ++i)
    {
        ar_stp[i].name = cfg[i];
        ar_stp[i].pr1 = "000";
        ar_stp[i].pr2 = "000";
        ar_stp[i].stup = "000";
    }
}

String readCom()
{
    delay(30);
    String buffer = "";
    int b = Serial.available();
    for (int i = 0; i < b; ++i)
    {
        char tmp = Serial.read();
        buffer += String(tmp);
    }
    return buffer;
}

int genAnswer(String com, Stp *arr_stp)
{
    // Serial.print(com);
    String com_part = com.substring(8, com.length() - 2);
    String bmk = com.substring(4, 7);
    if (DEBUG_LVL)
    {
        Serial.println("DEBUG: Get " + com_part);
        Serial.println("DEBUG: Get " + bmk);
    }
    if (com_part.equals("getStatus"))
    {
        for (int i = 0; i < STP_CNT; ++i)
        {
            // Serial.println(arr_stp[i].name);
            if (bmk.equals(arr_stp[i].name))
            {
                genGetStatus(arr_stp[i]);
            }
        }
        return 0;
    }
    if (com_part.equals("gPr"))
    {
        for (int i = 0; i < STP_CNT; ++i)
        {
            if (bmk.equals(arr_stp[i].name))
            {
                genGetPr(arr_stp[i]);
            }
        }
        return 0;
    }
    return 1;
}

void genGetStatus(Stp bmk)
{
    Serial.print("bmk=" + bmk.name + " bmkS=007 bmkSK=2 pr=" + bmk.pr1 + " pr0=" + bmk.pr1 + " pr1=" + bmk.pr1 + " temp=+232 P05=064 P10=125 P15=219 P20=316 P25=401 P30=489 P35=581 Err=00000000 uPit=23 temHeart=+05 timeW=00003053 prAtmCal0=+00 prAtmCal1=+00 Styp=00 l=000 temp2=+242 timeR=000006 cs=114\r\n");
}

void genGetPr(Stp bmk)
{
    Serial.print("bmk=" + bmk.name + " pr0=000 pr1=" + bmk.pr1 + " pr2=" + bmk.pr2 + " er=00000000 bmkC=007 prC0=003 prC1=000 erC=00000000 cs=016\r\n");
}

void setup()
{
    SetUpStp(list_of_bmk, arr_stp);
    Serial.begin(BAUD);
    while (!Serial)
    {
    };
}

void loop()
{
    while (Serial.available())
    {
        buf = readCom();
        if (millis() - currentTime >= timeToStpiter)
        {
            int randomInt = random(300, 501);
            arr_stp[currentStp].pr1 = String(randomInt);
            arr_stp[currentStp].pr2 = arr_stp[currentStp].pr1;
            if(currentStp > 0){
                arr_stp[currentStp-1].pr1 = "0";
                arr_stp[currentStp-1].pr2 = "0";
            }
            currentStp++;
            if (currentStp > 13)
            {
                currentStp = 0;
            }
            currentTime = millis();
        }
        genAnswer(buf, (Stp *)arr_stp);
    }
}
