<!-- #include file="config.asp" -->
<%
dim con, cmd, recordSet, strStoredProcedure, Str
'**************************************************************************************************
sub InitFarmNDB()
   dim strConnection
'   strConnection = "Provider=SQLOLEDB.1;Data Source="&SQL_SERVER_ID&";Initial Catalog="&SQL_INITIAL_CATALOG_FARMNPRODUCTION_ANIMALCHANGE&";User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
'   strConnection = "Provider=SQLOLEDB.1;Data Source="&SQL_SERVER_ID&";Initial Catalog="&SQL_INITIAL_CATALOG_FARMN&";User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
   strConnection = "Provider=SQLOLEDB.1;Data Source=172.20.105.139;Initial Catalog=FarmN_AnimalChange_new;User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
   set con = Server.CreateObject("ADODB.Connection")
   con.Open(strConnection)
   set recordSet = Server.CreateObject("ADODB.recordset")
   set cmd = Server.CreateObject("ADODB.Command")
   set cmd.ActiveConnection = con
end sub
'**************************************************************************************************
sub CloseFarmNDB()
   set cmd = nothing
   set recordSet=nothing  
   con.Close()
   set con = nothing
end sub
'**************************************************************************************************
function ExecuteCommandWithParameter(sql,arr)'generel funktion til indsættelse af variabelt antal parametre i insertUpdate stored procedure
  dim p
  cmd.CommandType   = 4
  cmd.CommandText   = sql
  for p = 0 to UBound(arr)
    cmd.Parameters(p+1) = arr(p)
  next
  cmd.Execute
  if IsNull(cmd.Parameters(0)) then
    ExecuteCommandWithParameter = 0
  else
    ExecuteCommandWithParameter = cmd.Parameters(0)
  end if
end function
'**************************************************************************************************
'***Following functions are to replace almost all individual GetData-functions***
function CallSP(sql,arr,dict)
'response.write "<br>" & sql
  CallSP = GetData(sql,con,recordSet,arr,dict)
end function
'**************************************************************************************************
function CallSPColumnsRows(sql,arr,dictcol,dictrow,fieldArr)
'response.write "<br>" & sql
  CallSPColumnsRows = GetDataColumnsRows(sql,con,recordSet,arr,dictcol,dictrow,fieldArr)
end function
'**************************************************************************************************
function CallSPStr(sql,arr,dict)
'response.write "<br>" & sql
  CallSPStr = GetDataStr(sql,con,recordSet,arr,dict)
end function
'**************************************************************************************************
function CallSPReturn(sql, arr)
'response.write "<br>" & sql
  CallSPReturn = ExecuteCommandWithParameter(cmd,sql,arr)
end function
'**************************************************************************************************
function ExecuteCommandWithParameter(command,sql,arr)
  dim p
'	 response.write "<br>" &sql &" "
  command.CommandType   = 4
  command.CommandText   = sql
  for p = 0 to UBound(arr)
'  response.write arr(p)&"-P:"&p&" "
    command.Parameters(p+1) = arr(p)
  next
  command.Execute
  if IsNull(command.Parameters(0)) then
    ExecuteCommandWithParameter = 0
  else
    ExecuteCommandWithParameter = command.Parameters(0)
  end if
end function
'**************************************************************************************************
function GetData (byVal sql,connection,recordSet,arr,dict)
  dim recordCount
'	response.write "<br>" & sql
  recordSet.Open sql, connection, adOpenStatic, adLockReadOnly
  MakeDictCols dict, recordSet
  recordCount = RecSetToArray(arr, recordSet)
  recordSet.Close()
  GetData = recordCount
end Function
'**************************************************************************************************
function GetDataColumnsRows (byVal sql,connection,recordSet,arr,dictcol,dictrow,fieldArr)
  dim recordCount
'	response.write sql
  recordSet.Open sql, connection, adOpenStatic, adLockReadOnly
  MakeDictCols dictcol, recordSet
'  MakeDictRows dictrow, recordSet, fieldArr
  
'  recordCount = RecSetToArray(arr, recordSet)
  recordCount = RecSetToArrayColumnsRows(arr, recordSet,dictrow,fieldArr)
  recordSet.Close()
  GetDataColumnsRows = recordCount
end Function
'**************************************************************************************************
function GetDataStr (byVal sql,connection,recordSet,arr,dict)
  dim recordCount
'	response.write sql
  recordSet.Open sql, connection, adOpenStatic, adLockReadOnly
  MakeDictCols dict, recordSet
  recordCount = RecSetToArrayStr(arr, recordSet)
  recordSet.Close()
  GetDataStr = recordCount
