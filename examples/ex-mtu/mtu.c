#include <utils/net.h>


int main(int argc, char** argv)
{
    char interface[5] = "lo";
    int lmtu = getInterfaceMTU(interface);
    printf("%s MTU is %d bytes\n", interface, lmtu);
    
    return 0;
}