#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include "PropertyTile.hpp" // 确保包含PropertyTile的完整定义
#include "SpecialTile.hpp"
#include "Map.hpp"  // 包含您的最终代码头文件
#include "Graph.hpp" // 添加Graph类的头文件
#include <functional>

using namespace std;

// 自定义测试框架基础结构
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

// 测试Graph类初始化
void testGraphInitialization() {
    Graph graph;
    
    // 测试邻接矩阵环形连接
    for (int i = 0; i < TOTAL_NODES; ++i) {
        int next = (i + 1) % TOTAL_NODES;
        TestRunner::myAssert(graph.hasEdge(i, next), 
            "Node " + to_string(i) + " should connect to " + to_string(next));
    }

    // 测试特殊节点位置
    TestRunner::myAssert(graph.getNodeName(LOS) == "LOS", 
        "Node 0 should be LOS");
    TestRunner::myAssert(graph.getNodeName(Hubschrauberlandeplatze) == "Hubschrauberlandeplatz",
        "Node 35 should be Hubschrauberlandeplatz");
}

// 测试地图显示功能
void testDisplayMap() {
    GameMap map;
    
    // 测试瓷砖属性顺序
    const std::string* names = map.nodeNames;
    TestRunner::myAssert(TOTAL_NODES == 40, 
        "Total nodes should match");

    // 验证前5个节点名称
    vector<string> expectedFirst5 = {"LOS", "Kronenstraße", "Gemeinschaftsfeld", 
                                    "Adlerstraße", "Einkommensteuer"};
    for (int i = 0; i < 5; ++i) {
        TestRunner::myAssert(names[i] == expectedFirst5[i], 
            "Node " + to_string(i) + " name mismatch");
    }
}

// 测试街道升级条件
void testCanUpgradeStreet() {
    GameMap map;
    
    // 获取测试用的街道瓷砖
    PropertyTile* street1 = dynamic_cast<PropertyTile*>(map.getTile(1).get());
    PropertyTile* street2 = dynamic_cast<PropertyTile*>(map.getTile(3).get());
    PropertyTile* street3 = dynamic_cast<PropertyTile*>(map.getTile(6).get());

    // 设置玩家所有权
    street1->setOwner(1);
    street2->setOwner(1);
    street3->setOwner(2);

    // 测试同组可升级
    TestRunner::myAssert(map.canUpgradeStreet(street1, 1), 
        "Should be able to upgrade when all in group");

    // 测试不同组不可升级
    TestRunner::myAssert(!map.canUpgradeStreet(street3, 1), 
        "Should not upgrade across groups");

    // 测试最高等级不可升级
    // street1->setBuildingLevel(5);
    // TestRunner::myAssert(!map.canUpgradeStreet(street1, 0), 
    //     "Cannot upgrade at max level");
}

// 测试移动逻辑
void testMovementMethods() {
    GameMap map;

    // 测试火车站移动
    int pos=map.movebahn();
    TestRunner::myAssert(pos == 5 || 
                     pos == 15 || 
                     pos == 25,
        "Should move to a train station");

    // 测试直升机移动
    pos=map.moveHub();
    TestRunner::myAssert(pos>=1 && pos<=40,
        "Should move to helicopter space");
}



int main() {
    // 运行所有测试
    TestRunner::runTest("Graph Initialization", testGraphInitialization);
    TestRunner::runTest("Display Map", testDisplayMap);
    TestRunner::runTest("Can Upgrade Street", testCanUpgradeStreet);
    TestRunner::runTest("Movement Methods", testMovementMethods);


    // 输出测试总结
    TestRunner::printSummary();

    return TestRunner::failed ? 1 : 0;
}