#import "MainTableViewCell.h"

@interface MainTableViewCell()

@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (weak, nonatomic) IBOutlet UIImageView *iconImageView;
@property (weak, nonatomic) IBOutlet UILabel *nbrOfUnsentReportsLabel;

@end

@implementation MainTableViewCell

- (void)setIconImage:(UIImage *)image {
    if(image != nil) {
        self.iconImageView.image = image;
    } else {
        //TODO add defualt Image;
    }
}

- (void)setTitle:(NSString *)title {
    self.titleLabel.text = title;
}

- (void)setNbrOfUnsentReports:(NSNumber *)nbrOfUnsentReports {
    if(nbrOfUnsentReports != nil) {
        self.nbrOfUnsentReportsLabel.text = [NSString stringWithFormat:@"( %@ )", nbrOfUnsentReports];
        return;
    }
    self.nbrOfUnsentReportsLabel.text = @"";
}


@end
