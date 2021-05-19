#pragma once

int calc_factorial (int N)
{
    return ( N < 0 ) ? 0 : ( N == 0 ) ? 1 : ( N * calc_factorial( N-1 ) );
}
