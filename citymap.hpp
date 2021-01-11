#ifndef _CITYMAP_H
#define _CITYMAP_H

#include "path.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <set>

struct Street
{
    std::string from, to;
};
class CityMap
{
    private:
        std::unordered_map <std::string, std::unordered_map<std::string, double> > adj;
        std::unordered_map <std::string, std::vector<std::string> >adjRev;
        std::unordered_map <std::string, int> crossroads;
        std::unordered_map <std::string, bool> used;
        std::unordered_map <std::string, bool> closedCrossroads;
        int numCross;
        std::string currCrossroad;
        bool interactive;
        void dfs(const std::string&);
        void dfsRev(const std::string&);
        bool dfsCycle(const std::string&, const std::string&);
        std::queue<std::string> eulerCycle(const std::string&);
        bool isStronglyConnected();
        public:
        void open(const std::string&);
        void setInteractive(){interactive = true;}
        bool getInteractive(){return interactive;}
        std::string isThereAPath(const std::string&, const std::string&);
        std::string fromOneToAll(const std::string&);
        std::vector <Street> deadEnds();
        std::string possibleToReturn(const std::string&);
        void possibleToVisitAllStreetsOnce();
        void dijkstraForKPaths(const std::string&, const std::string&);
        void dijkstraForKPathsWithClosed(const std::string&, const std::string&, std::vector <std::string>);
        void location()const;
        void change(const std::string&);
        void neighbours();
        void close(const std::string&);
        void openCross(const std::string&);
        void closed();
};
void CityMap::closed()
{
    std::unordered_map <std::string, bool> :: iterator it;
    for(it = closedCrossroads.begin(); it != closedCrossroads.end(); ++it)
        std::cout << it->first << std::endl;
}
void CityMap::openCross(const std::string& cross)
{
    closedCrossroads.erase(cross);
}
void CityMap::close(const std::string& cross)
{
    closedCrossroads[cross] = true;
}
void CityMap::neighbours()
{
    std::unordered_map <std::string, double> :: iterator it;
    for(it = adj[currCrossroad].begin(); it != adj[currCrossroad].end(); ++it)
    {
        std::cout << it->first << std::endl;
    }
}
void CityMap::change(const std::string& newCross)
{
    currCrossroad = newCross;
}
void CityMap::location()const
{
    std::cout << currCrossroad << std::endl;
}
void CityMap::dijkstraForKPathsWithClosed(const std::string& from, const std::string& to, std::vector <std::string> listClosed)
{
    for(int i = 0; i < listClosed.size(); i ++)
        closedCrossroads[listClosed[i]] = 1;
    dijkstraForKPaths(from, to);
}
void CityMap::dijkstraForKPaths(const std::string& s, const std::string& t)
{
    std::vector <Path> paths;
    std::priority_queue <Path> pq;
    std::unordered_map <std::string, int> countPaths;
    std::unordered_map <std::string, int> :: iterator it;
    for(it = crossroads.begin(); it != crossroads.end(); ++ it)
        countPaths[it->first] = 0;
    Path p;
    p.addCrossroad(s, 0.0);
    pq.push(p);
    while(!pq.empty())
    {
        Path curr = pq.top();
        std::string u = curr.getLastCrossroad();
        pq.pop();
        countPaths[u] ++;
        if(u == t)paths.push_back(curr);
        if(countPaths[u] <= 3)
        {
            std::unordered_map <std::string, double> :: iterator it;
            for(it = adj[u].begin(); it != adj[u].end(); ++it)
            {
                if(closedCrossroads.find(it->first) != closedCrossroads.end())continue;
                curr.addCrossroad(it->first, it->second);
                pq.push(curr);
                curr.removeLast(it->first, it->second);
            }
        }
    }
    if(paths.size() < 3) std::cout << "There are not 3 different ways to achieve this!" << std::endl;
    else
    for(int i = 0; i < 3; i ++)
        paths[i].print();
}
bool CityMap::isStronglyConnected()
{
     std::unordered_map <std::string, int> :: iterator it;
     std::string start;
     int cnt = 0;
     for(it = crossroads.begin(); it != crossroads.end(); ++ it)
     {
        if(adj[it->first].size() != 0)
        {
            start = it->first;
            cnt ++;
        }
     }
     used.clear();
     dfs(start);
     if(used.size() != cnt) return 0;
     used.clear();
     dfsRev(start);
     if(used.size() != cnt) return 0;
     return 1;
}
std::queue<std::string> CityMap::eulerCycle(const std::string& x)
{
    std::unordered_map <std::string, bool> visitedStreets;
    std::stack <std::string> eulerPathHelper;
    eulerPathHelper.push(x);
    std::queue <std::string> ans;
    while(!eulerPathHelper.empty())
    {
        std::string curr = eulerPathHelper.top();
        eulerPathHelper.pop();
        std::unordered_map <std::string, double> :: iterator it;
        for(it = adj[curr].begin(); it != adj[curr].end(); ++it)
        {
            if(visitedStreets.find(curr + it->first) == visitedStreets.end())
            {
                visitedStreets[curr + it->first] = true;
                eulerPathHelper.push(it->first);
            }
        }
        ans.push(curr);
    }
    return ans;
}
void CityMap::possibleToVisitAllStreetsOnce()
{
    std::unordered_map <std::string, int> :: iterator it;
    std::string in = "", out = "";
    int cnt = 0; //counter of number of vertices with no equal in and out vertices
    for(it = crossroads.begin(); it != crossroads.end(); ++ it)
    {
        if(adj[it->first].size() != adjRev[it->first].size())
        {
            if(adj[it->first].size() - adjRev[it->first].size() == 1) out = it->first;
            else
            if(adjRev[it->first].size() - adj[it->first].size() == 1) in = it->first;
            else cnt = 5; // this means we will stop it because we need 0 or 2 to have euler's path
            cnt ++;
        }
    }
    ///there are two options - euler's path or euler's circle
    if((cnt != 2 && cnt != 0) || (cnt == 2 && (in == "" || out == "")))
    {
        std::cout << "It is not possilbe to visit all streets once" << std::endl;
        return;
    }
    if(cnt == 2)adj[in][out] = 0;
    if(isStronglyConnected() == false)
    {
        if(cnt == 2)adj[in].erase(out);
        std::cout << "It is not possilbe to visit all streets once" << std::endl;
        return;
    }
    if(cnt == 2)
    {
        std::queue<std::string> ans = eulerCycle(out);
        int sz = ans.size();
        for(int i = 0; i < sz - 2; i ++)
        {
            std::cout << ans.front() << " --> ";
            ans.pop();
        }
        std::cout << ans.front() << std::endl;
        adj[in].erase(out);
    }
    else
    {
        std::string start;
        for(it = crossroads.begin(); it != crossroads.end(); ++ it)
        {
            if(adj[it->first].size() != 0)
            {
                start = it->first;
                break;
            }
        }
        std::queue<std::string> ans = eulerCycle(start);
        while(ans.size() > 1)
        {
            std::cout << ans.front() << " --> ";
            ans.pop();
        }
        std::cout << ans.front() << std::endl;
    }
}
std::string CityMap::possibleToReturn(const std::string& crossroad)
{
    used.clear();
    if(dfsCycle(crossroad, crossroad)) return "Yes, it is possible to return!";
    return "No, it isn't possible to return!";
}
bool CityMap::dfsCycle(const std::string& x, const std::string& from)
{
    used[x] = 1;
    std::unordered_map <std::string, double> :: iterator it;
    bool isInCycle = false;
    for(it = adj[x].begin(); it != adj[x].end(); ++it)
    {
        if(it->first == from) return true;
        if(used.find(it -> first) == used.end())
            isInCycle = isInCycle|dfsCycle(it -> first, from);
    }
    return isInCycle;

}
std::vector <Street> CityMap::deadEnds()
{
    std::unordered_map <std::string, int> :: iterator it;
    std::vector <Street> ans;
    Street a;
    for(it = crossroads.begin(); it != crossroads.end(); ++ it)
    {
        if(adj[it->first].size() == 0)
        {
            a.to = it->first;
            for(int i = 0; i < adjRev[it->first].size(); i ++)
            {
                a.from = adjRev[it->first][i];
                ans.push_back(a);
            }
        }
    }
    return ans;
}
std::string CityMap::fromOneToAll(const std::string& from)
{
    used.clear();
    dfs(from);
    if(used.size() == numCross) return "Yes, there is a path from " + from + " to all the other crossroads!";
    return "No, there isn't a path from " + from + " to all the other crossroads!";
}

