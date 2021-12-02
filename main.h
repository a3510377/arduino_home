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

/* time */
const int s = 1e3;
const int m = s * 60;
const int h = m * 60;

/* 溫度模型 */
class temperature
{
private:
    float h;
    float t;
    float f;
    String ErrorInfo;

public:
    bool error = false;
    temperature(float h, float t, float f) : h(h), t(t), f(f)
    {
        if (isnan(h) || isnan(t) || isnan(f))
        {
            ErrorInfo = "dht:error";
            error = true;
        }
        else
            ErrorInfo = "";
    }
    String checkError()
    {
        return ErrorInfo;
    }
    float getH()
    {
        return h;
    }
    float getT()
    {
        return t;
    }
    float getF()
    {
        return f;
    }
};
