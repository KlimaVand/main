Attribute VB_Name = "CropExperiments" 

Option Explicit

Const SYNCHRONIZE = &H100000
Const INFINITE = &HFFFF    'Wait forever
Const WAIT_OBJECT_0 = 0    'The state of the specified object is signaled.
'Const WAIT_TIMEOUT = &H102    'The time-out interval elapsed, and the object’s state is nonsignaled.

Private Declare Function OpenProcess Lib "kernel32" _
                                     (ByVal dwDesiredAccess As Long, _
                                      ByVal bInheritHandle As Long, _
                                      ByVal dwProcessId As Long) _
                                      As Long
Private Declare Function WaitForSingleObject Lib "kernel32" _
                                             (ByVal hHandle As Long, _
                                              ByVal dwMilliseconds As Long) _
                                              As Long
'Private Declare Function CloseHandle Lib "kernel32" _
 '                                     (ByVal hObject As Long) _
 '                                     As Long

Dim Base As database

Dim ManagementStart(40) As Integer
Dim ObsStart(40) As Long            ' start of observations  (exp sub index)
Dim SoilSimStart As Long
Dim SoilSimEnd As Long            ' start of soil sim (exp number)
Dim PlantSimStart As Long
Dim PlantSimEnd As Long           ' start of plant sim (exp number)
Dim BudgetRow As Long             ' Current row for budget tabel
Dim GraphIndex As Integer
Dim ExtractDataCol As Integer
Dim HarvestDataRow As Integer     ' Current Harvest row
Dim HarvestDataStart As Integer     ' Start Harvest row

Dim PlotStartDate As Date
Dim PlotEndDate As Date
Dim FirstSimDate As Date
Dim ExpID As String
Dim SowCrop(200) As String         ' name of sown crops (for budget)
Dim Directory As String
Sub InitSimulation()
    'Dim FileExist
    'FileExist = Dir("C:" & Sheets("Main").Range("M7"))
    'If FileExist <> "" Then
        Directory = "C:"
    'Else
     '   FileExist = Dir("D:" & Sheets("Main").Range("M7"))
   '     If FileExist <> "" Then
    '        Directory = "D:"
   '     Else

    '    End If
   ' End If
    ChDrive (Directory)
    Application.DisplayAlerts = False


    Worksheets("PS").Activate              ' delete plant sim sheet
    Columns("A:GA").Clear
    Worksheets("SS").Activate               ' delete soil sim sheet
    Columns("A:GA").Clear

    SoilSimStart = 1
    SoilSimEnd = 0
    PlantSimStart = 1
    PlantSimEnd = 0

    GraphIndex = 0

    'Clear observations
    ObsStart(15) = 0
    Sheets("Observations").Range("A:BZ").Clear

    'Clear all relevant pages
    Sheets("Graph").Select

    ActiveWindow.SelectedSheets.Delete
    Application.DisplayAlerts = True
    Sheets.Add
    Sheets(2).Name = "Graph"
    Sheets("PS").Range("A:IV").Clear
    Sheets("SS").Range("A:IV").Clear
End Sub
Sub execute()
    ObsStart(17) = 0


    ChDrive (Directory)
    Application.ScreenUpdating = False

    ExtractDataCol = 1
    BudgetRow = 1
    HarvestDataRow = 1

    Dim WarningsNr As Integer
    WarningsNr = 1
    ThisWorkbook.Sheets("Warnings").Cells.Clear
    Application.DisplayAlerts = False
    If Sheets("Main").Rhizo.Value = True Then
        Sheets("Rhizo").Range("A:AZ").Clear
    End If
    If Sheets("Main").HarvestData.Value = True Then
        Sheets("Harv").Activate
        Sheets("Harv").Range("A:AZ").Clear
        Columns("J:J").NumberFormat = "dd-mm-yy"
    End If
    If Sheets("Main").Budget.Value = True Then
        Sheets("Budget").Activate
        Sheets("Budget").Range("A:AZ").Clear
        Sheets("Budget").Columns("A:AZ").Select
        Selection.NumberFormat = "0"
    End If
    Call InitSimulation

    Set Base = DBEngine.Workspaces(0).OpenDatabase(Sheets("Main").Cells(6, 13))

    ObsStart(14) = 0

    Dim AIndex As Integer
    AIndex = 1
    While Sheets("Main").Cells(AIndex + 1, 1) <> ""
        Application.ScreenUpdating = False
        Sheets("Main").Activate
        ExpID = Cells(AIndex + 1, 1)
        Call GetManagement
        Call ReadObservations
        Call ExecuteFasset
        If Sheets("Main").HarvestData.Value = True Then
            Call HarvestData
        End If
        If Sheets("Main").DailyDetails.Value = True Then
            Call ReadSimulation
            Call GenerateGraph
        End If
        If Sheets("Main").Rhizo.Value = True Then
            Call GenerateExtraInformation
            Call ExtractData
        End If
        If Sheets("Main").Budget.Value = True Then
            Call GenerateBudget
        End If
        If Sheets("Main").SaveACopy.Value = True Then
            Dim aFileName As String
            aFileName = "\data\database\exp" + Format(AIndex) + ".xls"
            ActiveWorkbook.SaveCopyAs aFileName
            Call InitSimulation
        End If
        Application.ScreenUpdating = True
        Sheets("Graph").Activate
        On Error Resume Next
        If Sheets("Main").DailyDetails.Value = True Then
            ActiveWindow.ScrollRow = (GraphIndex - 2) * 15 + 1           ' makes this row top
            If Sheets("Main").PrintBox.Value = True And ((AIndex Mod Sheets("Main").Range("J6")) = 0) Then
                Call PagePrint
                Call InitSimulation
            End If
        End If
        AIndex = AIndex + 1
        ' collect warnings
        Workbooks.OpenText Filename:=Sheets("Main").Range("M4") + "warnings.txt", Origin:=xlWindows, _
                           StartRow:=2, DataType:=xlDelimited, TextQualifier:=xlDoubleQuote, _
                           ConsecutiveDelimiter:=False, Tab:=True, Semicolon:=False, Comma:=False _
                                                                                            , Space:=False, Other:=False, FieldInfo:=Array(Array(1, 2))
        Dim WarningRow As Integer
        WarningRow = 1
        While Cells(WarningRow, 1) <> "" And WarningRow < 65000
            ThisWorkbook.Sheets("Warnings").Cells(WarningsNr, 1) = ExpID
            ThisWorkbook.Sheets("Warnings").Cells(WarningsNr, 2) = WarningRow
            ThisWorkbook.Sheets("Warnings").Cells(WarningsNr, 3) = Cells(WarningRow, 1)
            WarningRow = WarningRow + 1
            WarningsNr = WarningsNr + 1
        Wend
        ActiveWindow.Close (False)
    Wend
    Base.Close
    Sheets("Graph").Activate
    Beep
End Sub

Sub ReadWrite()
    Set Base = DBEngine.Workspaces(0).OpenDatabase(Sheets("Main").Cells(6, 13))

    ObsStart(14) = 0
    SoilSimStart = 1
    SoilSimEnd = 0
    PlantSimStart = 1
    PlantSimEnd = 0
    AIndex = 1
    GraphIndex = 0
    'Clear all relevant pages
    Sheets("Graph").Select
    Application.DisplayAlerts = False
    ActiveWindow.SelectedSheets.Delete
    Application.DisplayAlerts = True
    Sheets.Add
    Sheets(2).Name = "Graph"


    Sheets("PS").Cells.Clear
    Sheets("SS").Cells.Clear

    Sheets("Observations").Activate
    Cells.Clear

    Sheets("Main").Activate
    ExpID = Range("A2")
    Call GetManagement
    Call ReadObservations
    Base.Close
End Sub
Sub AddManagement(SqlText, CurrentRow, Header1, Optional Header2 = "", Optional Header3 = "", Optional Header4 = "", Optional Header5 = "", Optional Header6 = "")
    Cells(CurrentRow, 1) = Header1
    Cells(CurrentRow, 2) = Header2
    Cells(CurrentRow, 3) = Header3
    Cells(CurrentRow, 4) = Header4
    Cells(CurrentRow, 5) = Header5
    Cells(CurrentRow, 6) = Header6
    CurrentRow = CurrentRow + 1
    Dim Record
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    Cells(CurrentRow, 1).CopyFromRecordset Record
    CurrentRow = CurrentRow + 1 + Record.RecordCount
End Sub
Sub GetManagement()
'
' Opdater 09/06/99 af Jørgen Berntsen

'Dim wrkJet As Workspace
'Set wrkJet = DBEngine.Workspaces(0)
'   Dim Record As Recordset
    Dim SqlText As String

    Sheets("Management").Select
    Cells.Clear

    ' start og slut på experiment
    SqlText = "SELECT Start_Date, End_Date, site_Id, soil_id FROM Experiments WHERE (ID='" & ExpID & "')"
    Dim CurrentRow As Integer

    CurrentRow = 1
    Call AddManagement(SqlText, CurrentRow, "Start", "End", "Site", "soil")

    'Climate
    SqlText = "SELECT Climate_File, CO2_concentration, PrecipitationCorrection, NitrogenInRain, N_deposition FROM [Def Climate] WHERE (Site_Id='" & Range("C2") & "')"
    Call AddManagement(SqlText, CurrentRow, "Climate", "CO2", "PrecipitationCorrection", "NitrogenInRain", "NitrogenDryDep")

    'Soil
    SqlText = "SELECT Soil_File, JB_NO FROM [Def Soil] WHERE (Soil_Id='" & Range("D2") & "')"
    Call AddManagement(SqlText, CurrentRow, "Soil", "JBNO")

    'Experimental Site
    SqlText = "SELECT Description, Latitude FROM [Experimental_site] WHERE (Site_Id='" & Range("C2") & "')"
    Call AddManagement(SqlText, CurrentRow, "Site")

    'såning
    ManagementStart(0) = CurrentRow
    SqlText = "SELECT Date, [Crop ID], Amount FROM [Treat Sow] WHERE (ID='" & ExpID & "') ORDER BY Date, Amount DESC"
    Call AddManagement(SqlText, CurrentRow, "SowingDate", "SowID", "SowAmount")

    'pløje
    ManagementStart(1) = CurrentRow
    SqlText = "SELECT Date, type, depth, [Fraction Incorporated] FROM [Treat Soil] WHERE (ID='" & ExpID & "') ORDER BY Date"
    Call AddManagement(SqlText, CurrentRow, "date", "type", "depth", "Fraction")

    'vanding
    ManagementStart(2) = CurrentRow
    SqlText = "SELECT Date, Amount, N_Content FROM [Treat Irrigation] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "irrigation", "")

    'høst
    ManagementStart(3) = CurrentRow
    SqlText = "SELECT Date, Crop_ID, avg(Storage_DM), avg(Straw_DM), avg(Total_DM), avg(Storage_N), avg(Straw_N), avg(Total_N), first(Remove_Straw),avg(Straw_removed),avg(Grain_removed) FROM [Measure Harvest] WHERE (ID='" & ExpID & "') GROUP BY Date, Crop_ID"
    Call AddManagement(SqlText, CurrentRow, "harvest", "Crop", "Storage", "Straw")

    'min fert
    ManagementStart(4) = CurrentRow
    SqlText = "SELECT Date, N, Ammonium_fraction, Cl, [15N], Type FROM [Treat MinFert] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "mineral fert", "N", "NH4 frac", "Cl", "15N", "Type")

    'org fert
    ManagementStart(5) = CurrentRow
    SqlText = "SELECT Date, Type, Added, DM, Organic_N, Ammonia, Nitrate, [15N-organic_N],[15N-NH4],Organic_C,Volatilisation FROM [Treat OrgFert] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "organic fert", "Type", "Added", "DM")
    Cells(ManagementStart(5), 5) = "Organic_N"
    Cells(ManagementStart(5), 6) = "Ammonia"
    Cells(ManagementStart(5), 7) = "Nitrate"
    Cells(ManagementStart(5), 8) = "15N-organic_N"
    Cells(ManagementStart(5), 9) = "15N-NH4"
    Cells(ManagementStart(5), 10) = "Organic_C"
    Cells(ManagementStart(5), 11) = "Volatilisation"

    'past crops
    ManagementStart(6) = CurrentRow
    SqlText = "SELECT CropID, PastYear, MineralN, Slurry, totalN, HarvestDate FROM [Past Crop] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "previous crops", "year", "kg-N", "Slurry", "totalN", "HarvestDate")

    'grazing
    ManagementStart(7) = CurrentRow
    SqlText = "SELECT Start, End, [N load] FROM [Treat Grazing] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "grazing", "start", "end", "load")

    'cutting
    ManagementStart(8) = CurrentRow
    SqlText = "SELECT Date, Crop_ID, Straw, Height FROM [Treat Cutting] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "Cutting", "Crop", "Straw", "Height")

    'pesticides
    ManagementStart(9) = CurrentRow
    SqlText = "SELECT Date,  Type FROM [Treat Pesticides] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "Pesticides", "date", "type")

    'main crop name
    ManagementStart(10) = CurrentRow
    SqlText = "SELECT HarvestYear, MainCrop FROM [MainCrop] WHERE (ID='" & ExpID & "')"
    Call AddManagement(SqlText, CurrentRow, "HarvestYear", "MainCrop")

    Call WriteFieldPlans
End Sub
Function CelleInCurrentYear(celle, currentYear) As Boolean
    If ((Year(celle) = currentYear) And (Month(celle) >= 8)) Or _
       ((Year(celle) = (currentYear + 1)) And (Month(celle) < 8)) Then

        CelleInCurrentYear = True
    Else
        CelleInCurrentYear = False
    End If

End Function
Function MaxFunc(a, b)
    If a > b Then
        MaxFunc = a
    Else
        MaxFunc = b
    End If
