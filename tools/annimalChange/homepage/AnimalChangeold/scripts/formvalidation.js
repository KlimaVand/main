// formvalidation.js
// Functions for validation of forms
// The functions are general in the sence that information about the form,
// e.g. name of the form and its elements, is not present.
// Therefore, the file can be included for different forms.
  var strLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  var strSpace   = " "
  var strNumber     = "0123456789";
  var strSign1   = "-_";
  var strSign2   = ",.:;)("
  var OKStr		= "";
  var AllowedText	= "";
  var ch = "";
  var res = "";
  var OKval ="";

  var Txt1 = "Only letters, numbers and the \nspecial signs - and _ are allowed ";
  var Txt2 = "Only letters, numbers, space and the \nspecial signs , . : ; ( ) - and _ are allowed ";
  var Txt3 = "Only numbers are allowed ";

var lanTheInputField        = "The input field";
var lanTheInputField2       = " the input field";
var lanTheValue             = "The value";
var lanMustBeFilled         = " must be filled.";
var lanMustContain          = " must contain";
var lanMustBe               = " must be";
var lanExactly              = " exactly";
var lanBetween              = " between";
var lanAnd                  = " and";
var lanCharacters           = " characters";
var lanContainsInvalidChar  = " contains at least one invalid character";
var lanIsNot                = " is not";
var lanFilled               = " filled with";
var lanValidEmail           = " a valid e-mail address.";
var lanValidTelephone       = " a valid phone number.";
var lanValidDate            = " a valid date.";
var lanValidFloat           = " a valid decimal number.";
var lanValidInteger         = " a valid integer.";
var lanDateFormat           = "Write the date on the format";
var lanOnly                 = "Only"
var lanLetters              = " letters (a-z,A-Z)";
var lanLettersDK            = " letters (a-å,A-Å)";
var lanNumbers              = " numbers";
var lanSpecialSigns         = " special signs"
var lanAreAllowed           = " are allowed";
var lanSpace                = " space";
var lanWritePassword        = "Write a password in the field";
var lanRepeatPassword       = " repeat the password in the field";
var lanMustSelectOption     = "You must select one of the options in ";
var lanWith                 = " with";
var lanDigits               = " digits";
var lanIn                   = " in";


function InputValidator(item, min, max, fieldname, type)
{
  item.value=trim(item.value);
  if ((item.value.length < min) || (item.value.length > max))
  {
    if (min==max)
    {
      alert("The input field \"" + fieldname + "\" must contain exactly " + min + " characters.");
    }
    else
    {
      alert("The input field \"" + fieldname + "\" must contain between " + min + " and " + max + " characters.");
    }
    focusfield(item);
    return (false);
  }


// The type parameter:
//    1: loginname, password;
//    2: text
//    3: numbers
//    4: email

  if (type==1)
  {
    OKStr = strLetter+strNumber+strSign1;
    AllowedText = Txt1;
  }
  else if (type==2)
  {
    OKStr = strLetter+strSpace+strNumber+strSign1+strSign2;
    AllowedText = Txt2;
  }
  else if (type==3)
  {
    OKStr = strNumber;
    AllowedText = Txt3;
  }

  if (type==4)
  {
    item.value=trim(item.value);
    if (!isEmail(item.value))
    {
      alert("The input field \"" + fieldname + "\" is not a valid email. \nIt is important that the email is correct \nin order to receive your registration data. ");
      focusfield(item);
      return (false);
    }
  }
  else
  {
    res = ValidStr(item.value, OKStr)
    if (res.length>0)
    {
      alert("The input field \"" + fieldname + "\" contains at least one invalid character ("+res+"). \n"+AllowedText);
      focusfield(item);
      return (false);
    }
  }
  return (true);
}

function ConfirmPasswordValidator (item1, fieldname1, item2, fieldname2)
{
  if (!(item1.value == item2.value))
  {
    alert("You must write your selected password in the input field \"" + fieldname1 + "\" \nand repeat it exactly in the input field \"" + fieldname2 + "\".");
    item2.value="";
    focusfield(item1);
    return (false);
  }
  return (true);
}

