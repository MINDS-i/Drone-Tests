#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"
#include "TestStreams.h"

#include "comms/NMEA.h"
//TESTING "comms/NMEA.cpp"
//TESTING "math/GreatCircle.cpp"
//TESTING "math/Waypoint.cpp"
//TESTING "math/SpatialMath.cpp"

typedef struct GPRMC{
    const char * desc;
    const char * str;
    float time;
    bool  active;
    float latitude;
    float longitude;
    float speed;
    float trackAngle;
    long  date;
    float magVar;
} GPRMC;

/*
lat/lon is in degrees+decimal minutes, code spec is decimal degrees
function decimalDegrees(degreeDecimalMinutes)
    minutes = degreeDecimalMinutes % 100.0
    degrees = (degreeDecimalMinutes - minutes) / 100.0
    decimalDegrees = degrees + minutes/60.0
    return decimalDegrees
end

speed in GPRMC is in knots, code spec returns mph
mph(knots) = knots * 1.15077945
*/

//examples taken from http://www.gpsinformation.org/dale/nmea.htm
const GPRMC goodData[] {
    { "NE GPRMC",
      "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A",
      123519, true, 48.11729, 11.51666, 25.77746, 84.4, 230394, 3.1
    },
    { "NW GPRMC",
      "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F",
      183729, true, 39.12260, -121.04136, 0.0, 360.0, 80301, 15.5
    },
    { "Bad Status",
      "$GPRMC,152926,V,6027.8259,N,02225.6713,E,10.8,0.0,190803,5.9,E,S*22",
      152926, false, 60.46376, 22.42786, 12.428418, 0.0, 190803, 5.9
    },
    { "Empty Fields",
      "$GPRMC,162254.00,A,3723.02837,N,12159.39853,W,0.820,188.36,110706,,,A*74",
      162254, true, 37.38380, -121.98998, 0.94364, 188.36, 110706, 0.0
    },
    { "DecimalTime",
      "$GPRMC,230611.016,V,3907.3813,N,12102.4635,W,0.14,136.40,041002,,*04",
      230611.016, false, 39.12302, -121.04106, 0.161109, 136.40, 41002, 0.0
    },
    { "Surrounded GPRMC",
      "$GPGSV,3,3,09,24,12,282,00*4D"
      "$GPGLL,3553.5295,N,13938.6570,E,002454,A,A*4F"
      "$GPBOD,,T,,M,,*47"
      "$PGRME,8.6,M,9.6,M,12.9,M*15"
      "$PGRMZ,51,f*30"
      "$HCHDG,101.1,,,7.1,W*3C"
      "$GPRTE,1,1,c,*37"
      "$GPRMC,002456,A,3553.5295,N,13938.6570,E,0.0,43.1,180700,7.1,W,A*3D"
      "$GPGGA,002454,3553.5295,N,13938.6570,E,1,05,2.2,18.3,M,39.0,M,,*7F"
      "$GPGSA,A,3,01,04,07,16,20,,,,,,,,3.6,2.2,2.7*35"
      "$GPGSV,3,1,09,01,38,103,37,02,23,215,00,04,38,297,37,05,00,328,00*70"
      "$GPGSV,3,2,09,07,77,299,47,11,07,087,00,16,74,041,47,20,38,044,43*73",
      2456, true, 35.89215, 139.64428, 0.0, 43.1, 180700,7.1
    },
};
const int numGoodData = sizeof(goodData)/sizeof(goodData[0]);

const char * badStrings[] {
    "$$$$$$$$$$$",
    "$,,,,,,,,,,,,,,,,,,,",
    "$GPRMC,,,,,,,,,,,,,,,,,,",
    "$GPRMC$",
    "$GPRMC,123519,X,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A",
    "$GPRMC,X,X,X,X,X,X,X,X,X,X,XXXX",
    "$GPRMC,123519549631488436214567/89321456,A,",
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
    "4807.038,N,01131.00000-100.4,E,022.4,084.4,2303946544563217896,003.1,W*6A",
};
const int numBadStrings = sizeof(badStrings)/sizeof(badStrings[0]);

