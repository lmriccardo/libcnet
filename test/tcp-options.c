#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


struct TcpOption
{
    u_int8_t _kind;    //!< The type of TCP Option
    u_int8_t _length;  //!< How many bytes is the option big
    void*    _value;   //!< The value of the option
};


int numOfOptions;


void addTcpOption(struct TcpOptions** _opts, struct TcpOptions* _opt)
{
    _opts[numOfOptions++] = _opt;
}


int main(void)
{
    numOfOptions = 0;
    struct TcpOption* _options[10];



    return 0;
}