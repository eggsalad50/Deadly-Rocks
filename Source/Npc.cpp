#include "Npc.h"

void Computer_controled(int sh)
{
    pship[sh].dirx*= 0.99;   //Ship speed decay
    pship[sh].diry*=0.99;
    pship[sh].thrust_flag = false;

    if (sound_thrust_flag[sh])
    {
        sound_thrust_flag[sh] = false;
        alSourceStop(thrust_sourceid[sh]);
    }
}
