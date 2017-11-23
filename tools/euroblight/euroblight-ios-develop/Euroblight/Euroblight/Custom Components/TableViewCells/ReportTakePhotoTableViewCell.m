#import "ReportTakePhotoTableViewCell.h"

@interface ReportTakePhotoTableViewCell()

@property (weak, nonatomic) IBOutlet UIImageView *customImageView;

@end

@implementation ReportTakePhotoTableViewCell

- (void)setCustomImage:(UIImage *)image {
    if (image == nil) {
        self.customImageView.image = [UIImage imageNamed:@"PhotoPlaceHolder"];
        return;
    }
    self.customImageView.image = image;
}

@end
