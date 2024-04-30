#include "graph.h"
#include <queue>
#include <algorithm>

bool BFS(const Graph& graph , GraghNode* src ,const GraghNode* goal , NodeToPath& path)
{   
    std::queue<const GraghNode*> que;
    que.push(src);
    path[src] = nullptr;

    while(!que.empty())
    {
        auto pa = que.front();
        que.pop();

        for(auto i: pa->adjacent)
        {
            auto parent = path[i];

            // 已有的路径不更改 . 同时避免成环
            if( parent == nullptr && i != src )
            {
                path[i] == pa;
                que.push(i);
            }

            if(i == goal) return true;
        }
    }
    
    return false;
}

bool GBFS(const WeightedGraph& , const WeightedGraphNode* src , const WeightedGraphNode* goal , GBFSMap& outMap)
{
    // closed . 考虑过的节点. 
    // open 正在考虑的节点,需要选择h(x) 最小 
    std::vector<const WeightedGraphNode* > openSet;

    const WeightedGraphNode* currentnode = src;
    outMap[src].inClosedSet = true;

    do
    {
        for(auto edge: currentnode->edges)
        {
            auto& gbfsinfo = outMap[edge->to];

            /// 不在闭集中还要更新一下保持路径
            if(!gbfsinfo.inClosedSet)
            {
                gbfsinfo.parentEdge = edge;

                /// 不在开集也不在闭集说明还没跟踪过
                if(!gbfsinfo.inOpenSet)
                {
                    gbfsinfo.heuristic = Heuristic(edge->to , goal);
                    gbfsinfo.inOpenSet = true;
                    openSet.push_back(edge->to);
                }
            }
        }
        /// 开集用完了还没走到说明死路
        if(openSet.empty())
        {
            break;
        }

        /// 选取开集中评估结果最优 -> 贪心之处
        auto iter = std::min_element(openSet.begin(),openSet.end(),
        [&outMap](const WeightedGraphNode* a ,const WeightedGraphNode* b){
            return outMap[a].heuristic < outMap[b].heuristic;
        });

        currentnode = *iter;

        /// 放到闭集
        openSet.erase(iter);
        outMap[currentnode].inClosedSet = true;
        outMap[currentnode].inOpenSet = false;

    }
    while(currentnode != goal);
    return currentnode == goal ;
}

bool AStar(const WeightedGraph& , const WeightedGraphNode* src , const WeightedGraphNode* goal , AStarMap& outMap)
{
   std::vector<const WeightedGraphNode* > openSet;

    const WeightedGraphNode* currentnode = src;
    outMap[src].inClosedSet = true;

    do
    {
        for(auto edge: currentnode->edges)
        {
            const auto* nextnode =edge->to;
            auto& info = outMap[nextnode];

            if(!info.inClosedSet)
            {
                /// 不在开集也不在闭集说明还没跟踪过
                if(!info.inOpenSet)
                {
                    info.heuristic = Heuristic(edge->to , goal);
                    info.inOpenSet = true;
                    info.parentEdge = edge;
                    openSet.push_back(nextnode);
                }
                else
                {
                    /// 正在考虑中, 注意是否需要更新路径

                    /// 当前路径的路径代价
                    float  newG = outMap[currentnode].actualFromStart +  edge->weight;
                    
                    /// 更小则选用此当前路径更新
                    if(newG < outMap[nextnode].actualFromStart)
                    {
                        outMap[nextnode].actualFromStart = newG;
                        outMap[nextnode].parentEdge = edge;
                    }
                }
            }
        }

        /// 开集用完了还没走到说明死路
        if(openSet.empty())
        {
            break;
        }

        /// 选取开集中评估结果最优 -> 贪心之处
        auto iter = std::min_element(openSet.begin(),openSet.end(),
        [&outMap](const WeightedGraphNode* a ,const WeightedGraphNode* b){
            return outMap[a].heuristic + outMap[a].actualFromStart < outMap[b].heuristic + outMap[a].actualFromStart;
        });

        currentnode = *iter;

        /// 放到闭集
        openSet.erase(iter);
        outMap[currentnode].inClosedSet = true;
        outMap[currentnode].inOpenSet = false;

    }
    while(currentnode != goal);
    return currentnode == goal ;
}



float Heuristic(const WeightedGraphNode* a , const WeightedGraphNode* b)
{
    // TODO: has any data Node contain to calculate?
    return 0.f;
}


