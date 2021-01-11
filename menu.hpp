#ifndef _MENU_H
#define _MENU_H
#include "citymap.hpp"
class Menu
{
private:
    CityMap cityMap;
    void help();
public:
    Menu();
};

void Menu::help()
{
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "1 <cross1> <cross2>                       |" << "checks for a path between two crossroads" << std::endl;
    std::cout << "2 <cross1> <cross2>                       |" << "finds the 3 lowest-cost paths between two crossroads" << std::endl;
    std::cout << "3 <cross1> <cross2> <num_closed_crosses>  |" << "finds the 3 lowest-cost paths between two crossroads" << std::endl;
    std::cout << "<closed_cross_1> <closed_cross_2> ...     |" << "and given list of closed crossroads" << std::endl;
    std::cout << "4 <cross1>                                |" << "checks if there is a cycle in the city" << std::endl;
    std::cout << "5                                         |" << "checks for Eulerian path in the city" << std::endl;
    std::cout << "6 <cross1>                                |" << "checks if a crossroad can reach all the others" << std::endl;
    std::cout << "7                                         |" << "finds all dead ends" << std::endl;
    std::cout << "8                                         |" << "help command" << std::endl;
    std::cout << "9                                         |" << "closes the menu" << std::endl;
    if(cityMap.getInteractive() == true)
    {
         std::cout << "10                                        |" << "outputs the current crossroad" << std::endl;
         std::cout << "11 <cross>                                |" << "changes the current crossroad" << std::endl;
         std::cout << "12                                        |" << "outputs the neighbours of the current crossroad" << std::endl;
         std::cout << "13 <cross>                                |" << "adds cross to closed crossroads" << std::endl;
         std::cout << "14 <cross>                                |" << "removes cross from closed crossroads" << std::endl;
         std::cout << "15                                        |" << "outputs all the closed crossroads" << std::endl;
         std::cout << "16                                        |" << "outputs an Eulerian path" << std::endl;
    }
}
Menu::Menu()
{
    std::cout << "Hello! Please, input the directory of the file you will be using:" << std::endl;
    std::string input;
    std::cin >> input;
    cityMap.open(input);
    std::cout << "Do you want to turn on interactive mode? Y/N" << std::endl;
    char ans;
    std::cin >> ans;
    if(ans == 'Y') cityMap.setInteractive();
    std::cout << "For more information input 8!" << std::endl;
    int num = 0;
    std::string cross1, cross2;
    while(num != 9)
    {
        std::cin >> num;
        if(num == 1)
        {
            std::cin >> cross1 >> cross2;
            std::cout << cityMap.isThereAPath(cross1, cross2) << std::endl;
        }
        else if(num == 2)
        {
            std::cin >> cross1 >> cross2;
            cityMap.dijkstraForKPaths(cross1, cross2);
        }
        else if(num == 3)
        {
            std::cin >> cross1 >> cross2;
            int numCrossesClosed;
            std::cin >> numCrossesClosed;
            std::vector <std::string> closed;
            std::string cross;
            for(int i = 0; i < numCrossesClosed; i ++)
            {
                std::cin >> cross;
                closed.push_back(cross);
            }
            cityMap.dijkstraForKPathsWithClosed(cross1, cross2, closed);
        }
        else if(num == 4)
        {
            std::cin >> cross1;
            std::cout << cityMap.possibleToReturn(cross1) << std::endl;
        }
        else if(num == 5)
            cityMap.possibleToVisitAllStreetsOnce();
        else if(num == 6)
        {
            std::cin >> cross1;
            std::cout << cityMap.fromOneToAll(cross1) << std::endl;
        }
        else if(num == 7)
        {
            std::vector <Street> v = cityMap.deadEnds();
            std::cout << "Dead ends:" << std::endl;
            for(int i = 0; i < v.size(); i ++)
            {
                std::cout <<"From " << v[i].from << " to " << v[i].to << std::endl;
            }
        }
        else if(num == 8)
            help();
        else if(num == 9)break;
        else if(cityMap.getInteractive() == true)
        {
            if(num == 10)
                cityMap.location();
            else if(num == 11)
            {
                std::cin >> cross1;
                cityMap.change(cross1);
            }
            else if(num == 12)
                cityMap.neighbours();
            else if(num == 13)
            {
                std::cin >> cross1;
                cityMap.close(cross1);
            }
            else if(num == 14)
            {
                std::cin >> cross1;
                cityMap.openCross(cross1);
            }
            else if(num == 15)
                cityMap.closed();
            else if(num == 16)
                cityMap.possibleToVisitAllStreetsOnce();
            else
            {
                std::cout << "Not a valid command!" << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "Not a valid command!" << std::endl;
            break;
        }
    }

}
#endif // _MENU_H
