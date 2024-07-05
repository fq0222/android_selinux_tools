#include "denieditem.h"
#include <iostream>

void DeniedItem::print()
{
    std::cout << scontext << std::endl;
    std::cout << tcontext << std::endl;
    std::cout << tclass << std::endl;
}
