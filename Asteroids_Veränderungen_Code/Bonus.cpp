//
//  Bonus.cpp
//  ZIP
//
//  Created by MetalRodTec on 10.06.18.
//  Copyright © 2018 NomNom @ MAC. All rights reserved.
//
#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <random>
#include <ctime>
#include <cassert>
#include <iostream>

using namespace std;
mt19937 therandomGenerator(time(0));
uniform_int_distribution<int> randomValue(1, 4);

Bonus::Bonus(std::string texturePath, const sf::Vector2f &pos) : GameObject(texturePath, pos)
{
    bonusBuffer = new sf::SoundBuffer();
    if (!bonusBuffer->loadFromFile("Audio/Bonus.wav"))
    {
        //
    }
    bonusSound = new sf::Sound();
    bonusSound->setBuffer(*bonusBuffer);
    bonusSound->setVolume(10.f);
    
    switch (randomValue(therandomGenerator)) {
        case 1:
            m_skill = 1;
            this->reload("Sprites/PNG/Power-ups/pill_green.png");
            break;
        case 2:
            m_skill = 2;
            this->reload("Sprites/PNG/Power-ups/shield_silver.png");
            break;
        case 3:
            m_skill = 3;
            this->reload("Sprites/PNG/Power-ups/bold_silver.png");
            break;
        case 4:
            m_skill = 4;
            this->reload("Sprites/PNG/Power-ups/powerupRed.png");
            break;
    }
    m_collisionRadius = 20.0f;
}

void Bonus::Update(sf::RenderWindow * window, float dt)
{
    GameObject::Update(window, dt);
    
}

void Bonus::Draw(sf::RenderWindow* window)
{
    GameObject::Draw(window);
}



void Bonus::CollidedWith(GameObject* other)
{
    Bullet* bullet = dynamic_cast<Bullet*>(other);
    if (bullet != nullptr)
    {
        Destroy();
    } else {
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr || bullet != nullptr)
    {
        Destroy();
        if (dynamic_cast<Player*>(other)->isInvulnerable() == false)
        {
            //other->Destroy();
            
            switch (m_skill) {
                case 1:
                    m_owner->setLive(m_owner->getLive()+1);
                    bonusSound->play();
                    break;
                case 2:
                    bonusSound->play();
                    dynamic_cast<Player*>(other)->setInvulernable();
                    dynamic_cast<Player*>(other)->invulSound();
                    break;
                case 3:
                    bonusSound->play();
                    dynamic_cast<Player*>(other)->shootSound();
                    for (int i = 0; i < 30; i++)
                    {
                        Bullet* bullet = new Bullet(m_pos);
                        bullet->SetAngle(-180 + ((360/30)*i));
                        bullet->SetVelocity(500);
                        m_owner->AddObject(bullet);
                    }
                    break;
                case 4:
                    dynamic_cast<Player*>(other)->DeadSound();
                    other->Destroy();
                    m_owner->RespawnPlayer();
                    break;
            }
            
            
        }
    }
    }
    
}
