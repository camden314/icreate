// Copyright camden314 2021
#ifndef __CC_DEFS_HPP__
#define __CC_DEFS_HPP__

#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <string>
#include <map>
#include <CCMenuItemSpriteExtra.h>
#include <cocos2dx/cocos2d.h>
#include <cocos2dext/cocos-ext.h>


#define GM (GameManager::sharedState())
#define LEL (GM->_editorLayer())
#define PL (GM->_playLayer())
#define GJBL (LEL ? static_cast<GJBaseGameLayer*>(LEL) : static_cast<GJBaseGameLayer*>(PL))
void setupTypeinfos();

typedef void(*queuefunc)(std::string);

typedef struct GameModes {
    bool cube;
    bool ship;
    bool ufo;
    bool ball;
    bool wave;
    bool robot;
    bool spider;
} GameModes;

typedef struct LevelDifficulty {
    int32_t denominator;
    int32_t numerator;  
} LevelDifficulty;

#define CLASS_PARAM(__TYPE__, __GETTER__, __OFFSET__) \
    inline __TYPE__& _##__GETTER__() { \
        return *((__TYPE__*)((long)this + __OFFSET__)); \
    }

#define STRUCT_PARAM(__TYPE__, __GETTER__, __OFFSET__) \
    inline __TYPE__ _##__GETTER__() { \
        return *(__TYPE__*)((long)this+__OFFSET__); \
    }

class GDObj { 
public:
    void* valOffset(long offset);
    void setValOffset(long offset, void* setter);
};

class MusicDownloadManager : public cocos2d::CCNode, public GDObj {
public:
    static MusicDownloadManager* sharedState();
    void addSong(std::string name);
};

class GJGameLevel : public GDObj {
public:
    static GJGameLevel* create();
    CLASS_PARAM(std::string, name, 0x138);
    CLASS_PARAM(int, levelId, 0x130);
    CLASS_PARAM(int, bestNormal, 0x214);
    CLASS_PARAM(int, bestPractice, 0x238);
    CLASS_PARAM(std::string, levelString, 0x148);
    CLASS_PARAM(std::string, author, 0x150);
    STRUCT_PARAM(LevelDifficulty, difficulty, 0x1b0);
};

class LevelSettingsObject : public GDObj {
public:
    CLASS_PARAM(GJGameLevel*, level, 0x150);
};

class GameLevelManager : public GDObj {
public:
    static GameLevelManager* sharedState();
    GJGameLevel* createNewLevel();
    cocos2d::CCDictionary* getAllUsedSongIds();
    CLASS_PARAM(cocos2d::CCDictionary*, timerDict, 0x1e8);
};

class AppDelegate : public GDObj {
public:
    CLASS_PARAM(cocos2d::CCScene*, runningScene, 0x28);
    static AppDelegate* get();
};

class GameSoundManager : public GDObj {
public:
    static GameSoundManager* sharedManager();
    void stopBackgroundMusic();
    virtual ~GameSoundManager();
};

class GameObject : public cocos2d::CCSprite, public GDObj{
public:
    GameObject();
    void init(char const* frame);
    void setPosition(cocos2d::CCPoint const& pt);
    void destroyObject();
    int getGroupID(int index);
    void playShineEffect();
    CLASS_PARAM(int, type, 0x370);
    CLASS_PARAM(int, id, 0x3c4);
    CLASS_PARAM(bool, touchTriggered, 0x378);
    CLASS_PARAM(bool, spawnTriggered, 0x379);
    CLASS_PARAM(int, uuid, 0x36c);
    CLASS_PARAM(int, colorID, 0x3bc);
};

class SpawnTriggerAction : public cocos2d::CCNode { // omg
 public:
    bool timerEnded;
    float delay;
    float timer;
    int group;
    int uuid;
};

class EffectGameObject : public GameObject {
public:
    CLASS_PARAM(bool, touchHoldMode, 0x579);
    CLASS_PARAM(float, spawnDelay, 0x588);

};

class LabelGameObject : public GameObject {
 public:
  static LabelGameObject* create(char const* frame);
};

class GameToolbox : public cocos2d::CCNode {
 public:
    static std::map<std::string, std::string> stringSetupToMap(std::string st, char const* seperator);
};

class PlayerObject : public GameObject { 
public:
    static PlayerObject* create(int icn, int icon, cocos2d::CCLayer*);
    void addAllParticles();
    void setColor(cocos2d::_ccColor3B const&);
    void setSecondColor(cocos2d::_ccColor3B const&);
    void flipGravity(bool, bool);
    void pushButton(int);
    CLASS_PARAM(double, yAccel, 0x760);
    CLASS_PARAM(float, xPos, 0x7c8);
    CLASS_PARAM(float, yPos, 0x7cc);
    CLASS_PARAM(bool, isShip, 0x770);
    CLASS_PARAM(bool, isUpsideDown, 0x776);
    CLASS_PARAM(float, vSize, 0x77c);
    CLASS_PARAM(bool, isHolding, 0x745);
    CLASS_PARAM(bool, hasJustHeld, 0x746);
    CLASS_PARAM(bool, onGround, 0x778);
    CLASS_PARAM(bool, isSliding, 0x7a0);
    CLASS_PARAM(bool, dead, 0x777);
    CLASS_PARAM(double, speed, 0x608);
};

