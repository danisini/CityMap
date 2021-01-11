#include <iostream>
#include <map>
#include <string>
#include "citymap.hpp"
using namespace std;
int main()
{
    string input;
    cin >> input;
    //D:\University\SDP\CityMap\input.1.txt
    CityMap cityMap(input);
    cout << cityMap.isThereAPath("NDK", "BSFS") << endl;;
    cout << cityMap.fromOneToAll("BSFS") << " " << cityMap.fromOneToAll("Popa") << " " << cityMap.fromOneToAll("5Kiosheta") << endl;
    vector <Street> v = cityMap.deadEnds();
    cout << "Dead ends:" << endl;
    for(int i = 0; i < v.size(); i ++)
        cout <<"From " << v[i].from << " to " << v[i].to << endl;

    cout << cityMap.possibleToReturn("BSFS") << endl;
    cityMap.possibleToVisitAllStreetsOnce();
    cityMap.dijkstraForKPaths("Popa", "5Kiosheta");
    return 0;
}
/*
Popa BSFS 150 5Kiosheta 1000
BSFS Popa 150
5Kiosheta NDK 400
NDK Popa 800
*/
