#import <UIKit/UIKit.h>
#import "InvalidInputIndicatorTableViewCell.h"

@interface ReportLocationTableViewCell : InvalidInputIndicatorTableViewCell

@property (weak, nonatomic) IBOutlet UILabel *latitudeValueLabel;
@property (weak, nonatomic) IBOutlet UILabel *longitudeValueLabel;
@property (weak, nonatomic) IBOutlet UIButton *button;
@property (weak, nonatomic) IBOutlet UILabel *latitudeLabel;
@property (weak, nonatomic) IBOutlet UILabel *longitudeLabel;

@end