#define nmeaOfString(s) \
    StringStream ss(s); \
    NMEA nmea(ss); \
    nmea.update(); \
    ss.reset("$"); \
    nmea.update();
    //the extra dollar sign is an artifact of the current NMEA implementation

bool parseTime(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getTimeOfFix(), goodData[i].time);
    }
    return true;
}
bool parseActive(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        CASSERT(nmea.getWarning(), !goodData[i].active);
    }
    return true;
}
bool parseLatitude(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getLatitude(), goodData[i].latitude);
    }
    return true;
}
bool parseLongitude(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getLongitude(), goodData[i].longitude);
    }
    return true;
}
bool parseSpeed(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getGroundSpeed(), goodData[i].speed);
    }
    return true;
}
bool parseTrackAngle(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getCourse(), goodData[i].trackAngle);
    }
    return true;
}
bool parseDate(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        CASSERT(nmea.getDateOfFix(), goodData[i].date);
    }
    return true;
}
bool parseMagVar(){
    for(int i=0; i<numGoodData; i++){
        nmeaOfString(goodData[i].str);
        FPASSERT(nmea.getMagVar(), goodData[i].magVar);
    }
    return true;
}
bool delayedInput(){
    char charContainer[] = {'_', '\0'};
    StringStream ss(charContainer);
    NMEA nmea(ss);
    for(int i=0; i<strlen(goodData[0].str); i++){
        charContainer[0] = goodData[0].str[i];
        ss.reset(charContainer);
        nmea.update();
    }
    ss.reset("$");
    nmea.update();
    FPASSERT(goodData[0].time, nmea.getTimeOfFix());
    CASSERT( goodData[0].active, !nmea.getWarning());
    FPASSERT(goodData[0].latitude, nmea.getLatitude());
    FPASSERT(goodData[0].longitude, nmea.getLongitude());
    FPASSERT(goodData[0].speed, nmea.getGroundSpeed());
    FPASSERT(goodData[0].trackAngle, nmea.getCourse());
    CASSERT( goodData[0].date, nmea.getDateOfFix());
    FPASSERT(goodData[0].magVar, nmea.getMagVar());
}
bool testNewInput(){
    StringStream ss(goodData[0].str);
    NMEA nmea(ss);
    ASSERT(!nmea.newData());
    nmea.update();
    ss.reset("$");
    nmea.update();
    ASSERT(nmea.newData());
    nmea.getLatitude();
    ASSERT(!nmea.newData());
    nmea.update();
    ASSERT(!nmea.newData());
    return true;
}
bool badStringFirst(){
    // test that the parser can recover from getting a bunch of bad data first
    for(int i=0; i<numBadStrings; i++){
        StringStream ss(badStrings[i]);
        NMEA nmea(ss);
        nmea.update();
        ss.reset(goodData[0].str);
        nmea.update();
        ss.reset("$");
        nmea.update();

        FPASSERT(goodData[0].time, nmea.getTimeOfFix());
        CASSERT( goodData[0].active, !nmea.getWarning());
        FPASSERT(goodData[0].latitude, nmea.getLatitude());
        FPASSERT(goodData[0].longitude, nmea.getLongitude());
        FPASSERT(goodData[0].speed, nmea.getGroundSpeed());
        FPASSERT(goodData[0].trackAngle, nmea.getCourse());
        CASSERT( goodData[0].date, nmea.getDateOfFix());
        FPASSERT(goodData[0].magVar, nmea.getMagVar());
    }
    return true;
}

int main(void){
    TEST(parseTime       );
    TEST(parseActive     );
    TEST(parseLatitude   );
    TEST(parseLongitude  );
    TEST(parseSpeed      );
    TEST(parseTrackAngle );
    TEST(parseDate       );
    TEST(parseMagVar     );
    TEST(badStringFirst  );
    TEST(delayedInput    );
    TEST(testNewInput    );
    return 0;
}
