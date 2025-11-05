#include "Animator.h"

Animator::Animator(sf::Sprite& sprite)
    : CurrentAnimation_(nullptr), sprite_(sprite), current_frame_(0), end_anim_(false) {}

Animator::Animation& Animator::createAnimation(std::string const& name,
    const sf::Texture* texture, sf::Time const& duration, bool loop) {
    Animations_.emplace_back(name, texture, duration, loop);

    if (CurrentAnimation_ == nullptr) {
        switchAnimation(&Animations_.back());
    }

    return Animations_.back();
}

void Animator::switchAnimation(Animator::Animation* animation) {
    if (animation != nullptr && animation->texture != nullptr) {
        sprite_.setTexture(*animation->texture);
    }

    CurrentAnimation_ = animation;
    current_time_ = sf::Time::Zero;
    end_anim_ = false;
}

bool Animator::switchAnimation(std::string const& name) {
    auto animation = findAnimation(name);
    if (animation != nullptr) {
        switchAnimation(animation);
        return true;
    }
    return false;
}

Animator::Animation* Animator::findAnimation(std::string const& name) {
    for (auto it = Animations_.begin(); it != Animations_.end(); it++) {
        if (it->name == name) {
            return &*it;
        }
    }
    return nullptr;
}

void Animator::restart() {
    current_time_ = sf::Time::Zero;
    end_anim_ = false;
    current_frame_ = 0;
}

void Animator::update(sf::Time const& dt) {
    if (CurrentAnimation_ == nullptr) {
        return;
    }

    current_time_ += dt;

    float scaledTime = (current_time_.asSeconds() / CurrentAnimation_->duration.asSeconds());
    auto numFrames = CurrentAnimation_->frames.size();

    if (numFrames == 0) return;

    current_frame_ = static_cast<int>(scaledTime * numFrames);

    if (CurrentAnimation_->looping) {
        current_frame_ %= numFrames;
    }
    else {
        if (current_frame_ >= numFrames) {
            current_frame_ = static_cast<unsigned short>(numFrames - 1);
            end_anim_ = true;
        }
    }

    sprite_.setTextureRect(CurrentAnimation_->frames[current_frame_]);
}

std::string Animator::getCurrentAnimationName() const {
    if (CurrentAnimation_ != nullptr) {
        return CurrentAnimation_->name;
    }
    return "";
}

bool Animator::getEndAnim() const {
    return end_anim_;
}

const int Animator::getCurrentFrame() const {
    return current_frame_;
}