//
//  CustomLabel.cpp
//
//  Created by Akihiro Matsuura on 2015/05/18.
//
//

#include "CustomLabel.h"

USING_NS_CC;
using namespace std;

CustomLabel::CustomLabel()
:_layer(nullptr)
,_letterSpacing(0)
,_lineSpacing(0)
,_dimension(Size::ZERO)
,_hAlignment(TextHAlignment::LEFT)
,_vAlignment(TextVAlignment::TOP)
,_labelHeight(0)
{
    
}

CustomLabel::~CustomLabel()
{
    
}

CustomLabel* CustomLabel::createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, float letterSpacing/*=0.0f*/, float lineSpacing/*=0.0f*/)
{
    auto ret = new (std::nothrow) CustomLabel();
    
    if (ret && FileUtils::getInstance()->isFileExist(fontFile))
    {
        TTFConfig ttfConfig(fontFile.c_str(),fontSize,GlyphCollection::DYNAMIC);
        if (ret->init(text, ttfConfig, letterSpacing, lineSpacing))
        {
            ret->autorelease();
            
            return ret;
        }
    }
    
    delete ret;
    return nullptr;
}

bool CustomLabel::init(std::string text, TTFConfig ttfConfig, float letterSpacing, float lineSpacing)
{
    if (! Node::init()) {
        return false;
    }
    
    CCLOG("text = %s", text.c_str());
    
    _originalUTF8String = text;
    _lineSpacing = lineSpacing;
    _letterSpacing = letterSpacing;
    _ttfConfig = ttfConfig;
    this->makeLabel();
    
    return true;
}

void CustomLabel::enableOutline(const Color4B& outlineColor,int outlineSize /* = -1 */)
{
    for (auto label : _labels) {
        label->enableOutline(outlineColor, outlineSize);
    }
}

void CustomLabel::setColor(const Color3B& color)
{
    for (auto label : _labels) {
        label->setColor(color);
    }
}

void CustomLabel::setDimensions(unsigned int width,unsigned int height)
{
    _dimension = Size(width, height);
    this->makeLabel();
}

void CustomLabel::makeLabel()
{
    if (_layer!=nullptr) {
        _layer->removeFromParent();
        _layer = nullptr;
    }
    
    _layer = Layer::create();
    this->addChild(_layer);
    
    std::u16string utf16String;
    if (StringUtils::UTF8ToUTF16(_originalUTF8String, utf16String))
    {
        _currentUTF16String  = utf16String;
        int strLen = static_cast<int>(_currentUTF16String.length());
        
        Size labelSize = Size();
        Vec2 position = Vec2();
        Vec2 original = position;
        Size nodeSize = Size();
        _labels.clear();
        _layers.clear();
        
        // 最初の一行目用のレイヤ
        auto layer = Layer::create();
        _layer->addChild(layer);
        _layers.push_back(layer);
        
        // 一文字づつラベルを生成
        for (int i=0; i<strLen; i++) {
            const char16_t* str = _currentUTF16String.substr(i,1).c_str();
            
            std::string utf8str;
            StringUtils::UTF16ToUTF8(str, utf8str);
            
            if (utf8str=="\n") {
                // 改行処理
                auto size = layer->getContentSize();
                size.width = position.x;
                layer->setContentSize(size);
                
                position.x = original.x;
                float height = labelSize.height + _lineSpacing;
                position.y -= height;
                nodeSize.height += height;
                
                // 行用のレイヤを追加
                layer = Layer::create();
                _layer->addChild(layer);
                _layers.push_back(layer);
            } else {
                // ラベル生成
                Label* label = Label::createWithTTF(utf8str, _ttfConfig.fontFilePath, _ttfConfig.fontSize);
                label->setAnchorPoint(Vec2(0, 0.5f));
                label->setPosition(position);
                layer->addChild(label);
                _labelHeight = label->getContentSize().height + _lineSpacing;
                
                labelSize = label->getBoundingBox().size;
                float width = labelSize.width + _letterSpacing;
                
                position.x += width;
                
                if (_dimension.width>0 && position.x > _dimension.width) {
                    // dimension の横幅一杯で改行処理
                    auto size = layer->getContentSize();
                    size.width = position.x - width;
                    layer->setContentSize(size);
                    
                    // 行用のレイヤを追加
                    layer = Layer::create();
                    _layer->addChild(layer);
                    _layers.push_back(layer);
                    
                    position.x = original.x;
                    float height = labelSize.height + _lineSpacing;
                    position.y -= height;
                    nodeSize.height += height;
                    
                    label->setPosition(position);
                    position.x += width;
                    
                    label->removeFromParent();
                    layer->addChild(label);
                }
                
                nodeSize.width = MAX(nodeSize.width, position.x);
                _labels.push_back(label);
            }
        }
        
        // 最終行のサイズ決定
        auto size = layer->getContentSize();
        size.width = position.x;
        layer->setContentSize(size);
        
        _contentSize = nodeSize;
        
        // 最終的な位置決定
        if (_dimension.width>0) {
            _layer->setPositionX(-_dimension.width/2);
        } else {
            _layer->setPositionX(-nodeSize.width/2);
        }
        if (_dimension.height>0) {
            _layer->setPositionY(_dimension.height/2 - _labelHeight/2);
        } else {
            _layer->setPositionY(_contentSize.height/2 - _labelHeight/2);
        }
    }
}

void CustomLabel::setAlignment(TextHAlignment hAlignment,TextVAlignment vAlignment)
{
    this->setHorizontalAlignment(hAlignment);
    this->setVerticalAlignment(vAlignment);
}

void CustomLabel::setHorizontalAlignment(cocos2d::TextHAlignment hAlignment)
{
    if (hAlignment==_hAlignment) {
        return;
    }
    
    _hAlignment = hAlignment;
    switch (_hAlignment) {
        case TextHAlignment::LEFT:
            for (auto layer : _layers) {
                layer->setPositionX(0);
            }
            break;
        case TextHAlignment::RIGHT:
            for (auto layer : _layers) {
                if (_dimension.width>0) {
                    layer->setPositionX(_dimension.width - layer->getContentSize().width);
                } else {
                    layer->setPositionX(_contentSize.width - layer->getContentSize().width);
                }
            }
            break;
        case TextHAlignment::CENTER:
            for (auto layer : _layers) {
                if (_dimension.width>0) {
                    layer->setPositionX(_dimension.width/2 - layer->getContentSize().width/2);
                } else {
                    layer->setPositionX(_contentSize.width/2 - layer->getContentSize().width/2);
                }
            }
            break;
        default:
            break;
    }
}

void CustomLabel::setVerticalAlignment(cocos2d::TextVAlignment vAlignment)
{
    if (vAlignment==_vAlignment) {
        return;
    }
    
    _vAlignment = vAlignment;
    switch (_vAlignment) {
        case TextVAlignment::TOP:
            if (_dimension.height>0) {
                _layer->setPositionY(_dimension.height/2 - _labelHeight/2);
            } else {
                _layer->setPositionY(_contentSize.height/2 - _labelHeight/2);
            }
            break;
        case TextVAlignment::BOTTOM:
            if (_dimension.height>0) {
                _layer->setPositionY(-_dimension.height/2 + _contentSize.height + _labelHeight/2);
            } else {
                _layer->setPositionY(-_contentSize.height/2 + _labelHeight/2);
            }
            break;
        case TextVAlignment::CENTER:
            _layer->setPositionY(_contentSize.height/2);
            break;
        default:
            break;
    }
}