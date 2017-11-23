#import "PressureViewController.h"
#import "APIRetrieveInfectionPressureHandler.h"
#import "Constants.h"
#import "DataManager.h"
#import "MBProgressHUD.h"
#import "UIBarButtonItem+Euroblight.h"
#import "ReachabilityManager.h"
#import "PickerValueManager.h"
#import "PickerValue.h"
#import "UILabel+Euroblight.h"
#import "UIImageView+WebCache.h"


@interface PressureViewController ()

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *textFieldInputView;
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UILabel *errorLabel;

@property (strong, nonatomic) UIImageView *imageView;
@property (strong, nonatomic) UIPickerView *pickerView;
@property (strong, nonatomic) UIToolbar *keyboardToolbar;
@property (strong, nonatomic) NSArray *pickerComponentCountries;
@property (strong, nonatomic) NSMutableArray *pickerComponentAlphabet;
@property (strong, nonatomic) NSArray *pickerComponentLocations;
@property (strong, nonatomic) NSArray *pickerComponentLocationsFiltered;

@property (nonatomic) NSInteger selectedLocationsPickerIndex;
@property (nonatomic) NSInteger lastSelectedLocationsPickerIndex;
@property (nonatomic) NSInteger selectedAlphabetPickerIndex;
@property (nonatomic) NSInteger lastSelectedAlphabetPickerIndex;

@property (nonatomic, strong) APIRetrieveInfectionPressureHandler *apiInfectionPressureHandler;
@property (nonatomic, strong) MBProgressHUD *progressHud;
@property (strong, nonatomic) UIAlertView *alertView;

@end

typedef NS_ENUM(NSInteger, PickerViewComponent) {
    PickerViewComponentCountries,
    PickerViewComponentAlphabet,
    PickerViewComponentLocations
};

@implementation PressureViewController

#pragma mark - View Life Cycle Methods

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = NSLocalizedString(@"Pressure", @"");
    self.apiInfectionPressureHandler = [[APIRetrieveInfectionPressureHandler alloc] init];
    [self initBarButtons];
    [self initPickerView];
    [self initTextField];
    [self initImageAndScrollView];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.textFieldInputView.backgroundColor = [[UIColor whiteColor] colorWithAlphaComponent:OVELAY_VIEWS_ALPHA];
    [self showTextFieldInputView];
    [self hideErrorLabel];
}

-(void)viewDidLayoutSubviews {
    self.scrollView.zoomScale = 1.0;
   self.imageView.frame = CGRectMake(0,0,self.scrollView.frame.size.width, self.scrollView.frame.size.height);
    self.scrollView.contentSize = self.imageView.frame.size;
}


#pragma mark - Private Methods

- (void)initBarButtons {
    UIBarButtonItem *refreshBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"RefreshIcon" target:self action:@selector(refreshBarButtonTapped:)];
    UIBarButtonItem *locationBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"LocationIcon" target:self action:@selector(locationBarButtonItemTapped:)];
    self.navigationItem.rightBarButtonItems = @[refreshBarButtonItem, locationBarButtonItem];
}

- (void)showTextFieldInputView {
    self.textFieldInputView.alpha = 1;
}

- (void)initTextField {
    self.textField.text = [NSString stringWithFormat:@"%@-%@",self.pickerComponentCountries[0], ((PickerValue *)self.pickerComponentLocations[0]).name];
    self.textField.inputView = self.pickerView;
    self.textField.inputAccessoryView = self.keyboardToolbar;
    self.textField.delegate = self;
}

- (void)initPickerView {
    self.pickerComponentCountries = [PickerValueManager sharedManager].infectionPressureCountries;
    self.pickerComponentLocations = [PickerValueManager sharedManager].infectionPressureLocations;
    self.pickerComponentLocations = [self sortPickerValueArray:self.pickerComponentLocations];
    self.pickerComponentLocationsFiltered = [[NSArray alloc] initWithArray:self.pickerComponentLocations];
    self.pickerComponentAlphabet = [self createAlphabetArray];
    self.keyboardToolbar = [self createKeyboardToolbar];
    self.pickerView = [[UIPickerView alloc] init];
    self.pickerView.delegate = self;
    self.pickerView.dataSource = self;
}

