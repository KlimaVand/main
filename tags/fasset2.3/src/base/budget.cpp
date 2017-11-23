/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "budget.h"
#include "message.h"

/****************************************************************************\
\****************************************************************************/
budget::budget ()
{
   ClassName  = "unknown";
   BudgetName = "unknown";
	input      = 0;
   output     = 0;
   lastError  = 0;

}

/****************************************************************************\
\****************************************************************************/
budget::budget (const budget& aBudget )
{
   ClassName  = aBudget.ClassName;
   BudgetName = aBudget.BudgetName;
	input      = aBudget.input;
   output     = aBudget.output;
   lastError  = aBudget.lastError;

}

/****************************************************************************\
\****************************************************************************/
budget& budget::operator=(const budget& aBudget)
{
   ClassName  = aBudget.ClassName;
   BudgetName = aBudget.BudgetName;
	input      = aBudget.input;
   output     = aBudget.output;
   lastError  = aBudget.lastError;

	return *this;
}

/****************************************************************************\
\****************************************************************************/
void budget::SetNames (string aClassName, string aBudgetName)
{
   ClassName = aClassName;
   BudgetName = aBudgetName;

}

/****************************************************************************\
\****************************************************************************/
void budget::SetInput (double aInput)
{

	input = aInput;
   if (aInput<0)
   	theMessage->FatalError("budget::SetInput - Input gone negative");
}

/****************************************************************************\
\****************************************************************************/
void budget::SetOutput (double aOutput)
{

	output = aOutput;
   if (output<0)
   	theMessage->FatalError("budget::SetInput - Output gone negative");
}

/****************************************************************************\
\****************************************************************************/
void budget::AddInput (double aInput)
{

   input += aInput;
}

/****************************************************************************\
\****************************************************************************/
void budget::AddOutput (double aOutput)
{

   output += aOutput;
}

/****************************************************************************\
\****************************************************************************/
void budget::Reset()
{

	input     = 0;
   output    = 0;
   lastError = 0;
}

/****************************************************************************\
\****************************************************************************/
bool budget::Balance(double remain)
{

   double diff;
   diff = input-output-remain;
   double maxRelError=(fabs(input)+fabs(output)+remain)*1E-6;
	if ((fabs(diff)>maxRelError) && (fabs(diff-lastError)>maxRelError))
   {
   	char numberStr[40];
      string outputString;
      if (fabs(diff)>1E10)
      {
         cout << "Enormous difference in budget class of " << diff << endl;
         cout << "Difference reduced to avoid memory error!" << endl;
         if (diff>0)
            diff = 1E10;
         else
            diff = -1E10;
      }

      sprintf(numberStr, "%f", diff);
		outputString = ClassName+":: Error in budget of "+BudgetName+ ". Difference is "+numberStr;

      if (theMessage)
	      theMessage->FatalError(outputString.c_str());
      else
      	cout << outputString.c_str() << endl;
      lastError=diff;
      return false;
   }
   else return true;
}

/****************************************************************************\
\****************************************************************************/
budget * budget::clone() const
{
	budget * b = new budget(*this);
   return b;
}


/****************************************************************************\
\****************************************************************************/
void budget::Add(budget aBudget, double fraction)
{

	input     = (1.0-fraction)*input + fraction*aBudget.input;
	output    = (1.0-fraction)*output + fraction*aBudget.output;
   lastError = (1.0-fraction)*lastError + fraction*aBudget.lastError;
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os,const budget& s)
{
  os << setiosflags(ios::left) << "Class " << s.GetClassName()<< " Budget " << s.GetBudgetName() << endl;
  os << setiosflags(ios::left) << "Input " << s.GetInput() << " Output " << s.GetOutput() << endl;
  return os;
}


