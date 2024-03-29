#include "pf/algorithms/BiDirectional.h"
#include "pf/algorithms/Utils.h"
#include <algorithm>
#include <queue>
#include <unordered_set>

namespace ai
{
  namespace path
  {

    std::vector<Vector2> GetPathBiDirectional(Node* middle_node)
    {
      std::vector<Vector2> path;

      Node* current_node = middle_node;
      while(current_node != nullptr)
      {
        path.push_back(current_node->GetPosition());
        current_node = current_node->GetParent();
      }

      std::reverse(path.begin(), path.end());
      // current_node = middle_node->GetBiDirectionalParent();

      while(current_node != nullptr)
      {
        path.push_back(current_node->GetPosition());
        // current_node = current_node->GetBiDirectionalParent();
      }

      return path;
    }

    std::vector<Vector2> BiDirectional(std::vector<Node*>& node_map,
                                       Node*               start_node,
                                       Node*               goal_node,
                                       Obstacle              layer)
    {
      ResetNodeMap(node_map);

      std::queue<Node*> start_frontier;
      std::queue<Node*> end_frontier;

      std::unordered_set<Node*> start_explored;
      std::unordered_set<Node*> end_explored;

      start_frontier.push(start_node);
      start_explored.insert(start_node);

      end_frontier.push(goal_node);
      end_explored.insert(goal_node);

      while(!start_frontier.empty() && !end_frontier.empty())
      {
        Node* current_start = start_frontier.front();
        start_frontier.pop();

        Node** neighbours = current_start->GetNeighbours();
        for(int i = 0; i < 4; ++i)
        {
          Node* neighbour = neighbours[i];
          if(!neighbour->IsObstacle(layer) && start_explored.count(neighbour) == 0)
          {
            start_explored.insert(neighbour);
            neighbour->SetParent(current_start);
            start_frontier.push(neighbour);

            if(end_explored.count(neighbour) > 0)
            {
              return GetPathBiDirectional(neighbour);
            }
          }
        }

        Node* current_end = end_frontier.front();
        end_frontier.pop();

        neighbours = current_end->GetNeighbours();
        for(int i = 0; i < 4; ++i)
        {
          Node* neighbour = neighbours[i];
          if(!neighbour->IsObstacle(layer) && end_explored.count(neighbour) == 0)
          {
            end_explored.insert(neighbour);
            //neighbor->SetBiDirectionalParent(current_end);
            end_frontier.push(neighbour);

            if(start_explored.count(neighbour) > 0)
            {
              return GetPathBiDirectional(neighbour);
            }
          }
        }
      }

      return std::vector<Vector2>(); // return an empty vector if no solution is found
    }
  } // namespace path
} // namespace ai