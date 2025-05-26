#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <vector>
#include <memory>
#include "tile.hpp"
class Graph {
public:
    // 使用宏定义节点ID
    #define TOTAL_NODES 40
    #define LOS 0
    #define Kronenstraße 1
    #define Gemeinschaftsfeld 2
    #define Adlerstraße 3
    #define Einkommensteuer 4
    #define Hauptbahnhof 5
    #define Ebertstraße 6
    #define Ereignisfeld 7
    #define Rüppurrerstraße 8
    #define Ettlingerstraße 9
    #define Gefängnis 10
    #define Amalienstraße 11
    #define Elektrizitätwerk 12
    #define Hirschstraße 13
    #define Kriegsstraße 14
    #define WestBahnhof 15
    #define Fastplatz 16
    #define Gemeinschaftsfeld2 17
    #define KaiserAllee 18
    #define DurlacherAllee 19
    #define FreiParken 20
    #define Zirkel 21
    #define Gemeinschaftsfeld3 22
    #define Karlstraße 23
    #define Brauerstraße 24
    #define OstBahnhof 25
    #define Hildapromenade 26
    #define Moltkestraße 27
    #define Wasserwerk 28
    #define Karlfriedrichstraße 29
    #define GeheinsGefängnis 30
    #define Herrenstraße 31
    #define Waldstraße 32
    #define Gemeinschaftsfeld4 33
    #define Erbprinzenstraße 34
    #define Hubschrauberlandeplatze 35  
    #define Ereignisfeld2 36
    #define Kaiserstraße 37
    #define Zusatzsteuer 38
    #define Schlossplatz 39

    // 节点名称数组
    static const std::string nodeNames[TOTAL_NODES];
    std::vector<std::shared_ptr<Tile>> tiles;
    const std::vector<std::tuple<int, int, std::vector<int>, std::vector<int>>> groups= {
    std::make_tuple(2, 60, std::vector<int>{2, 10, 30, 90, 160, 250}, std::vector<int>{50, 50}),
    std::make_tuple(3, 100, std::vector<int>{6, 30, 90, 270, 400, 550}, std::vector<int>{50, 50, 50}),
    std::make_tuple(3, 140, std::vector<int>{10, 50, 150, 450, 625, 750}, std::vector<int>{100, 100, 100}),
    std::make_tuple(3, 180, std::vector<int>{14, 70, 200, 550, 750, 950}, std::vector<int>{150, 150, 150}),
    std::make_tuple(3, 220, std::vector<int>{18, 90, 250, 700, 875, 1050}, std::vector<int>{200, 200, 200}),
    std::make_tuple(3, 260, std::vector<int>{22, 110, 330, 800, 975, 1150}, std::vector<int>{250, 250, 250}),
    std::make_tuple(3, 300, std::vector<int>{22, 110, 330, 800, 975, 1150}, std::vector<int>{250, 250, 250}),
    std::make_tuple(2, 400, std::vector<int>{50, 200, 600, 1400, 1700, 2000}, std::vector<int>{300, 300})
    };

    //街道列表
    const std::vector<int> PropertyTiles = {
    Kronenstraße,
    Adlerstraße,
    Ebertstraße,
    Rüppurrerstraße,
    Ettlingerstraße,
    Amalienstraße,
    Hirschstraße,
    Kriegsstraße,
    Fastplatz,
    KaiserAllee,
    DurlacherAllee,
    Zirkel,
    Karlstraße,
    Brauerstraße,
    Hildapromenade,
    Moltkestraße,
    Karlfriedrichstraße,
    Herrenstraße,
    Waldstraße,
    Erbprinzenstraße,
    Kaiserstraße,
    Schlossplatz
    };

    // 邻接矩阵
    std::vector<std::vector<bool>> adjacencyMatrix;

    // 构造函数初始化邻接矩阵
    Graph();

    // 获取下一个节点ID
    int getNextNode(int currentId) const;
    std::shared_ptr<Tile> getTile(int id) const;

    // 检查是否存在边
    bool hasEdge(int from, int to) const;

    // 获取节点名称
    std::string getNodeName(int id) const;
};
#endif // GRAPH_HPP