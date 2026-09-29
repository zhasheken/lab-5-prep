#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int FRAMES_PER_ANIM = 60;
const float PI = 3.14159f;
int ELAPSED_FRAMES = 0;

const int GRAPH_X = 50;
const int GRAPH_WIDTH = 700;
const int GRAPH_Y = WINDOW_HEIGHT - 50;
const int GRAPH_HEIGHT = 300;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======

        // 1: ease in out sine
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            std::cout << "1: ease in out sine" << std::endl;
            tween = [](float a, float b, float t) {
                float e = -(cos(PI*t) - 1) / 2;
                return a + (b - a) * e;
            };
        }

        // 2: ease in out cubic
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            std::cout << "2: ease in out cubic" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                if (t < 0.5f){
                    e = 4 * t * t * t;
                }
                else{
                    float j = -2 * t + 2;
                    e = 1 - (j * j * j) / 2;
                }
                return a + (b - a) * e;
            };
        }

        // 3: ease in out quint
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
            std::cout << "3: ease in out quint" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                if (t < 0.5f){ // 16 * x * x * x * x * x
                    e = 16 * t * t * t * t * t;
                }
                else{
                    float j = -2 * t + 2;
                    e = 1 - (j * j * j * j * j) / 2;
                }
                return a + (b - a) * e;
            };
        }

        // 4: ease in out circ
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
            std::cout << "4: ease in out circ" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                if (t < 0.5f){ // (1 - Math.sqrt(1 - Math.pow(2 * x, 2))) / 2
                    e = (1 - sqrt(1 - 4 * t * t)) / 2;
                }
                else{
                    float j = -2 * t + 2;
                    e = (sqrt(1 - (j * j)) + 1) / 2;
                }
                return a + (b - a) * e;
            };
        }

        // 5: ease in out elastic
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
            std::cout << "5: ease in out elastic" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                const float c5 = (2 * PI) / 4.5f;
                if (t == 0){ e = 0; }
                else if (t == 1){ e=1; }
                else if (t < 0.5f){ 
                    e = -(pow(2.0f, 20 * t - 10) * sin((20 * t - 11.125f) * c5)) / 2;
                }
                else{
                    e = (pow(2.0f, -20 * t + 10) * sin((20 * t - 11.125f) * c5)) / 2 + 1;
                }
                return a + (b - a) * e;
            };
        }
        
        // 6: ease in out quad
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {
            std::cout << "6: ease in out quad" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                if (t < 0.5f){ 
                    e = 2 * t * t;
                }
                else{
                    float j = -2 * t + 2;
                    e = 1 - (j * j) / 2;
                }
                return a + (b - a) * e;
            };
        }

        // 7: ease in out quart
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) {
            std::cout << "7: ease in out quart" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                if (t < 0.5f){ 
                    e = 8 * t * t * t * t;
                }
                else{
                    float j = -2 * t + 2;
                    e = 1 - (j * j * j * j) / 2;
                }
                return a + (b - a) * e;
            };
        }

        // 8: ease out elastic
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) {
            std::cout << "8: ease out elastic" << std::endl;
            tween = [](float a, float b, float t) {
                float e;
                const float c4 = (2 * PI) / 3;
                if (t == 0){ e = 0; }
                else if (t == 1){ e=1; }
                else{
                    e = (pow(2.0f, -10 * t) * sin((10 * t - 0.75f) * c4)) + 1;
                }
                return a + (b - a) * e;
            };
        }

        // 9: ease in cubic
        if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {
            std::cout << "9: ease in cubic" << std::endl;
            tween = [](float a, float b, float t) {
                float e = t * t * t;
                return a + (b - a) * e;
            };
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======

    // draw circle
    sf::CircleShape shape;
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Magenta);

    // calculate X position based on frame time and tween function, and Y position based on window height
    float animationTime = static_cast<float>(ELAPSED_FRAMES % FRAMES_PER_ANIM) / FRAMES_PER_ANIM;
    float x = tween(0, WINDOW_WIDTH - 40, animationTime);

    float y = WINDOW_HEIGHT / 3 - 20;

    /// set position and draw
    shape.setPosition(sf::Vector2f(x, y));
    window.draw(shape);

    // ====== ====== ======
    // (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    // draw graph lines
    for (int i = 0; i <= 2000; i++) {
        float t = i / 2000.0f;
        float value = tween(0, 1, t);   // a = 0, b = 1, so this returns e itself

        sf::CircleShape dot(1);
        dot.setFillColor(sf::Color::Cyan);
        dot.setPosition(sf::Vector2f(GRAPH_X + t * GRAPH_WIDTH - 1, GRAPH_Y - value * GRAPH_HEIGHT - 1));
        window.draw(dot);
    }

    // draw current position dot
    float v = tween(0, 1, animationTime);
    sf::CircleShape marker(12);
    marker.setFillColor(sf::Color::Magenta);
    marker.setPosition(sf::Vector2f(GRAPH_X + animationTime * GRAPH_WIDTH - 12, GRAPH_Y - v * GRAPH_HEIGHT - 12));
    window.draw(marker);

    // draw x axis
    sf::RectangleShape xAxis(sf::Vector2f(GRAPH_WIDTH, 2));
    xAxis.setPosition(sf::Vector2f(GRAPH_X, GRAPH_Y));
    window.draw(xAxis);

    // draw y axis
    sf::RectangleShape yAxis(sf::Vector2f(2, GRAPH_HEIGHT));
    yAxis.setPosition(sf::Vector2f(GRAPH_X, GRAPH_Y - GRAPH_HEIGHT));
    window.draw(yAxis);


    ++ELAPSED_FRAMES;
    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