function RadioValidator(item, fieldname)
{
  var radioSelected = false;
  for (i = 0;  i < item.length;  i++)
  {
    if (item[i].checked)
        radioSelected = true;
  }
  if (!radioSelected)
  {
    alert("You must select a \"" + fieldname + "\".");
    return (false);
  }
  return (true);
}
function ValidateHasValue(item, fieldname)
{
  var msg = ""
  if (!hasValue(item))
  {
    msg += lanTheInputField
    if (fieldname.length > 0)
      msg += " \"" + fieldname + "\"";
    msg += lanMustBeFilled
    alert(msg);

    focusandselect(item);
    return (false);
  }
  return true;
}

function ValidateLength(item, fieldname, min, max)
{
  var msg = ""
  var len=trim(item.value).length;
  if ((len < min) || (len > max))
  {
    if (min==max)
      alert(lanTheInputField + " \"" + fieldname + "\"" + lanMustContain + lanExactly + " " + min + lanCharacters + ".");
    else
      alert(lanTheInputField + " \"" + fieldname + "\"" + lanMustContain + lanBetween + " " + min + lanAnd + " " + max + " " + lanCharacters + ".");

    focusandselect(item);
    return (false);
  }
  return true;
}

function ValidateNumberLength(item, fieldname, min, max)
{
  var msg = ""
  var len=trim(item.value).length;
  if ((len < min) || (len > max))
  {
    if (fieldname.length > 0)
      msg += lanTheInputField + " \"" + fieldname + "\"";
    else
      msg += lanTheValue + " " + argvalue;
    msg += lanMustContain + lanNumbers + lanWith
    if (min==max)
      msg += lanExactly + " " + min + lanDigits + ".";
    else
      msg += lanBetween + " " + min + lanAnd + " " + max + lanDigits + ".";
    alert(msg)

    focusandselect(item);
    return (false);
  }
  return true;
}

function ValidateSize(item, fieldname, min, max)
{
  var msg = ""
  var argvalue=compress(item.value)
  if ((argvalue < min) || argvalue > max)
  {
    if (fieldname.length > 0)
      msg += lanTheInputField + " \"" + fieldname + "\"" + lanMustContain + lanNumbers + lanBetween + " " + min + lanAnd + " " + max;
    else
      msg += lanTheValue + " " + argvalue + lanIsNot + lanBetween + " " + min + lanAnd + " " + max;
    alert(msg)
    focusandselect(item);
    return false;
  }
  return true;
}

function ValidateStr(item, fieldname, OKStr, AllowedText)
{
  var msg = ""
  var argvalue=compress(item.value)
  var res = ValidStr(item.value, OKStr)
  if (res.length>0)
  {
    if (fieldname.length > 0)
      msg += lanTheInputField + " \"" + fieldname + "\"" + lanContainsInvalidChar + " (" + res + "). \n" + AllowedText;
    else
      msg += lanTheValue + " " + argvalue + lanContainsInvalidChar + " (" + res + "). \n" + AllowedText;
    alert(msg);
    focusandselect(item);
    return (false);
  }
  return true;
}

function ExcludeStr(item, fieldname, NotOKStr, AllowedText)
{
  var msg = ""
  var argvalue=compress(item.value)
  var res = NonValidStr(item.value, NotOKStr)
  if (res.length>0)
  {
    if (fieldname.length > 0)
      msg += lanTheInputField + " \"" + fieldname + "\"" + lanContainsInvalidChar + " (" + res + "). \n" + AllowedText;
    else
      msg += lanTheValue + " " + argvalue + lanContainsInvalidChar + " (" + res + "). \n" + AllowedText;
    alert(msg);
    focusandselect(item);
    return (false);
  }
  return true;
}

function ValidateEmail(item, fieldname, required)
{
  var msg = ""
  var argvalue=compress(item.value)
  item.value=argvalue

  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!isEmail(argvalue))
      {
        msg += lanTheValue + " " + argvalue;
        if (fieldname.length > 0)
          msg += lanIn + lanTheInputField2 + " \"" + fieldname + "\"";
        msg += lanIsNot + lanValidEmail
        alert(msg);
        focusandselect(item);
        return false;
      }
    }
  }
  return true;
}

function ValidateSMS(item, fieldname, required)
{
  var msg = ""
  var argvalue=compress(item.value)
  item.value=argvalue

  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!isSMS(argvalue))
      {
        msg += lanTheValue + " " + argvalue;
        if (fieldname.length > 0)
          msg += lanIn + lanTheInputField2 + " \"" + fieldname + "\"";
        msg += lanIsNot + lanValidTelephone
        alert(msg);
        focusandselect(item);
        return false;
      }
    }
  }
  return true;
}

