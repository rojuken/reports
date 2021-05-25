#include <iostream>
#include "Multistack.h"
int main()
{
    TMultiStack<int> s(5,2);
    for (int i = 0; i < 4; ++i) {
        s.push(1,0);
    }
    return 0;
}
