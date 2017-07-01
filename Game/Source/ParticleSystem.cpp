#include "ParticleSystem.h"
#include "HelpFunctions.h"


extern float dt;


Particle::Particle(sf::Vector2f emitterPos, sf::Vector2f emitterVel, Emitter* emitterIn)
{
    emitter = emitterIn;
    particeRotaionsSpeed = ((particeRotaionsSpeed * ((randomNumber(0,1)) ? (-1) : 1)) + (randomNumber(0, 10)-5))/10.0f; 

	lifeSpan = startLifeTime;
	
	body.setOrigin(sf::Vector2f(texture.getSize().x/2.0f, texture.getSize().y/2.0f));
	body.setTexture(texture);
	body.setScale(startScale, startScale);

    body.setPosition(emitterPos);

    vel = emitterVel;
    pos = emitterPos;
}


void Particle::explode()
{
    emitter->removeParticle();
}


void Particle::update()
{
    lifeSpan -= dt;

    if (lifeSpan < 0)
    {
        explode();
    }

    
    float timeLeft = lifeSpan / startLifetime;	
	sf::Color newColor = endColor;
	newColor += sf::Color ( startColor.r * timeLeft,
							startColor.g * timeLeft,
							startColor.b * timeLeft,
							startColor.a * timeLeft);

	newColor -= sf::Color ( endColor.r * timeLeft,
							endColor.g * timeLeft,
							endColor.b * timeLeft,
							endColor.a * timeLeft);
	body.setColor(newColor);


    body.setScale(endScale * timeLeft + startScale * timeLeft, endScale * timeLeft + startScale * timeLeft);



   pos = body.getPosition();

    vel.y -= GRAVITY;
/*
    switch (mode)
    {
        case gravity:
            if (magnitude(mousePos, pos) < WINDRANGE)
            {
                vel -= clampVector(pos - mousePos, 1) * CURSORWIND*4.0f * (WINDRANGE - (magnitude(pos, mousePos)));
            }
            break;


        case repultion:
            if (magnitude(mousePos, pos) < 3*WINDRANGE/5)
            {
                vel += clampVector(pos - mousePos, 1) * CURSORWIND * 5.0f * (WINDRANGE - (magnitude(pos, mousePos)));
            }
            break;


        case wind:
                vel += clampVector(emitterStartPosition - mousePos, 1) * CURSORWIND * 75.0f;
            break;


        case freeze:
            if (magnitude(mousePos, pos) < WINDRANGE/2)
            {
                vel *= 1.0f -  std::pow(magnitude(pos, mousePos) / WINDRANGE, 3);
            }
            break;
    }
    */
    
	vel *= AIRRESISTANCE;

    


    if ((pos.x < 50 && vel.x < 0) || (pos.x > windowWidth - 50 && vel.x > 0))
    {
        vel.x *= -0.60f;
    }

    if ((pos.y < 50 && vel.y < 0) || (pos.y > windowHeight - 50 && vel.y > 0))
    {
        vel.y *= -0.60f;
    }

    pos += vel * dt;
    body.setPosition(pos);
    body.rotate(rotationSpeed);

}


void Particle::draw()
{
    window.draw(body);
}




Emitter::Emitter()
{
    pos = sf::Vector2f(0,0);
    maxParticles = 10000;
    launchSpeed = 180;
    particleCooldown = 0.0002f;
    startLifeTime = 3.8f;      
    repultionStrength = 0.013f;             
    repultionRange = 230;           
    downwardGravity = -0.5f;
    particeRotaionsSpeed = 4.0f;
    startScale = 0.35f;
    endScale = 0.1f;
    airRecistance = 0.996f;
    startColor = sf::Color (255, 20, 20, 225);
    endColor = sf::Color (20, 20, 255, 120);

    particles = new Particle[maxParticles];

}



Emitter::Emitter(sf::Vector2f posIn, 
                int maxParticlesIn,
                float launchSpeedIn,
                float particleCooldownIn,
                float startLifeTimeIn,                    
                float repultionStrengthIn,                
                float repultionRangeIn,                 
                float downwardGravityIn,
                float particeRotaionsSpeedIn,
                float startScaleIn,
                float endScaleIn,
                float airRecistanceIn,
                sf::Color startColorIn,
                sf::Color endColorIn);
{
    pos = posIn;
    maxParticles = maxParticlesIn;
    launchSpeed = launchSpeedIn;
    particleCooldown = particleCooldownIn;
    startLifeTime =  startLifeTimeIn;
    repultionStrength = repultionStrengthIn;
    repultionRange = repultionRangeIn;
    downwardGravity = downwardGravityIn;
    particeRotaionsSpeed = particeRotaionsSpeedIn;
    startScale = startScaleIn;
    endScale = endScaleIn;
    airRecistance = airRecistanceIn;
    startColor = startColorIn;
    endColor = endColorIn;

}

void Emitter::shootParticle()
{   
        if (particleCount < MAXPARTICLES)
        {
            sf::Vector2f shootVel;
            float angle = 0;
            float time = 0;

            particleCount++;
            for (int i = particleCount; i > 0; i--)
            {
                particles[i] = particles[i - 1];       //Shift all the particles one step further to make space for new particle.

            }


            int rotation = getRandomNumber(0, 360);

            shootVel.x = std::cos(rotation) * LAUNCHSPEED;
            shootVel.y = std::sin(rotation) * LAUNCHSPEED;

            particles[0] = new Particle(pos, shootVel + vel, );

        }
        else
        {
           // std::cout << "\n No more space for new particles, " << particleCount << " particles is maximum.";
        }
    
}

void Emitter::removeParticle()
{
    delete particles[particleCount--];
}

void Emitter::update(sf::Vector2f parrent)
{
	pos = parrent;

    for (int i = 0; i < particleCount; i++)
    {
        particles[i]->update();
    }
}

void Emitter::draw()
{
    for (int i = 0; i < particleCount; i++)
    {
        particles[i]->draw();
    }
}