End Function
Sub WriteFieldPlans()

    Dim filnavn As String, SqlText As String

    'Dim KløverGræs As Boolean
    'Dim RajGræs As Boolean
    Dim firstCropRape As Boolean
    'Dim foundFirst As Boolean
    'Dim UnderSown As Boolean
    'Dim CatchCrop As Boolean

    Dim MainCrop As String, CropName As String
    Dim MainCropFound As Boolean

    Dim AmountSown As Double, amountOrg As Double, Fraction As Double
    Dim FractionStrawHarvested, FractionGrainHarvested As Double
    Dim Depth As Double

    Dim no As Integer, MinIndex As Integer, Index As Integer, WaterIndex As Integer
    Dim i As Integer, j As Integer, k As Integer, l As Integer
    Dim TillageNumber As Integer

    Dim TillageType
    Dim RemoveStraw
    Dim MainCropYear
    Dim MinDate
    Dim AddOrg15N, AddNH415N, celle1




    'UnderSown = False
    firstCropRape = False
    'foundFirst = False
    'CatchCrop = False
    MainCrop = ""
    MainCropFound = False
    'KløverGræs = False
    'RajGræs = False

    ChDir (Directory & Sheets("Main").Range("M2"))
    Open "system.dat" For Output As #1
    Print #1, "[run(0)]"
    Print #1, "StartYear "; Tab(30); Year(Sheets("Management").Range("A2")) - 2
    Print #1, "StopYear "; Tab(30); Year(Sheets("Management").Range("B2")) + 1
    Print #1, "EnvironmentalIndicator        "; Sheets("Main").Range("P2")
    Print #1, "InputDirectory        "; Sheets("Main").Range("M3")
    Print #1, "OutputDirectory       "; Sheets("Main").Range("M4")
    Print #1, "ClimateFileName       "; Sheets("Main").Range("M5"); Sheets("Management").Range("A5")
    Print #1, "ForcedOperations        1"
    i = 2
    While Sheets("Main").Cells(i, 4) <> ""
        Print #1, Sheets("Main").Cells(i, 4); "       "; Sheets("Main").Cells(i, 5)
        i = i + 1
    Wend
    Print #1, "[end]"
    Close #1

    ChDir (Directory & Sheets("Main").Range("M3"))
    Open "farm.dat" For Output As #1
    Print #1, "[farm]"
    Print #1, "ScaleBuildings 1"
    Print #1, "farmID 1"
    Print #1, "CapitalInterest 0.04"
    Print #1, "MinFieldArea 0.1"
    Print #1, "SellAllStraw 1"
    Print #1, "Watering 0"

    Print #1, "[livestock]"
    Print #1, "pigfarm 0"
    Print #1, "SowsKnowledge 0"
    Print #1, "BaconersKnowledge 0"
    Print #1, "dairyfarm 0"
    Print #1, "beeffarm 0"
    Print #1, "[end]"
    Close #1

    Open "fields.dat" For Output As #1
    Print #1, "[field(0)]"
    Print #1, "soilFile"; Tab(30); Sheets("Management").Range("A8")
    Print #1, "JBNO"; Tab(30); Sheets("Management").Range("B8")
    Print #1, "area"; Tab(30); "1"
    Print #1, "distance"; Tab(30); "0"
    Print #1, "fenced"; Tab(30); "0"
    Print #1, "CO2conc"; Tab(30); Sheets("Management").Range("B5")
    Print #1, "CROP0"; Tab(30); "W2"
    Print #1, "CROP1"; Tab(30); "W2"
    Print #1, "CROP2"; Tab(30); "W2"
    Print #1, "CROP3"; Tab(30); "W2"
    Print #1, "HUG-N0"; Tab(30); "0"
    Print #1, "HUG-N1"; Tab(30); "0"
    Print #1, "HUG-N2"; Tab(30); "0"
    Print #1, "HUG-N3"; Tab(30); "0"
    Print #1, "[end]"
    Close #1

    'de to første mark planer
    For i = 0 To 1
        If (i = 0) Then
            Open "Fields.f00" For Output As #1
        Else
            Open "Fields.f01" For Output As #1
        End If
        Print #1, "[fieldOrder(0)]"
        Print #1, "Ha"; Tab(30); "1"
        'If Range("J1") = 1 Then Print #1, "PEST-LEVEL"; Tab(30); "0"
        Dim RowIndex As Integer
        RowIndex = ManagementStart(6) + 1 + i
        If Not Sheets("Management").Cells(ManagementStart(6) + 1, 1) = "" Then
            'If Sheets("Management").Cells(RowIndex, 1) = "B8" Then KløverGræs = True
            Print #1, "CROP"; Tab(30); Sheets("Management").Cells(RowIndex, 1); "____00"
            If i = 1 Then SowCrop(0) = Sheets("Management").Cells(RowIndex, 1)
            If Sheets("Management").Cells(RowIndex, 3) > 0 Then Print #1, "kg-N-"; Tab(30); Sheets("Management").Cells(RowIndex, 3)
            If Sheets("Management").Cells(RowIndex, 4) > 0 Then
                Dim Slurry As String
                Slurry = Sheets("Management").Cells(RowIndex, 4)
                If Sheets("Management").Cells(RowIndex, 5) > 0 Then
                    Slurry = Slurry * Sheets("Management").Cells(RowIndex, 5) / 0.0046    ' converts amount to "standard" slurry with 0.0046 totalN
                End If
                Print #1, "SLUR.PS"; Tab(30); Slurry
            End If
            If Sheets("Management").Cells(RowIndex, 6) > 0 Then
                Print #1, "HarvestDate(0)      "; Sheets("Management").Cells(RowIndex, 6)
            End If
        Else



            If (firstCropRape And (i = 1)) Then
                Print #1, "CROP"; Tab(30); "B6____00"
                Print #1, "kg-N-"; Tab(30); "130.0"
            Else
                Print #1, "CROP"; Tab(30); "B2____00"
                Print #1, "kg-N-"; Tab(30); "120.0"
            End If
        End If
        Print #1, "[end]"
        Close #1
    Next i


    Sheets("Management").Select
    Dim NumberOfYears As Integer
    NumberOfYears = Year(Range("B2")) - Year(Range("A2"))
    If NumberOfYears < 1 Then NumberOfYears = 1
    For i = 0 To NumberOfYears - 1
        If (i + 2 < 100) Then
            filnavn = "fields.f" & Format(i + 2, "00")
        Else
            filnavn = "fields.f" & Format(i + 2, "000")
        End If

        Open filnavn For Output As #1
        Print #1, "[fieldOrder(0)]"
        Print #1, "Ha"; Tab(30); "1"
        'If Range("J1") = 1 Then Print #1, "PEST-LEVEL"; Tab(30); "0"
        Print #1, "Automatic"; Tab(30); "0"

        Dim celle
        Set celle = Sheets("Management").Cells(ManagementStart(0) + 1, 1)
        'Dim SowDate
        'Set SowDate = celle
        Dim currentYear As Integer
        currentYear = Year(Range("A2")) + i
        k = 0
        l = 0




        Dim CropToSow(12) As String
        Dim SowAmounts(12) As Double
        Dim Sowdates(12) As Date

        'sowing
        j = 0
        'Test if main crop is explicit
        While Sheets("Management").Cells(ManagementStart(10) + 1 + j, 1) <> ""
            MainCropYear = Sheets("Management").Cells(ManagementStart(10) + 1 + j, 1)
            If MainCropYear = currentYear + 1 Then
                MainCropFound = True
                MainCrop = Sheets("Management").Cells(ManagementStart(10) + 1 + j, 2)
            End If
            j = j + 1
        Wend
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                CropName = Sheets("Management").Cells(ManagementStart(0) + 1 + k, 2)
                AmountSown = Sheets("Management").Cells(ManagementStart(0) + 1 + k, 3)
                If CropName = "GR" Then CropName = "G1"
                If CropName = "LU" Then CropName = "PE"
                If CropName = "OA" Then CropName = "O2"
                If CropName = "TR" Then CropName = "W1"
                ' End If
                If Not MainCropFound And l = 0 Then
                    MainCrop = CropName
                End If
                CropToSow(l) = CropName
                SowAmounts(l) = AmountSown
                Sowdates(l) = celle
                l = l + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(0) + 1 + k, 1)
        Wend
        Print #1, "CROP"; Tab(30); MainCrop; "____00"
        For no = 0 To l - 1
            Print #1, "SowDate("; Format(no); ")"; Tab(30); Format(Sowdates(no), "dd/mm/yyyy")
            Print #1, "SowCrop("; Format(no); ")"; Tab(30); CropToSow(no)
            Print #1, "SowAmount("; Format(no); ")"; Tab(30); SowAmounts(no)
        Next



        'ploughing
        Set celle = Sheets("Management").Cells(ManagementStart(1) + 1, 1)
        TillageNumber = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                TillageType = Sheets("Management").Cells(ManagementStart(1) + 1 + k, 2)
                Depth = Sheets("Management").Cells(ManagementStart(1) + 1 + k, 3)
                Fraction = Sheets("Management").Cells(ManagementStart(1) + 1 + k, 4)
                'If Sowdates(1) >= celle Then
                Print #1, "TillageDate("; Format(TillageNumber); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                Print #1, "TillageType("; Format(TillageNumber); ")"; Tab(30); UCase(TillageType)
                If (Depth > 0) Then Print #1, "TillageDepth("; Format(TillageNumber); ")"; Tab(30); Depth
                If (Fraction > 0) Then Print #1, "TillageIncorporation("; Format(TillageNumber); ")"; Tab(30); Fraction
                TillageNumber = TillageNumber + 1
                'End If
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(1) + 1 + k, 1)
        Wend

        'irrigation
        Set celle = Sheets("Management").Cells(ManagementStart(2) + 1, 1)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                Print #1, "IrrigationDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                Print #1, "IrrigationAmount("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(2) + 1 + k, 2)
                Print #1, "IrrigationNcontent("; Format(j); ")"; Tab(30); Format(Sheets("Management").Cells(ManagementStart(2) + 1 + k, 3), "###0.000000")
                j = j + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(2) + 1 + k, 1)
        Wend

        'harvest
        Set celle = Sheets("Management").Cells(ManagementStart(3) + 1, 1)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And Year(celle) = currentYear + 1 Then
                If j = 0 Or (j > 0 And celle <> Sheets("Management").Cells(ManagementStart(3) + k, 1)) Then
                    Print #1, "HarvestDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                    RemoveStraw = 1
                    If Sheets("Management").Cells(ManagementStart(3) + k + 1, 9) = "LEAVE" Then RemoveStraw = 0
                    Print #1, "HarvestRemoveStraw("; Format(j); ")"; Tab(30); RemoveStraw
                    FractionStrawHarvested = Sheets("Management").Cells(ManagementStart(3) + k + 1, 10)
                    FractionGrainHarvested = Sheets("Management").Cells(ManagementStart(3) + k + 1, 11)
                    If FractionStrawHarvested > 0 Then Print #1, "FractionStrawHarvested("; Format(j); ")"; Tab(30); FractionStrawHarvested
                    If FractionGrainHarvested > 0 Then Print #1, "FractionGrainHarvested("; Format(j); ")"; Tab(30); FractionGrainHarvested
                    j = j + 1
                End If
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(3) + 1 + k, 1)
        Wend

        'cutting
        Set celle = Sheets("Management").Cells(ManagementStart(8) + 1, 1)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                'If Year(celle) = currentYear + 1 Then
                'If j = 0 Or (j > 0 And celle <> Sheets("Management").Cells(ManagementStart(8) + k, 1)) Then
                Print #1, "CuttingDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                RemoveStraw = 1
                If Sheets("Management").Cells(ManagementStart(8) + k + 1, 3) = "LEAVE" Then RemoveStraw = 0
                Print #1, "RemoveStraw("; Format(j); ")"; Tab(30); RemoveStraw
                If Sheets("Management").Cells(ManagementStart(8) + k + 1, 4) > "" Then
                    Print #1, "CuttingHeight("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(8) + k + 1, 4)
                End If
                j = j + 1
            End If
            'End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(8) + 1 + k, 1)
        Wend

        'min fertilizer
        Dim Dates(10) As Date    ' used for sorting as FASSET needs mineral and organic fertilisers sorted!
        Dim Info(10, 10) As String
        Set celle = Sheets("Management").Cells(ManagementStart(4) + 1, 1)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                Dates(j) = celle
                Info(j, 1) = "MINERAL"
                Info(j, 2) = 10 * Sheets("Management").Cells(ManagementStart(4) + 1 + k, 2)
                Info(j, 3) = Sheets("Management").Cells(ManagementStart(4) + 1 + k, 3)
                Info(j, 4) = 10 * Sheets("Management").Cells(ManagementStart(4) + 1 + k, 5)
                Info(j, 5) = 10 * Sheets("Management").Cells(ManagementStart(4) + 1 + k, 4)
                Info(j, 6) = 0
                If Sheets("Management").Cells(ManagementStart(4) + 1 + k, 6) = "Solution" Then
                    Info(j, 6) = 1
                End If
                j = j + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(4) + 1 + k, 1)
        Wend

        'org fertilizer
        Set celle = Sheets("Management").Cells(ManagementStart(5) + 1, 1)
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                amountOrg = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 3)
                Dates(j) = celle
                Info(j, 1) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 2)
                Info(j, 2) = amountOrg / 100
                Info(j, 3) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 5) / amountOrg        'TotalNitrogen
                Info(j, 4) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 8) / amountOrg        'TotalN15Nitrogen
                Info(j, 5) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 6) / amountOrg        'AmmoniumContent
                Info(j, 6) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 9) / amountOrg        'AmmoniumN15Content
                Info(j, 7) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 11)                   'Volatilisation
                Info(j, 9) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 10) / amountOrg       'Organic C
                Info(j, 8) = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 4)                    'drymatter
                If Info(j, 7) = "" Then Info(j, 7) = 0
                If Info(j, 8) = "" Then Info(j, 8) = 0
                'Print #1, "FertilizerDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                'Print #1, "FertilizerType("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(5) + 1 + k, 2)
                'Print #1, "ManureAmount("; Format(j); ")"; Tab(30); amountOrg / 100
                'Print #1, "AmmoniumContent("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(5) + 1 + k, 4) / amountOrg
                'Print #1, "TotalNitrogen("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(5) + 1 + k, 5) / amountOrg
                j = j + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(5) + 1 + k, 1)
        Wend

        Dim Used(10) As Boolean    ' primitive sorting algoritm
        For Index = 0 To j - 1
            Used(Index) = False
        Next

        For Index = 0 To j - 1
            MinDate = #1/1/9999#
            MinIndex = 100
            For k = 0 To j - 1
                If Dates(k) < MinDate And Not Used(k) Then
                    MinDate = Dates(k)
                    MinIndex = k
                End If
            Next k
            If MinIndex < 100 Then
                Used(MinIndex) = True
                Print #1, "FertilizerDate("; Format(Index); ")"; Tab(30); Format(Dates(MinIndex), "dd/mm/yyyy")
                Print #1, "FertilizerType("; Format(Index); ")"; Tab(30); UCase(Info(MinIndex, 1))
                If Info(MinIndex, 1) = "MINERAL" Then
                    Print #1, "FertilizerN("; Format(Index); ")"; Tab(30); Info(MinIndex, 2)
                    Print #1, "NH4_fraction("; Format(Index); ")"; Tab(30); Info(MinIndex, 3)
                    If Info(MinIndex, 3) Then
                        Print #1, "FertilizerN15("; Format(Index); ")"; Tab(30); Info(MinIndex, 4)
                    End If
                    If Info(MinIndex, 5) > 0 Then
                        Print #1, "FertilizerCl("; Format(Index); ")"; Tab(30); Info(MinIndex, 5)
                    End If
                    If Info(MinIndex, 6) = 1 Then
                        Print #1, "MineralForm("; Format(Index); ")"; Tab(30); "SOLUTION"
                    Else
                        Print #1, "MineralForm("; Format(Index); ")"; Tab(30); "SOLID"
                    End If
                Else
                    Print #1, "ManureAmount("; Format(Index); ")"; Tab(30); Info(MinIndex, 2)
                    ' this ensure that 15N is not greather than "normal" N (note fails if both fractions are to high)
                    AddOrg15N = CDbl(Info(MinIndex, 4))
                    AddNH415N = CDbl(Info(MinIndex, 6))
                    If AddOrg15N > CDbl(Info(MinIndex, 3)) Then
                        AddNH415N = AddNH415N + (AddOrg15N - CDbl(Info(MinIndex, 3)))
                        AddOrg15N = CDbl(Info(MinIndex, 3))
                    End If
                    If AddNH415N > CDbl(Info(MinIndex, 5)) Then
                        AddOrg15N = AddOrg15N + (AddNH415N - CDbl(Info(MinIndex, 5)))
                        AddNH415N = CDbl(Info(MinIndex, 5))
                    End If

                    If CDbl(Info(MinIndex, 3)) > 0 Then Print #1, "OrganicNitrogen("; Format(Index); ")"; Tab(30); CDbl(Info(MinIndex, 3))
                    If CDbl(Info(MinIndex, 5)) > 0 Then Print #1, "AmmoniumContent("; Format(Index); ")"; Tab(30); CDbl(Info(MinIndex, 5))
                    If AddOrg15N > 0 Then Print #1, "OrganicN15Nitrogen("; Format(Index); ")"; Tab(30); AddOrg15N
                    If AddNH415N > 0 Then Print #1, "AmmoniumN15Content("; Format(Index); ")"; Tab(30); AddNH415N

                    If CDbl(Info(MinIndex, 7)) > 0 Then Print #1, "Volatisation("; Format(Index); ")"; Tab(30); CDbl(Info(MinIndex, 7))
                    If CDbl(Info(MinIndex, 8)) > 0 Then Print #1, "DryMatter("; Format(Index); ")"; Tab(30); CDbl(Info(MinIndex, 8)) / 100
                    If CDbl(Info(MinIndex, 9)) > 0 Then Print #1, "OrganicCarbon("; Format(Index); ")"; Tab(30); CDbl(Info(MinIndex, 9))

                End If
            End If
        Next Index

        'grazing
        Set celle = Sheets("Management").Cells(ManagementStart(7) + 1, 1)
        Set celle1 = Sheets("Management").Cells(ManagementStart(7) + 1, 2)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Or CelleInCurrentYear(celle1, currentYear) Then
                If CelleInCurrentYear(celle, currentYear) Then
                    Print #1, "GrazingStartDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                Else
                    Print #1, "GrazingStartDate("; Format(j); ")"; Tab(30); DateSerial(currentYear, 8, 1)
                End If
                If CelleInCurrentYear(celle1, currentYear) Then
                    Print #1, "GrazingEndDate("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(7) + 1 + k, 2)
                Else
                    Print #1, "GrazingEndDate("; Format(j); ")"; Tab(30); DateSerial(currentYear + 1, 8, 1)
                End If
                Print #1, "NitrogenLoad("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(7) + 1 + k, 3)
                j = j + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(7) + 1 + k, 1)
            celle1 = Sheets("Management").Cells(ManagementStart(7) + 1 + k, 2)
        Wend

        'pesticides
        Set celle = Sheets("Management").Cells(ManagementStart(9) + 1, 1)
        j = 0
        k = 0
        While celle <> "" And Year(celle) <= Year(Range("B2"))
            If Year(celle) >= Year(Range("A2")) And CelleInCurrentYear(celle, currentYear) Then
                Print #1, "PesticideDate("; Format(j); ")"; Tab(30); Format(celle, "dd/mm/yyyy")
                Print #1, "PesticideType("; Format(j); ")"; Tab(30); Sheets("Management").Cells(ManagementStart(9) + 1 + k, 2)
                j = j + 1
            End If
            k = k + 1
            celle = Sheets("Management").Cells(ManagementStart(9) + 1 + k, 1)
        Wend

        Print #1, "[end]"
        Close #1

    Next i



    'slut med en byg eller to
    filnavn = "fields.f" & Format(Year(Range("B2")) - Year(Range("A2")) + 2, "00")
    Open filnavn For Output As #1
    Print #1, "[fieldOrder(0)]"
    Print #1, "Ha"; Tab(30); "1"
    Print #1, "CROP"; Tab(30); "B2____00"
    Print #1, "kg-N-"; Tab(30); "100.0"
    Print #1, "[end]"
    Close #1

    Open "environ.dat" For Output As #1
    If Range("B5") > 0 Or Range("B5") = -1 Then
        Print #1, "CO2Concentration"; Tab(30); Range("B5")
    End If
    If Range("C5") > 0 Then
        Print #1, "PrecipitationCorrection"; Tab(30); Range("C5")
    End If
    If Range("D5") > 0 Then
        Print #1, "NitrogenInRain"; Tab(30); Range("D5")
    End If
    If Range("E5") > 0 Then
        Print #1, "NitrogenDryDep"; Tab(30); Range("E5")
    End If
    If Range("B11") > 0 Then
        Print #1, "lattitude"; Tab(30); Range("B11")
    End If
    Print #1, "[end]"
    Close #1

    Open "output.dat" For Output As #1
    i = 0
    Dim Record As Recordset
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil Water] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[Water("; Format(i); ")]"
            Print #1, "StartDepth"; Tab(30); Record.Fields(0).Value
            Print #1, "EndDepth"; Tab(30); Record.Fields(1).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    i = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil N] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[Nmin("; Format(i); ")]"
            Print #1, "StartDepth"; Tab(30); Record.Fields(0).Value
            Print #1, "EndDepth"; Tab(30); Record.Fields(1).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    i = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Root] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[Root("; Format(i); ")]"
            Print #1, "StartDepth"; Tab(30); Record.Fields(0).Value
            Print #1, "EndDepth"; Tab(30); Record.Fields(1).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    i = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil C] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[SoilC("; Format(i); ")]"
            Print #1, "StartDepth"; Tab(30); Record.Fields(0).Value
            Print #1, "EndDepth"; Tab(30); Record.Fields(1).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    i = 0
    SqlText = "SELECT Depth FROM [Measure Pore Water] WHERE (ID='" & ExpID & "') GROUP BY Depth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        If Record.RecordCount = 1 Then
            Print #1, "[PoreWater("; Format(0); ")]"
            Print #1, "Depth"; Tab(30); 25
            Print #1, "[PoreWater("; Format(1); ")]"
            Print #1, "Depth"; Tab(30); 50
            i = 2
        End If
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[PoreWater("; Format(i); ")]"
            Print #1, "Depth"; Tab(30); Record.Fields(0).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    i = 0
    SqlText = "SELECT depth FROM [Measure Soil Temperature] WHERE (ID='" & ExpID & "') GROUP BY depth"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveFirst
        For WaterIndex = 0 To Record.RecordCount - 1
            Print #1, "[Temperature("; Format(i); ")]"
            Print #1, "Depth"; Tab(30); Record.Fields(0).Value
            Print #1, ""
            i = i + 1
            Record.MoveNext
        Next
    End If

    Print #1, "[Details]"
    Print #1, "PlantDetails 1"
    Print #1, "SoilDetails 1"
    Print #1, "[end]"
    Close #1
