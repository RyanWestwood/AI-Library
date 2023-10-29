#include "pf/algorithms/A_Star.h"
#include "pf/algorithms/Utils.h"
#include <algorithm>

namespace ai
{
  namespace path
  {
    namespace
    {
      auto Heuristic = [](Node* current, Node* destination) -> float {
        const Vector2& current_pos     = current->GetPosition();
        const Vector2& destination_pos = destination->GetPosition();

        const float xDist = std::fabs(current_pos.x - destination_pos.x);
        const float yDist = std::fabs(current_pos.y - destination_pos.y);

        return (xDist * xDist) + (yDist * yDist);
      };

      auto Compare = [](const Node* lhs, const Node* rhs) -> const bool{
        return lhs->m_Costs.m_TotalCost > rhs->m_Costs.m_TotalCost;
      };
    } // namespace

    std::vector<Vector2> A_Star(std::vector<Node*>& node_map,
                                Node*               start_node,
                                Node*               goal_node,
                                Obstacle            layer)
    {
      ResetNodeMap(node_map);

      start_node->m_Costs.m_FromCost  = 0.f;
      start_node->m_Costs.m_ToCost    = Heuristic(start_node, goal_node);
      start_node->m_Costs.m_TotalCost = start_node->m_Costs.m_FromCost + start_node->m_Costs.m_ToCost;

      std::vector<Node*> frontier;
      frontier.push_back(start_node);
      std::make_heap(frontier.begin(), frontier.end(), Compare);

      while(!frontier.empty())
      {
        std::pop_heap(frontier.begin(), frontier.end(), Compare);
        Node* current_node = frontier.back();
        if(AtGoal(current_node, goal_node))
        {
          return SolutionPath(current_node);
        }
        frontier.pop_back();
        current_node->SetVisited(true);

        Node** neighbours = current_node->GetNeighbours();
        for(int i = 0; i < 4; ++i)
        {
          Node* neighbour = neighbours[i];
          if(!neighbour->IsObstacle(layer) &&
             !neighbour->IsVisited())
          {
            const float gPossibleLowerGoal = current_node->m_Costs.m_FromCost + Heuristic(neighbour, goal_node);
            if(gPossibleLowerGoal < neighbour->m_Costs.m_FromCost)
            {
              neighbour->SetParent(current_node);
              neighbour->m_Costs.m_FromCost  = current_node->m_Costs.m_FromCost + 1;
              neighbour->m_Costs.m_ToCost    = Heuristic(neighbour, goal_node);
              neighbour->m_Costs.m_TotalCost = neighbour->m_Costs.m_FromCost + neighbour->m_Costs.m_ToCost;
              frontier.push_back(neighbour);
              std::push_heap(frontier.begin(), frontier.end(), Compare);
            }
          }
        }
      }
      return std::vector<Vector2>(); // This will only happen if no solution was available.
    }

    void PrecomputeHeuristics(std::vector<Node*>& node_map, Node* end_node)
    {
      for(Node* node : node_map)
      {
        node->m_Costs.m_ToCost = Heuristic(node, end_node);
      }
    }

    std::vector<Vector2> A_StarStatic(std::vector<Node*>& node_map,
                                      Node*               start_node,
                                      Node*               goal_node,
                                      Obstacle            layer)
    {
      ResetNodeMap(node_map);

      start_node->m_Costs.m_FromCost  = 0.f;
      start_node->m_Costs.m_TotalCost = start_node->m_Costs.m_FromCost + start_node->m_Costs.m_ToCost;

      std::vector<Node*> frontier;

      frontier.push_back(start_node);
      std::make_heap(frontier.begin(), frontier.end(), Compare);

      while(!frontier.empty())
      {
        std::pop_heap(frontier.begin(), frontier.end(), Compare);
        Node* current_node = frontier.back();
        if(AtGoal(current_node, goal_node))
        {
          return SolutionPath(current_node);
        }
        frontier.pop_back();
        current_node->SetVisited(true);

        Node** neighbours = current_node->GetNeighbours();
        for(int i = 0; i < 4; ++i)
        {
          Node* neighbour = neighbours[i];
          if(!neighbour->IsObstacle(layer) &&
             !neighbour->IsVisited())
          {
            const float gPossibleLowerGoal = current_node->m_Costs.m_FromCost + neighbour->m_Costs.m_ToCost;
            if(gPossibleLowerGoal < neighbour->m_Costs.m_FromCost)
            {
              neighbour->SetParent(current_node);
              neighbour->m_Costs.m_FromCost  = current_node->m_Costs.m_FromCost + 1;
              neighbour->m_Costs.m_TotalCost = neighbour->m_Costs.m_FromCost + neighbour->m_Costs.m_ToCost;
              frontier.push_back(neighbour);
              std::push_heap(frontier.begin(), frontier.end(), Compare);
            }
          }
        }
      }
      return std::vector<Vector2>(); // This will only happen if no solution was available.
    }
  } // namespace path
} // namespace ai