function ValidateText(item, fieldname, required, min, max, OKStr, AllowedText)
{
  if (required || hasValue(item))
  {
    if (!ValidateLength(item, fieldname, min, max))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!ValidateStr(item, fieldname, OKStr, AllowedText))
      {
        focusandselect(item);
        return false;
      }
    }
  }
  return true;
}

function ValidateTextExclude(item, fieldname, required, min, max)
{
  NotOKStr = strExclude;
	AllowedText = "";
  if (required || hasValue(item))
  {
    if (!ValidateLength(item, fieldname, min, max))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!ExcludeStr(item, fieldname, NotOKStr, AllowedText))
      {
        focusandselect(item);
        return false;
      }
    }
  }
  return true;
}

function ValidateNumber(item, fieldname, required, min, max)
{
  var msg = ""
  var argvalue=trimZero(compress(item.value))
  var AllowedText=""
  item.value=argvalue

  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!ValidateSize(item, fieldname, min, max))
      {
        focusandselect(item);
        return false;
      }
    }
  }
  return true;
}

function ValidateInteger(item, fieldname, required, min, max)
{
  var msg = ""
  var argvalue=trimZero(compress(item.value))
  var AllowedText=""
  item.value=argvalue

  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!isInteger(argvalue))
      {
        msg += lanTheValue + " " + argvalue;
        if (fieldname.length > 0)
          msg += lanIn + lanTheInputField2 + " \"" + fieldname + "\"";
        msg += lanIsNot + lanValidInteger
        alert(msg);
        focusandselect(item);
        return false;
      }
      else
      {
        if (!ValidateSize(item, fieldname, min, max))
        {
          focusandselect(item);
          return false;
        }
      }
    }
  }
  return true;
}

function ValidateFloat(item, fieldname, required, decimal, min, max)
{
  var msg = ""
  var argvalue=compress(item.value)
  item.value=argvalue

  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      if (!isFloat(argvalue))
      {
        msg += lanTheValue + " " + argvalue;
        if (fieldname.length > 0)
          msg += lanIn + lanTheInputField2 + " \"" + fieldname + "\"";
        msg += lanIsNot + lanValidFloat
        alert(msg);
        focusandselect(item);
        return false;
      }
      else
      {
        if (!ValidateSize(item, fieldname, min, max))
        {
          focusandselect(item);
          return false;
        }
      }
    }
  }
  item.value=correctDecimal(argvalue,decimal)
  return true;
}

function ValidatePassword(item, fieldname, required, min, max)
{
  var argvalue=compress(item.value)
  item.value=argvalue

  OKStr = strLetter+strNumber+strSign1;
  AllowedText = lanOnly + lanLetters + ", " + lanNumbers + lanAnd + lanSpecialSigns + " -" + lanAnd + " _" + lanAreAllowed;

  if (ValidateText(item, fieldname, required, min, max, OKStr, AllowedText))
    {return true;}
  else
    {return false;}
}

function ValidateDescription(item, fieldname, required, min, max)
{
  OKStr = strLetter+strLetterDK+strSpace+strNumber+strSign1+strSign2;
  AllowedText = lanOnly + lanLettersDK + ", " + lanNumbers + ", " + lanSpace + lanAnd + lanSpecialSigns + " , . : ; ( ) / -" + lanAnd + " _" + lanAreAllowed;

  if (ValidateText(item, fieldname, required, min, max, OKStr, AllowedText))
    {return true;}
  else {return false;}
}

function ValidateFormatDate(item, fieldname, required, format)
{
  var msg = ""
  var argvalue=trim(item.value)
  var val
  if (required || hasValue(item))
  {
    if (!ValidateHasValue(item, fieldname))
    {
      focusandselect(item);
      return false;
    }
    else
    {
      val = correctDate(argvalue,format,true)
      if (val == false)
      {
        focusandselect(item);
        return false;
      }
      if (!isGitDate(correctDate(argvalue,"dd-mm-yyyy",false)))
      {
        msg += lanTheValue + " " + argvalue;
        if (fieldname.length > 0)
          msg += lanIn + lanTheInputField2 + " \"" + fieldname + "\"";
        msg += lanIsNot + lanValidDate + "\n" + lanDateFormat + " " + format + "."
        alert(msg);
        focusandselect(item);
        return false;
      }
    }
    item.value = val;
    return true;
  }
  else {return false;}
}