End Sub
Sub ReadObservations()
    Dim Record

    Dim SqlText As String, SqlText1 As String
    Dim Replicates

    Dim PlantRecord As Recordset
    Dim DateRecord As Recordset
    Dim PlantNameString(10) As String, DateString(100) As String
    Dim WaterRecord As Recordset, SoilRecord As Recordset, TemperatureRecord As Recordset

    Dim RowIndex As Integer, maxRec As Integer, NumberOfPlants As Integer
    Dim plantIndex As Integer, NumberOfDates As Integer, DateIndex As Integer
    Dim MaxRecords As Integer, Index As Integer, PoreWaterObs As Integer, SoilIndex As Integer
    Dim TempIndex As Integer, MaxNminObs As Integer, MaxCminObs As Integer
    Dim Index1 As Integer, DrainageObs As Integer, MaxTemperatureObs As Integer
    Dim RowIndex1 As Integer, MaxWaterObs As Integer, WaterIndex As Integer
    Dim CurrentObsStart As Integer
    Dim DateVal
    Dim PlantName As String
    Dim Replicate
    Dim DMStorage, DMTop, DMRoot, DMRootPL, DMRootTubers, GLAI, YLAI, NTop
    Dim NStorage, NRoot, NRootPL, NRootTubers, N15
    Dim Distribution, deltaDM, StartDepth, EndDepth, Depth, Ntype
    Dim Nmin, NO3, NH4, Total15N, EmptyType, Ctype, SMB

    Sheets("Observations").Activate
    ObsStart(0) = ObsStart(17) + 2
    RowIndex = ObsStart(0)

    ' Get plant observations and sum correctly
    maxRec = 15     'max number of attributes for each record
    SqlText = "SELECT [Crop Id] FROM [Measure Plant] WHERE (ID='" & ExpID & "') GROUP BY [Crop Id]"
    Set PlantRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
    NumberOfPlants = PlantRecord.RecordCount
    For plantIndex = 0 To NumberOfPlants - 1
        PlantNameString(plantIndex) = PlantRecord.Fields(0).Value
        Cells(ObsStart(0) - 1, 2 + plantIndex * maxRec) = PlantNameString(plantIndex)
        PlantRecord.MoveNext
    Next
    For plantIndex = 0 To NumberOfPlants - 1
        Cells(ObsStart(0), plantIndex * maxRec + 1) = "Date"
        Cells(ObsStart(0), plantIndex * maxRec + 2) = "DMTotalTop"
        Cells(ObsStart(0), plantIndex * maxRec + 3) = "DMStorage"
        Cells(ObsStart(0), plantIndex * maxRec + 4) = "DMRoot"
        Cells(ObsStart(0), plantIndex * maxRec + 5) = "DMRootPL"
        Cells(ObsStart(0), plantIndex * maxRec + 6) = "DMRootTubers"
        Cells(ObsStart(0), plantIndex * maxRec + 7) = "GLAI"
        Cells(ObsStart(0), plantIndex * maxRec + 8) = "YLAI"
        Cells(ObsStart(0), plantIndex * maxRec + 9) = "NTop"
        Cells(ObsStart(0), plantIndex * maxRec + 10) = "NStorage"
        Cells(ObsStart(0), plantIndex * maxRec + 11) = "NRoot"
        Cells(ObsStart(0), plantIndex * maxRec + 12) = "NRootPL"
        Cells(ObsStart(0), plantIndex * maxRec + 13) = "NRootTubers"
        Cells(ObsStart(0), plantIndex * maxRec + 14) = "N15"
        Cells(ObsStart(0), plantIndex * maxRec + 15) = "deltaDM"
    Next

    SqlText = "SELECT Date FROM [Measure Plant] WHERE (ID='" & ExpID & "') GROUP BY Date"
    Set DateRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
    NumberOfDates = DateRecord.RecordCount
    For DateIndex = 0 To NumberOfDates - 1
        DateString(DateIndex) = Format(DateRecord.Fields(0).Value, "#####")
        DateRecord.MoveNext
    Next
    For DateIndex = 0 To NumberOfDates - 1
        DateVal = DateString(DateIndex)

        For plantIndex = 0 To NumberOfPlants - 1
            PlantName = PlantNameString(plantIndex)
            SqlText = "Select Replicate From [Measure Plant] Where (ID='" & ExpID & "' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "') GROUP BY Replicate"
            Set Replicates = Base.OpenRecordset(SqlText, dbOpenDynaset)

            While Not Replicates.EOF
                Replicate = Replicates.Fields(0).Value
                DMStorage = 0: DMTop = 0: DMRoot = 0: DMRootPL = 0: DMRootTubers = 0: GLAI = 0: YLAI = 0
                NTop = 0: NStorage = 0: NRoot = 0: NRootPL = 0: NRootTubers = 0: N15 = 0
                'green leaf
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'GL' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMTop = Record.Fields(0).Value

                    GLAI = Record.Fields(1).Value
                    NTop = Record.Fields(2).Value

                    N15 = Record.Fields(3).Value
                End If
                'yellow leaf
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'YL' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMTop = DMTop + Record.Fields(0).Value

                    YLAI = Record.Fields(1).Value
                    If IsNull(YLAI) Then YLAI = 0
                    NTop = NTop + Record.Fields(2).Value

                    N15 = N15 + Record.Fields(3).Value
                End If
                'root
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'ROOT' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMRoot = Record.Fields(0).Value
                    NRoot = Record.Fields(2).Value
                    N15 = N15 + Record.Fields(3).Value
                End If
                'roots in ploughlayer
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'ROOTPL' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMRootPL = Record.Fields(0).Value
                    NRootPL = Record.Fields(2).Value
                End If
                'roots and Tubers
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'ROOTaTUB' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMRootTubers = Record.Fields(0).Value
                    NRootTubers = Record.Fields(2).Value
                End If
                'Semi green leaf
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'SGL' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    DMTop = DMTop + Record.Fields(0).Value

                    GLAI = GLAI + 0.5 * Record.Fields(1).Value
                    YLAI = YLAI + 0.5 * Record.Fields(1).Value
                    NTop = NTop + Record.Fields(2).Value

                    N15 = N15 + Record.Fields(3).Value
                End If
                'Stem
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'STM' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    Distribution = 1
                    If GLAI > 0 Then Distribution = GLAI / (GLAI + YLAI)
                    DMTop = DMTop + Record.Fields(0).Value

                    GLAI = GLAI + Distribution * Record.Fields(1).Value             ' GLAI distributed as leafs
                    YLAI = YLAI + (1 - Distribution) * Record.Fields(1).Value
                    NTop = NTop + Record.Fields(2).Value

                    N15 = N15 + Record.Fields(3).Value
                End If
                'Vegetative
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'VEG' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    If Record.Fields(0).Value > 0 Then DMTop = Record.Fields(0).Value
                    If Record.Fields(1).Value > 0 Then
                        GLAI = Record.Fields(1).Value             ' previous values overruled
                        YLAI = 0
                    End If
                    If Record.Fields(2).Value > 0 Then NTop = Record.Fields(2).Value
                    If Record.Fields(3).Value > 0 Then N15 = Record.Fields(3).Value
                End If
                'Storage
                SqlText = "Select DM, LAI, N, [15N] From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'STO' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    Distribution = 1
                    If GLAI > 0 Then Distribution = GLAI / (GLAI + YLAI)
                    DMTop = DMTop + Record.Fields(0).Value
                    DMStorage = Record.Fields(0).Value
                    If Record.Fields(1).Value > 0 Then
                        GLAI = GLAI + Distribution * Record.Fields(1).Value             ' GLAI distributed as leafs
                        YLAI = YLAI + (1 - Distribution) * Record.Fields(1).Value
                    End If
                    NTop = NTop + Record.Fields(2).Value
                    NStorage = Record.Fields(2).Value
                    N15 = N15 + Record.Fields(3).Value
                End If
                'Total
                SqlText = "Select DM, LAI, N, [15N],deltaDM From [Measure Plant] Where (ID='" & ExpID & "' AND FractionID = 'TOT' AND Date = " & DateVal & " AND [Crop ID] = '" & PlantName & "' AND Replicate = " & Replicate & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
                If Not Record.EOF Then
                    deltaDM = 0
                    If Record.Fields(0).Value > 0 Then DMTop = Record.Fields(0).Value
                    If Record.Fields(1).Value > 0 Then
                        GLAI = Record.Fields(1).Value             ' previous values overruled
                        YLAI = 0
                    End If
                    If Record.Fields(2).Value > 0 Then NTop = Record.Fields(2).Value
                    If Record.Fields(3).Value > 0 Then N15 = Record.Fields(3).Value
                    If Record.Fields(4).Value > 0 Then deltaDM = Record.Fields(4).Value
                End If
                Replicates.MoveNext
                RowIndex = RowIndex + 1
                Cells(RowIndex, 1) = DateVal
                Cells(RowIndex, 1).NumberFormat = "dd-mm-yy"
                If DMTop > 0 Then Cells(RowIndex, plantIndex * maxRec + 2) = DMTop
                If DMStorage > 0 Then Cells(RowIndex, plantIndex * maxRec + 3) = DMStorage

                If DMRoot > 0 Then Cells(RowIndex, plantIndex * maxRec + 4) = DMRoot
                If DMRootPL > 0 Then Cells(RowIndex, plantIndex * maxRec + 5) = DMRootPL
                If DMRootTubers > 0 Then Cells(RowIndex, plantIndex * maxRec + 6) = DMRootTubers
                If GLAI > 0 Then Cells(RowIndex, plantIndex * maxRec + 7) = GLAI
                If YLAI > 0 Then Cells(RowIndex, plantIndex * maxRec + 8) = YLAI
                If NTop > 0 Then Cells(RowIndex, plantIndex * maxRec + 9) = NTop

                If NStorage > 0 Then Cells(RowIndex, plantIndex * maxRec + 10) = NStorage
                If NRoot > 0 Then Cells(RowIndex, plantIndex * maxRec + 11) = NRoot
                If NRootPL > 0 Then Cells(RowIndex, plantIndex * maxRec + 12) = NRootPL
                If NRootTubers > 0 Then Cells(RowIndex, plantIndex * maxRec + 13) = NRootTubers
                If N15 > 0 Then Cells(RowIndex, plantIndex * maxRec + 14) = N15
                If deltaDM > 0 Then Cells(RowIndex, plantIndex * maxRec + 15) = deltaDM
            Wend
        Next
    Next

    'Soil Water
    RowIndex = RowIndex + 3
    RowIndex1 = RowIndex
    ObsStart(1) = RowIndex - 1
    MaxWaterObs = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil Water] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set WaterRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not WaterRecord.EOF Then
        WaterRecord.MoveFirst
        For WaterIndex = 0 To WaterRecord.RecordCount - 1
            StartDepth = WaterRecord.Fields(0).Value
            EndDepth = WaterRecord.Fields(1).Value
            Cells(RowIndex - 1, WaterIndex * 2 + 1) = "Date"
            Cells(RowIndex - 1, WaterIndex * 2 + 2) = "water" & StartDepth * 10 & "-" & EndDepth * 10

            SqlText = "SELECT Date, WaterContent FROM [Measure Soil Water] WHERE (ID='" & ExpID & "' AND StartDepth=" & StartDepth & " AND EndDepth=" & EndDepth & ") GROUP BY Date, WaterContent "
            Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
            Record.MoveFirst
            RowIndex1 = RowIndex
            MaxRecords = Record.RecordCount
            For Index = 0 To MaxRecords - 1
                Cells(RowIndex1, WaterIndex * 2 + 1) = Record.Fields(0).Value   ' date
                Cells(RowIndex1, WaterIndex * 2 + 2) = (EndDepth - StartDepth) / 10 * Record.Fields(1).Value    ' water content
                'Cells(RowIndex1, WaterIndex * 2 + 2) = Record.Fields(1).Value ' water content
                RowIndex1 = RowIndex1 + 1
                Record.MoveNext
            Next
            If RowIndex1 - RowIndex > MaxWaterObs Then MaxWaterObs = RowIndex1 - RowIndex
            WaterRecord.MoveNext
        Next
    Else    ' empty then use defaults
        Cells(ObsStart(1), 1) = "Date"
        Cells(ObsStart(1), 2) = "Water0-200"
        Cells(ObsStart(1), 3) = "Date"
        Cells(ObsStart(1), 4) = "Water0-500"
        Cells(ObsStart(1), 3) = "Date"
        Cells(ObsStart(1), 4) = "Water0-1000"
    End If

    'Pore water
    ObsStart(2) = ObsStart(1) + MaxWaterObs + 2
    RowIndex = RowIndex1 + 3
    SqlText = "SELECT Depth FROM [Measure Pore Water] WHERE (ID='" & ExpID & "') GROUP BY Depth"
    Set SoilRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)

    For PoreWaterObs = 2 To 6    ' nitrate, ammonium, totalN, Cl og N15 observations
        MaxWaterObs = 0
        If Not SoilRecord.EOF Then
            SoilRecord.MoveFirst
            For SoilIndex = 0 To SoilRecord.RecordCount - 1

                Depth = SoilRecord.Fields(0).Value

                Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 1) = "Date"
                If PoreWaterObs = 2 Then Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 2) = Depth & "cmNit-N"
                If PoreWaterObs = 3 Then Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 2) = Depth & "cmAm-N"
                If PoreWaterObs = 4 Then Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 2) = Depth & "cmN"
                If PoreWaterObs = 5 Then Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 2) = Depth & "cmCl"
                If PoreWaterObs = 6 Then Cells(ObsStart(PoreWaterObs), SoilIndex * 2 + 2) = Depth & "cmN15"

                If PoreWaterObs = 2 Then SqlText = "SELECT Date, Nitrate FROM [Measure Pore Water] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
                If PoreWaterObs = 3 Then SqlText = "SELECT Date, Ammonium FROM [Measure Pore Water] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
                If PoreWaterObs = 4 Then SqlText = "SELECT Date, TotalN FROM [Measure Pore Water] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
                If PoreWaterObs = 5 Then SqlText = "SELECT Date, Cl FROM [Measure Pore Water] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
                If PoreWaterObs = 6 Then SqlText = "SELECT Date, [15N] FROM [Measure Pore Water] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
                Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)

                Record.MoveLast
                MaxRecords = Record.RecordCount
                Record.MoveFirst
                Index1 = 0

                For Index = 0 To MaxRecords - 1
                    If (Record.Fields(1).Value > 0) Then
                        Cells(ObsStart(PoreWaterObs) + 1 + Index1, SoilIndex * 2 + 1) = Record.Fields(0).Value    ' date
                        Cells(ObsStart(PoreWaterObs) + 1 + Index1, SoilIndex * 2 + 2) = Record.Fields(1).Value    ' value
                        Index1 = Index1 + 1
                    End If
                    Record.MoveNext
                Next
                If MaxWaterObs < Index1 Then MaxWaterObs = Index1

                SoilRecord.MoveNext
            Next
            SoilRecord.MoveFirst
        End If
        ObsStart(PoreWaterObs + 1) = ObsStart(PoreWaterObs) + MaxWaterObs + 2
    Next

    'Drainage
    RowIndex = RowIndex1 + 3
    MaxRecords = 0
    SqlText = "SELECT Date, Drainage FROM [Measure Drainage] WHERE (ID='" & ExpID & "')"
    DrainageObs = PoreWaterObs
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveLast
        MaxRecords = Record.RecordCount
        Record.MoveFirst
        Cells(ObsStart(DrainageObs), 1) = "date"
        Cells(ObsStart(DrainageObs), 2) = "Drainage"

        For Index = 0 To MaxRecords - 1
            Cells(ObsStart(DrainageObs) + 1 + Index, 1) = Record.Fields(0).Value    ' date
            Cells(ObsStart(DrainageObs) + 1 + Index, 2) = Record.Fields(1).Value    ' drainage
            Record.MoveNext
        Next
    End If
    ObsStart(DrainageObs + 1) = ObsStart(DrainageObs) + MaxRecords + 2

    'N Drainage
    RowIndex = RowIndex1 + 3
    MaxRecords = 0
    DrainageObs = DrainageObs + 1
    SqlText = "SELECT Date, [Drainage-N] FROM [Measure Drainage] WHERE (ID='" & ExpID & "')"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveLast
        MaxRecords = Record.RecordCount
        Record.MoveFirst
        Cells(ObsStart(DrainageObs), 1) = "date"
        Cells(ObsStart(DrainageObs), 2) = "Drainage-N"

        For Index = 0 To MaxRecords - 1
            Cells(ObsStart(DrainageObs) + 1 + Index, 1) = Record.Fields(0).Value    ' date
            Cells(ObsStart(DrainageObs) + 1 + Index, 2) = Record.Fields(1).Value  ' drainage N
            Record.MoveNext
        Next
    End If
    ObsStart(DrainageObs + 1) = ObsStart(DrainageObs) + MaxRecords + 2

    'Soil temperature
    RowIndex = ObsStart(9) + 1
    MaxTemperatureObs = 0
    SqlText = "SELECT depth FROM [Measure Soil Temperature] WHERE (ID='" & ExpID & "') GROUP BY depth"
    Set TemperatureRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not TemperatureRecord.EOF Then
        TemperatureRecord.MoveFirst
        For TempIndex = 0 To TemperatureRecord.RecordCount - 1
            Depth = TemperatureRecord.Fields(0).Value
            Cells(RowIndex - 1, TempIndex * 2 + 1) = "Date"
            Cells(RowIndex - 1, TempIndex * 2 + 2) = "t" & Depth * 10

            SqlText = "SELECT Date, Temperature FROM [Measure Soil Temperature] WHERE (ID='" & ExpID & "' AND Depth=" & Depth & ")"
            Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
            Record.MoveFirst
            RowIndex1 = RowIndex
            MaxRecords = Record.RecordCount
            For Index = 0 To Record.RecordCount - 1
                If Not Record.EOF Then
                    Cells(RowIndex1, TempIndex * 2 + 1) = Record.Fields(0).Value   ' date
                    Cells(RowIndex1, TempIndex * 2 + 2) = Record.Fields(1).Value   ' temperature
                    RowIndex1 = RowIndex1 + 1
                    Record.MoveNext
                End If
            Next
            If RowIndex1 - RowIndex > MaxTemperatureObs Then MaxTemperatureObs = RowIndex1 - RowIndex
            TemperatureRecord.MoveNext
        Next
    End If
    ObsStart(10) = ObsStart(9) + MaxTemperatureObs + 2

    'N2O emission
    MaxRecords = 0
    RowIndex = ObsStart(10) + 1
    SqlText = "SELECT Date, N2O, Denitrification FROM [Measure N gas] WHERE (ID='" & ExpID & "')"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveLast
        MaxRecords = Record.RecordCount
        Record.MoveFirst
        Cells(ObsStart(10), 1) = "date"
        Cells(ObsStart(10), 2) = "N2O"
        Cells(ObsStart(10), 4) = "Denitrification"
        For Index = 0 To MaxRecords - 1
            Cells(ObsStart(10) + 1 + Index, 1) = Record.Fields(0).Value    ' date
            Cells(ObsStart(10) + 1 + Index, 2) = Record.Fields(1).Value    ' N2O
            Cells(ObsStart(10) + 1 + Index, 3) = Record.Fields(0).Value    ' date
            Cells(ObsStart(10) + 1 + Index, 4) = Record.Fields(2).Value    ' denitrification
            Record.MoveNext
        Next
    End If
    ObsStart(11) = ObsStart(10) + MaxRecords + 2

    'CO2 emission
    RowIndex = ObsStart(11) + 3
    SqlText = "SELECT Date, CO2_flux FROM [Measure Mineralisation] WHERE (ID='" & ExpID & "')"
    Set Record = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not Record.EOF Then
        Record.MoveLast
        MaxRecords = Record.RecordCount
        Record.MoveFirst
        Cells(ObsStart(11), 1) = "date"
        Cells(ObsStart(11), 2) = "CO2"
        For Index = 0 To MaxRecords - 1
            Cells(ObsStart(11) + 1 + Index, 1) = Record.Fields(0).Value    ' date
            Cells(ObsStart(11) + 1 + Index, 2) = 24 * Record.Fields(1).Value    ' CO2 in g m-2 h-1 in database (convert to FASSET output of g C m-2 d-1)
            Record.MoveNext
        Next
    End If
    ObsStart(12) = ObsStart(11) + MaxRecords + 2


    'Soil N
    MaxNminObs = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil N] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    For Ntype = 1 To 4
        Set SoilRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
        If Not SoilRecord.EOF Then SoilRecord.MoveFirst
        CurrentObsStart = ObsStart(Ntype + 11)
        If Not SoilRecord.EOF Then
            For SoilIndex = 0 To SoilRecord.RecordCount - 1
                StartDepth = SoilRecord.Fields(0).Value
                EndDepth = SoilRecord.Fields(1).Value
                Cells(CurrentObsStart, SoilIndex * 2 + 1) = "Date"
                If (Ntype = 1) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "Nmin" & StartDepth * 10 & "-" & EndDepth * 10
                If (Ntype = 2) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "NO3-" & StartDepth * 10 & "-" & EndDepth * 10
                If (Ntype = 3) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "NH4-" & StartDepth * 10 & "-" & EndDepth * 10
                If (Ntype = 4) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "Total15N-" & StartDepth * 10 & "-" & EndDepth * 10

                SqlText1 = "SELECT Date, [Mineral-N], [Mineral-NO3], [Mineral-NH4],[15N-organic_N],[15N-Mineral_N] FROM [Measure Soil N] WHERE (ID='" & ExpID & "' AND StartDepth=" & StartDepth & " AND EndDepth=" & EndDepth & ")"
                Set Record = Base.OpenRecordset(SqlText1, dbOpenDynaset)
                Record.MoveLast
                MaxRecords = Record.RecordCount
                If MaxRecords > MaxNminObs Then MaxNminObs = MaxRecords
                Record.MoveFirst
                For Index = 0 To MaxRecords - 1
                    Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 1) = Record.Fields(0).Value    ' date
                    Nmin = 0
                    NO3 = 0
                    NH4 = 0
                    Total15N = 0
                    If Record.Fields(1) > 0 Then Nmin = Record.Fields(1)
                    If Record.Fields(2) > 0 Then NO3 = Record.Fields(2)
                    If Record.Fields(3) > 0 Then NH4 = Record.Fields(3)
                    If Record.Fields(4) > 0 Then Total15N = CDbl(Record.Fields(4))
                    If Record.Fields(5) > 0 Then Total15N = Total15N + CDbl(Record.Fields(5))

                    If (Nmin = 0) Then Nmin = NO3 + NH4
                    If (Ntype = 1 And Nmin > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * Nmin * 1.5
                    If (Ntype = 2 And NO3 > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * NO3 * 1.5
                    If (Ntype = 3 And NH4 > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * NH4 * 1.5
                    If (Ntype = 4 And Total15N > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * Total15N * 1.5
                    Record.MoveNext
                Next
                SoilRecord.MoveNext
            Next
        Else    ' empty then use defaults
            EmptyType = "Nmin"
            If (Ntype = 2) Then EmptyType = "NO3-"
            If (Ntype = 3) Then EmptyType = "NH4-"
            Cells(CurrentObsStart, 1) = "Date"
            Cells(CurrentObsStart, 2) = EmptyType + "0-20"
            Cells(CurrentObsStart, 3) = "Date"
            Cells(CurrentObsStart, 4) = EmptyType + "0-1000"
        End If
        ObsStart(Ntype + 12) = MaxNminObs + ObsStart(Ntype + 11) + 2
    Next

    'Soil C
    MaxCminObs = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil C] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    Set SoilRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
    If Not SoilRecord.EOF Then SoilRecord.MoveFirst
    Ctype = 1
    CurrentObsStart = ObsStart(Ctype + 15)
    If Not SoilRecord.EOF Then
        For SoilIndex = 0 To SoilRecord.RecordCount - 1
            StartDepth = SoilRecord.Fields(0).Value
            EndDepth = SoilRecord.Fields(1).Value
            Cells(CurrentObsStart, SoilIndex * 2 + 1) = "Date"
            If (Ctype = 1) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "SMB-C-" & StartDepth * 10 & "-" & EndDepth * 10

            SqlText1 = "SELECT Date,  [Microbial_Biomass] FROM [Measure Soil C] WHERE (ID='" & ExpID & "' AND StartDepth=" & StartDepth & " AND EndDepth=" & EndDepth & ")"
            Set Record = Base.OpenRecordset(SqlText1, dbOpenDynaset)
            Record.MoveLast
            MaxRecords = Record.RecordCount
            If MaxRecords > MaxCminObs Then MaxCminObs = MaxRecords
            Record.MoveFirst
            For Index = 0 To MaxRecords - 1
                Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 1) = Record.Fields(0).Value    ' date
                SMB = 0
                If Record.Fields(1) > 0 Then SMB = Record.Fields(1)

                If (Ctype = 1 And SMB > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = SMB
                Record.MoveNext
            Next
            SoilRecord.MoveNext
        Next
    Else    ' empty then use defaults
        EmptyType = "SMB-"
        Cells(CurrentObsStart, 1) = "Date"
        Cells(CurrentObsStart, 2) = EmptyType + "0-20"
        Cells(CurrentObsStart, 3) = "Date"
        Cells(CurrentObsStart, 4) = EmptyType + "0-1000"
    End If
    ObsStart(Ctype + 16) = MaxCminObs + ObsStart(Ctype + 15) + 2
End Sub
Function FindSimCol(SheetName As String, StartRow, ColName As String, NumerOfCol As Integer)
    Dim ColIndex As Integer
    Dim found As Boolean
    Dim NumberOfColsFound As Integer


    ColIndex = 1
    found = False
    Sheets(SheetName).Activate
    NumberOfColsFound = NumerOfCol
    While Cells(StartRow, ColIndex) > "" And Not found
        If LCase(ColName) = LCase(Cells(StartRow, ColIndex)) Then NumberOfColsFound = NumberOfColsFound - 1      ' used for finding several columns with same name
        If NumberOfColsFound <= 0 Then
            found = True
        Else
            ColIndex = ColIndex + 1
        End If
    Wend


    If found Then
        FindSimCol = ColIndex
    Else
        FindSimCol = 0
    End If
End Function

Sub FindPlotDates()
    Dim i As Integer
    Dim aDate


    FirstSimDate = CDate(Sheets("PS").Cells(PlantSimStart + 1, 1))
    PlotStartDate = CDate(Sheets("Management").Cells(ManagementStart(0) + 1, 1))
    PlotEndDate = CDate(Sheets("Management").Cells(2, 2))
    If (IsDate(Sheets("Management").Cells(ManagementStart(4) - 2, 1)) And (Sheets("Management").Cells(ManagementStart(4) - 2, 1) > PlotEndDate)) Then
        PlotEndDate = Sheets("Management").Cells(ManagementStart(4) - 2, 1)
    End If

    ' check managements dates
    For i = ManagementStart(0) To ManagementStart(8)
        aDate = Sheets("Management").Cells(i, 1)
        If IsDate(aDate) Then
            If aDate < PlotStartDate Then
                PlotStartDate = aDate
            End If
        End If
    Next i

    ' check observations dates
    For i = 0 To 10
        If (ObsStart(i) > 0) Then
            aDate = Sheets("Observations").Cells(ObsStart(i) + 1, 1)
            If IsDate(aDate) And aDate < PlotStartDate Then PlotStartDate = aDate
            If ObsStart(i) > 2 Then aDate = Sheets("Observations").Cells(ObsStart(i) - 2, 1)
            If IsDate(aDate) And aDate > PlotEndDate Then PlotEndDate = aDate
        End If
    Next i

    PlotStartDate = PlotStartDate - 14
    PlotEndDate = PlotEndDate + 30
End Sub

Sub GenerateSingleGraph(ObsRanges As Variant, SimRanges As Variant, x, y)
    Dim SheetIndex As Integer

    Sheets("Graph").Activate
    Charts.Add
    ActiveChart.ChartType = xlXYScatter
    'ActiveChart.ChartType = xlLineMarkers
    ActiveChart.Location WHERE:=xlLocationAsObject, Name:="Graph"
    Dim i As Integer
    Dim ymax As Double
    Dim rng As Range
    Dim Key

    ymax = 0
    i = 1

    While ActiveChart.SeriesCollection.Count > 0
        ActiveChart.SeriesCollection(1).Delete
    Wend

    For Each Key In SimRanges.Keys
        Set rng = SimRanges.Item(Key)
            ActiveChart.SeriesCollection.Add Source:=rng, Rowcol:=xlColumns, _
                                             SeriesLabels:=False, CategoryLabels:=True, Replace:=False
            With ActiveChart.SeriesCollection(ActiveChart.SeriesCollection.Count)
                .MarkerStyle = xlNone
                .Border.LineStyle = xlContinuous
                .Border.Weight = xlThin
                .Border.ColorIndex = i + 2
                .Name = Key
                i = i + 1
            End With
            ymax = WorksheetFunction.Max(ymax, _
                                         Range(rng.Cells(PlotStartDate - FirstSimDate + 15, 1), _
                                               rng.Cells(PlotEndDate - FirstSimDate, 1)))
    Next

    If Not ObsRanges Is Nothing Then
        i = 1
        For Each Key In ObsRanges.Keys
            Set rng = ObsRanges.Item(Key)
            If Application.WorksheetFunction.CountA(rng) > 0 Then
                ActiveChart.SeriesCollection.Add Source:=rng, Rowcol:=xlColumns, _
                                                 SeriesLabels:=False, CategoryLabels:=True, Replace:=False
                With ActiveChart.SeriesCollection(ActiveChart.SeriesCollection.Count)
                    .MarkerStyle = xlMarkerStyleCircle
                    .MarkerSize = 2
                    .Border.LineStyle = xlNone
                    .MarkerBackgroundColorIndex = i + 2
                    .MarkerForegroundColorIndex = i + 2
                    .Name = Key
                    i = i + 1
                End With
                ymax = WorksheetFunction.Max(ymax, _
                                             Range(rng.Cells(PlotStartDate - FirstSimDate + 15, 1), _
                                                   rng.Cells(PlotEndDate - FirstSimDate, 1)))
            End If
        Next
    End If

    With ActiveChart
        .HasTitle = False
        .HasLegend = True
        .PlotArea.ClearFormats
        '.ChartTitle.Characters.Text = Title
    End With
    With ActiveChart.Axes(xlCategory)
        '.CategoryType = xlTimeScale
        '.BaseUnit = xlDays
        '.MajorUnit = xlMonths
        .HasTitle = False
        .TickLabels.NumberFormat = "d. mmmm yy"
        .TickLabels.Orientation = 45  ' xlUpward
        .MinimumScale = DateSerial(Year(PlotStartDate), Month(PlotStartDate), 1)
        .MaximumScale = PlotEndDate
        .MajorUnit = 365.25 / 4
        .MinorUnitIsAuto = True
    End With
    With ActiveChart.Axes(xlValue)
        .MinimumScale = 0
        .HasTitle = False
        .HasMajorGridlines = False
        .TickLabels.NumberFormat = "General"
        If ymax > 1 And ymax < 3000 Then
            .MaximumScale = WorksheetFunction.RoundUp(ymax, 0)
        Else
            .MaximumScaleIsAuto = True
        End If
    End With

    SheetIndex = ActiveSheet.Shapes.Count
    With ActiveSheet.Shapes.Item(SheetIndex)
        .Height = Sheets("Main").Range("J1")
        .Width = Sheets("Main").Range("J2")
        .Top = Sheets("Main").Range("J3") * y
        .Left = 48 + Sheets("Main").Range("J4") * (x - 2)    ' not first coloumn
    End With

    If Not ObsRanges Is Nothing Then
        ObsRanges.RemoveAll
    End If

    If Not SimRanges Is Nothing Then
        SimRanges.RemoveAll
    End If

End Sub
Sub GenerateGraph()
    Dim r1 As Range, r2 As Range
    Dim ColIndex As Integer, PlantObsIndex As Integer, plantIndex As Integer, SimIndex As Integer
    Dim HarvestItem As Integer, ExtraTypeCount As Integer, ObsTypeCount As Integer
    Dim DateCol As Integer, SimCol As Integer
    Dim MaxNumberOfPlants As Integer    ', MaxObsVal As Integer
    Dim GraphNumberGenerated As Integer
    Dim obsfound As Boolean
    Dim MeasureName As String, MeasureNamesArray(25) As String
    Dim ObsRanges, SimRanges
    Dim Obsx As Range, Obsy As Range, Simx As Range, Simy As Range
    Dim idx As Integer
    Dim i As Integer
    Set ObsRanges = CreateObject("Scripting.Dictionary")
    Set SimRanges = CreateObject("Scripting.Dictionary")

    Call FindPlotDates

    'Plant Measurements

    ColIndex = 1
    MaxNumberOfPlants = 0
    Dim SimName As String

    For PlantObsIndex = 1 To 13
        Sheets("Observations").Activate
        MeasureName = Cells(ObsStart(0), PlantObsIndex + 1)
        obsfound = False

        For plantIndex = 1 To 8    ' Max 8 simlutaneous plants
            SimCol = FindSimCol("Observations", ObsStart(0), MeasureName, plantIndex)
            If SimCol > 0 Then
                Set r1 = Range(Cells(ObsStart(0) + 1, SimCol), Cells(ObsStart(1) - 2, SimCol))
                If Application.WorksheetFunction.CountA(r1) > 0 Then
                    Set r2 = Range(Cells(ObsStart(0) + 1, 1), Cells(ObsStart(1) - 2, 1))
                    ObsRanges.Add (MeasureName & plantIndex), Union(r1, r2)
                    obsfound = Application.WorksheetFunction.CountA(r1) > 0    ' Coloumn contains data
                    MaxNumberOfPlants = Application.WorksheetFunction.Max(MaxNumberOfPlants, plantIndex)
                End If
            End If
        Next

        If obsfound Then
            For SimIndex = 1 To MaxNumberOfPlants + 2
                SimCol = FindSimCol("PS", PlantSimStart, MeasureName, SimIndex)
                If SimCol > 0 Then
                    Set r1 = Sheets("PS").Range(Cells(PlantSimStart + 1, SimCol), Cells(PlantSimEnd, SimCol))  ' simulation
                    If Application.WorksheetFunction.CountA(r1) > 0 Then
                        Set r2 = Sheets("PS").Range(Cells(PlantSimStart + 1, 1), Cells(PlantSimEnd, 1))    ' dates
                        SimRanges.Add (MeasureName & SimIndex), Union(r1, r2)
                    End If
                End If
            Next
            ColIndex = ColIndex + 1
            Call GenerateSingleGraph(ObsRanges, SimRanges, ColIndex, GraphIndex)
        End If
    Next

    ' Extra Plant simulations graphs
    For ExtraTypeCount = 1 To 13
        SimName = Sheets("Main").Range("F2").Offset(ExtraTypeCount, 0)
        If SimName <> "" Then
            For SimIndex = 1 To MaxNumberOfPlants
                SimCol = FindSimCol("PS", PlantSimStart, SimName, SimIndex)
                If SimCol > 0 Then
                    Set r1 = Sheets("PS").Range(Cells(PlantSimStart + 1, SimCol), Cells(PlantSimEnd, SimCol))  ' simulation
                    If Application.WorksheetFunction.CountA(r1) > 0 Then
                        Set r2 = Sheets("PS").Range(Cells(PlantSimStart + 1, 1), Cells(PlantSimEnd, 1))    ' dates
                        SimRanges.Add (SimName & SimIndex), Union(r1, r2)
                    End If
                End If
            Next
            If SimRanges.Count > 0 Then
                ColIndex = ColIndex + 1
                Call GenerateSingleGraph(Nothing, SimRanges, ColIndex, GraphIndex)
            End If
        End If
    Next


    'Harvests graphs
    Dim s As String
    For HarvestItem = 0 To 1

        If HarvestItem = 1 Then
            s = "N Harvest"
        Else
            s = "DM Harvest"
        End If

        Sheets("Harv").Activate
        Set Obsx = Range(Cells(HarvestDataStart, 11), Cells(HarvestDataRow, 11))
        Set Obsy = Range(Cells(HarvestDataStart, 13 + HarvestItem * 3), _
                         Cells(HarvestDataRow, 13 + HarvestItem * 3))
        ObsRanges.Add s & "1", Union(Obsx, Obsy)
        'MaxObsVal = Application.WorksheetFunction.Max(Obsy)

        Set Obsy = Range(Cells(HarvestDataStart, 14 + HarvestItem * 3), _
                         Cells(HarvestDataRow, 14 + HarvestItem * 3))
        'MaxObsVal = MaxFunc(MaxObsVal, Application.WorksheetFunction.Max(Obsy))
        ObsRanges.Add s & "2", Union(Obsx, Obsy)

        Set Simx = Range(Cells(HarvestDataStart, 3), Cells(HarvestDataRow, 3))
        Set Simy = Range(Cells(HarvestDataStart, 4 + HarvestItem * 2), _
                         Cells(HarvestDataRow, 4 + HarvestItem * 2))
        'MaxObsVal = MaxFunc(MaxObsVal, Application.WorksheetFunction.Max(Simy))
        SimRanges.Add s & "1", Union(Simx, Simy)

        Set Simy = Range(Cells(HarvestDataStart, 5 + HarvestItem * 2), _
                         Cells(HarvestDataRow, 5 + HarvestItem * 2))
        'MaxObsVal = MaxFunc(MaxObsVal, Application.WorksheetFunction.Max(Simy))
        SimRanges.Add s & "2", Union(Simx, Simy)

        ColIndex = ColIndex + 1

        Call GenerateSingleGraph(ObsRanges, SimRanges, ColIndex, GraphIndex)
    Next

    Dim ObsCount As Integer

    'Soil Observations
    GraphIndex = GraphIndex + 1
    GraphNumberGenerated = 0
    For ObsTypeCount = 1 To 17
        If ObsStart(ObsTypeCount) > 0 And (ObsStart(ObsTypeCount + 1) - ObsStart(ObsTypeCount) > 2) Then
            ColIndex = 2
            ObsCount = 1
            Sheets("Observations").Activate
            While Cells(ObsStart(ObsTypeCount), ColIndex) <> ""
                MeasureNamesArray(ObsCount) = Cells(ObsStart(ObsTypeCount), ColIndex)
                Set r1 = Range(Cells(ObsStart(ObsTypeCount) + 1, ColIndex), _
                               Cells(ObsStart(ObsTypeCount + 1) - 2, ColIndex))
                If Application.WorksheetFunction.CountA(r1) > 0 Then
                    DateCol = ColIndex - 1
                    Set r2 = Range(Cells(ObsStart(ObsTypeCount) + 1, DateCol), _
                                   Cells(ObsStart(ObsTypeCount + 1) - 2, DateCol))
                    If Application.WorksheetFunction.CountA(r2) > 0 Then
                        ObsRanges.Add MeasureNamesArray(ObsCount), Union(r1, r2)
                    End If
                End If
                ColIndex = ColIndex + 2    ' only every second is an observaion
                ObsCount = ObsCount + 1
            Wend

            For i = 1 To ObsCount - 1
                SimCol = FindSimCol("SS", SoilSimStart, MeasureNamesArray(i), 1)
                If (SimCol > 0) Then
                    Set r1 = Range(Cells(SoilSimStart + 2, SimCol), Cells(SoilSimEnd, SimCol))    ' simulation
                    If Application.WorksheetFunction.CountA(r1) > 0 Then
                        Set r2 = Range(Cells(SoilSimStart + 2, 1), Cells(SoilSimEnd, 1))        ' dates
                        SimRanges.Add MeasureNamesArray(i), Union(r1, r2)
                    End If
                End If
            Next
            If ObsRanges.Count > 0 Then
                GraphNumberGenerated = GraphNumberGenerated + 1
                Call GenerateSingleGraph(ObsRanges, SimRanges, GraphNumberGenerated + 1, GraphIndex)
            End If
        End If
    Next


    ' Extra Soil simulations graphs
    For ExtraTypeCount = 1 To 10
        SimName = Sheets("Main").Range("G2").Offset(ExtraTypeCount, 0)
        If SimName <> "" Then
            SimCol = FindSimCol("SS", SoilSimStart, SimName, 1)
            If SimCol > 0 Then
                Set r1 = Range(Cells(SoilSimStart + 2, SimCol), Cells(SoilSimEnd, SimCol))    ' simulation
                If Application.WorksheetFunction.CountA(r1) > 0 Then
                    Set r2 = Range(Cells(SoilSimStart + 2, 1), Cells(SoilSimEnd, 1))    ' dates
                    SimRanges.Add SimName, Union(r1, r2)
                    Call GenerateSingleGraph(ObsRanges, SimRanges, _
                                             GraphNumberGenerated + ExtraTypeCount + 1, GraphIndex)
                End If
            End If
        End If
    Next

    For i = ManagementStart(0) + 1 To ManagementStart(1) - 2
        idx = 15 * (GraphIndex - 1) + i - ManagementStart(0)
        Sheets("Graph").Cells(idx, 1) = Year(Sheets("Management").Cells(i, 1)) & "-" & Sheets("Management").Cells(i, 2)
    Next i
    GraphIndex = GraphIndex + 1    ' new Graph Colomn
End Sub



Sub ExtractNminAndPlantN()
    Dim SimName As String

    SimNumber = 1
    RowSS = 1
    RowNmin = 1

    With Sheets("NminAndPlantN")
        .Range("a:aa").Clear
        .Cells(RowNmin, 2) = Sheets("SS").Cells(RowSS, 1)
        .Cells(RowNmin, 3) = Sheets("SS").Cells(RowSS, 5)
        .Cells(RowNmin, 4) = Sheets("SS").Cells(RowSS, 6)
        .Cells(RowNmin, 5) = Sheets("SS").Cells(RowSS, 7)
        .Cells(RowNmin, 6) = Sheets("SS").Cells(RowSS, 9)
        .Cells(RowNmin, 7) = Sheets("SS").Cells(RowSS, 10)
        .Cells(RowNmin, 8) = Sheets("SS").Cells(RowSS, 11)
        .Cells(RowNmin, 9) = Sheets("SS").Cells(RowSS, 13)
        .Cells(RowNmin, 10) = Sheets("SS").Cells(RowSS, 14)
        .Cells(RowNmin, 11) = Sheets("SS").Cells(RowSS, 15)
        .Cells(RowNmin, 12) = Sheets("SS").Cells(RowSS, 17)
        .Cells(RowNmin, 13) = Sheets("SS").Cells(RowSS, 18)
        .Cells(RowNmin, 14) = Sheets("SS").Cells(RowSS, 19)
        .Cells(RowNmin, 15) = Sheets("PS").Cells(RowSS, 22)
        .Cells(RowNmin, 16) = Sheets("PS").Cells(RowSS, 24)
    End With

    While Sheets("Main").Cells(SimNumber + 1, 1) <> ""
        SimName = Sheets("Main").Cells(SimNumber + 1, 1)
        RowSS = RowSS + 1
        RowNmin = RowNmin + 1

        While Sheets("SS").Cells(RowSS, 1) <> "date" And Sheets("SS").Cells(RowSS, 1) <> ""
            If DatePart("d", Sheets("SS").Cells(RowSS, 1)) = 9 And DatePart("m", Sheets("SS").Cells(RowSS, 1)) = 2 Then
                With Sheets("NminAndPlantN")
                    .Cells(RowNmin, 1) = SimName
                    .Cells(RowNmin, 2) = Sheets("SS").Cells(RowSS, 1)
                    .Cells(RowNmin, 3) = Sheets("SS").Cells(RowSS, 5)
                    .Cells(RowNmin, 4) = Sheets("SS").Cells(RowSS, 6)
                    .Cells(RowNmin, 5) = Sheets("SS").Cells(RowSS, 7)
                    .Cells(RowNmin, 6) = Sheets("SS").Cells(RowSS, 9)
                    .Cells(RowNmin, 7) = Sheets("SS").Cells(RowSS, 10)
                    .Cells(RowNmin, 8) = Sheets("SS").Cells(RowSS, 11)
                    .Cells(RowNmin, 9) = Sheets("SS").Cells(RowSS, 13)
                    .Cells(RowNmin, 10) = Sheets("SS").Cells(RowSS, 14)
                    .Cells(RowNmin, 11) = Sheets("SS").Cells(RowSS, 15)
                    .Cells(RowNmin, 12) = Sheets("SS").Cells(RowSS, 17)
                    .Cells(RowNmin, 13) = Sheets("SS").Cells(RowSS, 18)
                    .Cells(RowNmin, 14) = Sheets("SS").Cells(RowSS, 19)
                    .Cells(RowNmin, 15) = Sheets("PS").Cells(RowSS, 22)
                    .Cells(RowNmin, 16) = Sheets("PS").Cells(RowSS, 24)
                End With
                RowNmin = RowNmin + 1
            End If
            RowSS = RowSS + 1
        Wend
        SimNumber = SimNumber + 1
    Wend
End Sub
Sub ExtractData()
    Dim SimName As String
    Dim plantIndex As Integer
    'Plant Measurements
    For ExtraTypeCount = 1 To 10
        SimName = Sheets("Main").Range("H2").Offset(ExtraTypeCount, 0)
        If SimName > "" Then
            SimStart = Sheets("PS").Cells(PlantSimStart + 1, 1)
            For plantIndex = 1 To 5                                             ' Max 5 simlutaneous plants
                ExtractDataRow = (plantIndex - 1) * 100 + 1
                Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol) = SimName
                ExtractDataRow = ExtractDataRow + 1
                Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol) = "Date"
                Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol + 1) = ExpID + "-Obs"
                Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol + 2) = ExpID + "-Sim"
                ExtractDataRow = ExtractDataRow + 1
                SimCol = FindSimCol("PS", PlantSimStart, SimName, plantIndex)
                ObsCol = FindSimCol("Observations", ObsStart(0), SimName, plantIndex)
                For obsnumber = ObsStart(0) To ObsStart(1)
                    obsDate = Sheets("Observations").Cells(obsnumber + 1, 1)

                    If (ObsCol > 0) And IsDate(obsDate) Then
                        Obs = Sheets("Observations").Cells(obsnumber + 1, ObsCol)
                        If Obs > 0 Then
                            Sim = 0
                            If SimCol > 0 Then
                                Sim = Sheets("PS").Cells(PlantSimStart + (obsDate - SimStart), SimCol)
                                If (Sim = 0) Then
                                    Sim = Sheets("PS").Cells(PlantSimStart + (obsDate - SimStart) - 1, SimCol)
                                End If
                            End If
                            Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol) = obsDate
                            Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol + 1) = Obs
                            Sheets("Extract").Cells(ExtractDataRow, ExtractDataCol + 2) = Sim
                            ExtractDataRow = ExtractDataRow + 1
                        End If
                    End If
                Next
            Next
        End If
    Next
    ExtractDataCol = ExtractDataCol + 3
