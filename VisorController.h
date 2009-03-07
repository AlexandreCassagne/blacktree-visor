//
//  VisorController.h
//  Visor
//
//  Created by Nicholas Jitkoff on 6/1/06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NDHotKeyEvent.h"
@class TermController;
@class TermDefaults;
@class TTProfileManager;
@interface VisorController : NSObject {
    NSStatusItem *statusItem;
    TermController *controller;
    NSWindow *backgroundWindow;
    IBOutlet NSWindow *prefsWindow;
    IBOutlet NSMenu *statusMenu;
    IBOutlet NSWindow *aboutWindow;
    NDHotKeyEvent *hotkey;
    NDHotKeyEvent *escapeKey;
    NSString* previouslyActiveApp;
    BOOL hidden;
    float lastSlideOffset;
}
- (TermController *)controller;
- (void)setController:(TermController *)value;
- (IBAction)showPrefs:(id)sender;
- (IBAction)toggleVisor:(id)sender;
- (IBAction)setHotKey:(id)sender;
- (IBAction)chooseFile:(id)sender;
- (IBAction)showAboutBox:(id)sender;
- (void)hideWindow;
- (void)showWindow;
- (void)enableHotKey;
- (void)initEscapeKey;
- (void)maybeEnableEscapeKey:(BOOL)enable;
- (void)activateStatusMenu;
- (NSWindow *)backgroundWindow;
- (void) setBackgroundWindow: (NSWindow *) newBackgroundWindow;
- (void)saveDefaults;
- (IBAction)inspect:(id)sender;
@end
