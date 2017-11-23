#import "SaveButtonTableViewCell.h"
#import "UIImage+EuroBlight.h"
#import "UIColor+Euroblight.h"

@implementation SaveButtonTableViewCell

- (void)awakeFromNib {
    [self.saveButton setBackgroundImage:[UIImage imageWithColor:[UIColor applicationTintColor]] forState:UIControlStateNormal];
}

@end
