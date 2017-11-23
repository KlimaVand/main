#import "AppDelegate.h"
#import "UserCredentialsTableViewController.h"
#import "UIColor+Euroblight.h"
#import "ReachabilityManager.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [ReachabilityManager sharedManager];
    [self addCacheSettings];
    self.window.backgroundColor = [UIColor whiteColor];
    self.window.tintColor = [UIColor applicationTintColor];
    [[UIBarButtonItem appearance] setTintColor:[UIColor applicationTintColor]];
    return YES;
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
}

#pragma mark - Private Methods

- (void)addCacheSettings {
    int cacheSizeMemory = 2*1024*1024; // 2MB
    int cacheSizeDisk = 16*1024*1024; // 16MB
    NSURLCache *sharedCache = [[NSURLCache alloc] initWithMemoryCapacity:cacheSizeMemory diskCapacity:cacheSizeDisk diskPath:@"nsurlcache"];
    [NSURLCache setSharedURLCache:sharedCache];
}

@end
