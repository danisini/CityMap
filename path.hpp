#ifndef _PATH_H
#define _PATH_H

#include <iostream>
#include <string>
#include <vector>

class Path
{
private:
    std::vector <std::string> crossroads;
    double cost;
    int numCros;
public:
    Path();
    void addCrossroad(const std::string&, const double&);
    std::string getLastCrossroad()const;
    void removeLast(const std::string&, const double&);
    bool operator<(const Path&)const;
    void print();
};
bool Path::operator<(const Path& path)const
{
    if(this->cost < path.cost)return false;
    return true;
}
void Path::print()
{
    std::cout << cost << std::endl;
    for(int i = 0; i < numCros - 1; i ++)
    {
        std::cout << crossroads[i] << " --> ";
    }
    std::cout << crossroads[numCros - 1] << std::endl;
}
void Path::removeLast(const std::string& crossroad, const double& weight)
{
    crossroads.pop_back();
    cost -= weight;
    numCros --;
}
Path::Path()
{
    cost = 0.0;
    numCros = 0;
}
std::string Path::getLastCrossroad()const
{
    if(numCros > 0)return crossroads[numCros - 1];
    else return "Empty path!";
}
void Path::addCrossroad(const std::string& crossroad, const double& weight)
{
    crossroads.push_back(crossroad);
    cost += weight;
    numCros ++;
}
#endif