- (void)initImageAndScrollView {
    self.imageView = [[UIImageView alloc] initWithFrame:CGRectZero];
    self.imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self.scrollView addSubview:self.imageView];
    self.scrollView.minimumZoomScale = 1.0;
    self.scrollView.maximumZoomScale = 2.0;
    self.scrollView.zoomScale = 1.0;
}

- (NSMutableArray *)createAlphabetArray {
    NSString *alphabetString = NSLocalizedString(@"alphabet", @"");
    NSMutableArray *alphabetArray = [[NSMutableArray alloc] initWithObjects:NSLocalizedString(@"All", @""), nil];
    for (NSInteger i=0; i<alphabetString.length; i++) {
        NSString *stringChar = [NSString stringWithFormat:@"%C",[alphabetString characterAtIndex:i]];
        [alphabetArray addObject:stringChar];
    }
    return alphabetArray;
}

- (UIToolbar *)createKeyboardToolbar {
    UIToolbar *toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, TOOLBAR_HEIGHT)];
    UIBarButtonItem *cancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancelBarButtonItemTapped:)];
    UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
    UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(doneBarButtonItemTapped:)];
    toolBar.items = @[cancelButton, flexibleSpace, doneButton];
    return toolBar;
}

- (void)showErrorLabelWithText:(NSString *)text {
    self.errorLabel.text = text;
    self.errorLabel.alpha = 1;
    self.imageView.alpha = 0;
}

- (void)hideErrorLabel {
    self.errorLabel.text = @"";
    self.errorLabel.alpha = 0;
    self.imageView.alpha = 1;
}

- (void)showProgressHud {
    if(self.progressHud == nil) {
        self.progressHud = [MBProgressHUD showHUDAddedTo:[[UIApplication sharedApplication] keyWindow] animated:YES];
    }
    self.progressHud.mode = MBProgressHUDModeIndeterminate;
    self.progressHud.labelText = NSLocalizedString(@"Loading...", @"");
    [self.progressHud show:YES];
}

- (void)retrieveAndLoadInfectionPressureImage {
    if ([ReachabilityManager isUnreachable]) {
        [self showErrorLabelWithText:NSLocalizedString(@"Connection not available", @"")];
        return;
    }
    [self showProgressHud];
    [self.apiInfectionPressureHandler retrieveInfectionPressure:[[DataManager sharedManager] storedUsername]
        password:[[DataManager sharedManager] storedPassword]
        locationName:((PickerValue *)self.pickerComponentLocationsFiltered[self.selectedLocationsPickerIndex]).name
        locationCoordinate:((PickerValue *)self.pickerComponentLocationsFiltered[self.selectedLocationsPickerIndex]).value
        width:675
        height:235
        successHandler:^(NSString *result) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [self hideErrorLabel];
                [self.progressHud hide:YES];
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:result]];
                self.scrollView.zoomScale = 1.0;
            }];
        }
        errorHandler:^(NSNumber *errorCode) {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
                [self.progressHud hide:YES];
                [self showErrorLabelWithText:NSLocalizedString(@"No infection to display", @"")];
                if ([errorCode intValue] == ERROR_UNAUTHORIZED) {
                    if(![self.alertView isVisible]) {
                        self.alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Unauthorized", @"") message:NSLocalizedString(@"The server couldn´t authorize your credentials.", @"") delegate:self cancelButtonTitle:NSLocalizedString(@"Ok", @"") otherButtonTitles:nil];
                        [self.alertView show];
                    }
                    return;
                }
            }];
        }];
}

