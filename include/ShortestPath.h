#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <queue>

class ShortestPath {
public:
    struct Rect {
        sf::RectangleShape rect;
        bool isWall = false;
        Rect(float size, sf::Vector2f position,sf::Color color);
    };
private:
    sf::RenderWindow window;
    std::vector<std::vector<Rect>> grid;
    sf::Vector2i source;
    bool sourceSelected = false;
    sf::Vector2i destination;
    bool destinationSelected = false;
    bool mousePressed = false;

    int runningAlgorithmId = 0;
    bool threadIsDone = true;

    const sf::Color NORMAL;
    const sf::Color SOURCE;
    const sf::Color DESTINATION;
    const sf::Color WALL;
    const sf::Color VISITED;
    const sf::Color PATH;
    const sf::Color textColor;

    bool started = false;

    const int rectSize = 10;
    const int speed = rectSize * 0.1;
public:
    ShortestPath();
    void Run();
    void PollEvents(std::shared_ptr<std::thread> thread);
    void UpdateWalls();
    void Update();
    void Reset();
    void Instructions();



    void BFS(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i source, sf::Vector2i destination, bool& threadIsDonne, int& runningAlgorithmId);
    void BFSgetNeighbors(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i& rect, std::queue<sf::Vector2i>& queue, std::unordered_map<int, bool>& visited, std::unordered_map<int, sf::Vector2i>& parents);
    void AgetNeighbors(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i& rect, std::vector<sf::Vector2i>& queue, std::unordered_map<int, bool>& visited, std::unordered_map<int, sf::Vector2i>& parents, std::unordered_map<int, int>& distanceToSource);
    void AStar(std::vector<std::vector<ShortestPath::Rect>>& grid, sf::Vector2i source, sf::Vector2i destination, bool& threadIsDonne, int& runningAlgorithmId);
};
