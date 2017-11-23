#import "ReportTableViewController.h"
#import "ReportInputTableViewCell.h"
#import "ReportTakePhotoTableViewCell.h"
#import "ReportLocationTableViewCell.h"
#import "APIUploadReportHandler.h"
#import "SaveButtonTableViewCell.h"
#import "DataManager.h"
#import "Report.h"
#import "MBProgressHUD.h"
#import "ReachabilityManager.h"
#import "Constants.h"
#import "PickerValueManager.h"
#import "PickerValue.h"
#import "UILabel+Euroblight.h"


@interface ReportTableViewController ()

@property (strong, nonatomic) IBOutlet UITableView *reportTableView;
@property (strong, nonatomic) NSArray *tableViewReportCells;

@property (strong, nonatomic) NSArray *bbchValues;
@property (strong, nonatomic) NSArray *SeverityValues;
@property (strong, nonatomic) NSArray *typeOfProductionValues;

@property (strong, nonatomic) UIToolbar *keyboardToolbar;

@property (strong, nonatomic) UIPickerView *bbchPickerView;
@property (strong, nonatomic) UIPickerView *severityPickerView;
@property (strong, nonatomic) UIPickerView *typeOfProductionPickerView;

@property (strong, nonatomic) UITextField *activeTextField;
@property (strong, nonatomic) UITextField *fieldNameTextField;
@property (strong, nonatomic) UITextField *bbchTextField;
@property (strong, nonatomic) UITextField *severityTextField;
@property (strong, nonatomic) UITextField *typeOfProductionTextField;
@property (strong, nonatomic) UITextField *varityTextField;

@property (strong, nonatomic) CLLocationManager *locationManager;
@property (strong, nonatomic) MBProgressHUD *progressHud;

@property (nonatomic) BOOL isPhotoValid;
@property (nonatomic) BOOL isLocationValid;

@property (strong, nonatomic) APIUploadReportHandler *apiUploadReportHandler;

@end

typedef NS_ENUM(NSInteger, TableViewCellReport) {
    TableViewCellReportFieldNameInput,
    TableViewCellReportSeverityInput,
    TableViewCellReportBBCHInput,
    TableViewCellReportTypeOfProductionInput,
    TableViewCellReportVarityInput,
    TableViewCellReportTakePhoto,
    TableViewCellReportLocation,
    TableViewCellSendButton,
};

typedef NS_ENUM(NSInteger, ReportVCAlertViewTags) {
    ReportVCAlertViewTagNoConnection,
    ReportVCAlertViewTagClear,
    ReportVCAlertViewTagUnauthorized,
};

@implementation ReportTableViewController

#pragma mark - View Lifecycle Methods

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = NSLocalizedString(@"Report", @"");
    [self initBarButtons];
    [self initTableView];
    [self initPickerViews];
  
    
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    self.isLocationValid = YES;
    self.isPhotoValid = YES;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.tableView reloadData];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}

#pragma mark - Private Methods

- (void)initBarButtons {
    UIBarButtonItem *clearBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Clear", @"") style:UIBarButtonItemStyleBordered target:self action:@selector(clearBarButtonItemTapped:)];
    self.navigationItem.rightBarButtonItem = clearBarButtonItem;
}

- (void)initTableView {
    self.tableViewReportCells = @[@(TableViewCellReportFieldNameInput),@(TableViewCellReportSeverityInput), @(TableViewCellReportBBCHInput), @(TableViewCellReportTypeOfProductionInput), @(TableViewCellReportVarityInput),  @(TableViewCellReportTakePhoto), @(TableViewCellReportLocation), @(TableViewCellSendButton)];
}

- (void)initPickerViews {
    self.keyboardToolbar = [self createKeyboardToolbar];
    
    self.bbchValues = [PickerValueManager sharedManager].bbchValues;
    self.bbchPickerView = [[UIPickerView alloc] init];
    self.bbchPickerView.delegate = self;
    self.bbchPickerView.dataSource = self;
    
    self.SeverityValues = [PickerValueManager sharedManager].severityValues;
    self.severityPickerView = [[UIPickerView alloc] init];
    self.severityPickerView.delegate = self;
    self.severityPickerView.dataSource = self;
   
    self.typeOfProductionValues = [PickerValueManager sharedManager].typeOfProductionValues;
    self.typeOfProductionPickerView = [[UIPickerView alloc] init];
    self.typeOfProductionPickerView.delegate = self;
    self.typeOfProductionPickerView.dataSource = self;
    
    [self reloadPickers];
}