function ValidateDate(item, fieldname, required)
{
  return ValidateFormatDate(item, fieldname, required, "dd-mm-yyyy");
}

function ValidateShortDate(item, fieldname, required)
{
  return ValidateFormatDate(item, fieldname, required, "dd/mm/yy");
}


function ValidateConfirmPassword (item1, fieldname1, item2, fieldname2)
{
  if (!(item1.value == item2.value))
  {
    alert(lanWritePassword + " \"" + fieldname1 + "\" \n" + lanAnd + lanRepeatPassword + " \"" + fieldname2 + "\".");
    item2.value="";
    focusandselect(item1);
    return (false);
  }
  return (true);
}

function ValidateRadio(item, fieldname)
{
  var radioSelected = false;
  for (var i = 0;  i < item.length;  i++)
  {
    if (item[i].checked)
        radioSelected = true;
  }
  if (!radioSelected)
    alert(lanMustSelectOption + " \"" + fieldname + "\".");
  return radioSelected;
}

function ValidateSelect(item, fieldname, noselectvalue)
{
  if (item.options[item.selectedIndex].value==noselectvalue)
  {
    alert(lanMustSelectOption + " \"" + fieldname + "\".");
    focus(item);
    return false;
  }
  return true;
}


// Helping functions to validate various types of form input
//--------------------------------------------------

function focus(item)
{
  if(item)
  {
    item.focus()
    return false;
  }
  else {return true;}
}

function focusandselect(item)
{
  if(item)
  {
    item.focus();
    item.select();
    return false;
  }
  else {return true;}
}

function focusfield(item)
{
  if(item)
  {
    item.focus()
    item.select()
    return false;
  }
  else {return true;}
}


function hasValue(item)
{
  if (item.value.length > 0)
    if (trim(item.value).length > 0)
      return true;
  return false;
}

function isNumber(argvalue)
{
  if (ValidStr(compress(argvalue), strNumber).length>0)
    return false;
  return true;
}

function isInteger(argvalue)
{
  if (ValidStr(compress(argvalue), strNumber+strMinus).length>0)
    return false;
  return true;
}


function isSMS(argvalue)
{
  argvalue=compress(argvalue)
  if (!(isNumber(argvalue)))
    return false;
  if (argvalue.length != 8)
    return false;
  return true;
}

function isFloat(argvalue)
{
  var neg_position=argvalue.indexOf("-")                                      // position of negation sign (-)
  var comma_position=argvalue.indexOf(",")                                    // position of decimal (. or ,)
  var point_position=argvalue.indexOf(".")                                    // position of decimal (. or ,)
  var decimal
  var decimal_position
  if ((comma_position > -1) && (point_position > -1))                           // Both . and , is not allowed
    return false
  else if (comma_position > point_position)
  {
    decimal = ",";
    decimal_position=comma_position;
  }
  else
  {
    decimal = "."
    decimal_position=point_position;
  }

  var integerpart=argvalue.substring(0, decimal_position)                     // the first part of the float containing the integer
  var remainderpart=argvalue.substring(decimal_position+1, argvalue.length)   // the second part of the float containing the remainder

  if (neg_position > 0)                                   // If there is a negation sign it must be the first char
    return false;
  else if (decimal_position == (argvalue.length-1))       // decimal can not be the last char
    return false;
  else if (remainderpart.indexOf(",") > -1)                // Exactly one decimal 
    return false;
  else if (remainderpart.indexOf(".") > -1)                // Exactly one decimal 
    return false;
  else if (argvalue.substring(1, argvalue.length).indexOf("-") > -1)                // No negation char apart from first char
    return false;
  else if (ValidStr(argvalue, strNumber+strDecimal).length>0) return false;
  
  return true;

}

function isEmail(argvalue)
{
  var atsign_position=argvalue.indexOf("@")                             // position of @
  var namepart=argvalue.substring(0, atsign_position)                   // the first part of the email containing the name of the receiver
  var hostpart=argvalue.substring(atsign_position+1, argvalue.length)   // the second part of the email containing the mail host

  if (atsign_position == -1)                          // There must be an @
    return false;
  else if (atsign_position == 0)                      // @ can not be the first char
    return false;
  else if (atsign_position == (argvalue.length-1))    // @ can not be the last char
    return false;
  else if (hostpart.indexOf("@") != -1)                // Exactly one @ - hence no in the host part
    return false;
  else if (hostpart.indexOf(".") == -1)                // There must be at least one . in the host part
    return false;
  else if (hostpart.charAt(0) == ".")                 // . can not be the first char in the host part
    return false;
  else if (hostpart.charAt(hostpart.length-1) == ".")  // . can not be the last char in the host part
    return false;
  else if (namepart.charAt(0) == ".")                 // . can not be the first char of the name part
    return false;
  else if (ValidStr(argvalue, strLetter+strNumber+strEmail).length>0) return false;

  return true;
}


