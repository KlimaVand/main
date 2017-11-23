#import "MainTableViewController.h"
#import "DataManager.h"
#import "MainTableViewCell.h"
#import "UIBarButtonItem+Euroblight.h"

@interface MainTableViewController ()

@property (strong, nonatomic) NSArray *tableViewMainCells;

@end

typedef NS_ENUM(NSInteger, TableViewCellMain) {
    TableViewCellMainReport,
    TableViewCellMainMap,
    TableViewCellMainPressure,
    TableViewCellUnsentReports
};

@implementation MainTableViewController

#pragma mark - View Lifecycle Methods

-(void)viewDidLoad {
    [super viewDidLoad];
    self.title = NSLocalizedString(@"Blight Tracker 2.0", @"");
    [self initTableView];
    [self initBarButtons];
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.tableView reloadData];
}

-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    if(![[DataManager sharedManager] isStoredUserCredentialsSufficient]) {
        [self performSegueWithIdentifier:@"modalUserCredentialsSegue" sender:self];
    }
}

#pragma mark - Private Methods

- (void)initTableView {
    self.tableViewMainCells = @[@(TableViewCellMainReport), @(TableViewCellMainMap), @(TableViewCellMainPressure),@(TableViewCellUnsentReports) ];
}

- (void)initBarButtons {
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"HelpIcon"
                                                                                       target:self
                                                                                       action:@selector(infoBarButtonTapped:)];
    self.navigationItem.rightBarButtonItem = [UIBarButtonItem createBarButtonFromImageWithName:@"LoginIcon"
                                                                                        target:self
                                                                                        action:@selector(userCredentialsBarButtonItemTapped:)];
}

- (UIBarButtonItem *)createBarButtonFromImageWithName:(NSString *)imageName target:(id)target action:(SEL)action {
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    UIImage *image = [UIImage imageNamed:imageName];
    button.frame = CGRectMake(0, 0, image.size.width, image.size.height);
    [button setBackgroundImage:image forState:UIControlStateNormal];
    [button addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    button.showsTouchWhenHighlighted = YES;
    return [[UIBarButtonItem alloc] initWithCustomView:button];
}

#pragma mark - Action Methods

- (void)infoBarButtonTapped:(id)sender {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.euroblight.net"]];
}

- (void)userCredentialsBarButtonItemTapped:(id)sender {
    [self performSegueWithIdentifier:@"modalAnimatedUserCredentialsSegue" sender:self];
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.tableViewMainCells.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *cellIdentifier = NSStringFromClass([MainTableViewCell class]);
    MainTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
    switch (indexPath.row) {
        case TableViewCellMainReport:
            [cell setIconImage:[UIImage imageNamed:@"ReportIcon"]];
            [cell setTitle:NSLocalizedString(@"Report", @"")];
            [cell setNbrOfUnsentReports:nil];
            break;
        case TableViewCellMainMap:
            [cell setIconImage:[UIImage imageNamed:@"MapIcon"]];
            [cell setTitle:NSLocalizedString(@"Map", @"")];
            [cell setNbrOfUnsentReports:nil];
            break;
        case TableViewCellMainPressure:
            [cell setIconImage:[UIImage imageNamed:@"PressureIcon"]];
            [cell setTitle:NSLocalizedString(@"Infection Pressure", @"")];
            [cell setNbrOfUnsentReports:nil];
            break;
        case TableViewCellUnsentReports:
            [cell setIconImage:[UIImage imageNamed:@"AlertIcon"]];
            [cell setTitle:NSLocalizedString(@"Unsent Reports", @"")];
            [cell setNbrOfUnsentReports:@([[DataManager sharedManager] storedReports].count)];
            break;
        default:
            break;
    }
    return cell;
}

#pragma mark - Table view delegate Methods 

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    switch (indexPath.row) {
        case TableViewCellMainReport:
            [self performSegueWithIdentifier:@"pushReportSegue" sender:self];
            break;
        case TableViewCellMainMap:
            [self performSegueWithIdentifier:@"pushMapSegue" sender:self];
            break;
        case TableViewCellMainPressure:
              [self performSegueWithIdentifier:@"pushPressureSegue" sender:self];
            break;
        case TableViewCellUnsentReports:
            [self performSegueWithIdentifier:@"pushUnsendReportsSegue" sender:self];
            break;
        default:
            break;
    }
}

@end
