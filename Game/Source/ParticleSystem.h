#pragma once

#include <SFML/Graphics.hpp>        //for Graphics stuff

class ParticleSystem
{
    public:
        //Emitter values
        int maxParticles;
        float launchSpeed;
        float particleCooldown;         //Time between each particle launch. 0.01f


        //Particle values
        float startLifeTime;                    // 3.4f
        float repultionStrength;                 //0.07f
        float repultionRange;                    // 180
        float downwardGravity;
        float particeRotaionsSpeed;
        float startScale;
        float endScale;
        float airRecistance;

        sf::Color startColor;
        sf::Color endColor;

};




class Emitter : ParticleSystem
{
private:
    Particle* particles;

    sf::Vector2f pos;
    sf::Clock timer;                                     //Used to count delta-time.
    sf::Clock particleCooldownTimer;                     //Used to time the spawns of particles.

    float cooldown;
    int particleCount;                                  // How many particles are present in scene.

public:
	Emitter();
    Emitter(sf::Vector2f posIn, 
        int maxParticlesIn,
        float launchSpeedIn,
        float particleCooldownIn,         //Time between each particle launch. 0.01f
        float startLifeTimeIn,                    // 3.4f
        float repultionStrengthIn,                 //0.07f
        float repultionRangeIn,                    // 180
        float downwardGravityIn,
        float particeRotaionsSpeedIn,
        float startScaleIn,
        float endScaleIn,
        float airRecistanceIn,
        sf::Color startColorIn,
        sf::Color endColorIn);
    void shootParticle();
    void removeParticle();
    void update(sf::Vector2f parrent);
    void draw();

};





class Particle : ParticleSystem
{
private:
	Emitter* emitter;
    sf::Sprite body;
    sf::Texture texture;
    sf::Vector2f vel;                         //Velocity of the particle.
    sf::Vector2f pos;
    float lifeSpan;                       //Amount of seconds the seconds the particle has left to live.
    float rotationSpeed;

public:
    Particle(sf::Vector2f emitterPos, sf::Vector2f launchVel, Emitter* emitterIn);
    void explode();
    void update();
    void draw();

};


