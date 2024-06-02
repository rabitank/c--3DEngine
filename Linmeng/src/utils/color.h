#pragma once
#include "Umath.h"
#include <string>

namespace Color
{
    /// @brief convert hex string color to standard vector3 color
    /// @param str example 0x66ccff
    Vector3 FromHexStr(const char* str)
    {
        unsigned int num  = std::stoi(str,NULL,16);

        const int  r =static_cast<int>( (num & ( 255 << 16 )) >> 16);
        const int  g =static_cast<int>( (num & ( 255 << 8  )) >> 8) ;
        const int  b =static_cast<int>( (num & ( 255  )     )     );

        return Vector3(r/256.f, g/256.f ,b/256.f );
    };

}