class GJBaseGameLayer : public cocos2d::CCLayer, public GDObj {
public:
    void spawnGroup(int group);
    CLASS_PARAM(cocos2d::CCArray*, objects, 0x3a0);
    CLASS_PARAM(PlayerObject*, player1, 0x380);
    CLASS_PARAM(PlayerObject*, player2, 0x388);
    CLASS_PARAM(LevelSettingsObject*, levelSettings, 0x390);
    CLASS_PARAM(cocos2d::CCDictionary*, unknownDict, 0x398);
};

class LevelEditorLayer : public GJBaseGameLayer {
public:
    void createObjectsFromString(std::string st, bool idk);
    void removeAllObjects();
    void undoLastAction();
    void redoLastAction();
    CLASS_PARAM(cocos2d::CCArray*, objects, 0x3a0);
};

class EditButtonBar : public cocos2d::CCNode, public GDObj {
 public:
    void loadFromItems(cocos2d::CCArray*, int, int, bool);
    CLASS_PARAM(cocos2d::CCArray*, objectSlots, 0x130);
};
class EditorUI : public cocos2d::CCLayer, public GDObj {
public:
    void pasteObjects(std::string str);
    void undoLastAction();
    void redoLastAction();
    void updateButtons();
    CCMenuItemSpriteExtra* getCreateBtn(int obid, int four);
    cocos2d::CCArray* getSelectedObjects();
    void selectObjects(cocos2d::CCArray* objs, bool keep);
    CLASS_PARAM(LevelEditorLayer*, editorLayer, 0x408);
    CLASS_PARAM(std::string, clipboard, 0x458);
    CLASS_PARAM(cocos2d::CCArray*, editBars, 0x358);
};

class UILayer : public cocos2d::CCLayer {
 public:
    void toggleCheckpointsMenu(bool b);
};


class PlayLayer : public GJBaseGameLayer {
public:
    static PlayLayer* create(GJGameLevel* l);
    static void switchToScene(GJGameLevel* l);
    void resetLevel();
    virtual void update(float);
    void startMusic();
    void togglePracticeMode(bool p);

    CLASS_PARAM(bool, gameStarted, 0x4dc);
    CLASS_PARAM(float, length, 0x5f8);
    CLASS_PARAM(UILayer*, uiLayer, 0x708);
    CLASS_PARAM(int, attempt, 0x754);
    CLASS_PARAM(bool, practiceMode, 0x721);
    CLASS_PARAM(float, time, 0x760);
    STRUCT_PARAM(GameModes, gameModes, 0x76f);
};

class ObjectToolbox : public GDObj {
public:
    static ObjectToolbox* sharedState();
    char const* intKeyToFrame(int key);
    CLASS_PARAM(cocos2d::CCDictionary*, strKeyObjects, 0x120);
    CLASS_PARAM(cocos2d::CCDictionary*, intKeyObjects, 0x128);
};

class ButtonSprite : public cocos2d::CCSprite, public GDObj {
public:
    static ButtonSprite* create(char const* text, int width, int relativeWidth, float scale, bool relative);
    static ButtonSprite* create(char const* text);
};

class FLAlertLayer : public cocos2d::CCLayerColor, public GDObj {
public:
    FLAlertLayer();
    static FLAlertLayer* create(void* fdsg, char const* x, const std::string &thing, char const* l, char const* u, float f);
    
    static FLAlertLayer* create(char const* title, const std::string &desc, char const* btn) {
        return FLAlertLayer::create(NULL, title, desc, btn, NULL, 300.0);
    }
    virtual ~FLAlertLayer();
    virtual void onEnter(void);
    virtual bool ccTouchBegan(cocos2d::CCTouch *cct,cocos2d::CCEvent *cce) {return true;};
    virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void registerWithTouchDispatcher(void);
    virtual void keyBackClicked(void);
    virtual void keyDown(cocos2d::enumKeyCodes);
    virtual void show(void);
    CLASS_PARAM(cocos2d::CCLayer*, mainLayer, 0x220);
    CLASS_PARAM(cocos2d::CCMenu*, mainMenu, 0x1f8);

