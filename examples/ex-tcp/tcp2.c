#include <ip.h>

int main(void)
{
    u_int32_t v1 = 3438707201;
    u_int32_t v0 = 0;

    struct TcpOption* opt = TcpOption_newTimestamps(v1, v0);

    return 0;
}