End Sub
Sub HarvestData()
    Workbooks.OpenText Filename:=Sheets("Main").Range("M4") & "scenarie.txt", Origin:=xlWindows, _
                       StartRow:=1, DataType:=xlDelimited, TextQualifier:=xlDoubleQuote, _
                       ConsecutiveDelimiter:=False, Tab:=True, Semicolon:=False, Comma:=False _
                                                                                        , Space:=False, Other:=False, FieldInfo:=Array(Array(1, 1), Array(2, 4), _
                                                                                                                                       Array(3, 1), Array(4, 1), Array(5, 1), Array(6, 1), Array(7, 1), Array(8, 1), Array(9, 1))
    Dim CompareRow, obsDate
    Dim found As Boolean
    Dim i As Integer, j As Integer

    ' Collect data for harvest sheet
    HarvestDataRow = HarvestDataRow + 2
    HarvestDataStart = HarvestDataRow

    Range("A3:N150").Copy
    ThisWorkbook.Activate
    With Sheets("Harv")
        .Select
        .Cells(HarvestDataRow - 1, 1) = "ExpID"
        .Cells(HarvestDataRow - 1, 2) = "Harvested crop"
        .Cells(HarvestDataRow - 1, 3) = "Harvest date"
        .Cells(HarvestDataRow - 1, 4) = "Sim DM Storage"
        .Cells(HarvestDataRow - 1, 5) = "Sim DM Straw"
        .Cells(HarvestDataRow - 1, 6) = "Sim N Storage"
        .Cells(HarvestDataRow - 1, 7) = "Sim N Straw"

        .Cells(HarvestDataRow - 1, 11) = "Harvest date"
        .Cells(HarvestDataRow - 1, 12) = "Harvested crop"
        .Cells(HarvestDataRow - 1, 13) = "Obs DM Storage"
        .Cells(HarvestDataRow - 1, 14) = "Obs DM Straw"
        .Cells(HarvestDataRow - 1, 16) = "Obs N Storage"
        .Cells(HarvestDataRow - 1, 17) = "Obs N Straw"

        .Cells(HarvestDataRow, 2).Select
    End With
    ActiveSheet.Paste
    Windows("scenarie.txt").Activate
    Range("A1").Select
    Application.CutCopyMode = False
    ActiveWindow.Close (False)
    ThisWorkbook.Activate

    j = ManagementStart(3)
    CompareRow = HarvestDataRow
    While j < ManagementStart(4)
        obsDate = Sheets("Management").Cells(j, 1)
        If IsDate(obsDate) Then
            found = False
            While Not found And (CompareRow < 1000) And (Year(obsDate) >= Year(Sheets("Harv").Cells(CompareRow, 3)))
                If Year(obsDate) = Year(Sheets("Harv").Cells(CompareRow, 3)) Then
                    For i = 1 To 9
                        Sheets("Harv").Cells(CompareRow, 10 + i) = Sheets("Management").Cells(j, i)
                    Next i
                    found = True
                End If
                CompareRow = CompareRow + 1
            Wend
            If CompareRow = 1000 Then CompareRow = HarvestDataRow
        End If
        j = j + 1
    Wend
    While Sheets("Harv").Cells(HarvestDataRow, 2) <> ""
        Sheets("Harv").Cells(HarvestDataRow, 1) = ExpID
        HarvestDataRow = HarvestDataRow + 1
    Wend
