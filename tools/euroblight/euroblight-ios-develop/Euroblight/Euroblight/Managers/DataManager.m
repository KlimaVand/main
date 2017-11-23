#import "DataManager.h"
#import "Report.h"

@interface DataManager()

@property (strong,  nonatomic) NSString * documentsPath;

@end

@implementation DataManager

NSString *const USER_DEFAULTS_USERNAME_KEY = @"userDefaultsUsernameKey";
NSString *const USER_DEFAULTS_PASSWORD_KEY = @"userDefaultsPAsswordKey";
NSString *const FILE_NAME_REPORTS = @"reports";

#pragma mark - Public Methods

+ (DataManager *)sharedManager {
    static DataManager *sharedDataManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedDataManager = [[self alloc] init];
    });
    return sharedDataManager;
}

- (void)storeUsername:(NSString*)username password:(NSString *)password {
    [[NSUserDefaults standardUserDefaults] setObject:username forKey:USER_DEFAULTS_USERNAME_KEY];
    [[NSUserDefaults standardUserDefaults] setObject:password forKey:USER_DEFAULTS_PASSWORD_KEY];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (NSString *)storedUsername {
    return [[NSUserDefaults standardUserDefaults] objectForKey:USER_DEFAULTS_USERNAME_KEY];
}

- (NSString *)storedPassword {
    return [[NSUserDefaults standardUserDefaults] objectForKey:USER_DEFAULTS_PASSWORD_KEY];
}

- (BOOL)isStoredUserCredentialsSufficient {
    return [self storedUsername] != nil && [self storedPassword] != nil;
}

- (void)clearCurrrentReport {
    self.currentReport = [[Report alloc] init];
}

- (void)clearStoredReport:(Report *)report {
    NSMutableArray *reports = [self storedReports];
    NSInteger index = 0;
    for (Report *storedReport in reports) {
        if ([storedReport.observationDate isEqualToDate:report.observationDate]) {
            break;
        }
        index++;
    }
    [reports removeObjectAtIndex:index];
    [self storeReportsToDisk:reports];
}

- (void)removeSentReportFromStoredReportsIfNeeded {
    NSMutableArray *reports = [self storedReports];
    NSInteger index = 0;
    BOOL shouldRemoveCurrentReport = NO;
    for (Report *report in reports) {
        if([self isCurrentReportEqualToReport:report]) {
            shouldRemoveCurrentReport = YES;
            break;
        }
        index++;
    }
    if(shouldRemoveCurrentReport) {
        [reports removeObjectAtIndex:index];
        [self storeReportsToDisk:reports];
    }
}

- (void)storeCurrentReport {
    NSMutableArray *reports = [self storedReports];
    if(reports == nil) {
        reports = [NSMutableArray array];
    }
    if(reports.count == 0) {
       [self addCurrentReportToReports:reports];
        [self clearCurrrentReport];
        return;
    }
    BOOL shouldAddReport = YES;
    for (Report *report in reports) {
        if([self isCurrentReportEqualToReport:report]) {
            shouldAddReport = NO;
            break;
        }
    }
    if(shouldAddReport) {
        [self addCurrentReportToReports:reports];
        [self clearCurrrentReport];
    }
}

- (NSMutableArray *)storedReports {
    NSURL *filePathURL = [self documentsPathForFileName:FILE_NAME_REPORTS];
    NSData *encodedReports;
    if([[NSFileManager defaultManager] fileExistsAtPath:filePathURL.path]) {
        encodedReports = [[NSData alloc] initWithContentsOfFile:filePathURL.path];
    }
    return encodedReports ? (NSMutableArray *)[NSKeyedUnarchiver unarchiveObjectWithData:encodedReports] : nil;
}

- (void)clearAllStoredReports {
    NSURL *filePathURL = [self documentsPathForFileName:FILE_NAME_REPORTS];
    [[NSFileManager defaultManager] removeItemAtPath:filePathURL.path error:nil];
}

#pragma mark - Private Methods

- (id)init {
    self = [super init];
    if(self) {
        self.currentReport = [[Report alloc] init];
    }
    return self;
}

- (NSURL *)documentsPathForFileName:(NSString *)name {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *documentsPath = [paths objectAtIndex:0];
    return [NSURL fileURLWithPath:[documentsPath stringByAppendingPathComponent:name]];
}

- (BOOL)isCurrentReportEqualToReport:(Report *)report {
  return [report.observationDate isEqualToDate:self.currentReport.observationDate] && [report.latitudeValue isEqualToString:self.currentReport.latitudeValue] && [report.longitudeValue isEqualToString:self.currentReport.longitudeValue];
}

- (void)addCurrentReportToReports:(NSMutableArray *)reports {
    [reports addObject:self.currentReport];
    [self storeReportsToDisk:reports];
}

- (void)storeReportsToDisk:(NSMutableArray *)reports {
    NSData *encodedReports = [NSKeyedArchiver archivedDataWithRootObject:reports];
    NSURL *filePathURL = [self documentsPathForFileName:FILE_NAME_REPORTS];
    [[NSFileManager defaultManager] createFileAtPath:filePathURL.path contents:encodedReports attributes:nil];
}

@end
