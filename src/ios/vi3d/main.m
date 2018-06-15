//
//  main.m
//  vi3d
//
//  Created by tianye on 2017/4/21.
//  Copyright © 2017年 viware. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <pthread.h>
#import <sys/time.h>
#import "vi3d.h"


int actived = 0;

struct timeval t1, t2;

vi_msg* msg = NULL;


@interface OpenGLView : UIView

@end

@implementation OpenGLView

+ (Class)layerClass {
	return [CAEAGLLayer class];
}


- (void)setNeedsDisplay {
    //need refresh
}

- (void)setNeedsDisplayInRect:(CGRect)rect {
    //need refresh
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
    if((bool)(msg = vi_msg_push(VI_MSG_TOUCH_DOWN, 3))) {
        msg->data[0] = point.x;
        msg->data[1] = point.y;
        msg->data[2] = 0;
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
    if((bool)(msg = vi_msg_push(VI_MSG_TOUCH_MOVE, 3))) {
        msg->data[0] = point.x;
        msg->data[1] = point.y;
        msg->data[2] = 0;
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
    if((bool)(msg = vi_msg_push(VI_MSG_TOUCH_UP, 3))) {
        msg->data[0] = point.x;
        msg->data[1] = point.y;
        msg->data[2] = 0;
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
    if((bool)(msg = vi_msg_push(VI_MSG_TOUCH_UP, 3))) {
        msg->data[0] = point.x;
        msg->data[1] = point.y;
        msg->data[2] = 0;
    }
}

@end





@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    CGRect rect = UIScreen.mainScreen.bounds;

    self.window = [[UIWindow alloc] initWithFrame:rect];
    [self.window  setScreen:UIScreen.mainScreen];


    OpenGLView* eglview = [[OpenGLView alloc] initWithFrame:[self.window bounds]];
    eglview.layer.opaque = YES;
	
    UIViewController* viewctl = [[UIViewController alloc] init];
	[viewctl setView:eglview];
    [viewctl prefersStatusBarHidden];

    [self.window setRootViewController:viewctl];
	[self.window makeKeyAndVisible];

    

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    const char* savedir = [[paths objectAtIndex:0] UTF8String];
    const char* datadir = [[[NSBundle mainBundle] resourcePath] UTF8String];

    vi_app_set_screen_size(rect.size.width, rect.size.height);

    if(vi_app_init(eglview.layer, eglview.frame.size, datadir, savedir)) return NO;
	if(vi_app_main()) return NO;

    actived = 1;

    gettimeofday(&t1, NULL);
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(mainloop:)];
	[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    return YES;
}


- (void)mainloop:(CADisplayLink*)displayLink {
    gettimeofday(&t2, NULL);
    float dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
    t1 = t2;
    if(actived && eaglContext)
        vi_app_loop(dt);
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
    vi_app_exit();
    actived = 0;
}

@end



int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
