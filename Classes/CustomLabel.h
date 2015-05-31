//
//  CustomLabel.h
//
//  Created by Akihiro Matsuura on 2015/05/18.
//
//

#ifndef __HuntCook__CustomLabel__
#define __HuntCook__CustomLabel__

#include "cocos2d.h"

class CustomLabel : public cocos2d::Node
{
    
public:
    CustomLabel();
    virtual ~CustomLabel();
    bool init(std::string text, cocos2d::TTFConfig ttfConfig, float letterSpacing=0.0f, float lineSpacing=0.0f);
    static CustomLabel * createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, float letterSpacing=0.0f, float lineSpacing=0.0f);
    void enableOutline(const cocos2d::Color4B& outlineColor,int outlineSize=-1);
    void setColor(const cocos2d::Color3B& color);
    void setDimensions(unsigned int width,unsigned int height);
    void setAlignment(cocos2d::TextHAlignment hAlignment,cocos2d::TextVAlignment vAlignment);
    void setHorizontalAlignment(cocos2d::TextHAlignment hAlignment);
    void setVerticalAlignment(cocos2d::TextVAlignment vAlignment);
protected:
    
private:
    std::u16string _currentUTF16String;
    std::string    _originalUTF8String;
    std::vector<cocos2d::Label*> _labels;
    std::vector<cocos2d::Layer*> _layers;
    float _lineSpacing;
    float _letterSpacing;
    float _labelHeight;
    cocos2d::TTFConfig _ttfConfig;
    cocos2d::Layer* _layer;
    cocos2d::Size _dimension;
    cocos2d::TextHAlignment _hAlignment;
    cocos2d::TextVAlignment _vAlignment;
    
    void makeLabel();
};

#endif /* defined(__HuntCook__CustomLabel__) */
