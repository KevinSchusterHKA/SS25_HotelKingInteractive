/**
 * This is the Graph class. Implemented the Karlsruhe ring tile graph structure
 */
#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include "tile.hpp"

 ///
 /// Define node ID using macro
 /// 
#define TOTAL_NODES 40
#define LOS 0
#define Kronenstrasse 1
#define Gemeinschaftsfeld 2
#define Adlerstrasse 3
#define Einkommensteuer 4
#define Hauptbahnhof 5
#define Ebertstrasse 6
#define Ereignisfeld 7
#define Rueppurrerstrasse 8
#define Ettlingerstrasse 9
#define Gefaengnis 10
#define Amalienstrasse 11
#define Elektrizitaetwerk 12
#define Hirschstrasse 13
#define Kriegsstrasse 14
#define WestBahnhof 15
#define Fastplatz 16
#define Gemeinschaftsfeld2 17
#define KaiserAllee 18
#define DurlacherAllee 19
#define FreiParken 20
#define Zirkel 21
#define Gemeinschaftsfeld3 22
#define Karlstrasse 23
#define Brauerstrasse 24
#define OstBahnhof 25
#define Hildapromenade 26
#define Moltkestrasse 27
#define Wasserwerk 28
#define Karlfriedrichstrasse 29
#define GeheinsGefaengnis 30
#define Herrenstrasse 31
#define Waldstrasse 32
#define Gemeinschaftsfeld4 33
#define Erbprinzenstrasse 34
#define Hubschrauberlandeplatze 35  
#define Ereignisfeld2 36
#define Kaiserstrasse 37
#define Zusatzsteuer 38
#define Schlossplatz 39

class Graph {
public:

  
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
    ///
    /// street
    /// 
    const std::vector<int> PropertyTiles = {
    Kronenstrasse,
    Adlerstrasse,
    Ebertstrasse,
    Rueppurrerstrasse,
    Ettlingerstrasse,
    Amalienstrasse,
    Hirschstrasse,
    Kriegsstrasse,
    Fastplatz,
    KaiserAllee,
    DurlacherAllee,
    Zirkel,
    Karlstrasse,
    Brauerstrasse,
    Hildapromenade,
    Moltkestrasse,
    Karlfriedrichstrasse,
    Herrenstrasse,
    Waldstrasse,
    Erbprinzenstrasse,
    Kaiserstrasse,
    Schlossplatz
    };


    std::vector<std::vector<bool>> adjacencyMatrix;

    /// <summary>
    /// //Constructor, initialize the adjacency matrix to construct the edges of the graph, and use a loop to insert nodes (Tile) according to the rent table
    /// </summary>
    Graph();

    /// <summary>
    /// Function: Given the current position ID, returns the ID of the next feasible tile
    /// </summary>
    /// <param name="currentId"></param>
    /// <returns></returns>
    int getNextNode(int currentId) const;
    /// <summary>
    /// Function: Given a Tile ID, return a Tile object
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    std::shared_ptr<Tile> getTile(int id) const;

    /// <summary>
    /// Function:Given a Tile ID, return a Tile name
    /// </summary>
    /// <param name="from"></param>
    /// <param name="to"></param>
    /// <returns></returns>
    bool hasEdge(int from, int to) const;

    /// <summary>
    /// Function: Determine whether two Tiles are connected
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    std::string getNodeName(int id) const;
};
#endif // GRAPH_HPP