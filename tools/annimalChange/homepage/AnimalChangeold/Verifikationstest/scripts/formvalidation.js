// formvalidation.js
// Functions for validation of forms
// The functions are general in the sence that information about the form,
// e.g. name of the form and its elements, is not present.
// Therefore, the file can be included for different forms.

function ValidStr (checkStr, OKStr)
{
  var invalid = "";
  for (i = 0;  i < checkStr.length;  i++)
  {
    ch = checkStr.charAt(i);
    for (j = 0;  j < OKStr.length;  j++)
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

function ltrim(argvalue)
{
  while (argvalue.substring(0, 1) == " ")
  {
    argvalue = argvalue.substring(1, argvalue.length);
  }
  return argvalue;
}

function rtrim(argvalue)
{
  while (argvalue.substring(argvalue.length - 1, argvalue.length) == " ")
  {
    argvalue = argvalue.substring(0, argvalue.length - 1);
  }
  return argvalue;
}

function trim(argvalue)
{
  var tmpstr = ltrim(argvalue);

  return rtrim(tmpstr);
}

function focusfield(item)
{
  item.focus()
  item.select()
  return false;
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
  else if (hostpart.indexOf("@") != -1)			          // Exactly one @ - hence no in the host part
    return false;
  else if (hostpart.indexOf(".") == -1)			          // There must be at least one . in the host part
    return false;
  else if (hostpart.charAt(0) == ".")                 // . can not be the first char in the host part
    return false;
  else if (hostpart.charAt(hostpart.length-1) == ".")	// . can not be the last char in the host part
    return false;
  else if (namepart.charAt(0) == ".")                 // . can not be the first char of the name part
    return false;
  else if (ValidStr(argvalue, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-@%").length>0) return false;

  return true;
}

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

  var strLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  var strSpace   = " "
  var strNumber     = "0123456789";
  var strSign1   = "-_";
  var strSign2   = ",.:;)("

  var Txt1 = "Only letters, numbers and the \nspecial signs - and _ are allowed ";
  var Txt2 = "Only letters, numbers, space and the \nspecial signs , . : ; ( ) - and _ are allowed ";
  var Txt3 = "Only numbers are allowed ";

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
