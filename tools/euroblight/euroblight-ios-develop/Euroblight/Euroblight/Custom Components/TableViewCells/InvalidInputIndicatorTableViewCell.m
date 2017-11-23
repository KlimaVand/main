#import "InvalidInputIndicatorTableViewCell.h"
#import "UIColor+Euroblight.h"

@implementation InvalidInputIndicatorTableViewCell

- (void)setValidInput:(BOOL)validInput {
    if (validInput) {
        self.backgroundColor = [UIColor whiteColor];
    }
    else {
        self.backgroundColor = [UIColor cellInvalidInputColor];
    }
}

@end
