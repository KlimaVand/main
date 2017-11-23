#import <Foundation/Foundation.h>
@class Report;

@interface DataManager : NSObject

+ (DataManager *)sharedManager;

@property (strong, nonatomic) Report *currentReport;

- (void)storeUsername:(NSString*)username password:(NSString *)password;
- (BOOL)isStoredUserCredentialsSufficient;
- (NSString *)storedUsername;
- (NSString *)storedPassword;

- (void)storeCurrentReport;
- (void)clearCurrrentReport;
- (NSMutableArray *)storedReports;
- (void)removeSentReportFromStoredReportsIfNeeded;
- (void)clearStoredReport:(Report *)report;
- (void)clearAllStoredReports;

@end