- (void)reloadPickers {
    if([DataManager sharedManager].currentReport.BBCH == nil) {
        [self.bbchPickerView selectRow:0 inComponent:0 animated:NO];
        [DataManager sharedManager].currentReport.BBCH = self.bbchValues[[self.bbchPickerView selectedRowInComponent:0]];
    } else {
        [self.bbchPickerView selectRow:[DataManager sharedManager].currentReport.BBCH.index inComponent:0 animated:NO];
    }
    
    if([DataManager sharedManager].currentReport.severity == nil) {
        [self.severityPickerView selectRow:0 inComponent:0 animated:NO];
        [DataManager sharedManager].currentReport.severity = self.SeverityValues[[self.severityPickerView selectedRowInComponent:0]];
    } else {
        [self.severityPickerView selectRow:[DataManager sharedManager].currentReport.severity.index inComponent:0 animated:NO];
    }

    if([DataManager sharedManager].currentReport.typeOfProduction == nil) {
        [self.typeOfProductionPickerView selectRow:0 inComponent:0 animated:NO];
        [DataManager sharedManager].currentReport.typeOfProduction = self.typeOfProductionValues[[self.typeOfProductionPickerView selectedRowInComponent:0]];
    } else {
        [self.typeOfProductionPickerView selectRow:[DataManager sharedManager].currentReport.typeOfProduction.index inComponent:0 animated:NO];
    }
}

- (UIToolbar *)createKeyboardToolbar {
    UIToolbar *toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, TOOLBAR_HEIGHT)];
    UIBarButtonItem *cancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancelBarButtonItemTapped:)];
    UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
    UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(doneBarButtonItemTapped:)];
    toolBar.items = @[cancelButton, flexibleSpace, doneButton];
    return toolBar;
}

- (UIScrollView *)findHiddenScrollViewInTableViewCell:(UITableViewCell *)cell {
    for (UIView *view in cell.subviews) {
        if([view isKindOfClass:[UIScrollView class]]) {
            return (UIScrollView *)view;
        }
    }
    return nil;
}

- (BOOL)isUserInputsValid {
    if([DataManager sharedManager].currentReport.photo != nil) {
        self.isPhotoValid = YES;
    }
    else {
        self.isPhotoValid = NO;
    }
    if([DataManager sharedManager].currentReport.latitudeValue != nil && [DataManager sharedManager].currentReport.longitudeValue != nil) {
        self.isLocationValid = YES;
    }
    else {
        self.isLocationValid = NO;
    }
    return self.isPhotoValid && self.isLocationValid;
}

- (void)sendReport {
    
    if([ReachabilityManager isUnreachable]) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Connection not available", @"") message:NSLocalizedString(@"Would you like to store your report?", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"No", @"") otherButtonTitles:NSLocalizedString(@"Yes", @""), nil];
        alertView.tag = ReportVCAlertViewTagNoConnection;
        [alertView show];
        return;
    }
    
    [self showProgressHud];
    self.apiUploadReportHandler = [[APIUploadReportHandler alloc] initWithReport:[DataManager sharedManager].currentReport];
    [self.apiUploadReportHandler uploadReportWithUsername:[[DataManager sharedManager] storedUsername] password:[[DataManager sharedManager] storedPassword] successHandler:^(NSString *result, Report *report) {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
            [self handleUploadReportSuccess];
        }];
    } errorHandler:^(NSNumber *errorCode, Report *report) {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
            if ([errorCode intValue] == ERROR_UNAUTHORIZED) {
                [self.progressHud hide:YES];
                UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Unauthorized", @"") message:NSLocalizedString(@"The server couldn´t authorize your credentials.", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"Ok", @"") otherButtonTitles:nil];
                alertView.tag = ReportVCAlertViewTagUnauthorized;
                [alertView show];
                return;
            }
            [self handleUploadReportFailed];
        }];
    }];
}

