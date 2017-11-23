#import "UnsentReportsTableViewController.h"
#import "DataManager.h"
#import "Report.h"
#import "UnsentReportTableViewCell.h"
#import "APIUploadReportHandler.h"
#import "MBProgressHUD.h"
#import "ReachabilityManager.h"

@interface UnsentReportsTableViewController ()

@property (nonatomic, strong) NSDateFormatter *dateFormatter;
@property (nonatomic, strong) NSMutableArray *apiUploadReportHandlers;
@property (nonatomic, strong) MBProgressHUD *progressHud;
@property (nonatomic, strong) UIBarButtonItem *clearBarButtonItem;
@property (nonatomic, strong) UIBarButtonItem *sendBarButtonItem;

@property (strong, nonatomic) UIAlertView *alertView;

@end

typedef NS_ENUM(NSInteger, UnsentReportsVCAlertViewTags) {
    UnsentReportsVCAlertViewTagClear,
    UnsentReportsVCAlertViewTagUnauthorized,
    UnsentReportsVCAlertViewTagSend,
};

@implementation UnsentReportsTableViewController

#pragma mark - View Life Cycle Methods

- (void)viewDidLoad {
    self.apiUploadReportHandlers = [[NSMutableArray alloc] init];
    self.title = NSLocalizedString(@"Unsent Reports", @"");
    [self initDateFormatter];
    [self initBarButtons];
}

#pragma mark - Private Methods

- (void)initDateFormatter {
    self.dateFormatter = [[NSDateFormatter alloc] init];
    [self.dateFormatter setDateStyle:NSDateFormatterLongStyle];
    [self.dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
}

- (void)initBarButtons {
    self.clearBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Clear", @"") style:UIBarButtonItemStyleBordered target:self action:@selector(clearBarButtonItemTapped:)];
    self.sendBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Send", @"") style:UIBarButtonItemStyleBordered target:self action:@selector(sendBarButtonItemTapped:)];
    
    UIBarButtonItem *fixSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    fixSpace.width = 15;
    self.navigationItem.rightBarButtonItems = @[self.sendBarButtonItem,fixSpace, self.clearBarButtonItem];
    [self updateBarButtons];
}

- (void)showSuccessProgressHud {
    if(self.progressHud == nil) {
        self.progressHud = [MBProgressHUD showHUDAddedTo:self.view.window animated:YES];
    }
    self.progressHud.customView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"CheckmarkIcon"]];
    self.progressHud.mode = MBProgressHUDModeCustomView;
    self.progressHud.labelText = NSLocalizedString(@"Reports sent", @"");
    [self.progressHud show:YES];
    [self.progressHud hide:YES afterDelay:2];
}

- (void)showFailedProgressHud {
    if(self.progressHud == nil) {
        self.progressHud = [MBProgressHUD showHUDAddedTo:self.view.window animated:YES];
    }
    self.progressHud.mode = MBProgressHUDModeCustomView;
    self.progressHud.labelText = NSLocalizedString(@"Failed to send some reports!", @"");
    [self.progressHud show:YES];
    [self.progressHud hide:YES afterDelay:2];
}

- (void)showProgressHud {
    if(self.progressHud == nil) {
        self.progressHud = [MBProgressHUD showHUDAddedTo:self.view.window animated:YES];
    }
    self.progressHud.mode = MBProgressHUDModeIndeterminate;
    self.progressHud.labelText = NSLocalizedString(@"Sending reports", @"");
    [self.progressHud show:YES];
}

- (void)updateBarButtons {
    if([DataManager sharedManager].storedReports.count == 0) {
        self.clearBarButtonItem.enabled = NO;
        self.sendBarButtonItem.enabled = NO;
    } else {
        self.clearBarButtonItem.enabled = YES;
        self.sendBarButtonItem.enabled = YES;
    }
}

#pragma mark - Action Methods

- (void)clearBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    self.alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Clear reports", @"") message:NSLocalizedString(@"Are you shure you want to clear all reports?", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"No", @"") otherButtonTitles:NSLocalizedString(@"Yes", @""), nil];
    self.alertView.tag = UnsentReportsVCAlertViewTagClear;
    [self.alertView show];
}

- (void)sendBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    self.alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Send reports", @"") message:NSLocalizedString(@"Are you shure you want to send all reports?", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"No", @"") otherButtonTitles:NSLocalizedString(@"Yes", @""), nil];
    self.alertView.tag = UnsentReportsVCAlertViewTagSend;
    [self.alertView show];
}

- (void)sendReports {
    if([ReachabilityManager isUnreachable]) {
        [self showFailedProgressHud];
        return;
    }
    [self addReportHandlers];
    [self showProgressHud];
    for (APIUploadReportHandler *apiUploadReportHandler in self.apiUploadReportHandlers) {
        [apiUploadReportHandler uploadReportWithUsername:[[DataManager sharedManager] storedUsername]
        password:[[DataManager sharedManager] storedPassword]
        successHandler:^(NSString *result, Report *report) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [[DataManager sharedManager] clearStoredReport:report];
                [self removeUploadReportHandlerForReport:report];
            }];
        }
        errorHandler:^(NSNumber *errorCode, Report *report) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [self removeUploadReportHandlerForReport:report];
                if ([errorCode intValue] == ERROR_UNAUTHORIZED) {
                    [self.progressHud hide:YES];
                    if(![self.alertView isVisible]) {
                        self.alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Unauthorized", @"") message:NSLocalizedString(@"The server couldn´t authorize your credentials.", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"Ok", @"") otherButtonTitles:nil];
                        self.alertView.tag = UnsentReportsVCAlertViewTagUnauthorized;
                        [self.alertView show];
                    }
    
                }
            }];
        }];
    }
}

- (void)removeUploadReportHandlerForReport:(Report *)report {
    NSInteger indexToReomve = 0;
    for (APIUploadReportHandler *apiUploadHandler in self.apiUploadReportHandlers) {
        if([apiUploadHandler.report.observationDate isEqualToDate:report.observationDate]) {
            break;
        }
        indexToReomve++;
    }
    [self.apiUploadReportHandlers removeObjectAtIndex:indexToReomve];
    
    if(self.apiUploadReportHandlers.count == 0) {
        [self.tableView reloadData];
        if([[DataManager sharedManager] storedReports].count == 0) {
            [self showSuccessProgressHud];
        }
        else {
            [self showFailedProgressHud];
        }
        [self updateBarButtons];
    }
}

- (void)addReportHandlers {
    for (Report *report in [[DataManager sharedManager] storedReports]) {
        [self.apiUploadReportHandlers addObject:[[APIUploadReportHandler alloc] initWithReport:report]];
    }
}
#pragma mark - TableView Methods

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
   return [[DataManager sharedManager] storedReports].count;;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {

    UnsentReportTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:NSStringFromClass([UnsentReportTableViewCell class]) forIndexPath:indexPath];
    Report *report = [[DataManager sharedManager] storedReports][indexPath.row];
    cell.customImageView.image = report.photo;
    cell.dateLabel.text = [self.dateFormatter stringFromDate:report.observationDate];
    return cell;
}

#pragma mark - Alert view delegate methods

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (alertView.tag) {
        case UnsentReportsVCAlertViewTagClear:
            if(alertView.cancelButtonIndex != buttonIndex) {
                [[DataManager sharedManager] clearAllStoredReports];
                [self.tableView reloadData];
                [self updateBarButtons];
            }
            break;
        case UnsentReportsVCAlertViewTagSend:
            if(alertView.cancelButtonIndex != buttonIndex) {
                [self sendReports];
            }
        default:
            break;
    }
}

@end
