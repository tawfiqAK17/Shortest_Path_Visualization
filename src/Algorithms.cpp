

#include <iostream>
#include "ShortestPath.h"
#include <cmath>
#include <chrono>

void ShortestPath::BFSgetNeighbors(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i& rect, std::queue<sf::Vector2i>& queue, std::unordered_map<int, bool>& visited, std::unordered_map<int, sf::Vector2i>& parents) {
    std::vector<sf::Vector2i> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    for (const auto& dir : directions) {
        int newX = rect.x + dir.x;
        int newY = rect.y + dir.y;

        if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[newX].size()) {
            int key = newX * grid[0].size() + newY;

            if (!visited[key] && !grid[newX][newY].isWall) {
                grid[newX][newY].rect.setFillColor(VISITED);
                queue.emplace(newX, newY);
                parents[key] = rect;
                visited[key] = true;
            }
        }
    }
}

void ShortestPath::BFS(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i source, sf::Vector2i destination, bool& threadIsDone, int& runningAlgorithmId) {
    threadIsDone = false;
    std::vector<sf::Vector2i> path;
    std::queue<sf::Vector2i> queue;
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, sf::Vector2i> parents;

    int sourceKey = source.x * grid[0].size() + source.y;
    visited[sourceKey] = true;
    parents[sourceKey] = source;
    queue.push(source);

    sf::Vector2i current;
    bool found = false;

    while (!queue.empty()) {
        grid[destination.x][destination.y].rect.setFillColor(DESTINATION);
        current = queue.front();
        queue.pop();

        if (current == destination) {
            found = true;
            break;
        }

        BFSgetNeighbors(grid, current, queue, visited, parents);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }

    if (found) {
        for (sf::Vector2i node = destination; node != source; node = parents[node.x * grid[0].size() + node.y]) {
            path.push_back(node);
        }
        path.push_back(source);

        for (const auto& node : path) {
            if (node != source && node != destination)
            grid[node.x][node.y].rect.setFillColor(PATH);
        }
        std::cout << "\n";
    } else {
        std::cout << "Path not found.\n";
    }
    std::cout << "path length: " << path.size() << std::endl;

    threadIsDone = true;
    runningAlgorithmId = 0;
}

void ShortestPath::AgetNeighbors(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i& rect, std::vector<sf::Vector2i>& queue, std::unordered_map<int, bool>& visited, std::unordered_map<int, sf::Vector2i>& parents, std::unordered_map<int, int>& distanceToSource) {
    std::vector<sf::Vector2i> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    for (const auto& dir : directions) {
        int newX = rect.x + dir.x;
        int newY = rect.y + dir.y;

        if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[newX].size()) {
            int key = newX * grid[0].size() + newY;

            if (!visited[key] && !grid[newX][newY].isWall) {
                grid[newX][newY].rect.setFillColor(VISITED);
                queue.emplace_back(newX, newY);
                parents[key] = rect;
                visited[key] = true;
                distanceToSource[key] = distanceToSource[rect.x * grid[0].size() + rect.y] + 1;
            }
        }
    }
}
void ShortestPath::AStar(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i source, sf::Vector2i destination, bool& threadIsDonne, int& runningAlgorithmId) {
    std::cout << "a started" << std::endl;
    threadIsDone = false;
    std::vector<sf::Vector2i> path;
    std::vector<sf::Vector2i> queue;
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, sf::Vector2i> parents;
    std::unordered_map<int, int> distanceToSource;

    int sourceKey = source.x * grid[0].size() + source.y;
    visited[sourceKey] = true;
    parents[sourceKey] = source;
    distanceToSource[sourceKey] = 0;
    queue.push_back(source);

    sf::Vector2i current;
    bool found = false;

    while (!queue.empty()) {
        grid[destination.x][destination.y].rect.setFillColor(DESTINATION);
        current = queue.back();
        queue.pop_back();

        if (current == destination) {
            found = true;
            break;
        }

        AgetNeighbors(grid, current, queue, visited, parents, distanceToSource);
        std::sort(queue.begin(), queue.end(), [&](sf::Vector2i a, sf::Vector2i b) -> bool {
            return sqrt(pow(grid[a.x][a.y].rect.getPosition().x - grid[destination.x][destination.y].rect.getPosition().x, 2) + \
            pow(grid[a.x][a.y].rect.getPosition().y - grid[destination.x][destination.y].rect.getPosition().y, 2)) +\
            distanceToSource[a.x * grid[0].size() + a.y] >\
            sqrt(pow(grid[b.x][b.y].rect.getPosition().x - grid[destination.x][destination.y].rect.getPosition().x, 2) + \
            pow(grid[b.x][b.y].rect.getPosition().y - grid[destination.x][destination.y].rect.getPosition().y, 2)) +\
            distanceToSource[b.x * grid[0].size() + b.y];
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }

    if (found) {
        for (sf::Vector2i node = destination; node != source; node = parents[node.x * grid[0].size() + node.y]) {
            path.push_back(node);
        }
        path.push_back(source);

        for (const auto& node : path) {
            if (node != source && node != destination)
                grid[node.x][node.y].rect.setFillColor(PATH);
        }
        std::cout << "\n";
    } else {
        std::cout << "Path not found.\n";
    }

    std::cout << "path length: " << path.size() << std::endl;
    threadIsDone = true;
    runningAlgorithmId = 0;
}



