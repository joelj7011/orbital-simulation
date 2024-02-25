#include "SFML-2.5.1/include/SFML/Graphics.hpp"
#include <vector>
#include <cstdlib> // For random number generation
#include <ctime>   // For seeding random number generator

using namespace std;


// gravity class
class gravity {
public:
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;

    gravity(float pos_x, float pos_y, float strength) {
        pos.x = pos_x;
        pos.y = pos_y;
        this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(10);
    }

    void render(sf::RenderWindow& w) {
        w.draw(s);
    }

    sf::Vector2f get_pos() {
        return pos;
    }

    float get_strength() {
        return strength;
    }
};


//particle class
class particle{
public:
    sf::Vector2f pos;
    sf::Vector2f val;
    sf::CircleShape s;

    particle(float pos_x, float pos_y, float val_x, float val_y) {
        pos.x = pos_x;
        pos.y = pos_y;
        val.x = val_x;
        val.y = val_y;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(0.3);
    }
    void render(sf::RenderWindow& w) {
        s.setPosition(pos);
        w.draw(s);
    }

    void update_physics(gravity& s, const sf::Vector2u& windowSize) {

        //storing the difference
        float distance_x = s.get_pos().x - pos.x;
        float distance_y = s.get_pos().y - pos.y;

        //getting the distance 
        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        // inversquarelaw first inplimentation
        float inverse_distance = 1.f / distance; 

       
        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        // inversquarelaw second inplimentation if (distance++){gravity++}
        float inverse_square_dropoff = inverse_distance * inverse_distance;

        //inversquarelaw final inplimentation 
        float acceleration_x = normalized_x * s.get_strength()* inverse_square_dropoff;
        float acceleration_y = normalized_y * s.get_strength()* inverse_square_dropoff;

        //adding accelerationto the val to make it move
        val.x += acceleration_x;
        val.y += acceleration_y;

        //adding val to pos to change the pos
        pos.x += val.x;
        pos.y += val.y;


        //collision detectcion
        if (pos.x <= 0 || pos.x >= windowSize.x) {
            val.x = -val.x; 
        }
        if (pos.y <= 0 || pos.y >= windowSize.y) {
            val.y = -val.y; 
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "My Program");
    window.setFramerateLimit(60);

    //declaring vectors
    vector<gravity> sources;
    vector<particle> particles;

    //creating sources
    sources.push_back(gravity(500, 500, 7000));
    sources.push_back(gravity(1200, 500, 7000));

    //declaring number of particle
    int num_particles = 1000;
  
    //iterating over all  particles
    for (int i = 0; i < num_particles; i++) {
        float initial_x_position = 600;
        float initial_y_position = 700;
        float initial_velocity_x = 4;
        float initial_velocity_y = 0.2 + (0.1 / num_particles * i);
        particles.push_back(particle(initial_x_position, initial_y_position, initial_velocity_x, initial_velocity_y));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        //get window size for collision detection
        sf::Vector2u windowSize = window.getSize(); // Get window size

        window.clear();
        
    //updating physics for each particle
        for (int i = 0; i < sources.size(); i++) {
            for (int j = 0; j < particles.size(); j++) {
                particles[j].update_physics(sources[i], windowSize); // Pass windowSize to update_physics
            }
        }

        //rendering all the sources
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }

        //rendering all the particles
        for (int j = 0; j < particles.size(); j++) {
            particles[j].render(window);
        }

        window.display();
    }

    return 0;
}