End Sub
Sub GenerateExtraInformation()
'Harvest
    Dim plantIndex As Integer
    NumberOfHarvest = 1
    StartRow = GraphIndex * 15 + 1
    Sheets("Rhizo").Cells(StartRow, 1) = "Harvest"
    While Sheets("Management").Cells(ManagementStart(3) + NumberOfHarvest, 1) <> "" And NumberOfHarvest < 3
        HarvestDate = Sheets("Management").Cells(ManagementStart(3) + NumberOfHarvest, 1)
        Dmsto = 0: DmTot = 0: Nsto = 0: Ntot = 0
        For plantIndex = 1 To 5
            SimCol = FindSimCol("PS", PlantSimStart, "DMStorage", plantIndex)
            If SimCol > 0 Then    ' Simulation Found

                HarvestRow = PlantSimStart + (HarvestDate - Sheets("PS").Cells(PlantSimStart + 1, 1))
                Dmsto = Dmsto + Sheets("PS").Cells(HarvestRow, SimCol)
                DmTot = DmTot + Sheets("PS").Cells(HarvestRow, FindSimCol("PS", PlantSimStart, "DMTotalTop", plantIndex))
                Nsto = Nsto + Sheets("PS").Cells(HarvestRow, FindSimCol("PS", PlantSimStart, "NStorage", plantIndex))
                Ntot = Ntot + Sheets("PS").Cells(HarvestRow, FindSimCol("PS", PlantSimStart, "NTop", plantIndex))
            End If
        Next
        StartRow = StartRow + 1
        With Sheets("Rhizo")
            .Cells(StartRow, 1) = "DM"
            .Cells(StartRow, 2) = "HI"
            .Cells(StartRow, 3) = "N"
            .Cells(StartRow, 4) = "NHI"
            StartRow = StartRow + 1

            .Cells(StartRow, 1) = Dmsto
        End With
        If DmTot > 0 Then Sheets("Rhizo").Cells(StartRow, 2) = Dmsto / DmTot
        Sheets("Rhizo").Cells(StartRow, 3) = Nsto
        If Ntot > 0 Then Sheets("Rhizo").Cells(StartRow, 4) = Nsto / Ntot
        NumberOfHarvest = NumberOfHarvest + 1
    Wend

    'Rhizo
    StartRow = StartRow + 1
    NumberOfHarvest = 1
    FirstCropHarvested = Sheets("Management").Cells(ManagementStart(3) + 1, 2)
    While Sheets("Management").Cells(ManagementStart(3) + NumberOfHarvest, 2) = Sheets("Management").Cells(ManagementStart(3) + 1, 2)
        NumberOfHarvest = NumberOfHarvest + 1
    Wend
    NumberOfHarvest = NumberOfHarvest - 1    ' only last harvest of crop considered
    HarvestDate = Sheets("Management").Cells(ManagementStart(3) + NumberOfHarvest, 1)
    If IsEmpty(HarvestDate) Then HarvestDate = Sheets("Management").Cells(2, 2)
    FirstPloughDate = Sheets("Management").Cells(ManagementStart(1) + 1, 1)
    If IsDate(FirstPloughDate) And HarvestDate > FirstPloughDate Then HarvestDate = FirstPloughDate
    With Sheets("Rhizo")
        .Activate
        .Cells(StartRow, 1) = "Rhizo"
        .Cells(StartRow, 2) = HarvestDate
        .Cells(StartRow, 3) = HarvestDate - 2 * 21
        .Cells(StartRow, 4) = HarvestDate - 3 * 21
        .Cells(StartRow, 5) = HarvestDate - 4 * 21
        .Cells(StartRow, 6) = HarvestDate - 5 * 21
    End With
    Range(Sheets("Rhizo").Cells(StartRow, 2), Sheets("Rhizo").Cells(StartRow, 6)).NumberFormat = "dd/mm"
    For CNType = 1 To 2
        StartRow = StartRow + 1
        TypeString = "C "
        If CNType = 2 Then TypeString = "N "

        With Sheets("Rhizo")
            .Cells(StartRow, 1) = TypeString + "Net Pro"
            .Cells(StartRow, 2) = TypeString + "Root"
            .Cells(StartRow, 3) = TypeString + "Rhizo"
            .Cells(StartRow, 4) = TypeString + "Topdep"
            .Cells(StartRow, 5) = TypeString + "TOP"
            .Cells(StartRow, 6) = TypeString + "Root"
            .Cells(StartRow, 7) = TypeString + "Rhizo"
            .Cells(StartRow, 8) = TypeString + "Topdep"
        End With
        For i = 1 To 5
            Ctop = 0: Croot = 0: CRhizo = 0: CTopDep = 0
            Ctop1 = 0: Croot1 = 0: CRhizo1 = 0: CTopDep1 = 0
            nf = 0: nf1 = 0
            For plantIndex = 1 To 5
                SimCol = FindSimCol("PS", PlantSimStart, "DMStorage", plantIndex)
                If SimCol > 0 Then    ' Simulation Found
                    If CNType = 1 Then
                        ColCTop = FindSimCol("PS", PlantSimStart, "TopProd", plantIndex)
                        ColCRoot = FindSimCol("PS", PlantSimStart, "RootTrans", plantIndex)
                        ColCRhizo = FindSimCol("PS", PlantSimStart, "RootDeposit", plantIndex)
                        ColCTopdep = FindSimCol("PS", PlantSimStart, "TopDeposit", plantIndex)
                    Else
                        ColCTop = FindSimCol("PS", PlantSimStart, "NUptake", plantIndex)
                        ColCRoot = FindSimCol("PS", PlantSimStart, "NRoot", plantIndex)
                        ColCRhizo = FindSimCol("PS", PlantSimStart, "NRootDeposit", plantIndex)
                        ColCTopdep = FindSimCol("PS", PlantSimStart, "NTopDeposit", plantIndex)
                        NfixCol = FindSimCol("PS", PlantSimStart, "Nfixation", plantIndex)
                    End If
                    convertFactor = 0.44
                    If CNType = 2 Then convertFactor = 1
                    HarvestRow = PlantSimStart + (HarvestDate - Sheets("PS").Cells(PlantSimStart + 1, 1))
                    If i = 1 Then  ' total production
                        Ctop = Ctop + convertFactor * Sheets("PS").Cells(HarvestRow, ColCTop)
                        Croot = Croot + convertFactor * Sheets("PS").Cells(HarvestRow, ColCRoot)
                        CRhizo = CRhizo + convertFactor * Sheets("PS").Cells(HarvestRow, ColCRhizo)
                        CTopDep = CTopDep + convertFactor * Sheets("PS").Cells(HarvestRow, ColCTopdep)
                        If CNType = 2 And (NfixCol > 0) Then
                            nf = nf + Sheets("PS").Cells(HarvestRow, NfixCol)
                        End If
                    Else
                        HarvestRow = HarvestRow - i * 21
                        Ctop = Ctop + convertFactor * Sheets("PS").Cells(HarvestRow + 3, ColCTop)
                        Croot = Croot + convertFactor * Sheets("PS").Cells(HarvestRow + 3, ColCRoot)
                        CRhizo = CRhizo + convertFactor * Sheets("PS").Cells(HarvestRow + 3, ColCRhizo)
                        CTopDep = CTopDep + convertFactor * Sheets("PS").Cells(HarvestRow + 3, ColCTopdep)

                        Ctop1 = Ctop1 + convertFactor * Sheets("PS").Cells(HarvestRow - 4, ColCTop)
                        Croot1 = Croot1 + convertFactor * Sheets("PS").Cells(HarvestRow - 4, ColCRoot)
                        CRhizo1 = CRhizo1 + convertFactor * Sheets("PS").Cells(HarvestRow - 4, ColCRhizo)
                        CTopDep1 = CTopDep1 + convertFactor * Sheets("PS").Cells(HarvestRow - 4, ColCTopdep)

                        If CNType = 2 And (NfixCol > 0) Then
                            nf = nf + Sheets("PS").Cells(HarvestRow + 3, NfixCol)
                            nf1 = nf1 + Sheets("PS").Cells(HarvestRow - 4, NfixCol)
                        End If
                    End If
                End If
            Next
            StartRow = StartRow + 1

            With Sheets("Rhizo")
                If i = 1 Then
                    .Cells(StartRow, 1) = Ctop
                    .Cells(StartRow, 2) = Croot
                    .Cells(StartRow, 3) = CRhizo
                    .Cells(StartRow, 4) = CTopDep
                    If CNType = 2 Then .Cells(StartRow, 9) = nf
                Else
                    .Cells(StartRow, 1) = (Ctop - Ctop1) / 7
                    .Cells(StartRow, 2) = (Croot - Croot1) / 7
                    .Cells(StartRow, 3) = (CRhizo - CRhizo1) / 7
                    .Cells(StartRow, 4) = (CTopDep - CTopDep1) / 7
                    If CNType = 2 Then .Cells(StartRow, 9) = (nf - nf1) / 7
                End If
                Total = .Cells(StartRow, 1)
                If Total > 0 Then
                    .Cells(StartRow, 5) = (.Cells(StartRow, 1) - Sheets("Rhizo").Cells(StartRow, 2) - Sheets("Rhizo").Cells(StartRow, 4)) / Total * 100
                    .Cells(StartRow, 6) = .Cells(StartRow, 2) / Total * 100
                    .Cells(StartRow, 7) = .Cells(StartRow, 3) / Total * 100
                    .Cells(StartRow, 8) = .Cells(StartRow, 4) / Total * 100
                End If
                .Activate
            End If
        Next
        Range(.Cells(StartRow - 4, 1), .Cells(StartRow, 4)).NumberFormat = "0.0"
        Range(.Cells(StartRow - 4, 5), .Cells(StartRow, 8)).NumberFormat = "0"
    End With
