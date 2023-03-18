#include "exerciser.h"

void exercise(connection *C)
{
    cout << "TEST QUERY1:\n\n";
    query1(C, 2, 5, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    cout << "\n";
    
    cout << "TEST QUERY2:\n\n";
    query2(C, "Red");
    cout << "\n";

    cout << "TEST QUERY3:\n\n";
    query3(C, "Duke");
    cout << "\n";

    cout << "TEST QUERY4:\n\n";
    query4(C, "NC", "DarkBlue");
    cout << "\n";
  
    cout << "TEST QUERY5:\n\n";
    query5(C, 11);
    cout << "\n";
}