end function

'**************************************************************************************************
sub MakeDictCols(dict, recordSet)
  dim c, x
  c = 0
  for each x in recordSet.fields
    dict.Add CStr(x.name), c
    c = c + 1
  next
end sub
'**************************************************************************************************
function RecSetToArrayColumnsRows(arr, recordSet,rowdict,fieldArr)
  dim r, x,rowName,n
  dim rows, cols, c
'  r = 0
  cols = FieldNumber(recordSet)
 
   do while not recordSet.EOF
    rows = rows + 1
    recordSet.MoveNext
  loop
  recordSet.MoveFirst

  reDim arr(rows-1,cols-1)
  for r=0 to rows-1
  rowName=""
    for n=0 to ubound(fieldArr)
     rowName=rowName&Trim(recordSet(cint(fieldArr(n))))
      if n<ubound(fieldArr) then
        rowName=rowName&"-"
      end if
    next 
'     response.write "<br/>rowName:"&rowName
'     response.write "<br/>r:"& r
    rowdict.Add CStr(rowName), r
    for c = 0 to cols-1
      arr(r,c) = "" & Trim(recordSet(c))
      If arr(r,c) = "" Then arr(r,c) = 0
	next
    recordSet.MoveNext
  next
RecSetToArrayColumnsRows=r  
end function
'**************************************************************************************************
function RecSetToArray(arr, recordSet)
  dim rows, cols, r, c
  cols = FieldNumber(recordSet)
  rows = RecNumber(recordSet)
  reDim arr(rows-1,cols-1)
  for r = 0 to rows-1
    for c = 0 to cols-1
      arr(r,c) = "" & Trim(recordSet(c))
      If arr(r,c) = "" Then arr(r,c) = 0
	next
    recordSet.MoveNext
  next
  RecSetToArray = rows
end Function
'**************************************************************************************************
function RecSetToArrayStr(arr, recordSet)
  dim rows, cols, r, c
  cols = FieldNumber(recordSet)
  rows = RecNumber(recordSet)
  reDim arr(rows-1,cols-1)
  for r = 0 to rows-1
    for c = 0 to cols-1
      arr(r,c) = "" & Trim(recordSet(c))
	next
    recordSet.MoveNext
  next
  RecSetToArrayStr = rows
end function

'**************************************************************************************************
function FieldNumber(recordSet)
  FieldNumber = recordSet.Fields.Count
end function
'**************************************************************************************************
function RecNumber(recordSet)
  dim rows
  rows = 0
  do while not recordSet.EOF
    rows = rows + 1
    recordSet.MoveNext
  loop
  if rows>0 then recordSet.MoveFirst
  RecNumber = rows
end function
'**************************************************************************************************
Sub Print2(anArray,dict)
    Dim a,r,c
  a = dict.Keys
  

Response.Write "<br><table border width=""200px"">" & Chr(13)

Response.Write "<tr>" & Chr(13)

for c = 0 to UBound(a)
  response.write "<th style=""font-size: 8px;"" > " & a(c) & " </th>" & Chr(13)
next

Response.Write "</tr>" & Chr(13)	

	For r = 0 to UBound(anArray)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(a)
        If isnull(anArray(r,dict.Item(a(c)))) Then
		Response.Write "<td> - </td>" & Chr(13)
        Else 
		Response.Write "<td style=""font-size: 12px;"">" & anArray(r,dict.Item(a(c))) & "</td>" & Chr(13)
		End if
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)

End Sub
'**************************************************************************************************
function InsertManureFertilizerDeliveryOpenXML(aXMLDoc)
   dim objXMLDOM

   if not isNull(aXMLDoc) then
      set objXMLDOM = Server.CreateObject("MSXML2.DOMDocument")
      if not objXMLDOM.loadXML(aXMLDoc) then
         InsertManureFertilizerDeliveryOpenXML = 0   
         Response.Write("<h3>XML error: "&objXMLDOM.parseError.reason&"</h3>" & vbCr)
         Response.End
      end if
   else
      Response.Write("<h3>XML error: xmlDoc isNull</h3>" & vbCr)
      Response.End
   end if
'response.write aXMLDoc
   cmd.CommandText = "InsertManureFertilizerDeliveryOpenXML"
   cmd.CommandType = adCmdStoredProc
   cmd.Parameters.Refresh
   cmd.Parameters.Item(1).Value = objXMLDOM.xml
   cmd.Execute
   
   set objXMLDOM = nothing
   
   InsertManureFertilizerDeliveryOpenXML = 1   
end function
'**************************************************************************************************

%>
