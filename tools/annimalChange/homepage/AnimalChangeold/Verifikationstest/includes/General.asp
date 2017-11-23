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


'**************************************************************************************************
sub BeginHeader(title, keywords, description)
   Response.Write("<?xml version=""1.0"" encoding=""iso-8859-1""?>")
   Response.Write("<!DOCTYPE html PUBLIC ""-//W3C//DTD XHTML 1.0 Strict//EN"" ""DTD/xhtml1-strict.dtd"">")
   Response.Write("<html xmlns=""http://www.w3.org/1999/xhtml"" xml:lang=""en"">" & vbCr)
   Response.Write("<head>")
   Response.Write("<title>"&title&"</title>")
   Response.Write("<meta http-equiv=""Content-Type"" content=""text/html"">" & vbCr)
   if not keywords = "" then
      Response.Write("<meta http-equiv=""KEYWORDS"" content="""&keywords&""">" & vbCr)
   end if
   if not description = "" then
      Response.Write("<meta http-equiv=""DESCRIPTION"" content="""&description&""">" & vbCr)
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
   Response.Write("<script language=""JavaScript""> var FarmNPopupWin = ''</script>" & vbCr)

   Response.Write("<body class=""data"" onFocus=""if(FarmNPopupWin.open){FarmNPopupWin.close()};"" onUnload=""if (document.forms.update.saveUpdates.value=='true'){document.forms.update.submit();}else{window.top.location.reload();}"">" & vbCr)
   Response.Write("<script language=""JavaScript"" src=""scripts/formvalidation.js""></script>" & vbCr)
end sub
'**************************************************************************************************
sub EndBody()
'   Response.Write("<span style=""color:#C8E1F3;"">"&session("userType")&"</span>" & vbCr)
'   Response.Write("<span style=""color:#C8E1F3;"">"&session.timeout&"</span>" & vbCr)
   Response.Write("</body>" & vbCr)
   Response.Write("</html>" & vbCr)
end sub
'**************************************************************************************************
sub CreateStyleLink(strFileSpec)
   Response.Write("<link rel=""StyleSheet"" href="""&strFileSpec&""" type=""text/css"">" & vbCr)
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
   dim i

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