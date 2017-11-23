#import "UILabel+Euroblight.h"

@implementation UILabel (Euroblight)

+ (UILabel *)pickerLabel {
    UILabel *pickerLabel = [[UILabel alloc] init];
    pickerLabel.backgroundColor = [UIColor clearColor];
    pickerLabel.adjustsFontSizeToFitWidth = YES;
    pickerLabel.font= [UIFont boldSystemFontOfSize:20];
    [pickerLabel setMinimumScaleFactor:8/pickerLabel.font.pointSize];
    pickerLabel.textColor = [UIColor blackColor];
    pickerLabel.textAlignment = NSTextAlignmentCenter;
    pickerLabel.lineBreakMode = NSLineBreakByTruncatingTail;
    return pickerLabel;
}
@end
