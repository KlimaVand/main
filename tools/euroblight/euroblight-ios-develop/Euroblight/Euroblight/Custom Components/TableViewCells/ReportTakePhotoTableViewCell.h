#import <UIKit/UIKit.h>
#import "InvalidInputIndicatorTableViewCell.h"

@interface ReportTakePhotoTableViewCell : InvalidInputIndicatorTableViewCell

@property (weak, nonatomic) IBOutlet UIButton *button;

- (void)setCustomImage:(UIImage *)image;

@end
