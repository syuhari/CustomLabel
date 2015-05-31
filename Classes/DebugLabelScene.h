//
//  DebugLabelScene.h
//
//  Created by Akihiro Matsuura on 2015/05/18.
//
//

#ifndef __HuntCook__DebugLabelScene__
#define __HuntCook__DebugLabelScene__

#include "cocos2d.h"
#include <extensions/cocos-ext.h>
#include "CustomLabel.h"

class DebugLabelScene : public cocos2d::LayerColor
{
public:
    DebugLabelScene();
    virtual ~DebugLabelScene();
    virtual bool init();
    static cocos2d::Scene* createScene();
    CREATE_FUNC(DebugLabelScene);
    
protected:
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Vec2 center;
    float _lineSpacing;
    float _letterSpacing;
    cocos2d::Label* _lineLabel;
    cocos2d::Label* _letterLabel;
    std::string _inputText;
    CustomLabel* _customLabel;
    
    std::string replaceString( std::string String1, std::string String2, std::string String3 );
};

#endif /* defined(__HuntCook__DebugLabelScene__) */