void CityMap::dfs(const std::string& x)
{
    used[x] = 1;
    std::unordered_map <std::string, double> :: iterator it;
    for(it = adj[x].begin(); it != adj[x].end(); ++it)
        if(used.find(it -> first) == used.end())
            dfs(it -> first);
}
void CityMap::dfsRev(const std::string& x)
{
    used[x] = 1;
    for(int i = 0; i < adjRev[x].size(); i ++)
        if(used.find(adjRev[x][i]) == used.end())
            dfsRev(adjRev[x][i]);
}
std::string CityMap::isThereAPath(const std::string& from, const std::string& to)
{
    used.clear();
    dfs(from);
    if(used.find(to) == used.end())return "No, there isn't such a path from " + from + " to " + to +"!";
    return "Yes, there is a path from " + from + " to " + to +"!";
}
void CityMap::open(const std::string& input)
{
    currCrossroad = "There is no current crossroad!";
    numCross = 0;
    interactive = false;
    std::ifstream iFile;
    iFile.open(input, std::ios::in);
    if (!iFile.is_open())
    {
        std::cout << "Couldn't open the file or not a valid path!" << std::endl;
        return;
    }
    std::cout << "File opened successfully" << std::endl;
    std::string start, currWord;
    long long dist;
    char currSymb;
    bool flag;
    while(!iFile.eof())
    {
        iFile >> start;
        if(crossroads.find(start) == crossroads.end())
        {
            crossroads[start] = numCross;
            numCross ++;
        }
        currSymb = '#';
        flag = 1;
        iFile.get(currSymb);
        dist = 0;
        currWord = "";
        while(currSymb != '\n' && !iFile.eof())
        {
            iFile.get(currSymb);
            if(currSymb == ' ')
            {
                if(flag == 1) flag = 0;
                else
                {
                    if(crossroads.find(currWord) == crossroads.end())
                    {
                        crossroads[currWord] = numCross;
                        numCross ++;
                    }
                    adj[start][currWord] = dist;
                    adjRev[currWord].push_back(start);
                    flag = 1;
                    dist = 0;
                    currWord = "";
                }
            }
            if(flag == 1 && currSymb != ' ') currWord += currSymb;
            else if(flag == 0 && currSymb != ' ' && currSymb != '\n') dist = dist * 10 + (currSymb - '0');
        }
        if(crossroads.find(currWord) == crossroads.end())
        {
            crossroads[currWord] = numCross;
            numCross ++;
        }
        adj[start][currWord] = dist;
        adjRev[currWord].push_back(start);
    }
    iFile.close();
}
#endif
