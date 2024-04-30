#pragma once

#include "vector"
#include "unordered_map"

struct GraghNode
{
    std::vector<GraghNode*> adjacent;
    /* data */
};

struct Graph
{
    std::vector<GraghNode*> nodes;
    /* data */
};

struct WeightedEdge
{
    struct WeightedGraphNode* from; 
    struct WeightedGraphNode* to;

    float weight;
};

struct WeightedGraphNode
{
    std::vector<WeightedEdge*> edges;
};

struct WeightedGraph
{ 
    std::vector<WeightedGraphNode*> nodes;
};

/// @brief Chiled -> Father
using NodeToPath  = std::unordered_map<GraghNode* , GraghNode* >;


/// @brief 广度优先搜索 . 一般反向搜索以构建 src -> goal 的路径
/// @param 
/// @param src  起点
/// @param goal 目标
/// @param path 传递一个数据集,可以构建出 goal -> src 的 路径 
/// @return 
bool BFS(const Graph& , GraghNode* src , const GraghNode* goal , NodeToPath& path);

float Heuristic(const WeightedGraphNode* a , const WeightedGraphNode* b  );

struct GBFSScratch
{
    const WeightedEdge* parentEdge = nullptr;
    float heuristic = 0.f;
    bool inOpenSet = false;
    bool inClosedSet = false;
};

using GBFSMap = std::unordered_map<const WeightedGraphNode* , GBFSScratch > ;

bool GBFS(const WeightedGraph& , const WeightedGraphNode* src , const WeightedGraphNode* goal , GBFSMap& outMap);


struct AStarScratch
{
    const WeightedEdge* parentEdge = nullptr;
    float heuristic = 0.f;
    float actualFromStart = 0.f;
    bool inOpenSet = false;
    bool inClosedSet = false;
};

using AStarMap = std::unordered_map<const WeightedGraphNode* , AStarScratch > ;

/// @brief a sample of astar , can/t work. actualfromstar(gx) 和 hx 可以添加权重. 另外, Astar 不适合网格
/// @param  
/// @param src 
/// @param goal 
/// @param outMap 
/// @return 
bool AStar(const WeightedGraph& , const WeightedGraphNode* src , const WeightedGraphNode* goal , AStarMap& outMap);