Next
StartRow = StartRow + 2
End Sub
Sub ReadSimulation()

    Dim outputfilename As String
    Dim outputsheetname As String
    Dim SimEnd

    outputsheetname = "soil0_0_0_0_0"
    outputfilename = "soil0_0_0_0_0.xls"

    ' soil simulation
    Workbooks.Open Filename:=Sheets("Main").Range("M4") & outputfilename

    SoilSimStart = SoilSimEnd + 1
    SimEnd = Sheets(outputsheetname).Range("A:A").Find("").Row
    'ColEnd = Sheets(outputsheetname).Range("A:A").Find("").Column

    SoilSimEnd = SoilSimStart + SimEnd - 2
    Range("A1", Cells(SimEnd, 122)).Copy
    ThisWorkbook.Activate

    Sheets("SS").Activate
    Sheets("SS").Range("A1").Select
    Cells(SoilSimStart, 1).Activate
    ActiveSheet.Paste
    Windows(outputfilename).Activate
    Range("A1").Select
    Application.CutCopyMode = False
    ActiveWindow.Close

    'plant simulation
    Workbooks.Open Filename:=Sheets("Main").Range("M4") & "field0.xls"
    PlantSimStart = PlantSimEnd + 1

    SimEnd = Sheets("field0").Range("A:A").Find("").Row
    PlantSimEnd = PlantSimStart + SimEnd - 2
    Range("A1", Cells(SimEnd, 200)).Copy
    ThisWorkbook.Activate
    Sheets("PS").Activate
    Sheets("PS").Range("A1").Select
    Cells(PlantSimStart, 1).Select
    ActiveSheet.Paste
    Windows("field0.xls").Activate
    Range("A1").Activate
    Application.CutCopyMode = False
    ActiveWindow.Close
    ThisWorkbook.Activate

