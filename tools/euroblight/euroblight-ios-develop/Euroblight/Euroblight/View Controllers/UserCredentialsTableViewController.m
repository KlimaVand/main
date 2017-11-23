#import "UserCredentialsTableViewController.h"
#import "DataManager.h"
#import "SaveButtonTableViewCell.h"
#import "TextFieldInputTableViewCell.h"
#import "UIBarButtonItem+Euroblight.h"

@interface UserCredentialsTableViewController ()

@property (weak, nonatomic) IBOutlet UINavigationItem *cancelBarButtonItem;

@property (strong, nonatomic) NSArray *tableViewSections;
@property (strong, nonatomic) NSArray *tableViewSectionUserCredentialsCells;
@property (strong, nonatomic) NSArray *tableViewSectionSaveCells;

@property (strong, nonatomic) NSString *currentUsernameInput;
@property (strong, nonatomic) NSString *currentPasswordInput;

@property (weak, nonatomic) UITextField *usernameTextField;
@property (weak, nonatomic) UITextField *passwordTextField;

@property (nonatomic) BOOL isUsernameInputValid;
@property (nonatomic) BOOL isPasswordInputValid;

@end

typedef NS_ENUM(NSInteger, TableViewCellUserCredentials) {
    TableViewCellUserCredentialsEmail,
    TableViewCellUserCredentialsPassword
};

typedef NS_ENUM(NSInteger, TableViewCellUserCredentialsSave) {
    TableViewCellSave
};

typedef NS_ENUM(NSInteger, TableViewSections) {
    TableViewSectionUserCredentials,
    TableViewSectionSave
};

@implementation UserCredentialsTableViewController

#pragma mark - View Lifecycle Methods

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initTableView];
    self.title = NSLocalizedString(@"User Credentials", @"");
    [self initBarButtons];
    self.currentUsernameInput = [[DataManager sharedManager] storedUsername];
    self.currentPasswordInput = [[DataManager sharedManager] storedPassword];
    if(![[DataManager sharedManager] isStoredUserCredentialsSufficient]) {
        self.navigationItem.rightBarButtonItem = nil;
    }
    self.isUsernameInputValid = YES;
    self.isPasswordInputValid = YES;
}

#pragma mark - Private Methods

- (void)initTableView {
    self.tableViewSections = @[@(TableViewSectionUserCredentials), @(TableViewSectionSave)];
    self.tableViewSectionUserCredentialsCells = @[@(TableViewCellUserCredentialsEmail), @(TableViewCellUserCredentialsEmail)];
    self.tableViewSectionSaveCells = @[@(TableViewCellSave)];
}

- (void)initBarButtons {
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"HelpIcon"
                                                                                       target:self
                                                                                       action:@selector(infoBarButtonTapped:)];
}

- (BOOL)validateInputString:(NSString *)inputString {
   return inputString.length != 0;
}

- (UIScrollView *)findHiddenScrollViewInTableViewCell:(UITableViewCell *)cell {
    for (UIView *view in cell.subviews) {
        if([view isKindOfClass:[UIScrollView class]]) {
            return (UIScrollView *)view;
        }
    }
    return nil;
}

#pragma mark - Action Methods

- (void)saveButtonTapped:(id)sender {
    self.isUsernameInputValid = [self validateInputString:self.usernameTextField.text];
    self.isPasswordInputValid = [self validateInputString:self.passwordTextField.text];
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:TableViewSectionUserCredentials] withRowAnimation:UITableViewRowAnimationNone];
    if(self.isUsernameInputValid && self.isPasswordInputValid) {
        [[DataManager sharedManager] storeUsername:self.currentUsernameInput password:self.currentPasswordInput];
        [self dismissViewControllerAnimated:YES completion:nil];
        return;
    }
}

- (IBAction)cancelBarButtonTapped:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)infoBarButtonTapped:(id)sender {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.euroblight.net"]];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return self.tableViewSections.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSInteger returnValue = 0;
    switch (section) {
        case TableViewSectionUserCredentials:
            returnValue = self.tableViewSectionUserCredentialsCells.count;
            break;
        case TableViewSectionSave:
            returnValue = self.tableViewSectionSaveCells.count;
            break;
        default:
            break;
    }
    return returnValue;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *cellIdentifier;
    if(indexPath.section == TableViewSectionUserCredentials){
        cellIdentifier = NSStringFromClass([TextFieldInputTableViewCell class]);
        TextFieldInputTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        switch (indexPath.row) {
            case TableViewCellUserCredentialsEmail:
                self.usernameTextField = cell.textField;
                cell.textField.text = self.currentUsernameInput;
                cell.textField.placeholder = NSLocalizedString(@"Username", @"");
                cell.textField.returnKeyType = UIReturnKeyNext;
                cell.textField.keyboardType = UIKeyboardTypeEmailAddress;
                cell.textField.secureTextEntry = NO;
                cell.textField.tag = TableViewCellUserCredentialsEmail;
                cell.textField.delegate = self;
                cell.validInput = self.isUsernameInputValid;
                return cell;
                break;
            case TableViewCellUserCredentialsPassword:
                self.passwordTextField = cell.textField;
                cell.textField.text = self.currentPasswordInput;
                cell.textField.placeholder = NSLocalizedString(@"Password", @"");
                cell.textField.returnKeyType = UIReturnKeyDone;
                cell.textField.keyboardType = UIKeyboardTypeDefault;
                cell.textField.secureTextEntry = YES;
                cell.textField.tag = TableViewCellUserCredentialsPassword;
                cell.textField.delegate = self;
                cell.validInput = self.isPasswordInputValid;
                return cell;
                break;
            default:
                break;
        }
    }
    else if (indexPath.section == TableViewSectionSave) {
        cellIdentifier = NSStringFromClass([SaveButtonTableViewCell class]);
        SaveButtonTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        [self findHiddenScrollViewInTableViewCell:cell].delaysContentTouches = NO;
        [cell.saveButton addTarget:self action:@selector(saveButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
        [cell.saveButton setTitle:NSLocalizedString(@"Save", @"") forState:UIControlStateNormal];
        return cell;
    }
    return nil;
}

#pragma mark - Table View Delegate Methods

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 60;
}

#pragma mark - Texfield Delegate Methods

- (void)textFieldDidEndEditing:(UITextField *)textField {
    if(textField.tag == TableViewCellUserCredentialsEmail) {
        self.currentUsernameInput = textField.text;
    } else {
        self.currentPasswordInput = textField.text;
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    if(textField.tag == TableViewCellUserCredentialsEmail) {
        [self.passwordTextField becomeFirstResponder];
    } else {
        [self.passwordTextField resignFirstResponder];
        [self saveButtonTapped:nil];
    }
    return YES;
}

@end
