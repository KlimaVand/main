#import "MapViewController.h"
#import "UIBarButtonItem+Euroblight.h"
#import "Constants.h"
#import "APIRetrieveKMLFileHandler.h"
#import "DataManager.h"
#import "MBProgressHUD.h"
#import "ReachabilityManager.h"
#import "UILabel+Euroblight.h"

@interface MapViewController ()

@property (weak, nonatomic) IBOutlet UIView *textFieldInputView;
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UIWebView *webView;
@property (weak, nonatomic) IBOutlet UIView *helpView;
@property (weak, nonatomic) IBOutlet UILabel *errorLabel;
@property (weak, nonatomic) IBOutlet UILabel *greenCircleDescriptionLabel;
@property (weak, nonatomic) IBOutlet UILabel *redCircleDescriptionLabel;
@property (weak, nonatomic) IBOutlet UILabel *blueCircleDescriptionLabel;

@property (strong, nonatomic) NSArray *pickerValues;
@property (strong, nonatomic) UIToolbar *keyboardToolbar;
@property (strong, nonatomic) UIPickerView *pickerView;
@property (nonatomic) NSInteger selectedPickerIndex;
@property (nonatomic) NSInteger lastSelectedPickerIndex;

@property (strong, nonatomic) APIRetrieveKMLFileHandler *apiRetrieveKMLFileHandler;
@property (strong, nonatomic) MBProgressHUD *progressHud;
@property (strong, nonatomic) UIAlertView *alertView;

@end

typedef NS_ENUM(NSInteger, Country) {
    Denmark,
    Finland,
    Norway,
    Sweden,
};

@implementation MapViewController

#pragma mark - View Life Cycle Methods

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = NSLocalizedString(@"Map", @"");
    self.apiRetrieveKMLFileHandler = [[APIRetrieveKMLFileHandler alloc] init];
    [self initBarButtons];
    [self initPickerView];
    [self initTextField];
    [self initHelpView];
    [self loadHtml];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self hideErrorLabel];
    self.textFieldInputView.backgroundColor = [[UIColor whiteColor] colorWithAlphaComponent:OVELAY_VIEWS_ALPHA];
    self.helpView.backgroundColor = [[UIColor whiteColor] colorWithAlphaComponent:OVELAY_VIEWS_ALPHA];
    [self showTextFieldInputView];
    [self hideHelpView];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [self clearWebView];
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
    [self.progressHud hide:YES];
}

#pragma mark - Private Methods

- (void)initBarButtons {
    UIBarButtonItem *hidePickerBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"LocationIcon" target:self action:@selector(hideBarButtonItemTapped:)];
    UIBarButtonItem *infoBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"InfoIcon" target:self action:@selector(infoBarButtonItemTapped:)];
    UIBarButtonItem *refreshBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"RefreshIcon" target:self action:@selector(refreshBarButtonTapped:)];
    self.navigationItem.rightBarButtonItems = @[refreshBarButtonItem ,infoBarButtonItem, hidePickerBarButtonItem];
}

- (void)showTextFieldInputView {
     self.textFieldInputView.alpha = 1;
}

- (void)hideHelpView {
    self.helpView.alpha = 0;
}

- (void)initTextField {
    self.textField.text = self.pickerValues[0];
    self.textField.inputView = self.pickerView;
    self.textField.inputAccessoryView = self.keyboardToolbar;
    self.textField.delegate = self;
}

- (void)initPickerView {
    self.pickerValues = @[NSLocalizedString(@"Danmark", @""), NSLocalizedString(@"Finland", @""), NSLocalizedString(@"Norway", @""), NSLocalizedString(@"Sweden", @"")];
    self.keyboardToolbar = [self createKeyboardToolbar];
    self.pickerView = [[UIPickerView alloc] init];
    self.pickerView.delegate = self;
    self.pickerView.dataSource = self;
}

- (void)initHelpView {
    self.greenCircleDescriptionLabel.text = NSLocalizedString(@"No late blight found", @"");
    self.redCircleDescriptionLabel.text = NSLocalizedString(@"Late blight found", @"");
    self.blueCircleDescriptionLabel.text = NSLocalizedString(@"Late blight found more than 10 days ago", @"");
}

- (UIToolbar *)createKeyboardToolbar {
    UIToolbar *toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, TOOLBAR_HEIGHT)];
    UIBarButtonItem *cancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancelBarButtonItemTapped:)];
    UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
    UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(doneBarButtonItemTapped:)];
    toolBar.items = @[cancelButton, flexibleSpace, doneButton];
    return toolBar;
}

