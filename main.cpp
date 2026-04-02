#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define g 9.81f
#define G 100.f

class Body{
    private:
        float mass;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;

        sf::CircleShape shape;
        std::vector<sf::Vector2f> trail;
    public:
        Body(float m, sf::Vector2f pos, float radius)
            : mass(m), position(pos), velocity(0.f, 0.f), shape(radius) {
                shape.setOrigin({radius, radius});
                shape.setPosition(position);
            }

        float getMass() const {return mass;}
        sf::Vector2f getPosition() const {return position;}
        sf::Vector2f getVelocity() const {return velocity;}

        void setVelocity(sf::Vector2f v) {velocity = v;}

        void update(float dt) {
            position += velocity * dt + acceleration * dt * dt;
            shape.setPosition(position);

            if (trail.size() > 1000) {
                trail.shrink_to_fit();
            } 

            trail.push_back(position);
        }

        void applyForce(sf::Vector2f force, float dt) {
            sf::Vector2f acceleration = force / mass;
            velocity += acceleration * dt;
        }

        void draw(sf::RenderWindow& window) {
            window.draw(shape);
        }

        void setColor(int R, int Gr, int B) {
            shape.setFillColor(sf::Color(R, Gr, B));
        }

        void drawTrail(sf::RenderWindow& window) {
            for(size_t i = 1; i < trail.size(); ++i) {
                sf::Vertex v1;
                v1.position = trail[i - 1];
                v1.color = sf::Color(200, 200, 255, 150);

                sf::Vertex v2;
                v2.position = trail[i];
                v2.color = sf::Color(200, 200, 255, 150);

                sf::Vertex line[] = {v1, v2};
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }
};

int main() {
    sf::Clock clock;
    
    unsigned int window_width = 1280;
    unsigned int window_height = 960;
    sf::Vector2u windowSize({window_width, window_height});
    sf::RenderWindow scene(sf::VideoMode(windowSize), "2BodyProblem");

    //define the m1 body - the central body
    float m1_mass = 10000.f;
    sf::Vector2f m1_pos(640.f, 480.f);
    float m1_radius = 50.f;
    Body m1(m1_mass, m1_pos, m1_radius);
    m1.setColor(255, 200, 50);

    //define the m2 body - the orbiting body
    float m2_mass = 10.f;
    sf::Vector2f m2_pos(640.f + 150.f, 480.f);
    float m2_radius = 20.f;
    Body m2(m2_mass, m2_pos, m2_radius);
    sf::Vector2f m2_tangentialVelocity2 (0.f, -80.f);
    m2.setVelocity(m2_tangentialVelocity2);
    m2.setColor(80, 120, 255);

    //start the scene
    while(scene.isOpen()){
        float dt = clock.restart().asSeconds();

        while (const std::optional event = scene.pollEvent()){
            if (event -> is <sf::Event::Closed>()){
                scene.close();
            }
        }
    
    sf::Vector2f direction = m1.getPosition() - m2.getPosition();

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f normalized = direction / distance;

    if (distance > 0.f) {
        float forceMag = G * (m1.getMass() * m2.getMass()) / (distance * distance);
        sf::Vector2f force = normalized * forceMag;

        m2.applyForce(force, dt);
        m1.applyForce(-force, dt);
    }

    m1.update(dt);
    m2.update(dt);

    scene.clear();
    m1.draw(scene);
    m2.draw(scene);
    m2.drawTrail(scene);
    scene.display();   
    }

    return 0;
}