- (void)showProgressHud {
    self.progressHud = [MBProgressHUD showHUDAddedTo:self.view.window animated:YES];
    self.progressHud.mode = MBProgressHUDModeIndeterminate;
    self.progressHud.labelText = NSLocalizedString(@"Sending report", @"");
}

- (void)handleUploadReportSuccess {
    [[DataManager sharedManager] removeSentReportFromStoredReportsIfNeeded];
    self.progressHud.customView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"CheckmarkIcon"]];
    self.progressHud.mode = MBProgressHUDModeCustomView;
    self.progressHud.labelText = NSLocalizedString(@"Report sent", @"");
    [self.progressHud hide:YES afterDelay:2];
    [self performSelector:@selector(clearCurrentReport) withObject:nil afterDelay:2];
    
}

- (void)handleUploadReportFailed {
    self.progressHud.mode = MBProgressHUDModeCustomView;
    self.progressHud.labelText = NSLocalizedString(@"Failed to send report!", @"");
    [self.progressHud hide:YES afterDelay:2];
    [self performSelector:@selector(addReportToUnsentReports) withObject:nil afterDelay:2];
}

- (void)clearCurrentReport {
    [[DataManager sharedManager] clearCurrrentReport];
    [self reloadPickers];
    [self.tableView reloadData];
}

- (void)addReportToUnsentReports {
    [[DataManager sharedManager] storeCurrentReport];
    [self reloadPickers];
    [self.tableView reloadData];
}

- (BOOL)isInputCell:(NSInteger)row {
    if(row == TableViewCellReportFieldNameInput || row == TableViewCellReportBBCHInput || row == TableViewCellReportSeverityInput || row == TableViewCellReportTypeOfProductionInput || row == TableViewCellReportVarityInput) {
        return YES;
    }
    return NO;
}

- (void)reloadCellWithIdentifier:(TableViewCellReport)identifier {
     [self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:identifier inSection:0]] withRowAnimation:UITableViewRowAnimationNone];
}

- (void)setPickerTextForLabel:(UILabel *)label pickerView:(UIPickerView *)pickerView row:(NSInteger)row {
    if (pickerView == self.bbchPickerView) {
        label.text = NSLocalizedString(((PickerValue *)self.bbchValues[row]).name, @"");
    }
    else if (pickerView == self.severityPickerView) {
        label.text = NSLocalizedString(((PickerValue *)self.SeverityValues[row]).name, @"");
    }
    else if (pickerView == self.typeOfProductionPickerView) {
        label.text = NSLocalizedString(((PickerValue *)self.typeOfProductionValues[row]).name, @"");
    }
}

#pragma mark - Action Methods

- (void)clearBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Clear report", @"") message:NSLocalizedString(@"Are you shure you want to clear this report?", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"No", @"") otherButtonTitles:NSLocalizedString(@"Yes", @""), nil];
    alertView.tag = ReportVCAlertViewTagClear;
    [alertView show];
}

- (void)cancelBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.activeTextField resignFirstResponder];
}

- (void)doneBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.activeTextField resignFirstResponder];
    if(self.activeTextField == self.bbchTextField) {
        [self reloadCellWithIdentifier:TableViewCellReportBBCHInput];
    }
    else if(self.activeTextField == self.severityTextField) {
        [self reloadCellWithIdentifier:TableViewCellReportSeverityInput];
    }
    else if(self.activeTextField == self.typeOfProductionTextField) {
        [self reloadCellWithIdentifier:TableViewCellReportTypeOfProductionInput];
    }
}

- (void)takePhotoButtonTapped:(UIButton *)button {
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.allowsEditing = YES;
    picker.sourceType = UIImagePickerControllerSourceTypeCamera;
    [self presentViewController:picker animated:YES completion:NULL];
}

- (void)locationButtonTapped:(UIButton *)button {
    if ([self.locationManager respondsToSelector:@selector(requestWhenInUseAuthorization)]) {
        // iOS8
        [self.locationManager requestWhenInUseAuthorization];
    } else {
        // iOS7
        // Only way to trigger the permission question is to ask for locations
        [self.locationManager startUpdatingLocation];
    }

}

- (void)sendButtonTapped:(UIButton *)button {
    if([self isUserInputsValid]) {
        [self sendReport];
    }
    [self.tableView reloadData];
}

