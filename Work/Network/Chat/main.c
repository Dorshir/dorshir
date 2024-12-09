#include "clientnet.h"

int main()
{

    ClientNet* clientnet = ClientNet_Create("127.0.0.1", 5588);
    return 0;
}