#include <type_traits>
#include <concepts>

// Include GD and cocos2d classes before this

template <typename T, typename B, typename F, typename ...Args>
concept function_checks = std::derived_from<T, B> && requires(F a, Args... args) {
	a(args...);
};

#define needs_base(x) std::derived_from<x> = T
#define needs_same(x) std::same_as<x> = T

#define setter(base, name, oldname, ...) \
	template <typename ...Args> requires function_checks<T, base, void(__VA_ARGS__), Args...> \
	Build<T> name(Args... args) {\
		m_item->oldname(args...);\
		return *this;\
	}

template <typename T> requires (std::derived_from<T, CCObject>)
class Build {
	T* m_item;
 public:

 	Build<T> store(T*& in) {
 		in = m_item;
 	}

 	template <typename ...Args> requires requires(Args... args) {
 		T::create(args...);
 	}
 	static Build<T> create(Args... args) {
 		return Build(T::create(args...));
 	} 

	Build(T* item) : m_item(item) {}

	T* collect() {return m_item;}

	template <typename U> requires std::derived_from<T, U>
	operator U*() { return m_item; }

	// CCArray

	setter(CCArray, addItem, addObject, CCObject*);
	setter(CCArray, insertItemAt, insertObject, CCObject*, unsigned int)
	setter(CCArray, removeObject, removeObject, CCObject*)
	setter(CCArray, removeObjectAt, removeObjectAtIndex, unsigned int)
	setter(CCArray, reverse, reverseObjects, void)

	template <needs_base(CCArray), typename U>
	Build<U> intoItemAt(unsigned int index) {
		return Build<U>(m_item->objectAtIndex(index));
	}

	// CCNode
	setter(CCNode, pos, setPosition, CCPoint const&)
	setter(CCNode, pos, setPosition, float, float)
	setter(CCNode, posX, setPositionX, float)
	setter(CCNode, posY, setPositionY, float)
	setter(CCNode, scale, setScale, float)
	setter(CCNode, scaleX, setScaleX, float)
	setter(CCNode, scaleY, setScaleY, float)
	setter(CCNode, anchorPoint, setAnchorPoint, CCPoint const&)
	setter(CCNode, zOrder, setZOrder, int)
	setter(CCNode, skewX, setSkewX, float)
	setter(CCNode, skewY, setSkewY, float)
	setter(CCNode, contentSize, setContentSize, CCSize const&)
	setter(CCNode, visible, setVisible, bool)
	setter(CCNode, rotation, setRotation, float)
	setter(CCNode, child, addChild, CCNode*)
	setter(CCNode, parent, setParent, CCNode*)
	setter(CCNode, userData, setUserData, void*)
	setter(CCNode, userObject, setUserObject, CCObject*)

	template <needs_base(CCNode), typename U>
	Build<U> intoParent() {
		return Build(static_cast<U*>(m_item->getParent()));
	}

	template <needs_base(CCNode), typename U>
	Build<U> intoChild(unsigned int idx) {
		return Build(static_cast<U*>(m_item->getChildren()->objectAtIndex(idx)));
	}

	template <needs_base(CCNode), typename U>
	Build<U> intoChildByTag(int tag) {
		return Build(static_cast<U*>(m_item->getChildByTag(tag)));
	}

	template <needs_base(CCNode), typename U>
	Build<U> intoNewChild(U* newChild, int idx = 0, int tag = 0) {
		m_item->addChild(newChild, idx, tag);
		return Build<U>(newChild);
	}

	template <needs_base(CCNode), typename U>
	Build<U> intoNewSibling(U* newSibling, int idx = 0, int tag = 0) {
		return intoParent().intoNewChild(newSibling, idx, tag);
	}

	template <needs_base(CCNode), typename U>
	Build<U> intoNewParent(U* newParent) {
		m_item->setParent(newParent);
		return Build<U>(newParent);
	}

	template <needs_base(CCNode)>
	Build<T> contentSize(float w, float h) {
		return contentSize(CCSize(w, h));
	}

	template <needs_base(CCNode)>
	Build<T> width(float w) {
		return contentSize(w, m_item->getContentSize().height);
	}

	template <needs_base(CCNode)>
	Build<T> height(float h) {
		return contentSize(m_item->getContentSize().width, h);
	}

	template <needs_base(CCNode)>
	Build<T> anchorPoint(float x, float y) {
		return anchorPoint(ccp(x, y));
	}

	// CCRGBAProtocol
	setter(CCRGBAProtocol, opacity, setOpacity, unsigned char)
	setter(CCRGBAProtocol, color, setColor, const ccColor3B&)
	setter(CCRGBAProtocol, cascadeOpacity, setCascadeOpacityEnabled, bool)
	setter(CCRGBAProtocol, cascadeColor, setCascadeColorEnabled, bool)

	template <needs_base(CCRGBAProtocol)>
	Build<T> color(float r, float g, float b) {
		return color(ccc3(r, g, b));
	}

