#include <Arduino.h>

/* LoLin 腳位定義 */
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

/* 溫度模型 */
class temperature
{
private:
    float h;
    float t;
    float f;
    String ErrorInfo;

public:
    temperature(float h, float t, float f) : h(h), t(t), f(f)
    {
        if (isnan(h) || isnan(t) || isnan(f))
            ErrorInfo = "dht:error";
        else
            ErrorInfo = "";
    }
    float getH()
    {
        if (ErrorInfo.length() > 0)
            return ErrorInfo;
        return h;
    }
    float getT()
    {
        if (ErrorInfo.length() > 0)
            return ErrorInfo;
        return t;
    }
    float getF()
    {
        if (ErrorInfo.length() > 0)
            return ErrorInfo;
        return f;
    }
};
