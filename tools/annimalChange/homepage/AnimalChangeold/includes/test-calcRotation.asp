<%option explicit
%>
<!-- #include file="RotationModel2.asp" -->
<!-- #include file="General.asp" -->
<!-- #include file="adovbs.asp" -->
<!-- #include file="dbInterface.asp" -->
<%

   Dim nCrop, arrCrop, CropString, NumberCrops, test
   Dim nYield, arrYield
   Dim ObjectCoeff   ' matrix of objective function coefficients NUMBERCROPS * NUMBERCROPS
                     ' -10000 indicates that it is an impossible combination of precrop and crop
   Dim CropArea, i, j, k  ' array of crop areas NUMBERCROPS
   Dim arrCropAndArea, arrPreCrop
   Dim arrRotationList
   Dim aFarmNumber, aScenarioID, aSoilTypeID, aRotationID
	aFarmNumber = Session("farmNumber")
	aScenarioID = Session("scenarioNumber")
aSoilTypeID   = Session("soilType")
aRotationID   = 1
'aRotationID   = Request.querystring("RotationID")
' if Request.Form("SoilType_"&aRotationID&"")<>Session("soilType") then 
'   aSoilTypeID  = Request.Form("SoilType_"&aRotationID&"")
' end if
'	aRotationID = Request.QueryString("RotationID")
   
   InitFarmNDB
NumberCrops = GetFieldPlanCropAndAreaList(aFarmNumber,aScenarioID,aRotationID,arrCropAndArea)

	redim arrCrop(NumberCrops)
	for i=0 to NumberCrops-1
	  arrCrop(i)=arrCropAndArea(0,i)
	next
	CropString=mid(join(arrCrop,","),1,len(join(arrCrop,","))-1)
	nYield = GetCropDefaultYieldList(aSoilTypeID,CropString,2,arrYield)
'if nYield<>0 then	
	Redim ObjectCoeff(NumberCrops-1,NumberCrops-1)
	Redim CropArea(NumberCrops-1)
	Redim arrPreCrop(1,NumberCrops-1)
	Redim arrRotationList(NumberCrops-1,NumberCrops-1)
	
	   InitArray ObjectCoeff,-10000
	   InitArray arrRotationList,-1
	  for i=0 to NumberCrops-1
	     CropArea(i)=arrCropAndArea(1,i)
	  next
	   
	   k=0
	   for i=0 to NumberCrops-1
	     for j=0 to NumberCrops-1
		   if k<=Ubound(arrYield,2) then
			   if arrYield(0,k)=arrCropAndArea(0,i) and arrYield(1,k)=arrCropAndArea(0,j) then
		       ObjectCoeff(j,i)=arrYield(2,k)
			   arrPreCrop(0,j)=arrYield(3,k)
			   arrPreCrop(1,j)=arrYield(1,k)
	           else
			   arrPreCrop(0,j)=arrCropAndArea(0,i)
			   arrPreCrop(1,j)=arrCropAndArea(0,j)
			   end if
            end if
'		   response.write "arrPreCrop(1,"&j&") :"&arrPreCrop(1,j)&"<br>"
		   k=k+1
		 next
	   next
	
	   test=calcRotation(CropArea,CropArea,ObjectCoeff,arrRotationList)
   
   Response.write "Test = "&test &"<br>"
   
   PrintArrayInTable(arrRotationList)
   for i=0 to NumberCrops-1
    for j=0 to NumberCrops-1
     if arrRotationList(i,j)<>0 then response.write "Crop:"&arrCrop(i)&"PreCrop:"&arrPreCrop(j)&"area:"& arrRotationList(i,j)&"<br>"
'     response.write "Crop:"&arrCrop(i)&"PreCrop:"&arrPreCrop(j)&"area:"& arrRotationList(i,j)&"<br>"
	next
   next

%>