    cocos2d::CCMenu* m_buttonMenu; // 0x1f8
    int m_controlConnected; // 0x200
    void* m_alertProtocol; // 0x208
    cocos2d::CCNode* m_scene; // 0x210
    bool m_reverseKeyBack; // 0x211
    cocos2d::ccColor3B m_color; // 0x212
    cocos2d::CCLayer* m_mainLayer;
    int m_ZOrder;
    bool m_noElasticity;
    cocos2d::ccColor3B m_color2;
    ButtonSprite* m_button1;
    ButtonSprite* m_button2;
    CCLayerColor* m_scrollingLayer;
    int m_joystickConnected;
    bool m_containsBorder;
    bool m_noAction;
};


class TextArea : public ButtonSprite {
public:
    static TextArea* create(std::string a, char const*, float textSize, float maxWidth, cocos2d::CCPoint position, float returnSize, bool idk);
};


class MenuLayer : public cocos2d::CCLayer, public GDObj {
public:
    void onQuit(cocos2d::CCObject*);
    void keyBackClicked();
};
class MoreVideoOptionsLayer : public FLAlertLayer {
public:
    static MoreVideoOptionsLayer* create();
    bool init();

};

class GJSearchObject : public GDObj {
public:
    static GJSearchObject* create(int, std::string, std::string, std::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
};

class LevelBrowserLayer : public GDObj {
public:
    static cocos2d::CCScene* scene(GJSearchObject* search);
};

class GJRotationControl : public cocos2d::CCLayer, public GDObj {
public:
    CLASS_PARAM(float, rotation, 0x18c);
    
};

class EditorPauseLayer : public GDObj {
public:
    static EditorPauseLayer* create(LevelEditorLayer* editor);
    void saveLevel();
    virtual ~EditorPauseLayer();
};

class PauseLayer : public FLAlertLayer {
    
};

class GJAccountManager : public GDObj {
public:
    static GJAccountManager* sharedState();

    CLASS_PARAM(char const*, password, 0x128);
    CLASS_PARAM(char const*, username, 0x130);
};

class GameManager : public cocos2d::CCNode, public GDObj {
public:
    static GameManager* sharedState();
    bool getGameVariable(char const* var);
    void setGameVariable(char const* var, bool val);
    void fadeInMusic(char const* ye);
    void reloadAllStep5();
    void doQuickSave();
    void reloadAll(bool a, bool b, bool c);
    void accountStatusChanged();
    void load();

    void setSecondColorIdx(int idx);
    void setFirstColorIdx(int idx);
    cocos2d::_ccColor3B const& colorForIdx(int idx);
    std::string& manFile();
    virtual ~GameManager();

    CLASS_PARAM(PlayLayer*, playLayer, 0x180);
    CLASS_PARAM(LevelEditorLayer*, editorLayer, 0x188);
    CLASS_PARAM(int, scene, 0x1f4);
};

class InfoLayer : public cocos2d::CCLayer, public GDObj {
public:
    void loadPage(int type, bool yes);
    void onRefreshComments(cocos2d::CCObject*);
    void onClose(cocos2d::CCObject*);
    CLASS_PARAM(cocos2d::CCLayer*, mainLayer, 0x220);
    CLASS_PARAM(cocos2d::CCMenu*, rightMenu, 0x1f8);
};

class CCMenuItemToggler : public cocos2d::CCNodeRGBA, public GDObj {
public:
    static CCMenuItemToggler* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, void (cocos2d::CCObject::*)(cocos2d::CCObject*));
    void toggle(bool t);
    void setSizeMult(float);
};

class CCTextInputNode : public cocos2d::CCNode, public cocos2d::CCIMEDelegate, public GDObj {
public:
    static CCTextInputNode* create(float x, float y, char const* placeholder, char const* font, int, char const*);
    void setAllowedChars(std::string allowed);
    void setMaxLabelScale(float max);
    void setMaxLabelWidth(float max);
    std::string getString();
    char const* getString_s(); // modification, spooky
    void setString(std::string update);
};

class GJEffectManager : public cocos2d::CCNode, public GDObj {
 public:
    CLASS_PARAM(GJBaseGameLayer*, gameLayer, 0x120);
    CLASS_PARAM(cocos2d::CCArray*, spawnActions, 0x1c18);
};

class SetupSpawnPopup : public FLAlertLayer {
 public:
    CLASS_PARAM(EffectGameObject*, object, 0x258);
    CLASS_PARAM(CCTextInputNode*, numInput, 0x290);
    void createToggleButton(std::string name, void (cocos2d::CCObject::*)(cocos2d::CCObject*), bool enabled, cocos2d::CCMenu* menu, cocos2d::CCPoint location, cocos2d::CCArray* idk);
    virtual ~SetupSpawnPopup();
    virtual void keyBackClicked();
    virtual void show();
    virtual void textInputClosed(CCTextInputNode*);
    virtual void textChanged(CCTextInputNode*);
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);
};
#endif