function y2k(number)
{
  // JavaScript uses year number after 1900, so 2002 is year number 102
  number = parseInt(trimZero(number));
  number = (number > 99)&&(number < 1000) ? number + 1900 : number;
  return parseInt(number);
}

function yyyy(number)
{
  //  Changes a one- or two-digit year to a four-digit year
  //  With separator=29, 29 is changed to 2029 but 30 to 1930
  var separator = 29;
  var year4=parseInt(trimZero(number));
  if (year4 < 100)
  {
    if (year4>separator)
      year4=1900+year4;
    else
      year4=2000+year4;
  }
  return year4;
}


function isDate (day,month,year)
{
  month=month-1
  if((!year)||(year==''))
  {
    var today=new Date();
    year=y2k(today.getYear());
  }
  else
  {
    year=parseInt(trimZero(year));
  }
//  alert('dd:'+day+' mm:'+month+' yy:'+year+' yy:'+y2k(year)+' yy:'+yyyy(y2k(year)))
  if (!day)
    return false
  var test = new Date(year,month,day);
//  if ( (year == yyyy(y2k(test.getYear()))) &&
  if ( (String(parseInt(year)).length != 3) &&
       (!(String(parseInt(year)).length > 4)) &&
       (yyyy(year) == yyyy(y2k(test.getYear()))) &&
       (month == test.getMonth()) &&
       (day == test.getDate()) )
    return true;
  else
    return false
}

function isPIDate(argvalue,separator)
{
  d = compress(argvalue).split(separator);
  day = d[0];
  month = d[1];
  year = d[2];

//  if ( (!day) || (!month) || (!year) || (!(isDate(day,month,year))) )
  if ( (!day) || (!month) || (!(isDate(day,month,year))) )
    return false;
  return true;
}

function isGitDate(argvalue)
{
  var day   = "";
  var month = "";
  var year  = "";
  var d = argvalue.split("-");
  day  = d[0];
  if (d.length>1)
    month = d[1];
  if (d.length>2)
    year  = d[2];

  if ( (!day) || (!month) || (!(isDate(day,month,year))) )
  {
    return false;
  }
  return true;
}
function correctDate(argvalue, frmt, ask)
{
  var str = trim(argvalue);
  var day, month, year;
  var sep1 = "";
  var sep2 = "";
  var pos1 = 0;
  var pos2 = 0;
  var c; 
  var askOK = false;
  var notNumber;
  var readingSep=false;
  var nSeps = 0;
  
  for (var i=0; i<str.length; i++)
  {
    c = str.substring(i, i+1) 
    notNumber = (ValidStr(c, strNumber).length > 0)
    if ((notNumber) && !(readingSep))
      readingSep = true;
    if (!(notNumber) && (readingSep))
    {
      readingSep = false;
      nSeps++;
    }
    
    if (readingSep)
    {
      if (nSeps == 0)
      {
        if (sep1.length == 0) 
          pos1 = i;
        sep1 += c;
      }
      else if (nSeps == 1)
      {
        if (sep2.length == 0) 
          pos2 = i;
        sep2 += c;
      }
    }
  }
  if (sep1 == "")
  {
    // ddmmyy
    pos1 = 2
    sep1 = ""
    pos2 = 4
    sep2 = ""
  }
  else if (sep2 == "")
  {
    // ddxxxmmyy
    pos2 = pos1 + 2 + sep1.length
    sep2 = ""
  }
  if (pos1 == 4)
  {
    // ddmmxxxyy
    pos2 = pos1
    sep2 = sep1
    pos1 = 2
    sep1 = ""
  }
  
  
  if (ask && ((sep1.length<1) || (sep1.length>1) || (sep2.length>1) || (ValidStr(sep1, ",.- /").length>0) || (ValidStr(sep2, ",.- /").length>0)) )
    askOK = true;
//  alert(day+' '+month+' '+year)

//  var mySep = frmt.substring(frmt.length-1,frmt.length)
  var newdate = ""
  newdate += frmt.substring(0,frmt.indexOf('d'))
  if (frmt.indexOf('dd') >= 0)
    day   = fixedlength(str.substring(0,pos1),2,'0')
  newdate += day
  newdate += frmt.substring(frmt.lastIndexOf('d')+1,frmt.indexOf('m'))
  if (frmt.indexOf('mm') >= 0)
    month = fixedlength(str.substring(pos1+sep1.length,pos2),2,'0')
  newdate += month
  if (frmt.indexOf('y') > 0)
  {
    year  = str.substring(pos2+sep2.length,str.length)
    if (year.length>0)
    {
//alert('0 year = ' + year+ ' ' + y2k(year)+ ' ' + yyyy(y2k(year)))
      year  = ""+yyyy(y2k(year))
    }
    else 
    {
      var today=new Date();
      year=""+yyyy(y2k(today.getYear()));
    }
    var yearlen = frmt.lastIndexOf('y')-frmt.indexOf('y')+1
    newdate += frmt.substring(frmt.lastIndexOf('m')+1,frmt.indexOf('y'))
    newdate += year.substring(year.length-yearlen,year.length)
  }
  
//alert (newdate)    
  if (askOK)
    if (!confirm('Do you mean '+newdate+'?'))
        return false

  return newdate
}

