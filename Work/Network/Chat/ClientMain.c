#include <stdio.h>
#include "ClientApp.h"
int main()
{
    ClientApp* app = NULL;
    app = CreateApp();
    if(app == NULL)
    {
        printf("application not create\n");
        return 0;
    }
    int status = RunApp(app);
    if(status == 1)
    {
        DestroyApp(&app);
    }
    return 0;
}