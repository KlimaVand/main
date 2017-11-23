#import "UIBarButtonItem+Euroblight.h"

@implementation UIBarButtonItem (Euroblight)

+ (UIBarButtonItem *)createBarButtonFromImageWithName:(NSString *)imageName target:(id)target action:(SEL)action {
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    UIImage *image = [UIImage imageNamed:imageName];
    button.frame = CGRectMake(0, 0, image.size.width, image.size.height);
    [button setBackgroundImage:image forState:UIControlStateNormal];
    [button addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    button.showsTouchWhenHighlighted = YES;
    return [[UIBarButtonItem alloc] initWithCustomView:button];
}

@end
