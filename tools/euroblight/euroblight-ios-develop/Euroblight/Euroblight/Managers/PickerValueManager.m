#import "PickerValueManager.h"
#import "PickerValue.h"

@implementation PickerValueManager

#pragma mark - Public Methods

+ (PickerValueManager *)sharedManager {
    static PickerValueManager *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    return sharedManager;
}

- (NSArray *)bbchValues {
    if(_bbchValues == nil) {
        _bbchValues = [self importFileWithName:@"type_of_bbch" withExtension:@"json"];
    }
    return _bbchValues;
}

- (NSArray *)severityValues {
    if(_severityValues == nil) {
       _severityValues = [self importFileWithName:@"type_of_severity" withExtension:@"json"];
    }
    return _severityValues;
}

- (NSArray *)typeOfProductionValues {
    if(_typeOfProductionValues == nil) {
        _typeOfProductionValues = [self importFileWithName:@"type_of_production" withExtension:@"json"];
    }
    return _typeOfProductionValues;
}

- (NSArray *)infectionPressureCountries {
    if(_infectionPressureCountries == nil) {
        _infectionPressureCountries = @[NSLocalizedString(@"Denmark", @"")];
    }
    return _infectionPressureCountries;
}

- (NSArray *)infectionPressureLocations {
    if(_infectionPressureLocations == nil) {
        _infectionPressureLocations = [self importFileWithName:@"infection_pressure_locations_dk" withExtension:@"json"];
    }
    return _infectionPressureLocations;
}

#pragma mark - Private Methods

- (NSArray *)importFileWithName:(NSString *)fileName withExtension:(NSString*)extension {
    NSMutableArray *returnArray = [[NSMutableArray alloc] init];
    NSURL *path = [[NSBundle mainBundle] URLForResource:fileName withExtension:extension];
    NSData *data = [NSData dataWithContentsOfURL:path];
    NSError *error;
    id JSONObject = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
    if(error != nil) {
        NSLog(@"Error parsing file: %@", error);
        return nil;
    }
    if([JSONObject isKindOfClass:[NSArray class]]) {
        NSInteger index = 0;
        for (NSDictionary *pickerValueDict in JSONObject) {
            [returnArray addObject:[[PickerValue alloc] initWithJSONDictionary:pickerValueDict index:index]];
            index++;
        }
    }
    return returnArray;
}
@end
