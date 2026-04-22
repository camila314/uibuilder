# UIBuilder

Header-only UI Builder class for Geometry Dash. Supports any modding library as long as it has all of the functions defined. Can be easily integrated via CPM if you wish.

## Tutorial for Geode

Adding UIBuilder to your Geode project is simple using CPM:

```cmake
CPMAddPackage("gh:camila314/UIBuilder#main")
target_link_libraries(${PROJECT_NAME} UIBuilder)
```

UIBuilder works via the `Build` class, which can wrap any CCObject subclass. Chainable helper functions are added depending on the object's class, for example a CCNode subclass has access to the `move` and `scale` functions:

```cpp
Build(someNode)
	.center().move(30, 0)
	.scale(0.5)
	.id("my-id");
```

Specialized functions are added for specific types, such as `color`for CCRGBAProtocol objects or `initTouch` for CCLayer objects. The `Build` wrapper also has shorthands for creating new objects:

```cpp
Build<CCLabelBMFont>::create("Hello", "bigFont.fnt")
	.color(255, 0, 0);
```

Some classes have custom creator functions built-in, such as CCSprite and CCMenuItemToggler:

```cpp
// shorthand for Build(CCSprite::createWithSpriteFrameName(_));
Build<CCSprite>::createSpriteName("GJ_lock_001.png");

// creates a toggle with default checkboxes
Build<CCMenuItemToggler>::createToggle([](auto) { log::info("toggled!"); });
```

UIBuilder has some functions for dealing with children and node hierarchy. Here's some examples:

```cpp
auto node = Build<CCNode>::create();

// Equivalent to node->addChild(_) but much cleaner
Build<CCLabelBMFont>::create("Hello", "bigFont.fnt")
	.parent(node);

// Equivalent to multiple addChild calls.
node.children(
	Build<CCSprite>::createSpriteName("GJ_lock_001.png")
		.rotation(90),
	Build<CCLabelBMFont>::create("Hi!", "bigFont.fnt")
		.scale(0.8)
		.move(4, 0));
```

Any UIBuilder function that starts with `into` allows you to move between objects without escaping the function chain. You can use this to converts sprites to buttons, move between parent-child, and more:

```cpp
Build(exampleLayer)
	.intoScene() // returns Build<CCScene>
	.pushScene();

Build<ButtonSprite>::create("Hey", "bigFont.fnt", "GJ_button_01.png")
	.intoMenuItem([] { log::info("Button Pressed!!"); }) // returns Build<CCMenuItemSpriteExtra>
	.pos(40, 20)
	.intoNewParent(CCMenu::create()) // returns Build<CCMenu>
	.parent(exampleLayer);

// Returns Build<CCSprite>. This does NOT perform a runtime check! 
Build(playLayer)
	.intoChildRecurseID<CCSprite>("progress-bar")
	.visible(false);
```

UIBuilder has built-in support for Geode Layouts. Here's an example of it in use:

```cpp
Build<CCMenu>::create()
	.layout(
		// Populates align and crossAlign automatically
		Build<RowLayout>::createWithAligns(AxisAlignment::Between, AxisAlignment::Center)
			.autoScale(false)
			.reverse(true))
	.contentSize(400, 40)
	.children(
		Build<ButtonSprite>::create("Hello", "bigFont.fnt", "GJ_button_04.png")
			// Layout Options
			.layoutOpts(Build<AxisLayoutOptions>::create()
				.nextGap(10.0)
				.prevGap(5.0)),
		Build<ButtonSprite>::create("Goodbye", "bigFont.fnt", "GJ_button_04.png"))
	.updateLayout();

auto layer = Build<CCLayer>::create()
	.layout(AnchorLayout::create())
	.child(
		Build<CCSprite>::createSpriteName("checkpoint_01_001.png")
			// Creates AnchorLayoutOptions and sets both anchor and offset
			.setAnchorOpts(Anchor::Center, ccp(5, 0)))
	.updateLayout();

Build<CCLabelBMFont>::create("Wow!", "goldFont.fnt")
	.anchorPoint({ 0, 0.5 })
	// Equivalent to layer->addChildAtPosition
	.parentAtPos(layer, Anchor::Top);
```

These builder statements can start to get pretty nested with complex UI. If you need to store certain objects into variables, UIBuilder has a solution that lets you keep everything in one statement using `store`:

```cpp
CCSprite* diamond;
CCSprite* star;

Build<CCNode>::create()
	.layout(AnchorLayout::create())
	.child(
		Build<CCNode>::create()
			.setAnchorOpts(Anchor::Bottom)
			.layout(RowLayout::create())
			.children(
				Build<CCSprite>::createSpriteName("GJ_diamondsIcon_001.png")
					.scale(0.5)
					// Stores in diamond
					.store(diamond),
				Build<CCSprite>::createSpriteName("GJ_starsIcon_001.png")
					.scale(0.7)
					// Stores in star
					.store(star)));
```

UIBuilder supports implicit casting of `Build<T>` into `T*`, meaning you can use these wrappers within functions that take normal cocos2d objects with ease. If you need explicit casting you can use `collect()`. You are also able to use `operator->` if you need to call specific functions not in UIBuilder:

```cpp
// Both of these work:

Build(PlayLayer::get())
	.scale(0.5)
	.collect()
	->pauseGame();

Build(PlayLayer::get())
	.scale(1.0)
	->resume();
```

There is much more UIBuilder can do such as scheduling, CCActions, CCArrays, iterating children, and many more functions. Peruse the source code to find all sorts of functions! If there's a helper function you think I missed, message me and I may add it.

## Example Projects that use UIBuilder

- [Icon Profile by Capeling](https://github.com/Capeling/icon-profile-geode/)
- [UIDesigner by Me](https://github.com/camila314/UIDesigner)
- [BetterMenuMod by MuhXd](https://github.com/MuhXd/BetterMenuMod)
