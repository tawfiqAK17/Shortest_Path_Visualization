#include "ShortestPath.h"
#include <iostream>
ShortestPath::Rect::Rect(float size, sf::Vector2f position, sf::Color color) {
    rect.setSize({size, size});
    rect.setPosition(position);
    rect.setFillColor(color);
}

ShortestPath::ShortestPath(): window(sf::VideoMode::getDesktopMode(), "Shortest Path"),
                              NORMAL(158, 185, 212), SOURCE(255, 0, 0),
                              DESTINATION(0, 0, 255), WALL(132, 132, 130),
                              VISITED(0, 190, 3), PATH(205, 91, 62),
                              textColor(255, 255, 255){
    grid.resize(window.getSize().y / ShortestPath::rectSize);
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].reserve(window.getSize().x / ShortestPath::rectSize);
        for (int j = 0; j < window.getSize().x / ShortestPath::rectSize; j++)
        {
            grid[i].push_back(ShortestPath::Rect(ShortestPath::rectSize, {(float)j * (ShortestPath::rectSize + 2), (float)i * (ShortestPath::rectSize + 2)}, NORMAL));
        }
    }
    Run();
}

void ShortestPath::Run() {
    Instructions();
    std::shared_ptr<std::thread> threadPtr = nullptr;
    while (window.isOpen())
    {
        if (threadIsDone){
            if (runningAlgorithmId != 0)
                if (runningAlgorithmId == 1) {
                    if (threadPtr) threadPtr->detach();
                    std::shared_ptr<std::thread> tempThreadPtr1(new std::thread([&]() {
                        ShortestPath::BFS(grid, source, destination, threadIsDone, runningAlgorithmId);
                    }));
                    threadPtr = tempThreadPtr1;
                }
            if (runningAlgorithmId == 2) {
                if (threadPtr) threadPtr->detach();
                std::shared_ptr<std::thread> tempThreadPtr1(new std::thread([&]() {
                    ShortestPath::AStar(grid, source, destination, threadIsDone, runningAlgorithmId);
                }));
                threadPtr = tempThreadPtr1;
            }
        }
        PollEvents(threadPtr);
        UpdateWalls();
        Update();
    }
}

void ShortestPath::PollEvents(std::shared_ptr<std::thread> thread) {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            thread->detach();
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (threadIsDone) {
                switch (event.key.code) {
                    case sf::Keyboard::S :
                        bool differentThanDestination;
                        if (destinationSelected)
                            differentThanDestination = sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2) != destination.x\
                            || sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2) != destination.y;
                        else
                            differentThanDestination = true;

                        if (!sourceSelected && differentThanDestination) {
                            grid[sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2)]\
                            [sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2)].rect.setFillColor(SOURCE);
                            source.x = sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2);
                            source.y = sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2);
                            sourceSelected = true;
                            break;
                        }
                        if ((int) sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2) == source.x &&
                            (int) sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2) == source.y) {
                            grid[sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2)]\
                            [sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2)].rect.setFillColor(NORMAL);
                            sourceSelected = false;
                        }
                        break;
                    case sf::Keyboard::D :
                        bool differentThanSource;
                        if (sourceSelected)
                            differentThanSource = sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2) != source.x\
                            || sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2) != source.y;
                        else
                            differentThanSource = true;

                        if (!destinationSelected && differentThanSource) {
                            grid[sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2)]\
                            [sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2)].rect.setFillColor(DESTINATION);
                            destination.x = sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2);
                            destination.y = sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2);
                            destinationSelected = true;
                            break;
                        }
                        if ((int) sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2) == destination.x &&
                            (int) sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2) == destination.y) {
                            grid[sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2)]\
                            [sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2)].rect.setFillColor(NORMAL);
                            destinationSelected = false;
                        }
                        break;
                    case sf::Keyboard::B:
                        if (destinationSelected && sourceSelected)
                            runningAlgorithmId = 1;
                        break;
                    case sf::Keyboard::A:
                        if (destinationSelected && sourceSelected)
                            runningAlgorithmId = 2;
                        break;
                    case sf::Keyboard::Escape:
                        Reset();
                        break;
                }
            }
            break;
        }
        if (event.type == sf::Event::MouseButtonPressed){
            mousePressed = true;
            break;
        }
        if (event.type == sf::Event::MouseButtonReleased){
            mousePressed = false;
            break;
        }
    }
}

void ShortestPath::UpdateWalls() {
    int x = sf::Mouse::getPosition(window).y / (ShortestPath::rectSize + 2);
    int y = sf::Mouse::getPosition(window).x / (ShortestPath::rectSize + 2);
    bool validPos = sf::Mouse::getPosition(window).y > 0 && sf::Mouse::getPosition(window).x > 0\
                    && sf::Mouse::getPosition(window).y < window.getSize().y && sf::Mouse::getPosition(window).x <  window.getSize().x;
    if (mousePressed && validPos)
    {
        grid[x][y].isWall = true;
    }
}

void ShortestPath::Update() {
    window.clear();
    for (auto& line : grid)
    {
        for (auto& rect : line) {
            if(rect.isWall) rect.rect.setFillColor(WALL);
            window.draw(rect.rect);
        }
    }
    window.display();
}

void ShortestPath::Reset() {
    destinationSelected = false;
    sourceSelected = false;
    for (auto& rects : grid) {
        for (auto& rect : rects) {
            rect.isWall = false;
            rect.rect.setFillColor(NORMAL);
        }
    }
}

void ShortestPath::Instructions() {
    sf::Font font;
    if (!font.loadFromFile("src/FiraMono.ttf")) return;

    sf::Text setSource("To select start position press : S", font, 50);
    setSource.setFillColor(textColor);
    setSource.setPosition(50, 100);

    sf::Text setSDestination("To select destination position press : D", font, 50);
    setSource.setFillColor(textColor);
    setSDestination.setPosition(50, 200);

    sf::Text bfs("BFS algorithm: B", font, 50);
    setSource.setFillColor(textColor);
    bfs.setPosition(50, 300);

    sf::Text AStar("A star algorithm : A", font, 50);
    setSource.setFillColor(textColor);
    AStar.setPosition(50, 400);

    sf::Text drawWalls("You can draw walls using the mouse", font, 50);
    setSource.setFillColor(textColor);
    drawWalls.setPosition(50, 500);

    sf::Text clear("You can clear the screen by pressing ESC", font, 50);
    setSource.setFillColor(textColor);
    clear.setPosition(50, 600);

    sf::Text start(">> Enter <<", font, 50);
    setSource.setFillColor(textColor);
    start.setPosition(800, 900);

    sf::Event event;
    while (!started) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) started = true;
            }
        }
        window.clear();
        window.draw(setSource);
        window.draw(setSDestination);
        window.draw(bfs);
        window.draw(AStar);
        window.draw(drawWalls);
        window.draw(clear);
        window.draw(start);
        window.display();
    }
}



