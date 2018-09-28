#pragma once
#ifndef PCH_H
#define PCH_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <map>
#include <bitset>
#include <deque>
#include <math.h>
#include <Windows.h>
#include <functional>
#include <fstream>
#include <chrono>


#define M_PI 3.141592654
#define M_RAD_DEGREE 0.0174532925
#define M_DEGREE_RAD 57.295779513

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define PARALL

#ifdef PARALL
#include <ppl.h>
#endif // PARALL


#endif // !PCH_H
