#include <Instrumentor.h>

int main(void)
{
    Instrumentor::Get().BeginSession("report");

    {
        InstrumentationTimer timer("Division");

        float b = 6;

        for (int i = 0; i < 10000000; i++)
        {
            float a = b / 32;
        }
    }

    {
        InstrumentationTimer timer("Bit Shift");

        int b = 6;

        for (int i = 0; i < 10000000; i++)
        {
            int a = b >> 5;
        }
    }

    {
        InstrumentationTimer timer("Module");

        int b = 80;

        for (int i = 0; i < 10000000; i++)
        {
            int a = b % 32;
        }
    }

    {
        InstrumentationTimer timer("And");

        int b = 6;

        for (int i = 0; i < 10000000; i++)
        {
            int a = b & 31;
        }
    }

    Instrumentor::Get().EndSession();
}