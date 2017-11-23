<%
'Grazing
Private Const PARAMETER_FE_GRAZING      = 301
'Feeding requirement
Private Const PARAMETER_FE              = 302 'FE for cattle in the table: "Parameter"
Private Const PARAMETER_TS              = 303 'ts for cattle in the table: "Parameter"
Private Const PARAMETER_N               = 304 'N for cattle in the table: "Parameter"
Private Const PARAMETER_P               = 305 'P for cattle in the table: "Parameter"
'Production
Private Const PARAMETER_MILK_N          = 306
Private Const PARAMETER_GROWTH_N        = 307
'Manure
Private Const PARAMETER_MANURE_N        = 308
Private Const PARAMETER_MANURE_N_FIELD  = 309
Private Const PARAMETER_MANURE_N_FAECES = 310
Private Const PARAMETER_MANURE_N_URINE  = 311
Private Const PARAMETER_BEDDING_N       = 312
dim time1, time2,time3,time4,time5,time6,time7,time8,time9,time10,time11,time12,time13,time14,time15,time16,time17

'**************************************************************************************************
sub BeginHeader(title, keywords, description)
   Response.Write("<!DOCTYPE html PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"">")
   Response.Write("<html xmlns=""http://www.w3.org/1999/xhtml"">" & vbCr)
   Response.Write("<head>")
   Response.Write("<title>"&title&"</title>")
   Response.Write("<meta http-equiv=""Content-Type"" content=""text/html;charset=iso-8859-1""></meta>" & vbCr)
   if not keywords = "" then
      Response.Write("<meta http-equiv=""KEYWORDS"" content="""&keywords&"""></meta>" & vbCr)
   end if
   if not description = "" then
      Response.Write("<meta http-equiv=""DESCRIPTION"" content="""&description&"""></meta>" & vbCr)
   end if
end sub
'**************************************************************************************************
sub EndHeader()
   Response.Write("</head>" & vbCr)
end sub
'**************************************************************************************************
sub BeginBody()
   Response.Write("<body>" & vbCr)
end sub
'**************************************************************************************************
sub BeginBodyData()

'   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onUnload=""if (document.forms.update.saveUpdates.value=='true'){document.forms.update.submit();}else{window.top.location.reload();}"">" & vbCr)
'   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onUnload=""if (document.forms.update.saveUpdates.value=='true'){}else{window.top.location.reload();}"">" & vbCr)
   Response.Write("<style>" & vbCr) 
   Response.Write(".animation" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("position:relative;" & vbCr)
   Response.Write("top: 25%;" & vbCr)
   Response.Write("left: 20px;" & vbCr)
   Response.Write("animation:mymove 10s infinite;" & vbCr)
   Response.Write("animation-timing-function:linear;" & vbCr)

   Response.Write("-webkit-animation:mymove 5s infinite;" & vbCr)
   Response.Write("-webkit-animation-timing-function:linear;" & vbCr)
   Response.Write("}" & vbCr)

   Response.Write("@keyframes mymove" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("from {left:20px;}" & vbCr)
   Response.Write("to {left:500px;}" & vbCr)
   Response.Write("}" & vbCr)
   Response.Write("@-webkit-keyframes mymove" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("from {left:20px;}" & vbCr)
   Response.Write("to {left:500px;}" & vbCr)
   Response.Write("}" & vbCr)
   Response.Write("</style>" & vbCr)
   EndHeader
   Response.Write("<script type=""text/javascript""> var FarmNPopupWin = ''</script>" & vbCr)
   Response.Write("<script type=""text/javascript"" src=""scripts/formvalidation.js""></script>" & vbCr)
   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"">" & vbCr)
   Response.Write("<span id=""veil"" style=""position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;""><blink class=""animation"">Please wait...</blink></span>" & vbCr)
   Response.Write("<script type=""text/javascript"">function veil() {document.getElementById('veil').style.display='block';if(document.getElementById('dynamichide')) {document.getElementById('dynamichide').style.display='none';}}</script>" & vbCr)
end sub
'**************************************************************************************************
sub BeginBodyDataKeydown(keydownString)

'   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onUnload=""if (document.forms.update.saveUpdates.value=='true'){document.forms.update.submit();}else{window.top.location.reload();}"">" & vbCr)
'   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onUnload=""if (document.forms.update.saveUpdates.value=='true'){}else{window.top.location.reload();}"">" & vbCr)
   Response.Write("<style>" & vbCr) 
   Response.Write(".animation" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("position:relative;" & vbCr)
   Response.Write("top: 25%;" & vbCr)
   Response.Write("left: 20px;" & vbCr)
   Response.Write("animation:mymove 10s infinite;" & vbCr)
   Response.Write("animation-timing-function:linear;" & vbCr)

   Response.Write("-webkit-animation:mymove 5s infinite;" & vbCr)
   Response.Write("-webkit-animation-timing-function:linear;" & vbCr)
   Response.Write("}" & vbCr)

   Response.Write("@keyframes mymove" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("from {left:20px;}" & vbCr)
   Response.Write("to {left:500px;}" & vbCr)
   Response.Write("}" & vbCr)
   Response.Write("@-webkit-keyframes mymove" & vbCr)
   Response.Write("{" & vbCr)
   Response.Write("from {left:20px;}" & vbCr)
   Response.Write("to {left:500px;}" & vbCr)
   Response.Write("}" & vbCr)
   Response.Write("</style>" & vbCr)
   Response.Write("<script type=""text/javascript"">" & vbCr)
   Response.Write(" <!-- //hide" & vbCr)
'   Response.Write("    window.onload = MyOnLoad;" & vbCr)  
'   Response.Write("    function MyOnLoad(){" & vbCr)
'   Response.Write("        var txtBox = document.getElementById('txtTest');" & vbCr)					  
'   Response.Write("        txtBox.onkeydown=DisplayKeyCode;" & vbCr)
'   Response.Write("    }" & vbCr)  
   Response.Write("    function DisplayKeyCode(event){" & vbCr)        
   Response.Write("     //var ev=new document.event(event);" & vbCr)
   Response.Write("     alert('Key code: ' + event.keyCode);" & vbCr)
   Response.Write("    }" & vbCr)
   Response.Write("  //hide -->" & vbCr)
   Response.Write("</script>" & vbCr)   
   Response.Write("<script type=""text/javascript""> var FarmNPopupWin = ''</script>" & vbCr)
   Response.Write("<script type=""text/javascript"" src=""scripts/formvalidation.js""></script>" & vbCr)
   EndHeader
   Response.Write("<body class=""data"" id=""txtTest"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onkeydown="""&keydownString&""" onkeypress="""&keydownString&""">" & vbCr)
   Response.Write("<span id=""veil"" style=""position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;""><blink class=""animation"">Please wait...</blink></span>" & vbCr)
   Response.Write("<script type=""text/javascript"">function veil() {document.getElementById('veil').style.display='block';if(document.getElementById('dynamichide')) {document.getElementById('dynamichide').style.display='none';}}</script>" & vbCr)
end sub
'**************************************************************************************************
sub EndBody()
'   Response.Write("<span style=""color:#C8E1F3;"">"&session("userType")&"</span>" & vbCr)
'   Response.Write("<span style=""color:#C8E1F3;"">"&session.timeout&"</span>" & vbCr)
dim t
   Response.Write("</body>" & vbCr)
   Response.Write("</html>" & vbCr)
end sub
'**************************************************************************************************
sub CreateStyleLink(strFileSpec)
   Response.Write("<link rel=""StyleSheet"" href="""&strFileSpec&""" type=""text/css""></link>" & vbCr)
end sub
'**************************************************************************************************
sub CreateSelectList(aType,nItem,arrItem)
   dim i

   for i = 0 to nItem-1
      if arrItem(0,i) = aType then
         Response.Write("<option value="""&arrItem(0,i)&""" selected>"&arrItem(1,i)&"</option>")
      else  
         Response.Write("<option value="""&arrItem(0,i)&""">"&arrItem(1,i)&"</option>")
      end if 
   next   
end sub
'**************************************************************************************************
sub CreateSelectListLanguage(aType,nItem,arrItem)
   dim i

   for i = 0 to nItem-1
      if arrItem(0,i) = aType then
         Response.Write("<option value="""&arrItem(0,i)&""" selected>"&Str.Item("str"&arrItem(1,i))&"</option>")
      else  
         Response.Write("<option value="""&arrItem(0,i)&""">"&Str.Item("str"&arrItem(1,i))&"</option>")
      end if 
   next   
end sub
'**************************************************************************************************
sub CreateSelectList_(aType,nItem,arrItem)
   dim i,isValidSelected
   isValidSelected=false
'   for i = 0 to nItem-1
'      if arrItem(i,0) = aType then
'         isValidSelected=true
'      end if 
'   next
'   for i = 0 to 0
'   if isValidSelected=true then
'      if arrItem(i,0) = aType then
'         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&arrItem(i,1)&"</option>")
'      end if
'   else
'         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&arrItem(i,1)&"</option>")
'   end if
'   next   

   for i = 0 to nItem-1
      if arrItem(i,0) = aType then
         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&arrItem(i,1)&"</option>")
      else  
         Response.Write("<option value="""&arrItem(i,0)&""">"&arrItem(i,1)&"</option>")
      end if 
   next   
end sub
'**************************************************************************************************
sub CreateSelectList_Language(aType,nItem,arrItem)
   dim i

   for i = 0 to nItem-1
      if cstr(arrItem(i,0)) = cstr(aType) then
         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&Str.Item("str"&arrItem(i,1))&"</option>")
      else  
         Response.Write("<option value="""&arrItem(i,0)&""">"&Str.Item("str"&arrItem(i,1))&"</option>")
      end if 
   next   
end sub
'**************************************************************************************************
sub CreateSelectList_SoilType(aType,nItem,arrItem)
   dim i

   for i = 0 to nItem-1
      if arrItem(i,0) = aType then
         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&Str.Item("str"&arrItem(i,1))&"  "&arrItem(i,2)&"</option>")
      else  
         Response.Write("<option value="""&arrItem(i,0)&""">"&Str.Item("str"&arrItem(i,1))&"  "&arrItem(i,2)&"</option>")
      end if 
   next   
end sub
'**************************************************************************************************
%>