#include "Time_Order.h"

TimeRange getTimeRange(Time t)
{
    if(t.minute < 30)
    {
        if(t.hour == 9) return first;
        else if(t.hour == 10) return third;
        else if(t.hour == 11) return fifth;
        else if(t.hour == 12) return seventh;
        else if(t.hour == 13) return ninth;
        else if(t.hour == 14) return eleventh;
        else if(t.hour == 15) return thirteenth;
    }
    else
    {
        if(t.hour == 9) return second;
        else if(t.hour == 10) return fourth;
        else if(t.hour == 11) return sixth;
        else if(t.hour == 12) return eighth;
        else if(t.hour == 13) return tenth;
        else if(t.hour == 14) return twelfth;
        else if(t.hour == 15) return fourteenth;
    }
}

Time getTimeBase(TimeRange tr)
{
    switch(tr)
    {
        case first:
            return Time(9, 5);
        case second:
            return Time(9, 35);
        case third:
            return Time(10, 5);
        case fourth:
            return Time(10, 35);
        case fifth:
            return Time(11, 5);
        case sixth:
            return Time(11, 35);
        case seventh:
            return Time(12, 5);
        case eighth:
            return Time(12, 35);
        case ninth:
            return Time(13, 5);
        case tenth:
            return Time(13, 35);
        case eleventh:
            return Time(14, 5);
        case twelfth:
            return Time(14, 35);
        case thirteenth:
            return Time(15, 5);
        case fourteenth:
            return Time(15, 35);
    }
}