End Sub
Sub FindBudget(BudgetNr, BudgetCell As Range)
    Dim i As Integer
    Dim c
    Set c = Columns("B:B").Find(BudgetNr, LookIn:=xlValues)
    Set c = Columns("B:B").FindNext(c)
    Set c = Columns("B:B").FindNext(c)

    i = 1
    BudgetCell = Mid(c.Text, 7)
    Do
        BudgetCell.Offset(0, i) = c.Offset(0, 1)
        Set c = Columns("B:B").FindNext(c)
        i = i + 1
    Loop While Not c Is Nothing And c.Offset(0, -1) > 0
End Sub

Sub GenerateBudget()
    Dim BudgetCell As Range
    Dim BudgetCount As Integer
    Dim i As Integer
    'Activate
    Set BudgetCell = Sheets("Budget").Range("C2")
    BudgetCell.Offset(BudgetRow - 1, 0) = "Crop"
    BudgetCell.Offset(BudgetRow, 0) = "Year"
    For i = 0 To 40
        BudgetCell.Offset(BudgetRow - 1, i + 2) = SowCrop(i)
        If Not SowCrop(i) = "" Then BudgetCell.Offset(BudgetRow, i + 2) = Year(Sheets("Management").Range("A2")) + i + 1
    Next i
    BudgetRow = BudgetRow + 1

    ChDir (Sheets("Main").Range("M4"))
    Workbooks.OpenText Filename:="INDICATX.XLS", Origin _
                                                 :=xlWindows, StartRow:=1, DataType:=xlDelimited, TextQualifier:= _
                       xlDoubleQuote, ConsecutiveDelimiter:=False, Tab:=True, Semicolon:=False, _
                       Comma:=False, Space:=False, Other:=False, FieldInfo:=Array(Array(1, 1), _
                                                                                  Array(2, 1), Array(3, 1), Array(4, 1))
    Columns("A:D").Select
    Selection.Sort Key1:=Range("B1"), Order1:=xlAscending, Header:=xlGuess, _
                   OrderCustom:=1, MatchCase:=False, Orientation:=xlTopToBottom

    Dim N_Input As Variant
    Dim N_output As Variant
    Dim N_Status As Variant
    N_Input = Array("31.01", "31.02", "31.03", "31.04", "31.05", "31.06", "31.07", "31.08")
    N_output = Array("31.20", "31.21", "31.22", "31.23", "31.40", "31.41", "31.42", "31.43", _
                     "31.44", "31.45", "31.46", "31.47", "31.48", "32.40", "32.41")
    N_Status = Array("31.60", "31.61", "32.10", "32.20", "32.21", "32.22")

    BudgetCell.Offset(BudgetRow - 2, -1) = ExpID
    BudgetCell.Offset(BudgetRow - 2, -1).Font.Bold = True
    BudgetCell.Offset(BudgetRow, -1) = "N-Input"
    For BudgetCount = 0 To 7
        Call FindBudget(N_Input(BudgetCount), BudgetCell.Offset(BudgetRow, 0))
        BudgetRow = BudgetRow + 1
        BudgetCell.Offset(BudgetRow - 1, -2) = ExpID
    Next

    BudgetCell.Offset(BudgetRow, -1) = "N-Output"
    For BudgetCount = 0 To 14
        Call FindBudget(N_output(BudgetCount), BudgetCell.Offset(BudgetRow, 0))
        BudgetRow = BudgetRow + 1
        BudgetCell.Offset(BudgetRow - 1, -2) = ExpID
    Next

    BudgetCell.Offset(BudgetRow, -1) = "N-Status"
    For BudgetCount = 0 To 5
        Call FindBudget(N_Status(BudgetCount), BudgetCell.Offset(BudgetRow, 0))
        BudgetRow = BudgetRow + 1
        BudgetCell.Offset(BudgetRow - 1, -2) = ExpID
    Next
    BudgetRow = BudgetRow + 2

    Dim C_Budget As Variant
    C_Budget = Array("40.01", "40.02", "41.10", "40.40", "40.23", "40.20", "40.21", _
                     "40.22", "40.60", "40.80", "40.81", "40.82")
    BudgetCell.Offset(BudgetRow, -1) = "C-Budget"
    For BudgetCount = 0 To 11
        Call FindBudget(C_Budget(BudgetCount), BudgetCell.Offset(BudgetRow, 0))
        BudgetRow = BudgetRow + 1
        BudgetCell.Offset(BudgetRow - 1, -2) = ExpID
    Next
    BudgetRow = BudgetRow + 2

    Dim Water_Budget As Variant
    Water_Budget = Array("70.01", "70.02", "70.40", "70.12", "70.42", "70.15")
    BudgetCell.Offset(BudgetRow, -1) = "Water_Budget"
    For BudgetCount = 0 To 5
        Call FindBudget(Water_Budget(BudgetCount), BudgetCell.Offset(BudgetRow, 0))
        BudgetRow = BudgetRow + 1
        BudgetCell.Offset(BudgetRow - 1, -2) = ExpID
    Next
    BudgetRow = BudgetRow + 2

    Windows("Indicatx.xls").Activate
    ActiveWindow.Close (False)
    ThisWorkbook.Activate
