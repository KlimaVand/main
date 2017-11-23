<!-- #include virtual="config.asp" -->
<%
dim con, cmd, recordSet, strStoredProcedure, Str
'**************************************************************************************************
sub InitFarmNDB()
   dim strConnection
   strConnection = "Provider=SQLOLEDB.1;Data Source="&SQL_SERVER_ID&";Initial Catalog="&SQL_INITIAL_CATALOG_FARMNPRODUCTION&";User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
'   strConnection = "Provider=SQLOLEDB.1;Data Source="&SQL_SERVER_ID&";Initial Catalog="&SQL_INITIAL_CATALOG_FARMN&";User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
'   strConnection = "Provider=SQLOLEDB.1;Data Source="&SQL_SERVER_ID&";Initial Catalog="&SQL_INITIAL_CATALOG_FARMNTEST&";User ID="&SQL_USER_ID&";PassWord="&SQL_PASSWORD
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
'function GetUserByLogin(aLoginName, aPassword, byref arr)
'   dim index,field
'   strStoredProcedure = "GetUserByLogin "&aLoginName&","&aPassword
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetUserByLogin = index   
'end function
'**************************************************************************************************
'function GetUserByEmail(aEmail, byref arr)
'   dim index,field
'   strStoredProcedure = "GetUserByEmail '"&aEmail&"'"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetUserByEmail = index   
'end function
''**************************************************************************************************
'function GetFarmList(aUserID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFarmList "&aUserID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFarmList = index   
'end function
''**************************************************************************************************
'function GetFarmData(aFarmNumber, byref arr)
''   dim index,field
'   strStoredProcedure = "GetFarmData "&aFarmNumber
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFarmData = index   
'end function
''**************************************************************************************************
'function GetFarmTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetFarmTypeList"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFarmTypeList = index   
'end function
''**************************************************************************************************
'function GetCattleHerdList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleHerdList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdList = index   
'end function
''**************************************************************************************************
'function GetCattleHerdMaxID(aFarmNumber, aScenarioID, byref arr)
'   dim index
'   strStoredProcedure = "GetCattleHerdMaxID "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdMaxID = index   
'end function
''**************************************************************************************************
'function GetCattleHerdStableStorageList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleHerdStableStorageList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdStableStorageList = index   
'end function
''**************************************************************************************************
'function GetCattleHerdCattleTypeList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleHerdCattleTypeList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdCattleTypeList = index   
'end function
''**************************************************************************************************
'function GetPigHerdPigTypeList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetPigHerdPigTypeList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetPigHerdPigTypeList = index   
'end function
''**************************************************************************************************
'function GetCattleHerdTotalList(aFarmNumber, aScenarioID, aCattleTypeID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleHerdTotalList "&aFarmNumber&","&aScenarioID&","&aCattleTypeID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'        arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdTotalList = index   
'end function
''**************************************************************************************************
'function GetPigHerdTotalList(aFarmNumber, aScenarioID, aPigTypeID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetPigHerdTotalList "&aFarmNumber&","&aScenarioID&","&aPigTypeID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'        arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetPigHerdTotalList = index   
'end function
''**************************************************************************************************
'function GetFarmTotalList(aFarmNumber, aScenarioID, aProductionTypeID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFarmTotalList "&aFarmNumber&","&aScenarioID&","&aProductionTypeID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFarmTotalList = index   
'end function
''**************************************************************************************************
'function GetSoilTypeList( byref arr)
'   dim index,field
'   strStoredProcedure = "GetSoilTypeList"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSoilTypeList = index   
'end function
''**************************************************************************************************
'function GetCattleHerdTotalData(aFarmNumber, aScenarioID, aCattleTypeID, aParameterID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleHerdTotalData "&aFarmNumber&","&aScenarioID&","&aCattleTypeID&","&aParameterID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCattleHerdTotalData = index   
'end function
''**************************************************************************************************
'function GetFarmTotalData(aFarmNumber, aScenarioID, aParameterID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFarmTotalData "&aFarmNumber&","&aScenarioID&","&aParameterID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFarmTotalData = index   
'end function
''**************************************************************************************************
'function GetCattleTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleTypeList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   recordSet.Close
'   
'   GetCattleTypeList = index   
'end function
''**************************************************************************************************
'function GetCattleStableList(aCattleTypeID,aStableID,byref arr)
'   dim index,field
'   strStoredProcedure = "GetCattleStableList "&aCattleTypeID&","&aStableID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   recordSet.Close
'   
'   GetCattleStableList = index   
'end function
''**************************************************************************************************
'function GetPigStableList(aPigTypeID,aStable1ID,aStable2ID,byref arr)
'   dim index,field
'   strStoredProcedure = "GetPigStableList "&aPigTypeID&","&aStable1ID&","&aStable2ID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   recordSet.Close
'   
'   GetPigStableList = index   
'end function
''**************************************************************************************************
'function GetPigHerdList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetPigHerdList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetPigHerdList = index   
'end function
''**************************************************************************************************
'function GetPigHerdMaxID(aFarmNumber, aScenarioID, byref arr)
'   dim index
'   strStoredProcedure = "GetPigHerdMaxID "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetPigHerdMaxID = index   
'end function
''**************************************************************************************************
'function GetPigTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetPigTypeList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetPigTypeList = index   
'end function
''**************************************************************************************************
'function GetStableList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetStableList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStableList = index   
'end function
''**************************************************************************************************
'function GetStableData(aFarmNumber, aScenarioID, aStableID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetStableData "&aFarmNumber&","&aScenarioID&","&aStableID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStableData = index   
'end function
''**************************************************************************************************
'function GetStableTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetStableTypeList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStableTypeList = index   
'end function
''**************************************************************************************************
'function GetStableTypeListSelect(aCattleTypeID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetStableTypeListSelect "&aCattleTypeID 
''   response.write strStoredProcedure
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStableTypeListSelect = index   
'end function
''**************************************************************************************************
'function GetStorageList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetStorageList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStorageList = index   
'end function
''**************************************************************************************************
'function GetStorageData(aFarmNumber, aScenarioID, aStorageID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetStorageData "&aFarmNumber&","&aScenarioID&","&aStorageID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStorageData = index   
'end function
''**************************************************************************************************
'function GetStorageTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetStorageTypeList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStorageTypeList = index   
'end function
''**************************************************************************************************
'function GetSalesOrderList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSalesOrderList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSalesOrderList = index   
'end function
''**************************************************************************************************
'function GetStorageTypeListNoGrazing(byref arr)
'   dim index,field
'   strStoredProcedure = "GetStorageTypeListNoGrazing" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStorageTypeListNoGrazing = index   
'end function
''**************************************************************************************************
'function DeleteCattleHerd(aFarmNumber, aScenarioID, aCattleHerdID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteCattleHerd"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aCattleHerdID
'   cmd.Execute
'   DeleteCattleHerd = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteCattleHerdTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteCattleHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteCattleHerdTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeletePigHerdTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeletePigHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeletePigHerdTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteFarmTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteFarmTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteFarmTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteResult(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteResult"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteResult = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateFarm(aFarmNumber, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown, aSoilType, aIrrigation, aDelivery, aFarmType, aSelectedScenario,aYieldLevel)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateFarm"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aFarmOwnerName
'   cmd.Parameters(3) = aFarmOwnerAddress
'   cmd.Parameters(4) = aFarmOwnerZipCode
'   cmd.Parameters(5) = aFarmOwnerTown
'   cmd.Parameters(6) = aSoilType
'   cmd.Parameters(7) = aIrrigation
''   cmd.Parameters(8) = aDelivery
'   cmd.Parameters(8) = aFarmType
'   cmd.Parameters(9)= aSelectedScenario
'   cmd.Parameters(10)= aYieldLevel
'   cmd.Execute
'   InsertUpdateFarm = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateCattleHerd(aFarmNumber, aScenarioID, aCattleHerdID, aCattleType, aAnimalYear, aGrazing, aProductionLevel, aProductionLevel2, aEffectivity, aStableType)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateCattleHerd"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aCattleHerdID
'   cmd.Parameters(4) = aCattleType
'   cmd.Parameters(5) = aAnimalYear
'   cmd.Parameters(6) = aGrazing
'   cmd.Parameters(7) = aProductionLevel
'   cmd.Parameters(8) = aProductionLevel2
'   cmd.Parameters(9) = aEffectivity
'   cmd.Parameters(10) = aStableType
'   cmd.Execute
'   InsertUpdateCattleHerd = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateCattleHerdTotal(aFarmNumber, aScenarioID, aCattleType, aParameterID, aParameterValue)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateCattleHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aCattleType
'   cmd.Parameters(4) = aParameterID
'   cmd.Parameters(5) = aParameterValue
'   cmd.Execute
'   InsertUpdateCattleHerdTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertNewUser(aUserID, aUserLoginName, aUserPassword, aUserName)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertNewUser"
'   cmd.Parameters(1) = aUserID
'   cmd.Parameters(2) = aUserLoginName
'   cmd.Parameters(3) = aUserPassword
'   cmd.Parameters(4) = aUserName
'   cmd.Execute
'   InsertNewUser = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function IncrementCattleHerdTotal(aFarmNumber, aScenarioID, aCattleType, aParameterID, aParameterValue)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "IncrementCattleHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aCattleType
'   cmd.Parameters(4) = aParameterID
'   cmd.Parameters(5) = aParameterValue
'   cmd.Execute
'   IncrementCattleHerdTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function IncrementPigHerdTotal(aFarmNumber, aScenarioID, aPigType, aParameterID, aParameterValue)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "IncrementPigHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aPigType
'   cmd.Parameters(4) = aParameterID
'   cmd.Parameters(5) = aParameterValue
'   cmd.Execute
'   IncrementPigHerdTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateFarmTotal(aFarmNumber, aScenarioID, aParameterID, aParameterValue)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateFarmTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aParameterID
'   cmd.Parameters(4) = aParameterValue
'   cmd.Execute
'   InsertUpdateFarmTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeletePigHerd(aFarmNumber, aScenarioID, aPigHerdID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeletePigHerd"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aPigHerdID
'   cmd.Execute
'   DeletePigHerd = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdatePigHerd(aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProteinLevel, aFeedLevel, aProductionVar1, aProductionVar2, aProductionVar3, aStableType1, aStableType2)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdatePigHerd"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aPigHerdID
'   cmd.Parameters(4) = aPigType
'   cmd.Parameters(5) = aAnimalYear
'   cmd.Parameters(6) = aProteinLevel
'   cmd.Parameters(7) = aFeedLevel
'   cmd.Parameters(8) = aProductionVar1
'   cmd.Parameters(9) = aProductionVar2
'   cmd.Parameters(10) = aProductionVar3
'   cmd.Parameters(11) = aStableType1
'   cmd.Parameters(12) = aStableType2
'   cmd.Execute
'   InsertUpdatePigHerd = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function GetFieldPlanList(aFarmNumber, aScenarioID, aRotationID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetFieldPlanList "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanList = index   
'end function
''**************************************************************************************************
'function GetFieldPlanRotationList(aFarmNumber, aScenarioID, aRotationID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetFieldPlanRotationList_1 "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanRotationList = index   
'end function
''**************************************************************************************************
'function GetFieldPlanMaxID(aFarmNumber, aScenarioID, aRotationID, byref arr)
'   dim index
'   strStoredProcedure = "GetFieldPlanMaxID "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanMaxID = index   
'end function
''**************************************************************************************************
'function GetFirstUnusedCrop(aFarmNumber, aScenarioID, aRotationID)
'   dim index
'   strStoredProcedure = "GetFirstUnusedCrop "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   If not recordSet.EOF then
'      GetFirstUnusedCrop = recordSet("CropID")
'   else
'      GetFirstUnusedCrop=1
'   end if
'   recordSet.Close
'end function
''**************************************************************************************************
'function GetFieldPlanNumberCropsPrRotation(aFarmNumber, aScenarioID, aRotationID)
'   dim index
'   strStoredProcedure = "GetNumberFieldPlanCropsPrRotation "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   GetFieldPlanNumberCropsPrRotation = recordSet("NumberCrops") 
'   
'   recordSet.Close
'end function
''**************************************************************************************************
'function GetFieldPlanTotalList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetFieldPlanTotalList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanTotalList = index   
'end function
''**************************************************************************************************
'function GetGrazingManure(aFarmNumber, aScenarioID, aRotationID)
'   dim index,field,aGrazingManure
'   
'   strStoredProcedure = "GetGrazingManure "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
''   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         aGrazingManure = aGrazingManure + recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetGrazingManure = aGrazingManure   
'end function
''**************************************************************************************************
'function GetTotalGrazingManure(aFarmNumber, aScenarioID, aRotationID)
'   dim index,field,aGrazingManure
'   
'   strStoredProcedure = "GetTotalGrazingManure "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         aGrazingManure = aGrazingManure + recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetTotalGrazingManure = aGrazingManure   
'end function
''**************************************************************************************************
'function GetFertilizerBeforeLoss(aFarmNumber, aScenarioID, aRotationID)
'   dim index,field,aGetFertilizerBeforeLoss
'   
'   strStoredProcedure = "GetFertilizerBeforeLoss1 "&aFarmNumber&","&aScenarioID&","&aRotationID
''Response.write "<br> udskrift " & strStoredProcedure
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         aGetFertilizerBeforeLoss = aGetFertilizerBeforeLoss + recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   if isnull(aGetFertilizerBeforeLoss) then aGetFertilizerBeforeLoss=0
'   recordSet.Close
'   GetFertilizerBeforeLoss = aGetFertilizerBeforeLoss  
'end function
'**************************************************************************************************
'function GetConversionFactor(aStorageType)
'   dim index
'   strStoredProcedure = "GetConversionFactorForStorageType "&aStorageType
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   GetConversionFactor = recordSet(recordSet(0).name) 
'   
'   recordSet.Close
'end function
''**************************************************************************************************
'function GetManureNSpread(aFarmNumber, aScenarioID, aRotationID)
'   dim index
'   strStoredProcedure = "GetManureNSpread "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   If not recordSet.EOF then
'      GetManureNSpread = recordSet(recordSet(0).name)
'   else
'      GetManureNSpread=0
'   end if
'   recordSet.Close
'end function
'**************************************************************************************************
'function GetManureNIncorp(aFarmNumber, aScenarioID, aRotationID)
'   dim index
'   strStoredProcedure = "GetManureNIncorp "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   If not recordSet.EOF then
'       if not IsNull(recordSet(recordSet(0).name)) then 
'	      GetManureNIncorp = recordSet(recordSet(0).name)
'	   else
'	      GetManureNIncorp=0
'	   end if
'   else
'      GetManureNIncorp=0
'   end if
'   recordSet.Close
'end function
''**************************************************************************************************
'function GetSummarizedCerealYield(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedCerealYield_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedCerealYield = index   
'end function
''**************************************************************************************************
'function GetSummarizedSoldCrop(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedSoldCrop_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedSoldCrop = index   
'end function
''**************************************************************************************************
'function GetSummarizedNonCerealYield(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedNonCerealYield_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedNonCerealYield = index   
'end function
''**************************************************************************************************
'function GetSummarizedGrazingYield(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedGrazingYield_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedGrazingYield = index   
'end function
''**************************************************************************************************
'function GetSummarizedNonGrazedYield(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedNonGrazedYield_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedNonGrazedYield = index   
'end function
''**************************************************************************************************
'function GetSummarizedStrawKg(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetSummarizedStrawKg_1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSummarizedStrawKg = index   
'end function
''**************************************************************************************************
'function GetTotalCarbonFromManure(aFarmNumber, aScenarioID, aRotationID)
'   dim totalCarbon, field
'   
'   strStoredProcedure = "GetTotalCarbonFromManure "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
''   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
''   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   totalCarbon = recordSet(recordSet(field).name)
'		 else 
'		   totalCarbon = 0
'		 end if
'      next
''      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetTotalCarbonFromManure = totalCarbon   
'end function
'**************************************************************************************************
'function GetTotalCarbonFromCrops(aFarmNumber, aScenarioID, aRotationID)
'   dim totalCarbon, field
'   
'   strStoredProcedure = "GetTotalCarbonFromCrops "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   if not isnull(recordSet("totalCarbonFromCrops")) then
'		   totalCarbon = recordSet("totalCarbonFromCrops")
'   else
''   recordSet.Close
''   strStoredProcedure = "GetTotalCarbonFromCrops1 "&aFarmNumber&","&aScenarioID
''   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'		   totalCarbon = -10000
'   end if
'   
'   recordSet.Close
' '  Response.write "<br> GetTotalCarbonFromCrops i dbinterface " & totalCarbon
'   GetTotalCarbonFromCrops = totalCarbon   
'end function
''**************************************************************************************************
'sub GetTotalCarbonFromCrops_Test(aFarmNumber, aScenarioID,arr)
'   dim totalCarbon, field, index
'   
'   strStoredProcedure = "GetTotalCarbonFromCrops_Test "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   if isnull(recordSet("CarbonTop")) then
'   else
'   recordSet.Close
'   strStoredProcedure = "GetTotalCarbonFromCrops1_Test "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   end if
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         if not IsNull(recordSet(recordSet(field).name)) then
'		   arr(field,index) = recordSet(recordSet(field).name)
'		 else 
'		   arr(field,index) = 0
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
''   GetTotalCarbonFromCrops = totalCarbon   
'end sub
''**************************************************************************************************
'function GetTotalNInSeed(aFarmNumber, aScenarioID)
'   dim totalN, field
'   
'   strStoredProcedure = "GetTotalNInSeed "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   totalN = recordSet(recordSet(0).name)
'   
'   recordSet.Close
'   
'   if isnull(totalN) then totalN=0
'
'   GetTotalNInSeed = totalN   
'end function
''**************************************************************************************************
'function GetTotalNNeed(aFarmNumber, aScenarioID)
'   dim totalLoss, field
'   
'   strStoredProcedure = "GetTotalNNeed "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   If not recordSet.EOF then
'       if not IsNull(recordSet(recordSet(0).name)) then 
'	      totalLoss = recordSet(recordSet(0).name)
'	   else
'	      totalLoss = 0
'	   end if
'   else
'      totalLoss = 0
'   end if
'   
'   recordSet.Close
'   GetTotalNNeed = totalLoss   
'end function
''**************************************************************************************************
'function GetTotalLossManure(aFarmNumber, aScenarioID)
'   dim totalLoss, field
'   
'   strStoredProcedure = "GetTotalLossManure "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'   If not recordSet.EOF then
'       if not IsNull(recordSet(recordSet(0).name)) then 
'	      totalLoss = recordSet(recordSet(0).name)
'	   else
'	      totalLoss = 0
'	   end if
'   else
'      totalLoss = 0
'   end if
'   
'   recordSet.Close
'   GetTotalLossManure = totalLoss   
'end function
''**************************************************************************************************
'function GetManure_N_Content(aFarmNumber, aScenarioID, byref arr)
'   dim index,field,totalNContent
'   
'   strStoredProcedure = "GetManure_N_Content "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="Manure_N" then totalNContent=totalNContent+Round(arr(field,index),2)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManure_N_Content = totalNContent  
'end function
''**************************************************************************************************
'function GetManureNorm(aFarmNumber, aScenarioID, byref arr)
'   dim index,field,totalNContent
'   
'   strStoredProcedure = "GetManureNorm "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="Manure_N" then totalNContent=totalNContent+Round(arr(field,index),2)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManureNorm = totalNContent  
'end function
''**************************************************************************************************
'function GetSoldManure(aFarmNumber, aScenarioID, byref arr)
'   dim index,field,totalNContent
'   
'   strStoredProcedure = "GetSoldManure "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="SoldManure" then totalNContent=totalNContent+Round(arr(field,index),2)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSoldManure = totalNContent  
'end function
''**************************************************************************************************
'function GetBoughtManure(aFarmNumber, aScenarioID, byref arr)
'   dim index,field,totalNContent
'   
'   strStoredProcedure = "GetBoughtManure "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="BoughtManure" then totalNContent=totalNContent+Round(arr(field,index),2)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBoughtManure = totalNContent  
'end function
''**************************************************************************************************
'function GetManureShareOfN_Need(aFarmNumber, aScenarioID, aMeanUtilization, byref arr)
'   dim index,field,totalNNeed,i
'   
'   strStoredProcedure = "GetManureShareOfN_Need "&aFarmNumber&","&aScenarioID&",'"&aMeanUtilization&"'"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
' 
'  
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   totalNNeed = 0
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'	 arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="ManureShareOfN_Need" then
'		    if recordSet(recordSet(field).name)>=0 then
'			  totalNNeed = totalNNeed+Round(arr(field,index),2)
'			else
'			  arr(field,index)=0
'			end if
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManureShareOfN_Need = totalNNeed  
'end function
''**************************************************************************************************
'function GetManureShareOfN_Need1(aFarmNumber, aScenarioID, byref arr)
'   dim index,field,totalNNeed,i
'   
'   strStoredProcedure = "GetManureShareOfN_Need1 "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'		 if (recordSet(field).name)="ManureShareOfN_Need" then
'		    if recordSet(recordSet(field).name)>=0 then
'			  totalNNeed=totalNNeed+Round(arr(field,index),2)
'			else
'			  arr(field,index)=0
'			end if
'		 end if
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManureShareOfN_Need1 = totalNNeed  
'end function
''**************************************************************************************************
'function GetManureFertilizerDelivery(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   
'   strStoredProcedure = "GetManureFertilizerDelivery "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManureFertilizerDelivery = index   
'end function
''**************************************************************************************************
'function GetLoss(aStorageID, aDeliveryID)
'   dim index,field,loss
'   
'   strStoredProcedure = "GetLoss "&aStorageID&","&aDeliveryID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'         loss = recordSet(recordSet(0).name)
'   
'   recordSet.Close
'   GetLoss = loss   
'end function
''**************************************************************************************************
'function GetUserLossList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetUserLossList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetUserLossList = index   
'end function
''**************************************************************************************************
'function GetUtilizationDegree(aFarmNumber, aScenarioID,aRotationID,aFieldPlanRotationID,aStorageID,a,d)
'   dim index,utilizationDegree
'   
'   strStoredProcedure = "GetUtilizationDegree "&aFarmNumber&","&aScenarioID&","&aRotationID&","&aFieldPlanRotationID&","&aStorageID
' 'Response.write "<br> udskrift " & strStoredProcedure
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   if not recordSet.eof then
'	   utilizationDegree = recordSet(recordSet(0).name)
'	   a                 = recordSet(recordSet(1).name)
'	   d                 = recordSet(recordSet(2).name)
'   else
'	   utilizationDegree = Null
'	   a                 = Null
'	   d                 = Null
'   end if
'   recordSet.Close
'   GetUtilizationDegree = utilizationDegree  
'end function
''**************************************************************************************************
'function GetFieldPlanCropList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFieldPlanCropList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanCropList = index   
'end function
''**************************************************************************************************
'function GetFieldPlanCropAndAreaList(aFarmNumber, aScenarioID, RotationID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFieldPlanCropAndAreaList "&aFarmNumber&","&aScenarioID&","&RotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanCropAndAreaList = index   
'end function
''**************************************************************************************************
'function GetFieldPlanAreaSum(aFarmNumber, aScenarioID, aSoilTypeID, aIrrigation, byref arr)
'   dim index,field
'   strStoredProcedure = "GetFieldPlanAreaSum "&aFarmNumber&","&aScenarioID&","&aSoilTypeID&","&aIrrigation
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetFieldPlanAreaSum = index   
'end function
'**************************************************************************************************
'function GetRotationList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetRotationList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetRotationList = index   
'end function
''**************************************************************************************************
'function GetRotationMaxID(aFarmNumber, aScenarioID, byref arr)
'   dim index
'   strStoredProcedure = "GetRotationMaxID "&aFarmNumber&","&aScenarioID
''response.write strStoredProcedure
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetRotationMaxID = index   
'end function
''**************************************************************************************************
'function GetRotationSoilType(aFarmNumber, aScenarioID, aRotationID)
'   dim field, arr, index
'   strStoredProcedure = "GetRotationSoilType "&aFarmNumber&","&aScenarioID&","&aRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("SoilType")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetRotationSoilType = arr(0,0)
'end function
'**************************************************************************************************
'function GetCropList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetCropList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCropList = index   
'end function
''**************************************************************************************************
'function GetCropDefaultYieldList(soilType,irrigation,cropString,notPreCropGroup,aYieldLevel,byref arr)
'   dim index,field,i,j
'   strStoredProcedure = "GetCropDefaultYieldList "&soilType&","&irrigation&",'"&cropString&"',"&notPreCropGroup&","&aYieldLevel 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''response.write strStoredProcedure
'
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCropDefaultYieldList = index   
'end function
''**************************************************************************************************
'function GetCropDefaultPriceList(soilType,irrigation,cropString,notPreCropGroup,aYieldLevel,byref arr)
'   dim index,field,i,j
'   strStoredProcedure = "GetCropDefaultPriceList "&soilType&","&irrigation&",'"&cropString&"',"&notPreCropGroup&","&aYieldLevel 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''response.write strStoredProcedure
'
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetCropDefaultPriceList = index   
'end function
''**************************************************************************************************
'function GetParameterList(aProductionType,byref arr)
'   dim index,field
'   strStoredProcedure = "GetParameterList "& aProductionType 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetParameterList = index   
'end function
''**************************************************************************************************
'function GetScenarioList(aFarmNumber, byref arr)
'   dim index,field
'   strStoredProcedure = "GetScenarioList "&aFarmNumber
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-5
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetScenarioList = index   
'end function
''**************************************************************************************************
'function GetStrawUseTypeList(byref arr)
'   dim index,field
'   strStoredProcedure = "GetStrawUseTypeList" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to 1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetStrawUseTypeList = index   
'end function
''**************************************************************************************************
'function DeleteFieldPlan(aFarmNumber, aScenarioID, aRotationID, aFieldPlanID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteFieldPlan"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aRotationID
'   cmd.Parameters(4) = aFieldPlanID
'   cmd.Execute
'   DeleteFieldPlan = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteFieldPlanRotation(aFarmNumber, aScenarioID, aRotationID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteFieldPlanRotation"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aRotationID
'   cmd.Execute
'   DeleteFieldPlanRotation = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteManureFertilizerDelivery(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteManureFertilizerDelivery"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteManureFertilizerDelivery = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteBoughtManure(aFarmNumber, aScenarioID,aBoughtManureID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteBoughtManure "
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aBoughtManureID
'   cmd.Execute
'   DeleteBoughtManure = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteRotation(aFarmNumber, aScenarioID, aRotationID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteRotation"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aRotationID
'   cmd.Execute
'   DeleteRotation = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteFieldPlanTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteFieldPlanTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteFieldPlanTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateFieldPlan(aFarmNumber, aScenarioID, aRotation, aFieldPlanID, aCrop, aArea)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateFieldPlan"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aRotation
'   cmd.Parameters(4)  = aFieldPlanID
'   cmd.Parameters(5)  = aCrop
'   cmd.Parameters(6)  = aArea
'   cmd.Execute
'   InsertUpdateFieldPlan = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateRotation(aFarmNumber, aScenarioID, aRotationID, aRotationName, aSoilTypeID, aIrrigation, aYieldLevel)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateRotation"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aRotationID
'   cmd.Parameters(4)  = aRotationName
'   cmd.Parameters(5)  = aSoilTypeID
'   cmd.Parameters(6)  = aIrrigation
'   cmd.Parameters(7)  = aYieldLevel
'   cmd.Execute
'   InsertUpdateRotation = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateFieldPlanRotation(aFarmNumber, aScenarioID, aRotationID, aFieldPlanRotationID, aPreviousCropID, aPreCropOriginalID, aCropID, aArea, aOrganicFertilizer, aGrazingPart, aSalePart, aStrawUseType, aUnderSown)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateFieldPlanRotation"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aRotationID
'   cmd.Parameters(4)  = aFieldPlanRotationID
'   cmd.Parameters(5)  = aPreviousCropID
'   cmd.Parameters(6)  = aPreCropOriginalID
'   cmd.Parameters(7)  = aCropID
'   cmd.Parameters(8)  = aArea
'   cmd.Parameters(9)  = aOrganicFertilizer
'   cmd.Parameters(10)  = aGrazingPart
'   cmd.Parameters(11) = aSalePart
'   cmd.Parameters(12) = aStrawUseType
'   cmd.Parameters(13) = aUnderSown
'   cmd.Execute
'   InsertUpdateFieldPlanRotation = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateManureFertilizerDelivery(aFarmNumber, aScenarioID, aRotationID, aFieldPlanRotationID, aStorageID, aDeliveryID, aKg_N_Delivered, aKg_N_Loss, aKg_N_Utilized)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateManureFertilizerDelivery"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aRotationID
'   cmd.Parameters(4)  = aFieldPlanRotationID
'   cmd.Parameters(5)  = aStorageID
'   cmd.Parameters(6)  = aDeliveryID
'   cmd.Parameters(7)  = aKg_N_Delivered
'   cmd.Parameters(8)  = aKg_N_Loss
'   cmd.Parameters(9)  = aKg_N_Utilized
'   cmd.Execute
'   InsertUpdateManureFertilizerDelivery = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateFieldPlanTotal(aParameterID, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateFieldPlanTotal"
'   cmd.Parameters(1)  = aParameterID
'   cmd.Parameters(2)  = aFarmNumber
'   cmd.Parameters(3)  = aScenarioID
'   cmd.Parameters(4)  = aRotationID
'   cmd.Parameters(5)  = aSoilTypeID
'   cmd.Parameters(6)  = aIrrigation
'   cmd.Execute
'   InsertUpdateFieldPlanTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function GetBalanceFeedData(aFarmNumber, aScenarioID, aCropGroupID, aBalanceID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetBalanceFeedData "&aFarmNumber&","&aScenarioID&","&aCropGroupID&","&aBalanceID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBalanceFeedData = index   
'end function
''**************************************************************************************************
'function GetBalanceFeedList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetBalanceFeedList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBalanceFeedList = index   
'end function
''**************************************************************************************************
'function GetBalanceStorageData(aFarmNumber, aScenarioID, aStorageID, aBalanceID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetBalanceStorageData "&aFarmNumber&","&aScenarioID&","&aStorageID&","&aBalanceID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBalanceStorageData = index   
'end function
''**************************************************************************************************
'function GetBalanceStorageList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetBalanceStorageList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBalanceStorageList = index   
'end function
''**************************************************************************************************
'function GetBoughtManureList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetBoughtManureList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBoughtManureList = index   
'end function
''**************************************************************************************************
'function GetSoldManureList(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetSoldManureList "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSoldManureList = index   
'end function
''**************************************************************************************************
'function GetResultList(aFarmNumber, aScenarioID, aResultTypeID, byref arr)
'   dim index,field
'   strStoredProcedure = "GetResultList "&aFarmNumber&","&aScenarioID&","&aResultTypeID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-5,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-5
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetResultList = index   
'end function
''**************************************************************************************************
'function CalcSumArea(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "CalcSumArea "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   CalcSumArea = index   
'end function
''**************************************************************************************************
'function CalcTotalMilkkg(aFarmNumber, aScenarioID, byref arr)
'   dim index,field
'   strStoredProcedure = "CalcTotalMilkkg "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      for field=0 to recordSet.fields.Count-1
'         arr(field,index) = recordSet(recordSet(field).name)
'      next
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   CalcTotalMilkkg = index   
'end function
''**************************************************************************************************
'function CalcSumResult(aFarmNumber, aScenarioID, aResultType)
'   dim sum
'   
'   strStoredProcedure = "CalcSumResult "&aFarmNumber&","&aScenarioID&","&aResultType
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'         sum = recordSet(recordSet(0).name)
'   
'   recordSet.Close
'   CalcSumResult = sum   
'end function
''**************************************************************************************************
'function InsertUpdateResult(aFarmNumber, aScenarioID, aResultID, aResultValue)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertUpdateResult"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aResultID
'   cmd.Parameters(4)  = aResultValue
'   cmd.Execute
'   InsertUpdateResult = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function CalcFarmTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "CalcFarmTotal"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Execute
'   CalcFarmTotal = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteStable(aFarmNumber, aScenarioID, aProductionTypeID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteStable"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aProductionTypeID
'   cmd.Execute
'   DeleteStable = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function IncrementStable(aFarmNumber, aScenarioID, aProductionTypeID, aStableID, aDryMatterLoss, aDryMatterAfterLoss, aNitrogenLoss, aNitrogenAfterLoss)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "IncrementStable"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aProductionTypeID
'   cmd.Parameters(4)  = aStableID
'   cmd.Parameters(5)  = aDryMatterLoss
'   cmd.Parameters(6)  = aDryMatterAfterLoss
'   cmd.Parameters(7)  = aNitrogenLoss
'   cmd.Parameters(8)  = aNitrogenAfterLoss
'   cmd.Execute
'   IncrementStable = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteStorage(aFarmNumber, aScenarioID, aProductionTypeID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteStorage"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aProductionTypeID
'   cmd.Execute
'   DeleteStorage = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteStorageNorm(aFarmNumber, aScenarioID, aProductionTypeID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteStorageNorm"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aProductionTypeID
'   cmd.Execute
'   DeleteStorageNorm = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function IncrementStorage(aFarmNumber, aScenarioID, aProductionTypeID, aStorageID, aNitrogenStableBeforeLoss, aNitrogenStableLoss, aNitrogenStableAfterLoss, aNitrogenStorageLossAmmonia, aNitrogenStorageLossDenitrification, aNitrogenStorageAfterLoss, aDryMatterStableBeforeLoss, aDryMatterStableLoss, aDryMatterStableAfterLoss, aDryMatterStorageLoss, aDryMatterStorageAfterLoss)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "IncrementStorage"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aProductionTypeID
'   cmd.Parameters(4)  = aStorageID
'   cmd.Parameters(5)  = aNitrogenStableBeforeLoss
'   cmd.Parameters(6)  = aNitrogenStableLoss
'   cmd.Parameters(7)  = aNitrogenStableAfterLoss
'   cmd.Parameters(8)  = aNitrogenStorageLossAmmonia
'   cmd.Parameters(9)  = aNitrogenStorageLossDenitrification
'   cmd.Parameters(10) = aNitrogenStorageAfterLoss
'   cmd.Parameters(11) = aDryMatterStableBeforeLoss
'   cmd.Parameters(12) = aDryMatterStableLoss
'   cmd.Parameters(13) = aDryMatterStableAfterLoss
'   cmd.Parameters(14) = aDryMatterStorageLoss
'   cmd.Parameters(15) = aDryMatterStorageAfterLoss
'   cmd.Execute
'   IncrementStorage = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function IncrementStorageNorm(aFarmNumber, aScenarioID, aProductionTypeID, aStorageID, aNitrogenNoGrazingAfterLoss)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "IncrementStorageNorm"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aProductionTypeID
'   cmd.Parameters(4)  = aStorageID
'   cmd.Parameters(5)  = aNitrogenNoGrazingAfterLoss
'   cmd.Execute
'   IncrementStorageNorm = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateBalanceStorage(aFarmNumber, aScenarioID, aStorageID, aBalanceID, aBalanceValue)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateBalanceStorage"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Parameters(4) = aBalanceID
'   cmd.Parameters(5) = aBalanceValue
'   cmd.Execute
'   InsertUpdateBalanceStorage = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateBalanceFeed(aFarmNumber, aScenarioID, aStorageID, aBalanceID, aBalanceValue)
'   cmd.CommandType   = 4 'stored procedure
'   cmd.CommandText   = "InsertUpdateBalanceFeed"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Parameters(4) = aBalanceID
'   cmd.Parameters(5) = aBalanceValue
'   cmd.Execute
'   InsertUpdateBalanceFeed = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function GetSelectListScenario(aFarmNumber,byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListScenario "&aFarmNumber
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListScenario = index   
'end function
''**************************************************************************************************
'function GetSelectListSoilType(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListSoilType"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListSoilType = index   
'end function
''**************************************************************************************************
'function GetSelectListFarmType(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListFarmType"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListFarmType = index   
'end function
''**************************************************************************************************
'function GetSelectListCrop(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListCrop"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListCrop = index   
'end function
''**************************************************************************************************
''function GetSelectListStrawUseType(byref arr)
''   dim index,field
''   strStoredProcedure = "GetSelectListStrawUseType"
''   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''
''   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
''   index = 0
''   
''   while not recordSet.eof
''     for field=0 to recordSet.fields.Count-1
''      arr(field,index) = recordSet(recordSet(field).name)
''     next
''     index = index + 1
''     recordSet.MoveNext
''   wend
''   
''   recordSet.Close
''   GetSelectListStrawUseType = index   
''end function
''**************************************************************************************************
'function GetSelectListCattleType(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListCattleType"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListCattleType = index   
'end function
''**************************************************************************************************
'function GetSelectListPigType(byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListPigType"
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListPigType = index   
'end function
''**************************************************************************************************
'function GetSelectListStableType(aCattleType,byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListStableType "&aCattleType
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListStableType = index   
'end function
''**************************************************************************************************
'function GetSelectListStableTypePig(aPigType,aPigTypeSub,byref arr)
'   dim index,field
'   strStoredProcedure = "GetSelectListStableTypePig "&aPigType&","&aPigTypeSub
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSelectListStableTypePig = index   
'end function
'
''**************************************************************************************************
'function GetNumberParameter(aProductionTypeID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNumberParameter"
'   cmd.Parameters(1) = aProductionTypeID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNumberParameter = 0
'   else
'      GetNumberParameter = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStableBeforeLoss(aFarmNumber, aScenarioID, aStorageID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStableBeforeLoss"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStableBeforeLoss = 0
'   else
'      GetNitrogenStableBeforeLoss = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStableLoss(aFarmNumber, aScenarioID, aStorageID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStableLoss"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStableLoss = 0
'   else
'      GetNitrogenStableLoss = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStableLossTotal(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStableLossTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStableLossTotal = 0
'   else
'      GetNitrogenStableLossTotal = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStorageLoss(aFarmNumber, aScenarioID, aStorageID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStorageLoss"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStorageLoss = 0
'   else
'      GetNitrogenStorageLoss = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStorageLossTotal(aFarmNumber, aScenarioID) ' Dette er kun Ammonia delen.
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStorageLossTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStorageLossTotal = 0
'   else
'      GetNitrogenStorageLossTotal = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenStorageDenitrificationTotal(aFarmNumber, aScenarioID) ' Dette er kun Ammonia delen.
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenStorageDenitrificationTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenStorageDenitrificationTotal = 0
'   else
'      GetNitrogenStorageDenitrificationTotal = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenSpreadLoss(aFarmNumber, aScenarioID, aStorageID) 
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenSpreadLoss"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenSpreadLoss = 0
'   else
'      GetNitrogenSpreadLoss = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenFieldNeed(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenFieldNeed"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenFieldNeed = 0
'   else
'      GetNitrogenFieldNeed = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetNitrogenFieldUse(aFarmNumber, aScenarioID, aStorageID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetNitrogenFieldUse"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetNitrogenFieldUse = 0
'   else
'      GetNitrogenFieldUse = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetFarmTotal(aFarmNumber, aScenarioID, aParamaterID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetFarmTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aParamaterID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetFarmTotal = 0
'   else
'      GetFarmTotal = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetPigHerdTotal(aFarmNumber, aScenarioID, aPigTypeID, aParamaterID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetPigHerdTotal"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aPigTypeID
'   cmd.Parameters(4) = aParamaterID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetPigHerdTotal = 0
'   else
'      GetPigHerdTotal = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetTotalArea(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetTotalArea"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetTotalArea = 0
'   else
'      GetTotalArea = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetTotalAreaRotation(aFarmNumber, aScenarioID, aRotationID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetTotalAreaRotation"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aRotationID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetTotalAreaRotation = 0
'   else
'      GetTotalAreaRotation = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetFertEqProd(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetFertEqProd"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetFertEqProd = 0
'   else
'      GetFertEqProd = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetFertEqNorm(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetFertEqNorm"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetFertEqNorm = 0
'   else
'      GetFertEqNorm = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function GetTotalAnimalUnitCattle(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetTotalAnimalUnitCattle"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetTotalAnimalUnitCattle = 0
'   else
'      GetTotalAnimalUnitCattle = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function InsertFertilizer(aFarmNumber, aScenarioID, aFertilizerValue)
'   cmd.CommandType    = 4 'sp
'   cmd.CommandText    = "InsertFertilizer"
'   cmd.Parameters(1)  = aFarmNumber
'   cmd.Parameters(2)  = aScenarioID
'   cmd.Parameters(3)  = aFertilizerValue
'   cmd.Execute
'   InsertFertilizer = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteFertilizer(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteFertilizer"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteFertilizer = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function GetFertilizer(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'default
'   cmd.CommandText   = "GetFertilizer"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   if IsNull(cmd.Parameters(0)) then
'      GetFertilizer = 0
'   else
'      GetFertilizer = cmd.Parameters(0)
'   end if
'end function
''**************************************************************************************************
'function InsertUpdateBoughtSoldManure(aFarmNumber, aScenarioID, aStorageID, aBoughtManure, aSoldManure)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateBoughtSoldManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aStorageID
'   cmd.Parameters(4) = aBoughtManure
'   cmd.Parameters(5) = aSoldManure
'   cmd.Execute
'   InsertUpdateBoughtSoldManure = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateBoughtManure(aFarmNumber, aScenarioID, aBoughtManureID, aBoughtManureAmount, aBoughtManureType)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateBoughtManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aBoughtManureID
'   cmd.Parameters(4) = aBoughtManureAmount
'   cmd.Parameters(5) = aBoughtManureType
'   cmd.Execute
'   InsertUpdateBoughtManure = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function InsertUpdateSoldManure(aFarmNumber, aScenarioID, aSoldManureID, aSoldManureAmount, aSoldManureType)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateSoldManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aSoldManureID
'   cmd.Parameters(4) = aSoldManureAmount
'   cmd.Parameters(5) = aSoldManureType
'   cmd.Execute
'   InsertUpdateSoldManure = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function GetManure(aFarmNumber, aScenarioID, aStorageID, aColumnName, byref arr)
'   dim index,field
'   strStoredProcedure = "GetManure "&aFarmNumber&","&aScenarioID&","&aStorageID&","&aColumnName
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'   arr(0,0)=0
'  
'   index = 0
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetManure = index   
'end function
''**************************************************************************************************
'function DeleteCalcBoughtSoldManure(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteCalcBoughtSoldManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteCalcBoughtSoldManure = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteBoughtManure(aFarmNumber, aScenarioID, aBoughtManureID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteBoughtManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aBoughtManureID
'   cmd.Execute
'   DeleteBoughtManure = cmd.Parameters(0)
'end function
'
''**************************************************************************************************
'function DeleteSoldManure(aFarmNumber, aScenarioID, aSoldManureID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteSoldManure"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aSoldManureID
'   cmd.Execute
'   DeleteSoldManure = cmd.Parameters(0)
'end function
'
''**************************************************************************************************
'function GetSoldManureMaxID(aFarmNumber, aScenarioID, byref arr)
'   dim index
'   strStoredProcedure = "GetSoldManureMaxID "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetSoldManureMaxID = index   
'end function
'
''**************************************************************************************************
'function GetBoughtManureMaxID(aFarmNumber, aScenarioID, byref arr)
'   dim index
'   strStoredProcedure = "GetBoughtManureMaxID "&aFarmNumber&","&aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetBoughtManureMaxID = index   
'end function
''**************************************************************************************************
'function GetScenarioMaxID(aFarmNumber, byref arr)
'   dim index
'   strStoredProcedure = "GetScenarioMaxID "&aFarmNumber
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,recordSet.RecordCount)
'   index = 0
'   
'   while not recordSet.eof
'      arr(0,index) = recordSet("MaxID")
'      index = index + 1
'      recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetScenarioMaxID = index   
'end function
''**************************************************************************************************
'function InsertUpdateScenario(aFarmNumber, aScenarioID, aScenarioName)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "InsertUpdateScenario"
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Parameters(3) = aScenarioName
'   cmd.Execute
'   InsertUpdateScenario = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function DeleteScenario(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "DeleteScenario "
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   DeleteScenario = cmd.Parameters(0)
'end function
''**************************************************************************************************
'function UpdateSelectedScenario(aFarmNumber, aScenarioID)
'   cmd.CommandType   = 4 'sp
'   cmd.CommandText   = "UpdateSelectedScenario "
'   cmd.Parameters(1) = aFarmNumber
'   cmd.Parameters(2) = aScenarioID
'   cmd.Execute
'   UpdateSelectedScenario = cmd.Parameters(0)
'end function
'
''**************************************************************************************************
'function GetYieldAndN_LP(cropString,byref arr)
'   dim index,field,i,j
'   strStoredProcedure = "GetYieldAndN_LP '"&cropString&"'" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''response.write strStoredProcedure
'
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetYieldAndN_LP = index   
'end function
''**************************************************************************************************
'function GetCropName(aCropID)
'   dim index,arr
'   strStoredProcedure = "GetCropName "& aCropID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'   
'      arr(0,0) = recordSet("CropName")
'   
'   recordSet.Close
'   GetCropName = arr(0,0)  
'end function
''**************************************************************************************************
'function GetYieldAndN_LP2(cropString,byref arr)
'   dim index,field,i,j
'   strStoredProcedure = "GetYieldAndN_LP2 '"&cropString&"'" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''response.write strStoredProcedure
'
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   index = 0
'   
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   
'   recordSet.Close
'   GetYieldAndN_LP2 = index   
'end function
''**************************************************************************************************
'function AllowedCombination(id1,id2,id3,id4)
'   dim index,field,i,j,test,cropString, arrCropID, Tid1, Tid2, Tid3, Tid4
'   Tid1 = Timer
'   If id1>-1 Then 
'   ReDim arrCropID(3)
'   arrCropID(3)=id1
'   Else 
'   ReDim arrCropID(2)
'   End If
'   arrCropID(0)=id2
'   arrCropID(1)=id3
'   arrCropID(2)=id4
' 
'   cropString = join(arrCropID,",")
'
'   Tid2 = Timer
''  response.write "<br>    cropString " & cropString
'   strStoredProcedure = "AllowedCombination '"&cropString&"'" 
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
''response.write strStoredProcedure
'   if recordSet.RecordCount < 0 then
'         while not recordSet.eof
'            i = i + 1
'            recordSet.MoveNext
'         wend
'         reDim arr(recordSet.Fields.Count-1,i)
'         if not recordSet.bof then recordSet.MoveFirst
'   else
'      reDim arr(recordSet.fields.Count-1,recordSet.RecordCount)
'   end if
'   
'   Tid3 = Timer
'   
'   index = 0
'   test = 1
'   while not recordSet.eof
'     for field=0 to recordSet.fields.Count-1
'      arr(field,index) = recordSet(recordSet(field).name)
''	  Response.write "<br> Test af før statement" &  arr(field,index)
'	  If arr(field,index)>0 Then 
''	  Response.write "<br> Test af if statement"
'	  test = 0
'	  Exit For
'	  End if
'     next
'     index = index + 1
'     recordSet.MoveNext
'   wend
'   Tid4 = Timer
'   recordSet.Close
'
''   Response.write "<br> Tid 1-2 i LProtation " & Tid2-Tid1 
''   Response.write "<br> Tid 2-3 i LProtation " & Tid3-Tid2 
''   Response.write "<br> Tid 3-4 i LProtation " & Tid4-Tid3 
'
'   AllowedCombination = test  
'end function
''**************************************************************************************************
'function GetPreviousCropGroupID(aCropID)
'   dim index,arr
'   strStoredProcedure = "GetPreviousCropGroupID "& aCropID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'   
'      arr(0,0) = recordSet("PreviousCropID")
'   
'   recordSet.Close
'   GetPreviousCropGroupID = arr(0,0)  
'end function
''**************************************************************************************************
'function GetPotentialDMDeposition(aFarmNumber, aScenarioID, aRotationID, aFieldPlanRotationID) 
'   dim index,arr
'   strStoredProcedure = "GetPotentialDMDeposition "& aFarmNumber& "," & aScenarioID&","& aRotationID&","& aFieldPlanRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'   
'      arr(0,0) = recordSet("PotentialDMDeposition")
'   
'   recordSet.Close
'   GetPotentialDMDeposition = arr(0,0)  
'end function
''**************************************************************************************************
'
'function GetStrawDMRemoved(aFarmNumber, aScenarioID, aRotationID, aFieldPlanRotationID) 
'   dim index,arr
'   strStoredProcedure = "GetStrawDMRemoved "& aFarmNumber& "," & aScenarioID&","& aRotationID&","& aFieldPlanRotationID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'
'	   if not recordSet.eof And not IsNull(recordSet("StrawDMRemoved")) then
'		   arr(0,0) = recordSet("StrawDMRemoved")
'       Else 
'	       arr(0,0) = 0
'	   End If 
'
'   recordSet.Close
'
'   GetStrawDMRemoved = arr(0,0)  
'end function
''**************************************************************************************************
'
'function GetAnimalUnit(aFarmNumber, aScenarioID) 
'   dim index,arr
'   strStoredProcedure = "GetAnimalUnit "& aFarmNumber& "," & aScenarioID
'   recordSet.Open strStoredProcedure , con, adOpenStatic, adLockReadOnly 
'
'   reDim arr(0,0)
'
'	   if not recordSet.eof And not IsNull(recordSet("DE")) then
'		   arr(0,0) = recordSet("DE")
'       Else 
'	       arr(0,0) = 0
'	   End If 
'
'   recordSet.Close
'
'   GetAnimalUnit = arr(0,0)  
'end function
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
'	response.write sql
  command.CommandType   = 4
  command.CommandText   = sql
  for p = 0 to UBound(arr)
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
'	response.write sql
  recordSet.Open sql, connection, adOpenStatic, adLockReadOnly
  MakeDictCols dict, recordSet
  recordCount = RecSetToArray(arr, recordSet)
  recordSet.Close()
  GetData = recordCount
end Function
'**************************************************************************************************
function GetDataColumnsRows (byVal sql,connection,recordSet,arr,dictcol,dictrow,fieldArr)
  dim recordCount
	response.write sql
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
