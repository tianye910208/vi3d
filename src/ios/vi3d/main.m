//
//  main.m
//  vi3d
//
//  Created by tianye on 2017/4/21.
//  Copyright © 2017年 viware. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <pthread.h>
#import "vi3d.h"


void* _main(void* args)
{
    while(true){
        vi_log("hello");
        usleep(1000000);
    }
    return NULL;
}




@interface AppWindow : UIWindow

@end

@implementation AppWindow { }

- (void)setNeedsDisplay
{
    //need refresh
}

- (void)setNeedsDisplayInRect:(CGRect)rect
{
    //need refresh
}

+(Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

@end





@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) AppWindow *window;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    CGRect rect = UIScreen.mainScreen.bounds;
    
    self.window = [[AppWindow alloc] initWithFrame:rect];
    self.window.backgroundColor = [UIColor whiteColor];
    
    self.window.rootViewController = [[UIViewController alloc] init];
    [self.window.rootViewController prefersStatusBarHidden];
    
    [self.window setScreen:UIScreen.mainScreen];
    [self.window makeKeyAndVisible];
    
    vi_app_set_screen_size(rect.size.width, rect.size.height);
    
    pthread_t tid;
    pthread_create(&tid, 0, &_main, 0);
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end



int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
