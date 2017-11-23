#import "Report.h"

@implementation Report

#pragma mark - Private Methods

-(id)init {
   self = [super init];
    if(self){
        self.BBCH = nil;
        self.fieldName = @"";
        self.institutionName = @"";
        self.locationName = @"";
        self.observationDate =[NSDate date];
        self.photo = nil;
        self.typeOfProduction = nil;
        self.severity = nil;
        self.surveyorName = @"";
        self.variety = @"";
    }
    return self;
}

#pragma mark NSCoding

NSString *const BBCH_VALUE_KEY = @"BbchValueKey";
NSString *const FIELD_NAME_KEY = @"FieldNameKey";
NSString *const INSTITUTION_NAME_KEY = @"InstitutionNameKey";
NSString *const LATITUDE_VALUE_KEY = @"LatitudeValueKey";
NSString *const LOCATION_NAME_KEY = @"LocationNameKey";
NSString *const LONGITUDE_VALUE_KEY = @"LongitudeValueKey";
NSString *const OBERVATION_DATE_KEY = @"ObservationDateKey";
NSString *const PHOTO_KEY = @"PhotoKey";
NSString *const SELECTED_TYPE_OF_PRODUCTION_INDEX_KEY = @"SelectedTypeOFProductionKey";
NSString *const SEVERITY_VALUE_KEY = @"SeverityValueKey";
NSString *const SURVEYOR_NAME_KEY = @"SurveyorNameKey";
NSString *const VARITY_KEY = @"VarityKey";

- (void) encodeWithCoder:(NSCoder *)encoder {
    [encoder encodeObject:self.BBCH forKey:BBCH_VALUE_KEY];
    [encoder encodeObject:self.fieldName forKey:FIELD_NAME_KEY];
    [encoder encodeObject:self.institutionName forKey:INSTITUTION_NAME_KEY];
    [encoder encodeObject:self.severity forKey:SEVERITY_VALUE_KEY];
    [encoder encodeObject:self.latitudeValue forKey:LATITUDE_VALUE_KEY];
    [encoder encodeObject:self.locationName forKey:LOCATION_NAME_KEY];
    [encoder encodeObject:self.longitudeValue forKey:LONGITUDE_VALUE_KEY];
    [encoder encodeObject:self.observationDate forKey:OBERVATION_DATE_KEY];
    [encoder encodeObject:UIImagePNGRepresentation(self.photo) forKey:PHOTO_KEY];
    [encoder encodeObject:self.typeOfProduction forKey:SELECTED_TYPE_OF_PRODUCTION_INDEX_KEY];
    [encoder encodeObject:self.surveyorName forKey:SURVEYOR_NAME_KEY];
    [encoder encodeObject:self.variety forKey:VARITY_KEY];
}

- (id)initWithCoder:(NSCoder *)decoder {
    self.BBCH = [decoder decodeObjectForKey:BBCH_VALUE_KEY];
    self.fieldName = [decoder decodeObjectForKey:FIELD_NAME_KEY];
    self.institutionName = [decoder decodeObjectForKey:INSTITUTION_NAME_KEY];
    self.severity = [decoder decodeObjectForKey:SEVERITY_VALUE_KEY];
    self.latitudeValue = [decoder decodeObjectForKey:LATITUDE_VALUE_KEY];
    self.locationName = [decoder decodeObjectForKey:LOCATION_NAME_KEY];
    self.longitudeValue = [decoder decodeObjectForKey:LONGITUDE_VALUE_KEY];
    self.observationDate =  [decoder decodeObjectForKey:OBERVATION_DATE_KEY];
    self.photo = [UIImage imageWithData:[decoder decodeObjectForKey:PHOTO_KEY]];
    self.typeOfProduction = [decoder decodeObjectForKey:SELECTED_TYPE_OF_PRODUCTION_INDEX_KEY];
    self.surveyorName = [decoder decodeObjectForKey:SURVEYOR_NAME_KEY];
    self.variety = [decoder decodeObjectForKey:VARITY_KEY];
    return self;
}

@end
