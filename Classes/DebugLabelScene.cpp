//
//  DebugLabelScene.cpp
//
//  Created by Akihiro Matsuura on 2015/05/18.
//
//

#include "DebugLabelScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace extension;

DebugLabelScene::DebugLabelScene()
:_inputText("")
,_lineSpacing(0)
,_letterSpacing(0)
,_customLabel(nullptr)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    center = Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
}

DebugLabelScene::~DebugLabelScene()
{
    
}

Scene* DebugLabelScene::createScene()
{
    auto scene = Scene::create();
    auto layer = DebugLabelScene::create();
    scene->addChild(layer);
    return scene;
}

bool DebugLabelScene::init()
{
    if (! LayerColor::init()) {
        return false;
    }
    
    auto *menu = Menu::create();
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    auto textField = ui::TextField::create("文字を入力してください", "Arial", 20);
    textField->setPosition(Vec2(center.x, visibleSize.height-textField->getContentSize().height));
    this->addChild(textField);
    textField->addEventListener([&](Ref* sender, ui::TextField::EventType type){
        auto field = dynamic_cast<ui::TextField*>(sender);
        if (field!=nullptr) {
            if (type==ui::TextField::EventType::DETACH_WITH_IME) {
                _inputText = field->getString();
                _inputText = this->replaceString(_inputText, "\\n", "\n");
                CCLOG("text:%s", _inputText.c_str());
            }
        }
    });
    
    {
        auto label = Label::createWithSystemFont("行間隔：0px", "Arial", 20);
        label->setPosition(Vec2(center.x, textField->getPositionY()-50));
        this->addChild(label);
        _lineLabel = label;
        
        auto slider = ui::Slider::create();
        slider->loadBarTexture("extensions/sliderTrack.png");
        slider->loadProgressBarTexture("extensions/sliderProgress.png");
        slider->loadSlidBallTextureNormal("extensions/sliderThumb.png");
        slider->loadSlidBallTexturePressed("extensions/sliderThumb.png");
        slider->setPosition(textField->getPosition() + Vec2(0, -100));
        slider->setPercent(50);
        this->addChild(slider);
        slider->addEventListener([&](Ref* sender, ui::Slider::EventType type){
            auto slider = dynamic_cast<ui::Slider*>(sender);
            if (type==ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
                _lineSpacing = 0.5f * (slider->getPercent()-50);
                std::string text = StringUtils::format("行間隔：%.1fpx", _lineSpacing);
                _lineLabel->setString(text);
            }
        });
    }
    
    {
        auto label = Label::createWithSystemFont("文字間隔：0px", "Arial", 20);
        label->setPosition(Vec2(center.x, textField->getPositionY()-150));
        this->addChild(label);
        _letterLabel = label;
        
        auto slider = ui::Slider::create();
        slider->loadBarTexture("extensions/sliderTrack.png");
        slider->loadProgressBarTexture("extensions/sliderProgress.png");
        slider->loadSlidBallTextureNormal("extensions/sliderThumb.png");
        slider->loadSlidBallTexturePressed("extensions/sliderThumb.png");
        slider->setPosition(textField->getPosition() + Vec2(0, -200));
        slider->setPercent(50);
        this->addChild(slider);
        slider->addEventListener([&](Ref* sender, ui::Slider::EventType type){
            auto slider = dynamic_cast<ui::Slider*>(sender);
            if (type==ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
                _letterSpacing = 0.5f * (slider->getPercent()-50);
                std::string text = StringUtils::format("文字間隔：%.1fpx", _letterSpacing);
                _letterLabel->setString(text);
            }
        });
    }
    
    auto refresh = MenuItemLabel::create(Label::createWithSystemFont("更新", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->removeFromParent();
        }
        
        _customLabel = CustomLabel::createWithTTF(_inputText, "fonts/arial.ttf", 30, _letterSpacing, _lineSpacing);
        _customLabel->setPosition(Vec2(center.x, center.y));
        this->addChild(_customLabel);
        _customLabel->setDimensions(visibleSize.width, visibleSize.height);
        
    });
    refresh->setPosition(Vec2(center.x, center.y));
    menu->addChild(refresh);
    
    auto left = MenuItemLabel::create(Label::createWithSystemFont("左寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        }
    });
    left->setPosition(Vec2(visibleSize.width/4, refresh->getPositionY()-left->getContentSize().height-20));
    menu->addChild(left);
    
    auto acenter = MenuItemLabel::create(Label::createWithSystemFont("中央寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        }
    });
    acenter->setPosition(Vec2(center.x, left->getPositionY()));
    menu->addChild(acenter);
    
    auto right = MenuItemLabel::create(Label::createWithSystemFont("右寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
        }
    });
    right->setPosition(Vec2(visibleSize.width*0.75f, left->getPositionY()));
    menu->addChild(right);
    
    auto top = MenuItemLabel::create(Label::createWithSystemFont("上寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setVerticalAlignment(TextVAlignment::TOP);
        }
    });
    top->setPosition(Vec2(visibleSize.width/4, left->getPositionY()-top->getContentSize().height-20));
    menu->addChild(top);
    
    auto vcenter = MenuItemLabel::create(Label::createWithSystemFont("上下中央寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setVerticalAlignment(TextVAlignment::CENTER);
        }
    });
    vcenter->setPosition(Vec2(center.x, top->getPositionY()));
    menu->addChild(vcenter);
    
    auto bottom = MenuItemLabel::create(Label::createWithSystemFont("下寄せ", "Arial", 30), [&](Ref *sender){
        if (_customLabel!=nullptr) {
            _customLabel->setVerticalAlignment(TextVAlignment::BOTTOM);
        }
    });
    bottom->setPosition(Vec2(visibleSize.width*0.75f, top->getPositionY()));
    menu->addChild(bottom);

    return true;
}

std::string DebugLabelScene::replaceString(std::string string1, std::string string2, std::string string3)
{
    std::string::size_type  pos(string1.find(string2));
    
    while(pos!=std::string::npos) {
        string1.replace(pos, string2.length(), string3);
        pos = string1.find(string2, pos + string3.length());
    }
    
    return string1;
}
