#include <utils/net.h>

void test_get_local_mtu(const char* interface)
{
    int lmtu = getInterfaceMTU(interface);
    printf("%s MTU is %d bytes\n", interface, lmtu);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./mtu [option] [arg1, ...]\n");
        exit(EXIT_FAILURE);
    }

    if (strncmp(argv[1], "lmtu", 4) == 0)
    {
        test_get_local_mtu(argv[2]);
        return 0;
    }

    if (strncmp(argv[1], "pmtu", 4) == 0)
    {
        return 0;
    }

    fprintf(stderr, "Usage: [option] must be either: lmtu or pmtu\n");
    exit(EXIT_FAILURE);
}