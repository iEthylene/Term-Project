#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include <string>
#include <utility>
#include "PopupLayer.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace ui; 

extern pair<int, Hero*> heroCard[5];
extern pair<int, Hero*> prepare[9];

extern Button* shopbutton;
extern Button* upbutton;
extern Button* rebutton;

EventListenerTouchOneByOne* listener; //声明监听器对象作为成员变量或全局变量

/* 初始化卡牌 */
void PopupLayer::Cardsinit() {
    for (int i = 0; i < 5; i++)
        if (heroCard[i].first != -1) {
            /* 移除原来的卡牌 */
            if (heroCard[i].second != nullptr)
                heroCard[i].second->removeFromParent();

            switch (heroCard[i].first) {
                case 0:
                    heroCard[i].second = new HeroPhysicalTank(false, 1);
                    heroCard[i].second->setTexture("Hero/hero0.png");
                    break;
                case 1:
                    heroCard[i].second = new HeroMagicTank(false, 1);
                    heroCard[i].second->setTexture("Hero/hero1.png");
                    break;
                case 2:
                    heroCard[i].second = new HeroPhysicalWarrior(false, 1);
                    heroCard[i].second->setTexture("Hero/hero2.png");
                    break;
                case 3:
                    heroCard[i].second = new HeroMagicalWarrior(false, 1);
                    heroCard[i].second->setTexture("Hero/hero3.png");
                    break;
                case 4:
                    heroCard[i].second = new HeroMarksman(false, 1);
                    heroCard[i].second->setTexture("Hero/hero4.png");
                    break;
                case 5:
                    heroCard[i].second = new HeroMage(false, 1);
                    heroCard[i].second->setTexture("Hero/hero5.png");
                    break;
            }

            heroCard[i].second->setRotation(180);
            heroCard[i].second->setPosition(Vec2(300 + 200 * i, 874.5));
            heroCard[i].second->setScale(1.5F);
            this->addChild(heroCard[i].second, 1);
        }

}

bool PopupLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 touchPos = touch->getLocation(); //获取触摸位置

    /* 判断触摸位置是否在弹窗内部 */
    if ((140 <= touchPos.x && touchPos.x <= 1140 && 643 <= touchPos.y && touchPos.y <= 893) ||
        (1026.25 <= touchPos.x && touchPos.x <= 1213.75 && 26.25 <= touchPos.y && touchPos.y <= 213.75)) {
        int i = (touchPos.x - 140) / 200;
        if (heroCard[i].first != -1)
            for (int j = 0; j < 9; j++)
                if (prepare[j].first == -1) { //将卡牌移动到备战区域
                    heroCard[i].second->setScale(0.8F);

                    prepare[j].first = heroCard[i].first;
                    prepare[j].second = heroCard[i].second;

                    heroCard[i].first = -1;
                    heroCard[i].second = nullptr;

                    prepare[j].second->setPosition(Vec2(310 + 87.25 * j, 207));
                    return true;
                }
        //触摸在弹窗内部，不关闭弹窗，继续传递触摸事件
    }
    else { //触摸在弹窗外部，关闭弹窗
        hide();
        shopbutton->setEnabled(true);
        shopbutton->loadTextures("buttons/ShopNormal.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    }

    return false;
}

/* 初始化弹窗层 */
bool PopupLayer::init() {
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) {
        return false;
    }

    /* 创建弹窗背景 */
    auto popupBg = Sprite::create("ChessBoard/Cards.png");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    popupBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));
    addChild(popupBg);

    /* 创建刷新按钮 */
    rebutton = Button::create("buttons/RefreshNormal.png", "buttons/RefreshSelected.png");
    rebutton->setPosition(Vec2(0.875 * visibleSize.width, 0.400 * visibleSize.height));
    rebutton->addClickEventListener(CC_CALLBACK_1(PopupLayer::reonButtonClicked, this));
    addChild(rebutton);

    /* 点击屏幕外关闭/移动英雄到棋盘 */
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Cardsinit(); //初始化卡牌

    return true;
}

void PopupLayer::reonButtonClicked(Ref* sender) {
    //刷新卡牌
    PlayingScene::randCard();
    Cardsinit();
}

void PopupLayer::hide() {
    Node* parent = getParent();
    for (int i = 0; i < 9; i++) {
        if (prepare[i].first != -1) {
            //将背战区域的卡牌移回父节点
            prepare[i].second->removeFromParent();
            parent->addChild(prepare[i].second);
        }
    }
    removeFromParent();
}