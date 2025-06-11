/**
*Unit Test Class
*/
#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include "PropertyTile.hpp" 
#include "SpecialTile.hpp"
#include "Map.hpp"  
#include "Graph.hpp" 
#include <functional>

using namespace std;


class TestRunner {
public:
    static int passed;
    static int failed;

    static void myAssert(bool condition, const string& message) {
        if (!condition) {
            cerr << "Assertion failed: " << message << endl;
            failed++;
        } else {
            passed++;
        }
    }

    static void runTest(const string& testName, function<void()> testFunc) {
        cout << "Running test: " << testName << "... ";
        passed = failed = 0;
        try {
            testFunc();
            cout << "OK (" << passed << "/" << (passed+failed) << ")" << endl;
        } catch (...) {
            cerr << "Error occurred during test" << endl;
            failed++;
        }
    }

    static void printSummary() {
        cout << "\n=== Test Summary ===" << endl;
        cout << "Passed: " << passed << endl;
        cout << "Failed: " << failed << endl;
    }
};

int TestRunner::passed = 0;
int TestRunner::failed = 0;


void testGraphInitialization() {
    Graph graph;
    

    for (int i = 0; i < TOTAL_NODES; ++i) {
        int next = (i + 1) % TOTAL_NODES;
        TestRunner::myAssert(graph.hasEdge(i, next), 
            "Node " + to_string(i) + " should connect to " + to_string(next));
    }

    TestRunner::myAssert(graph.getNodeName(LOS) == "LOS", 
        "Node 0 should be LOS");
    TestRunner::myAssert(graph.getNodeName(Hubschrauberlandeplatze) == "Hubschrauberlandeplatz",
        "Node 35 should be Hubschrauberlandeplatz");
}

void testDisplayMap() {
    GameMap map;
    

    const std::string* names = map.nodeNames;
    TestRunner::myAssert(TOTAL_NODES == 40, 
        "Total nodes should match");


    vector<string> expectedFirst5 = {"LOS", "Kronenstraße", "Gemeinschaftsfeld", 
                                    "Adlerstraße", "Einkommensteuer"};
    for (int i = 0; i < 5; ++i) {
        TestRunner::myAssert(names[i] == expectedFirst5[i], 
            "Node " + to_string(i) + " name mismatch");
    }
}


void testCanUpgradeStreet() {
    GameMap map;
    

    PropertyTile* street1 = dynamic_cast<PropertyTile*>(map.getTile(1).get());
    PropertyTile* street2 = dynamic_cast<PropertyTile*>(map.getTile(3).get());
    PropertyTile* street3 = dynamic_cast<PropertyTile*>(map.getTile(6).get());

    street1->setOwner(1);
    street2->setOwner(1);
    street3->setOwner(2);

 
    TestRunner::myAssert(map.canUpgradeStreet(street1, 1), 
        "Should be able to upgrade when all in group");


    TestRunner::myAssert(!map.canUpgradeStreet(street3, 1), 
        "Should not upgrade across groups");


}


void testMovementMethods() {
    GameMap map;


    int pos=map.movebahn();
    TestRunner::myAssert(pos == 5 || 
                     pos == 15 || 
                     pos == 25,
        "Should move to a train station");


    pos=map.moveHub();
    TestRunner::myAssert(pos>=1 && pos<=40,
        "Should move to helicopter space");
}



int map_main() {

    TestRunner::runTest("Graph Initialization", testGraphInitialization);
    TestRunner::runTest("Display Map", testDisplayMap);
    TestRunner::runTest("Can Upgrade Street", testCanUpgradeStreet);
    TestRunner::runTest("Movement Methods", testMovementMethods);



    TestRunner::printSummary();

    return TestRunner::failed ? 1 : 0;
}