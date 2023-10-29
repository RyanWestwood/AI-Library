#define SDL_MAIN_HANDLED
#include "Renderer.h"
#include <ai/pf/Algorithms.h>

#define SEARCH_ALGORITHM_CASE(algorithm)                                    \
  case SearchAlgorithm::algorithm:                                          \
    solution_path = ai::path::algorithm(map, start_node, goal_node, layer); \
    break;

constexpr static int MAP_WIDTH  = 80;
constexpr static int MAP_HEIGHT = 45;
constexpr static int TILE_SIZE  = 16;

std::vector<Vector2> CreatePath(std::vector<ai::path::Node*>& map,
                                Vector2                       start,
                                Vector2                       goal,
                                SearchAlgorithm               algorithm = SearchAlgorithm::A_Star,
                                ai::path::Obstacle            layer     = ai::path::Obstacle::All)
{
  ai::path::Node*      start_node = map.at(start.x + (start.y * MAP_WIDTH));
  ai::path::Node*      goal_node  = map.at(goal.x + (goal.y * MAP_WIDTH));
  std::vector<Vector2> solution_path{};
  switch(algorithm)
  {
    SEARCH_ALGORITHM_CASE(A_Star)
    SEARCH_ALGORITHM_CASE(BestFirst)
    SEARCH_ALGORITHM_CASE(BiDirectional)
    SEARCH_ALGORITHM_CASE(DepthFirst)
    SEARCH_ALGORITHM_CASE(DepthLimited)
    SEARCH_ALGORITHM_CASE(GreedyBestFirst)
    SEARCH_ALGORITHM_CASE(IterativeDeepeningDepthFirst)
  }
  return solution_path;
}

int main(int argc, char* args[])
{
  auto renderer = InitializeRenderer(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);

  std::vector<ai::path::Node*> node_map = ai::path::CreateNodeMap(MAP_WIDTH, MAP_HEIGHT);

  TextureData white_block = LoadSolidColourTexture({255, 255, 255, 255}, {32, 32}, renderer.renderer);
  TextureData black_block = LoadSolidColourTexture({0, 0, 0, 255}, {32, 32}, renderer.renderer);
  TextureData green_block = LoadSolidColourTexture({0, 255, 0, 255}, {32, 32}, renderer.renderer);
  TextureData red_block   = LoadSolidColourTexture({255, 0, 0, 255}, {32, 32}, renderer.renderer);
  TextureData blue_block  = LoadSolidColourTexture({0, 0, 255, 255}, {32, 32}, renderer.renderer);

  SDL_Rect src    = {0, 0, 32, 32};
  Vector2  start  = {10, 10};
  Vector2  goal   = {40, 40};
  int      offset = 1;
  while(1)
  {
    SDL_RenderClear(renderer.renderer);

    // Create obstacles.
    for(int i = 10; i < 30; i++)
    {
      //node_map.at(23 * MAP_WIDTH + i)->SetObstacle(ai::path::All);
    }

    // Create path.
    std::vector<Vector2> as_path = CreatePath(node_map, start, goal);

    // Draw the map along with any obstacles. 
    // Background = White
    // Obstacle   = Red
    for(auto& node : node_map)
    {
      const Vector2 pos       = node->GetPosition();
      SDL_Rect      dest      = {TILE_SIZE * pos.x + offset, TILE_SIZE * pos.y + offset, TILE_SIZE - offset, TILE_SIZE - offset};
      TextureData   xTileType = node->IsObstacle(ai::path::All) ? red_block : white_block;

      SDL_RenderCopy(renderer.renderer, xTileType.m_Texture, &src, &dest);
    }

    // Draw path.
    for(auto& pos : as_path)
    {
      SDL_Rect dest = {TILE_SIZE * pos.x + offset, TILE_SIZE * pos.y + offset, TILE_SIZE - offset, TILE_SIZE - offset};
      SDL_RenderCopy(renderer.renderer, blue_block.m_Texture, &src, &dest);
    }

    // Draw the start square, Green.
    SDL_Rect dest = {TILE_SIZE * start.x + offset, TILE_SIZE * start.y + offset, TILE_SIZE - offset, TILE_SIZE - offset};
    SDL_RenderCopy(renderer.renderer, green_block.m_Texture, &src, &dest);

    // Draw the end square, Black.
    SDL_Rect dest1 = {TILE_SIZE * goal.x + offset, TILE_SIZE * goal.y + offset, TILE_SIZE - offset, TILE_SIZE - offset};
    SDL_RenderCopy(renderer.renderer, black_block.m_Texture, &src, &dest1);

    SDL_RenderPresent(renderer.renderer);
  }

  return 0;
}