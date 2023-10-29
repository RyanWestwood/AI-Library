#pragma once
#include "../Node.h"

namespace ai
{
  namespace path
  {
    /**
     * @brief A simple A_Star implementation
     *
     * This can be called on dynamically changing maps with changing goals. 
     */
    std::vector<Vector2> A_Star(std::vector<Node*>& node_map,
                                Node*               start_node,
                                Node*               goal_node,
                                Obstacle            layer);

    /**
     * @brief A simple A_StarStatic implementation
     *
     * This can be called on a static map, same goal. Change nothing. 
     * Can have a huge performance boost based on expensive heuristics.
     */
    std::vector<Vector2> A_StarStatic(std::vector<Node*>& node_map,
                                      Node*               start_node,
                                      Node*               goal_node,
                                      Obstacle            layer);

    void                 PrecomputeHeuristics(std::vector<Node*>& node_map, Node* end_node);
  } // namespace path
} // namespace ai