function isPIShortDate(argvalue,separator)
{
  d = compress(argvalue).split(separator);
  day = d[0];
  month = d[1];
  year = 2000;  // 2000 is a leap year, so February 29 is valid

  if ( (!day) || (!month) || (!year) || (!(isDate(day,month,year))) )
    return false;
  return true;
}


// String operations
//-----------------------

function ValidStr (checkStr, OKStr)
{
  var invalid = "";
  for (var i = 0;  i < checkStr.length;  i++)
  {
    ch = checkStr.charAt(i);
    for (var j = 0;  j < OKStr.length;  j++)
      if (ch == OKStr.charAt(j))
        break;
    if (j == OKStr.length)
    {
      // We have reached the end of OKStr without finding ch. Therefore ch is an invalid character.
      invalid = ch;
      break;
    }
  }
  return (invalid);
}

function NonValidStr (checkStr, NonOKStr)
{
  var invalid = "";
  for (var i = 0;  i < checkStr.length;  i++)
  {
    ch = checkStr.charAt(i);
    for (var j = 0;  j < NonOKStr.length;  j++)
      if (ch == NonOKStr.charAt(j))
			{
        invalid = ch;
        break;
			}
  }
  return (invalid);
}

function ltrimsign(argvalue,sign)
{
  while ((argvalue.length>0) && (argvalue.substring(0,1) == sign))
    argvalue = argvalue.substring(1, argvalue.length);

  return argvalue;
}

function ltrim(argvalue)
{
  return ltrimsign(argvalue," ");
}

function rtrim(argvalue)
{
  while (argvalue.substring(argvalue.length - 1, argvalue.length) == " ")
    argvalue = argvalue.substring(0, argvalue.length - 1);

  return argvalue;
}

function trim(argvalue)
{
  var tmpstr = ltrim(argvalue);
  return rtrim(tmpstr);
}

function trimZero(argvalue)
{
  argvalue = ''+argvalue;
  if (argvalue.charAt(0) == '0')
  {
    // Remove leading zeros but accept 00...0 as 0
    argvalue=ltrimsign(argvalue,"0")
    if (argvalue == '')
      argvalue = '0'
  }
  return argvalue;
}

function replacechar(argvalue, oldch, newch)
{
  var newstr = "";
  for (var i = 0;  i < argvalue.length;  i++)
  {
    ch = argvalue.charAt(i);
    if (ch == oldch)
      newstr = newstr + newch;
    else
      newstr = newstr + ch;
  }
  return (newstr);
}

function compress(argvalue)
{
  return replacechar(argvalue," ", "")
}

function correctDecimal(argvalue,decimal)
{
  if (decimal == ",")
    return replacechar(''+argvalue, ".", ",")
  else
  {if (decimal == ".")
    return replacechar(''+argvalue, ",", ".")
  else
   return argvalue;}
}

function fixedlength(str,len,sign)
{
  // Give the string 'str' the length 'len' by adding a number of 'signs' before 'str'
  str=ltrimsign(str,sign)
  while (str.length<len)
    str=sign+str

  return str
}

