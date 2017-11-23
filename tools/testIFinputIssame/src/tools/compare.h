#include <math.h>

 using namespace std;

 bool doubleEquals(double left, double right, double epsilon) {
   return (fabs(left - right) < epsilon);
 }

 bool doubleLess(double left, double right, double epsilon, bool orequal = false) {
   if (fabs(left - right) < epsilon) {
     return (orequal);
   }
   return (left < right);
 }

 bool doubleGreater(double left, double right, double epsilon, bool orequal = false) {
   if (fabs(left - right) < epsilon) {
     return (orequal);
   }
   return (left > right);
 }