End Sub
Sub Optimize()
    ExtractDataCol = 1

    Application.ScreenUpdating = False
    Set Base = DBEngine.Workspaces(0).OpenDatabase(Sheets("Main").Cells(6, 13))

    ObsStart(11) = 0
    SoilSimStart = 1
    SoilSimEnd = 0
    PlantSimStart = 1
    PlantSimEnd = 0
    AIndex = 1
    GraphIndex = 0
    'Clear all relevant pages
    Sheets("Graph").Select
    Application.DisplayAlerts = False
    ActiveWindow.SelectedSheets.Delete
    Application.DisplayAlerts = True
    Sheets.Add
    Sheets(2).Name = "Graph"

    Sheets("PS").Cells.Clear
    Sheets("SS").Cells.Clear

    Sheets("Observations").Cells.Clear
    Sheets("Extract").Cells.Clear

    Application.ScreenUpdating = False
    Sheets("Main").Activate
    ExpID = Cells(AIndex + 1, 1)
    Call GetManagement
    Call ReadObservations
    OptIndex = 0
    For mobility = 0.7 To 0.95 Step 0.1
        For mobility1 = 0.4 To 0.9 Step 0.1
            Open "\fasset\CropParameters.dat" For Output As #1
            Print #1, "[Soil]"
            Print #1, "Mobility "; mobility
            Print #1, "Mobility1 "; mobility1
            Print #1, "[end]"
            Close #1

            Call ExecuteFasset
            Call ReadSimulation
            Call GenerateGraph
            OptIndex = OptIndex + 1
            Sheets("Opt").Cells(OptIndex, 7) = mobility
            Sheets("Opt").Cells(OptIndex, 8) = mobility1
            Sheets("Opt").Range("E5") = PlantSimStart + 1
            Sheets("Opt").Cells(OptIndex, 9) = Sheets("Opt").Range("E2")
        Next
    Next
    Open "\fasset\CropParameters.dat" For Output As #1

    Print #1, "[end]"
    Close #1

    Application.ScreenUpdating = True

    Base.Close
    Sheets("Graph").Activate
End Sub
Sub ChangeAxes()
    Sheets("Graph").Activate
    Axes.Show
    For Each AGraph In ActiveSheet.ChartObjects
        AGraph.Activate
        ActiveChart.Axes(xlCategory).Select
        With ActiveChart.Axes(xlCategory)

            .MinimumScale = Axes.xmin
            .MaximumScale = Axes.xmax
        End With
    Next
End Sub
Sub ExtractSeries()
    Dim StartVal As Integer
    Dim EndVal As Integer
    Dim SimIndex As Integer
    Dim ObsIndex As Integer

    Application.Calculation = xlCalculationManual

    ExtractDataBox.Show
    ThisWorkbook.Activate
    Sheets("Extract").Cells.Clear
    Sheets("Graph").Activate

    ' data
    SimIndex = ExtractDataBox.SimIndex
    ObsIndex = ExtractDataBox.ObsIndex

    Obsx = ActiveChart.SeriesCollection(ObsIndex).XValues
    Obsy = ActiveChart.SeriesCollection(ObsIndex).Values
    NumberObs = ActiveChart.SeriesCollection(ObsIndex).Points.Count
    'sim
    Simx = ActiveChart.SeriesCollection(SimIndex).XValues
    Simy = ActiveChart.SeriesCollection(SimIndex).Values
    NumberSim = ActiveChart.SeriesCollection(SimIndex).Points.Count

    Offset = 0    ' SimX(1)
    StartVal = DateValue(ExtractDataBox.xmin) - Simx(1)
    EndVal = DateValue(ExtractDataBox.xmax) - Simx(1)
    Factor = 1    ' 365


    For i = 1 To NumberObs
        Sheets("Extract").Cells(i, 1) = (Obsx(i) - Offset) * Factor
        Sheets("Extract").Cells(i, 2) = Obsy(i)
    Next

    For i = StartVal To EndVal
        Sheets("Extract").Cells(i - StartVal + 1, 3) = (Simx(i) - Offset) * Factor
        Sheets("Extract").Cells(i - StartVal + 1, 4) = Simy(i)
    Next i

    For CurrentObs = 1 To NumberObs
        Offset = Obsx(CurrentObs) - Simx(StartVal)
        SimValue = Simy(StartVal + Offset)
        'If SimValue = 0 Then SimValue = Simy(StartVal + Offset - 1)
        Sheets("Extract").Cells(CurrentObs, 5) = Obsy(CurrentObs)
        Sheets("Extract").Cells(CurrentObs, 6) = SimValue
    Next


    '
    '   If CurrentObs <= NumberObs Then
    '      If SimX(i) >= Obsx(CurrentObs) Then
    '          While SimX(i) >= Obsx(CurrentObs) And CurrentObs < NumberObs
    '              Interpol = 0
    '              If i > 1 Then Interpol = (SimX(i) - Obsx(CurrentObs)) / (SimX(i) - SimX(i - 1))
    '              If i > 1 Then
    '                  InterpolVal = Simy(i - 1) + (1 - Interpol) * (Simy(i) - Simy(i - 1))
    '              Else
    '                  InterpolVal = Simy(1)
    '              End If
    '              Sheets("Extract").Cells(CurrentObs, 5) = Obsy(CurrentObs)
    '              Sheets("Extract").Cells(CurrentObs, 6) = InterpolVal
    '              CurrentObs = CurrentObs + 1
    '          Wend
    '      End If
    '  End If
    'Next
    Application.Calculation = xlCalculationAutomatic
    Sheets("Extract").Activate
    Columns("A:A").Select
    Selection.NumberFormat = "d/m/yyyy"
    Columns("C:C").Select
    Selection.NumberFormat = "d/m/yyyy"
    Columns("A:D").Copy

End Sub
' Execution options
Sub ExecuteFasset()
    Dim RetVal As Boolean
    ChDir (Directory & Sheets("Main").Range("M2"))
    RetVal = ShellWait(Sheets("Main").Range("N2"), INFINITE, vbMinimizedNoFocus)
End Sub

Sub Makro1()
'
' Makro1 Makro
' Makro indspillet 04/08/2003 af Jørgen Berntsen
'

    ActiveWindow.Visible = False
    ThisWorkbook.Activate
    Sheets("Extract").Select
    Columns("A:A").Select
    Selection.NumberFormat = "d/m/yyyy"
End Sub
Sub test()
    MaxCminObs = 0
    SqlText = "SELECT StartDepth, EndDepth FROM [Measure Soil C] WHERE (ID='" & ExpID & "') GROUP BY StartDepth, EndDepth"
    For Ctype = 1 To 4
        Set SoilRecord = Base.OpenRecordset(SqlText, dbOpenDynaset)
        If Not SoilRecord.EOF Then SoilRecord.MoveFirst
        CurrentObsStart = ObsStart(Ctype + 13)
        If Not SoilRecord.EOF Then
            For SoilIndex = 0 To SoilRecord.RecordCount - 1
                StartDepth = SoilRecord.Fields(0).Value
                EndDepth = SoilRecord.Fields(1).Value
                Cells(CurrentObsStart, SoilIndex * 2 + 1) = "Date"
                If (Ctype = 1) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "Org_C%-" & StartDepth * 10 & "-" & EndDepth * 10
                If (Ctype = 2) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "Org_C-" & StartDepth * 10 & "-" & EndDepth * 10
                If (Ctype = 3) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "SMB" & 150
                If (Ctype = 4) Then Cells(CurrentObsStart, SoilIndex * 2 + 2) = "C14-" & StartDepth * 10 & "-" & EndDepth * 10

                SqlText1 = "SELECT Date, [Organic_C-Pct], [Organic_C-Content], [Microbial_Biomass],[C14] FROM [Measure Soil C] WHERE (ID='" & ExpID & "' AND StartDepth=" & StartDepth & " AND EndDepth=" & EndDepth & ")"
                Set Record = Base.OpenRecordset(SqlText1, dbOpenDynaset)
                Record.MoveLast
                MaxRecords = Record.RecordCount
                If MaxRecords > MaxCminObs Then MaxCminObs = MaxRecords
                Record.MoveFirst
                For Index = 0 To MaxRecords - 1
                    Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 1) = Record.Fields(0).Value    ' date
                    OrgCpct = 0
                    OrgC = 0
                    SMB = 0
                    C14 = 0
                    If Record.Fields(1) > 0 Then OrgCpct = Record.Fields(1)
                    If Record.Fields(2) > 0 Then OrgC = Record.Fields(2)
                    If Record.Fields(3) > 0 Then SMB = Record.Fields(3)
                    If Record.Fields(4) > 0 Then C14 = Record.Fields(4)

                    If (Ctype = 1 And OrgCpct > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * OrgCpct * 1.5    'Tjek lige disse udregninger
                    If (Ctype = 2 And OrgC > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * OrgC * 1.5
                    If (Ctype = 3 And SMB > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * SMB * 1.5
                    If (Ctype = 4 And C14 > 0) Then Cells(CurrentObsStart + 1 + Index, SoilIndex * 2 + 2) = (EndDepth - StartDepth) / 100 * C14 * 1.5
                    Record.MoveNext
                Next
                SoilRecord.MoveNext
            Next
        Else    ' empty then use defaults
            EmptyType = "OrgCpct-"
            If (Ctype = 2) Then EmptyType = "OrgC-"
            If (Ctype = 3) Then EmptyType = "SMB-"
            If (Ctype = 4) Then EmptyType = "C14-"
            Cells(CurrentObsStart, 1) = "Date"
            Cells(CurrentObsStart, 2) = EmptyType + "0-20"
            Cells(CurrentObsStart, 3) = "Date"
            Cells(CurrentObsStart, 4) = EmptyType + "0-1000"
        End If
        ObsStart(Ctype + 14) = MaxCminObs + ObsStart(Ctype + 13) + 2
    Next
End Sub

Function ShellWait(CommandLine As String, TimeOut As Long, WindowState As VbAppWinStyle) As Boolean

    Dim ProcessID As Long
    Dim hProcess As Long

    ProcessID = Shell(CommandLine, WindowState)
    If ProcessID <> 0 Then
        ' non-zero (True) so Shell worked
        ' Get a process handle for the PID (Wait takes a handle)
        hProcess = OpenProcess(SYNCHRONIZE, False, ProcessID)
        If hProcess <> 0 Then
            ' Got process handle
            ' Wait until process finishes before going on
            If WaitForSingleObject(hProcess, TimeOut) = WAIT_OBJECT_0 Then
                ShellWait = True
            Else
                ShellWait = False
            End If
        Else
            'Failed to get process handle. Perhaps the process terminated very quickly
            'or it might not really have executed at all even though Windows
            ' started a process.
            ShellWait = False
        End If
    Else
        ' PID zero (False) so Shell failed
        ShellWait = False
    End If
End Function



