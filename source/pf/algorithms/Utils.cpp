#include "pf/algorithms/Utils.h"

namespace ai
{
  namespace path
  {
    bool AtGoal(Node* current, Node* destination)
    {
      return current->GetPosition() == destination->GetPosition();
    };

    std::vector<Vector2> SimplifyPath(std::vector<Vector2>& path)
    {
      std::vector<Vector2> way_points{};
      Vector2              old_direction{0, 0};

      for(int i = 1; i < path.size(); i++)
      {
        Vector2 new_direction = {
          path.at(i - 1).x - path.at(i).x,
          path.at(i - 1).y - path.at(i).y,
        };
        if(new_direction != old_direction)
        {
          way_points.push_back(path.at(i));
        }
        old_direction = new_direction;
      }
      return way_points;
    }

    std::vector<Vector2> SolutionPath(Node* solution_node)
    {
      if(!solution_node)
      {
        return {};
      }

      std::vector<Vector2> path;
      while(solution_node->GetParent() != nullptr)
      {
        path.push_back(solution_node->GetPosition());
        solution_node = solution_node->GetParent();
      }
      path.push_back(solution_node->GetPosition());

      std::vector<Vector2> simplified_path = SimplifyPath(path);
      std::reverse(std::begin(simplified_path), std::end(simplified_path));
      return simplified_path;
    }
  } // namespace path
} // namespace ai