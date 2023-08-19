#ifndef POINT_H
#define POINT_H

struct Point {
public:
    double latitude;
    double longitude;
    double altitude;
    Point(){};
    Point(double p_latitude,double p_longitude,double p_altitude):latitude(p_latitude)
    ,longitude(p_longitude),altitude(p_altitude){}

};

#endif // POINT_H