	// CCLayer
	setter(CCLayer, registerTouchDispatcher, registerWithTouchDispatcher, void)
	setter(CCLayer, touchEnabled, setTouchEnabled, bool)
	setter(CCLayer, keyboardEnabled, setKeyboardEnabled, bool)
	setter(CCLayer, mouseEnabled, setMouseEnabled, bool)
	setter(CCLayer, touchMode, setTouchMode, ccTouchesMode)
	setter(CCLayer, touchPrio, setTouchPriority, bool)
	setter(CCLayer, keypad_enabled, setKeypadEnabled, bool)

	template <needs_base(CCLayer)>
	Build<T> initTouch() {
		registerTouchDispatcher();
		CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
		return touchEnabled(true).mouseEnabled(true);
	}

	// CCMenu
	setter(CCMenu, enabled, setEnabled, bool)

	// CCMenuItem
	setter(CCMenuItem, enabled, setEnabled, bool)
	setter(CCMenuItem, target, setTarget, CCObject*, SEL_MenuHandler)

	// CCMenuItemSpriteExtra
	setter(CCMenuItemSpriteExtra, sizeMult, setSizeMult, float)

	// CCSprite
	template <needs_same(CCSprite)>
	static Build<T> createSpriteName(char const* frame) {
		return Build(CCSprite::createWithSpriteFrameName(frame));
	}

	setter(CCSprite, texture, setTexture, CCTexture2D*)
	setter(CCSprite, childColor, setChildColor, CCSprite*)
	setter(CCSprite, childOpacity, setChildOpacity, unsigned char)
	setter(CCSprite, textureRect, setTextureRect, const CCRect&)
	setter(CCSprite, displayFrame, setDisplayFrame, CCSpriteFrame*)
	setter(CCSprite, dirty, setDirty, bool)
	setter(CCSprite, flipX, setFlipX, bool)
	setter(CCSprite, flipY, setFlipY, bool)

	template <needs_base(CCSprite)>
	Build<CCMenuItemSpriteExtra> intoMenuItem(CCObject* target, SEL_MenuHandler selector) {
		return Build<CCMenuItemSpriteExtra>::create(m_item, m_item, target, selector);
	}

	// CCLabelProtocol
	setter(CCLabelProtocol, string, setString, const char*)

	// CCLabelBMFont
	setter(CCLabelBMFont, alignment, setAlignment, CCTextAlignment)
	setter(CCLabelBMFont, limitLabelWidth, limitLabelWidth, float, float, float)

	// CCTextInputNode
	setter(CCTextInputNode, labelColor, setLabelNormalColor, ccColor3B const&)
	setter(CCTextInputNode, placeholderColor, setLabelPlaceholderColor, ccColor3B const&)
	setter(CCTextInputNode, placeholderScale, setLabelPlaceholderScale, float)
	setter(CCTextInputNode, maxScale, setMaxLabelScale, float)
	setter(CCTextInputNode, maxWidth, setMaxLabelWidth, float)
	setter(CCTextInputNode, allowedChars, setAllowedChars, std::string const&)
	setter(CCTextInputNode, string, setString, std::string const&)
	setter(CCTextInputNode, delegate, setDelegate, TextInputDelegate*)
	setter(CCTextInputNode, trackOnClick, onClickTrackNode, bool)

	// CCScale9Sprite
	setter(CCScale9Sprite, preferredSize, setPreferredSize, CCSize const&)
	setter(CCScale9Sprite, capInsets, setCapInsets, CCRect const&)

	// Slider
	setter(Slider, value, setValue, float)
	setter(Slider, barVisible, setBarVisibility, bool)

	// ButtonSprite
	setter(ButtonSprite, string, setString, char const*)


	/// Actions

	// CCAction
	setter(CCAction, target, setTarget, CCNode*)
	setter(CCAction, tag, setTag, int)
	setter(CCAction, speedMod, setSpeedMod, float)

	// CCFollow
	setter(CCFollow, boundarySet, setBoundarySet, bool)

	// CCFiniteTimeAction
	setter(CCFiniteTimeAction, duration, setDuration, float)

	template <needs_base(CCFiniteTimeAction)>
	Build<T> reverse() {
		m_item->reverse();
		return *this;
	}

	// CCCallFunc
	setter(CCCallFunc, target, setTargetCallback, CCObject*)

	// CCCallFuncO
	setter(CCCallFuncO, object, setObject, CCObject*)

	// CCActionInterval

	template <needs_base(CCActionInterval)>
	Build<CCRepeat> repeat(int times) {
		return Build<CCRepeat>::create(m_item, times);
	}

	template <needs_base(CCActionInterval)>
	Build<CCRepeatForever> repeatForever() {
		return Build<CCRepeatForever>::create(m_item);
	}

	template <needs_base(CCActionInterval)>
	Build<CCSequence> sequence(CCActionInterval* action) {
		return Build(CCSequence::createWithTwoActions(m_item, action));
	}

	template <needs_base(CCActionInterval)>
	Build<CCRepeatForever> wait_after(float d) {
		return sequence(m_item, Build<CCDelayTime>::create(d));
	}

	template <needs_base(CCActionInterval)>
	Build<CCRepeatForever> wait_before(float d) {
		return sequence(Build<CCDelayTime>::create(d), m_item);
	}

	// CCEaseRateAction
	setter(CCEaseRateAction, easeRate, setRate, float)
};

#undef setter
#undef needs_base
#undef needs_same