- (NSArray *)locationsThatStartsWithString:(NSString *)string {
    NSString* filter = @"%K BEGINSWITH[c] %@";
    if([string isEqualToString:NSLocalizedString(@"All", @"")]) {
        return self.pickerComponentLocations;
    }
    else {
        NSPredicate* predicate = [NSPredicate predicateWithFormat:filter, @"name", string];
        NSArray *filteredArray = [self.pickerComponentLocations filteredArrayUsingPredicate:predicate];
        return [self sortPickerValueArray:filteredArray];
    }
}

- (NSArray *)sortPickerValueArray:(NSArray *)pickerValueArray {
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc]
                                        initWithKey: @"name" ascending:YES];
    return [pickerValueArray sortedArrayUsingDescriptors:@[sortDescriptor]];
}

#pragma mark - Action Methods

- (void)locationBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    [UIView animateWithDuration:0.3 animations:^{
        if(self.textFieldInputView.alpha == 1) {
            self.textFieldInputView.alpha = 0;
        } else {
            [self showTextFieldInputView];
        }
    }];
}

- (void)cancelBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    self.selectedLocationsPickerIndex = self.lastSelectedLocationsPickerIndex;
    self.selectedAlphabetPickerIndex = self.lastSelectedAlphabetPickerIndex;
    self.pickerComponentLocationsFiltered = [self locationsThatStartsWithString:self.pickerComponentAlphabet[self.selectedAlphabetPickerIndex]];
    [self.pickerView selectRow:self.selectedAlphabetPickerIndex inComponent:PickerViewComponentAlphabet animated:NO];
    [self.pickerView selectRow:self.selectedLocationsPickerIndex inComponent:PickerViewComponentLocations animated:NO];
}

- (void)doneBarButtonItemTapped:(UIBarButtonItem *)barButtonItem {
    [self.textField resignFirstResponder];
    self.textField.text = [NSString stringWithFormat:@"%@-%@",self.pickerComponentCountries[0], ((PickerValue *)self.pickerComponentLocationsFiltered[[self.pickerView selectedRowInComponent:PickerViewComponentLocations]]).name];
    [self retrieveAndLoadInfectionPressureImage];
}

- (void)refreshBarButtonTapped:(UIBarButtonItem *)barButtonItem {
    [self retrieveAndLoadInfectionPressureImage];
}

#pragma mark - Text Field Delegate Methods

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    self.lastSelectedAlphabetPickerIndex = self.selectedAlphabetPickerIndex;
    self.lastSelectedLocationsPickerIndex = self.selectedLocationsPickerIndex;
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {
    return NO;
}

#pragma mark - Picker View Data Source Delegate Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 3;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    if(component == PickerViewComponentCountries) {
        return self.pickerComponentCountries.count;
    } else if (component == PickerViewComponentAlphabet) {
        return self.pickerComponentAlphabet.count;
    }
    return self.pickerComponentLocationsFiltered.count;
}

#pragma mark - Picker View Delegate Methods

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view{
    UILabel* pickerLabel = (UILabel*)view;
    if (!pickerLabel){
        pickerLabel = [UILabel pickerLabel];
    }
    if(component == PickerViewComponentCountries) {
        pickerLabel.text = self.pickerComponentCountries[row];
    }
    else if(component == PickerViewComponentAlphabet)
        pickerLabel.text = self.pickerComponentAlphabet[row];
    else {
        pickerLabel.text = ((PickerValue *)self.pickerComponentLocationsFiltered[row]).name;
    }
    return pickerLabel;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    if( component == PickerViewComponentLocations) {
        self.selectedLocationsPickerIndex = row;
    }
    else if(component == PickerViewComponentAlphabet) {
        self.selectedAlphabetPickerIndex = row;
        self.pickerComponentLocationsFiltered = [self locationsThatStartsWithString:self.pickerComponentAlphabet[row]];
        [self.pickerView reloadComponent:PickerViewComponentLocations];
    }
}

#pragma mark - ScrollView Delegate Methods

-(UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView {
    return self.imageView;
}

@end