#pragma mark - Table view data source

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    if([self isInputCell:indexPath.row]) {
        return 100;
    }
    if(indexPath.row == TableViewCellReportTakePhoto) {
        return 80;
    }
    if(indexPath.row == TableViewCellReportLocation) {
        return 90;
    }
    if(indexPath.row == TableViewCellSendButton) {
        return 80;
    }
    return 0;
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.tableViewReportCells.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell;
    if([self isInputCell:indexPath.row]) {
        NSString *cellIdentifier = NSStringFromClass([ReportInputTableViewCell class]);
        ReportInputTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        switch (indexPath.row) {
            case TableViewCellReportFieldNameInput:
                self.fieldNameTextField = cell.textField;
                cell.textField.inputView = nil;
                cell.customTitleLabel.text = NSLocalizedString(@"Field name", @"");
                cell.textField.placeholder = NSLocalizedString(@"Enter a field name", @"");
                cell.textField.text = [DataManager sharedManager].currentReport.fieldName;
                cell.textField.delegate = self;
                return cell;
                break;
            case TableViewCellReportBBCHInput:
                cell.customTitleLabel.text = NSLocalizedString(@"BBCH", @"");
                self.bbchTextField = cell.textField;
                cell.textField.text = NSLocalizedString([DataManager sharedManager].currentReport.BBCH.name,@"");
                cell.textField.inputView = self.bbchPickerView;
                cell.textField.inputAccessoryView = self.keyboardToolbar;
                cell.textField.delegate = self;
                return cell;
                break;
            case TableViewCellReportSeverityInput:
                cell.customTitleLabel.text = NSLocalizedString(@"Severity", @"");
                self.severityTextField = cell.textField;
                cell.textField.text =  NSLocalizedString([DataManager sharedManager].currentReport.severity.name,@"");
                cell.textField.inputView = self.severityPickerView;
                cell.textField.inputAccessoryView = self.keyboardToolbar;
                cell.textField.delegate = self;
                return cell;
                break;
            case TableViewCellReportTypeOfProductionInput:
                cell.customTitleLabel.text = NSLocalizedString(@"Type of production", @"");
                self.typeOfProductionTextField = cell.textField;
                cell.textField.text = NSLocalizedString([DataManager sharedManager].currentReport.typeOfProduction.name,@"");
                cell.textField.inputView = self.typeOfProductionPickerView;
                cell.textField.inputAccessoryView = self.keyboardToolbar;
                cell.textField.delegate = self;
                return cell;
                break;
            case TableViewCellReportVarityInput:
                self.varityTextField = cell.textField;
                 cell.textField.inputView = nil;
                cell.customTitleLabel.text = NSLocalizedString(@"Variety", @"");
                cell.textField.placeholder = NSLocalizedString(@"Enter a variety", @"");
                cell.textField.text = [DataManager sharedManager].currentReport.variety;
                cell.textField.delegate = self;
                return cell;
                break;
            default:
                break;
        }
    }
 
    if(indexPath.row == TableViewCellReportTakePhoto) {
        NSString *cellIdentifier = NSStringFromClass([ReportTakePhotoTableViewCell class]);
        ReportTakePhotoTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        [cell setCustomImage:[DataManager sharedManager].currentReport.photo];
        [cell.button addTarget:self action:@selector(takePhotoButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
        [cell setValidInput:self.isPhotoValid];
         [self findHiddenScrollViewInTableViewCell:cell].delaysContentTouches = NO;
        return cell;
    }
    if(indexPath.row == TableViewCellReportLocation) {
        NSString *cellIdentifier = NSStringFromClass([ReportLocationTableViewCell class]);
        ReportLocationTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        [cell.button addTarget:self action:@selector(locationButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
        cell.latitudeLabel.text = NSLocalizedString(@"Latitude:", @"");
        cell.latitudeValueLabel.text = [DataManager sharedManager].currentReport.latitudeValue;
        cell.longitudeLabel.text = NSLocalizedString(@"Longitude:", @"");
        cell.longitudeValueLabel.text = [DataManager sharedManager].currentReport.longitudeValue;
        [self findHiddenScrollViewInTableViewCell:cell].delaysContentTouches = NO;
        [cell setValidInput:self.isLocationValid];
        return cell;
    }
    if(indexPath.row == TableViewCellSendButton) {
        NSString *cellIdentifier = NSStringFromClass([SaveButtonTableViewCell class]);
        SaveButtonTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        [self findHiddenScrollViewInTableViewCell:cell].delaysContentTouches = NO;
        [cell.saveButton addTarget:self action:@selector(sendButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
        [cell.saveButton setTitle:NSLocalizedString(@"Send", @"") forState:UIControlStateNormal];
        return cell;
    }

    return cell;
}

#pragma mark - Text Field Delegate Methods

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    self.activeTextField = textField;
    return YES;
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField {
    if(textField == self.fieldNameTextField) {
        [DataManager sharedManager].currentReport.fieldName = self.fieldNameTextField.text;
    }
    else if (textField == self.bbchTextField) {
        [DataManager sharedManager].currentReport.BBCH = self.bbchValues[[self.bbchPickerView selectedRowInComponent:0]];
    }
    else if (textField == self.severityTextField) {
        [DataManager sharedManager].currentReport.severity = self.SeverityValues[[self.severityPickerView selectedRowInComponent:0]];
    }
    else if (textField == self.typeOfProductionTextField) {
        [DataManager sharedManager].currentReport.typeOfProduction = self.typeOfProductionValues[[self.typeOfProductionPickerView selectedRowInComponent:0]];
    }
    else if(textField == self.varityTextField) {
        [DataManager sharedManager].currentReport.variety = self.varityTextField.text;
    }
    return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [self.activeTextField resignFirstResponder];
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {
    if(self.activeTextField == self.bbchTextField || self.activeTextField == self.severityTextField || self.activeTextField == self.typeOfProductionTextField ) {
        return NO;
    }
    return YES;
}

#pragma mark - Picker View Data Source Delegate Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    if (pickerView == self.bbchPickerView) {
        return self.bbchValues.count;
    }
    if (pickerView == self.severityPickerView) {
        return self.SeverityValues.count;
    }
    if (pickerView == self.typeOfProductionPickerView) {
        return self.typeOfProductionValues.count;
    }
    
    return 0;
}

#pragma mark - Picker View Delegate Methods

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view{
    UILabel* pickerLabel = (UILabel*)view;
    if (!pickerLabel){
        pickerLabel = [UILabel pickerLabel];
    }
    [self setPickerTextForLabel:pickerLabel pickerView:pickerView row:row];
    return pickerLabel;
}

#pragma mark - UIImagePicker Delegate Methods

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    [DataManager sharedManager].currentReport.photo = info[UIImagePickerControllerEditedImage];
    [picker dismissViewControllerAnimated:YES completion:NULL];
    [self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:TableViewCellReportTakePhoto inSection:0]] withRowAnimation:UITableViewRowAnimationNone];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

#pragma mark - Location Manager Delegate Methods

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    
    if (status == kCLAuthorizationStatusAuthorizedAlways ||
        status == kCLAuthorizationStatusAuthorizedWhenInUse ||
        status == kCLAuthorizationStatusAuthorized) {
        [self.locationManager startUpdatingLocation];
    } else if (status == kCLAuthorizationStatusRestricted || status == kCLAuthorizationStatusDenied) {

    }
    
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations {
    [manager stopUpdatingLocation];
    CLLocation *location = [locations lastObject];
    [DataManager sharedManager].currentReport.latitudeValue = [NSString stringWithFormat:@"%f", location.coordinate.latitude];
    [DataManager sharedManager].currentReport.longitudeValue = [NSString stringWithFormat:@"%f", location.coordinate.longitude];
    [self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:TableViewCellReportLocation inSection:0]] withRowAnimation:UITableViewRowAnimationNone];
}

#pragma mark - Alert view delegate methods

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (alertView.tag) {
        case ReportVCAlertViewTagNoConnection:
            if(alertView.cancelButtonIndex != buttonIndex) {
                [self addReportToUnsentReports];
            }
            break;
        case ReportVCAlertViewTagClear:
            if(alertView.cancelButtonIndex != buttonIndex) {
                [self clearCurrentReport];
            }
            break;
        default:
            break;
    }
}

@end
