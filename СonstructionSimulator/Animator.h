#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <string>

class Animator
{
public:

    struct Animation {
        std::string name;
        const sf::Texture* texture;

        std::vector<sf::IntRect> frames;
        sf::Time duration;
        bool looping;

        Animation(std::string const& name, const sf::Texture* texture,
            sf::Time const& duration, bool looping)
            : name(name), texture(texture), duration(duration), looping(looping) {}

        void addFrames(sf::Vector2i const& startFrom, sf::Vector2i const& frameSize,
            sf::Vector2i const& framesOffset, unsigned int framesNumber, unsigned int rows) {
            sf::Vector2i current = startFrom;
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < framesNumber; j++) {
                    frames.push_back(sf::IntRect(current.x, current.y, frameSize.x, frameSize.y));
                    current.x += framesOffset.x;
                }
                current.y += framesOffset.y;
                current.x = startFrom.x;
            }
        }
    };

    explicit Animator(sf::Sprite& sprite);

    Animator::Animation& createAnimation(std::string const& name,
        const sf::Texture* texture, sf::Time const& duration,
        bool loop = false);

    void update(sf::Time const& dt);
    bool switchAnimation(std::string const& name);
    void restart();

    std::string getCurrentAnimationName() const;
    bool getEndAnim() const;
    const int getCurrentFrame() const;

private:
    Animator::Animation* findAnimation(std::string const& name);
    void switchAnimation(Animator::Animation* animation);

    std::list<Animator::Animation> Animations_;
    Animator::Animation* CurrentAnimation_;
    sf::Sprite& sprite_;
    sf::Time current_time_;
    bool end_anim_;
    unsigned short current_frame_;
};