- (void)loadHtml {
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"map" withExtension:@"html"];
    [self.webView loadRequest:[NSURLRequest requestWithURL:url]];
}

- (void)retrieveAndLoadKMLFile {
    if ([ReachabilityManager isUnreachable]) {
        [self showErrorLabelWithText:NSLocalizedString(@"Connection not available", @"")];
        return;
    }
    [self showProgressHud];
    [self.apiRetrieveKMLFileHandler retrieveKMLFileForUsername:[[DataManager sharedManager] storedUsername]
        password:[[DataManager sharedManager] storedPassword]
        countryId:[self countryIdForCountry:[self.pickerView selectedRowInComponent:0]]
        successHandler:^(NSString *result) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [self hideErrorLabel];
                [self.progressHud hide:YES];
                [self.webView stringByEvaluatingJavaScriptFromString:[NSString stringWithFormat:@"drawMap('%@')", result]];
            }];
        } errorHandler:^(NSNumber *errorCode) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [self.progressHud hide:YES];
                [self showErrorLabelWithText:NSLocalizedString(@"No records to display", @"")];
                if ([errorCode intValue] == ERROR_UNAUTHORIZED) {
                    if(![self.alertView isVisible]) {
                        self.alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Unauthorized", @"") message:NSLocalizedString(@"The server couldn´t authorize your credentials.", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"Ok", @"") otherButtonTitles:nil];
                        [self.alertView show];
                    }
                    return;
                }
            }];
        }
    ];
}

- (void)showProgressHud {
    if(self.progressHud == nil) {
        self.progressHud = [MBProgressHUD showHUDAddedTo:[[UIApplication sharedApplication] keyWindow] animated:YES];
    }
    self.progressHud.mode = MBProgressHUDModeIndeterminate;
    self.progressHud.labelText = NSLocalizedString(@"Loading...", @"");
    [self.progressHud show:YES];
}

- (NSString *)countryIdForCountry:(Country)country {
    NSString *returnValue;
    switch (country) {
        case Sweden:
            returnValue = @"SE";
            break;
        case Denmark:
            returnValue = @"DK";
            break;
        case Norway:
            returnValue = @"NO";
            break;
        case Finland:
            returnValue = @"FI";
            break;
        default:
            break;
    }
    return returnValue;
}

- (void)showErrorLabelWithText:(NSString *)text {
    self.errorLabel.text = text;
    self.errorLabel.alpha = 1;
    self.webView.alpha = 0;
}

- (void)hideErrorLabel {
    self.errorLabel.text = @"";
    self.errorLabel.alpha = 0;
    self.webView.alpha = 1;
}

- (void)clearWebView {
    [self.webView stopLoading];
    [self.webView loadHTMLString:@"" baseURL:nil];
    self.webView.delegate = nil;
}

#pragma mark - Action Methods

- (void)hideBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    [UIView animateWithDuration:0.3 animations:^{
        if(self.textFieldInputView.alpha == 1) {
            self.textFieldInputView.alpha = 0;
        } else {
            [self showTextFieldInputView];
        }
    }];
}

- (void)infoBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    [UIView animateWithDuration:0.3 animations:^{
        if(self.helpView.alpha == 1) {
            [self hideHelpView];
        } else {
           self.helpView.alpha = 1;
        }
    }];
}

- (void)refreshBarButtonTapped:(UIBarButtonItem *)barButtonItem {
    [self retrieveAndLoadKMLFile];
}

- (void)cancelBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    self.selectedPickerIndex = self.lastSelectedPickerIndex;
    [self.pickerView selectRow:self.selectedPickerIndex inComponent:0 animated:NO];
}

- (void)doneBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    self.textField.text = self.pickerValues[[self.pickerView selectedRowInComponent:0]];
    [self retrieveAndLoadKMLFile];
}

#pragma mark - Text Field Delegate Methods

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    self.lastSelectedPickerIndex = self.selectedPickerIndex;
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {
    return NO;
}

#pragma mark - Picker View Data Source Delegate Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return self.pickerValues.count;
}

#pragma mark - Picker View Delegate Methods

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view{
    UILabel* pickerLabel = (UILabel*)view;
    if (!pickerLabel){
        pickerLabel = [UILabel pickerLabel];
    }
    pickerLabel.text = self.pickerValues[row];
    return pickerLabel;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    self.selectedPickerIndex = row;
}

@end
