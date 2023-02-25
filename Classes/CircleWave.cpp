#include "CircleWave.h"

USING_NS_AX;

void CircleWave::draw(ax::Renderer *renderer, const ax::Mat4& transform, uint32_t flags) {
   // log_ << "ASSA";
    glBlendFunc(0x302, 1);
    glLineWidth(this->m_fLineWidth);
    if (this->m_bFilled)
        this->drawSolidCircle(this->getPosition(), this->m_fRadius, 0.0f, (this->m_fRadius <= 400.0) ? (30) : (60), this->m_eColor);
    else
        this->drawCircle(this->getPosition(), this->m_fRadius, 0.0f, this->m_fRadius <= 400.0 ? 30 : 60, false, this->m_eColor);
    glBlendFunc(1u, 0x303u);

    this->drawSolidCircle(Director::getInstance()->getWinSize() / 2, 100, 90, 100, {1,1,1,1});
}

void CircleWave::setColor(Color4B col) {
    this->m_eColor = {col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f};
}

Color4B CircleWave::getColor() {
    return {(uint8_t)(m_eColor.r * 255.f), (uint8_t)(m_eColor.g * 255.f), (uint8_t)(m_eColor.b * 255.f), (uint8_t)(m_eColor.a * 255.f)};
}

bool CircleWave::init(float duration, Color4B color, float radiusMin, float radiusMax, bool easing, float lineWidth, bool filled) {
    if (!DrawNode::init()) return false;

    this->setColor(color);
    this->m_fRadius = radiusMin;
    this->m_fLineWidth = lineWidth;
    this->m_bFilled = filled;

    if (easing) {
        this->m_eColor.a = 0;
        auto radAction = ActionTween::create(duration, "radius", radiusMin, radiusMax);
        auto opacityAction = ActionTween::create(duration / 2, "opacity", 0.0, 1.0);
        auto opacityAction2 = ActionTween::create(duration / 2, "opacity", 1.0, 0.0);
        auto seq = Sequence::create(opacityAction, opacityAction2, CallFunc::create([=]() {
            this->removeFromParent();
        }), nullptr);
        auto action = Spawn::create(radAction, seq, nullptr);
        Director::getInstance()->getActionManager()->addAction(action, this, false);
    } else {
        auto radAction = EaseOut::create(ActionTween::create(duration, "radius", radiusMin, radiusMax), 2.0f);
        auto opacityAction = EaseOut::create(ActionTween::create(duration / 2, "opacity", this->m_eColor.a, 0.0), 2.0f);
        auto spawn = Spawn::create(radAction, opacityAction, nullptr);
        auto action = Sequence::create(spawn, CallFunc::create([=]() {
            this->removeFromParent();
        }), nullptr);
        Director::getInstance()->getActionManager()->addAction(action, this, false);
    }

    this->scheduleUpdate();

    return true;
}

void CircleWave::updateTweenAction(float value, std::string_view key) {
    if (key == "radius") {
        this->m_fRadius = value;
    } else if (key == "opacity") {
        this->m_eColor.a = value;
    }
}

void CircleWave::update(float dt) {
    if (this->m_pFollowedNode) {
        this->setPosition(m_pFollowedNode->getPosition() / 2);
    }

    this->clear();
    glBlendFunc(0x302, 1);
    glLineWidth(this->m_fLineWidth);
    if (this->m_bFilled)
        this->drawSolidCircle(this->getPosition(), this->m_fRadius, 0.0f, (this->m_fRadius <= 400.0) ? (30) : (60), this->m_eColor);
    else
        this->drawCircle(this->getPosition(), this->m_fRadius, 0.0f, this->m_fRadius <= 400.0 ? 30 : 60, false, this->m_eColor);
    glBlendFunc(1u, 0x303u);
}

void CircleWave::followNode(Node* node) {
    this->m_pFollowedNode = node;
}

CircleWave* CircleWave::create(float duration, Color4B color, float radiusMin, float radiusMax, bool easing, bool filled, float lineWidth) {
    auto pRet = new(std::nothrow) CircleWave();

    if (pRet && pRet->init(duration, color, radiusMin, radiusMax, easing, lineWidth, filled)) {
        pRet->autorelease();
        return pRet;
    } else {
        AX_SAFE_DELETE(pRet);
        return nullptr;
    }
}