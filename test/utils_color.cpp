#include <stdio.h>
#include "../Linmeng/src/utils/color.h"


int main()
{
    auto vec = Color::FromHexStr("0x66ccff");
    printf("\n\
    /*-------------------------------------------------------------------------*/\n\
    ");
    printf("hexstr color test:\n\
     0x66ccff: %f,%f,%f",vec.x,vec.y,vec.z);
    return 0;
}