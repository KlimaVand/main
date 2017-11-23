<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<%
Private Const TITLE = "Tab"

   InitFarmNDB
   CreateLanguageDictionary
   CloseFarmNDB


BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
   dim TabView, TabItem
   
   ' Create the Component
   set TabView = Server.CreateObject("VASPTB.ASPTabView")

   TabView.LicenseKey       = "4E75-C7AB-A797" ' enter a valid license key to license the component.
   TabView.Class            = "TabView"  ' class name in visualasp.css file
   TabView.ImagePath        = "images\blue" ' image path where the images are located
   TabView.Orientation      = 0          ' TopLeft=0; TopRight=1; BottomLeft=2; BottomRight=3; 
   TabView.NoTabLine        = True       ' when = true removes the trailing line on the tabs.
   TabView.UpLevelClientTab = False      ' Set this property to True which will run the tab navigaiton on the client browser.
   TabView.StartTab         = Session("tabNumber")
'   Response.Write("<h1>TabView.StartTab: "&TabView.StartTab&"</h1>" & vbCr)
   
   TabView.BackColor         = "#DEEBF7"
   TabView.SelectedBackColor = "#94BEE7"
   TabView.SelectedForeColor = "#ffffff"
   TabView.SelectedBold      = True
   TabView.BodyBackground    = "#ffffff"

   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 1
   TabItem.Text = str.Item("strFARM")
   TabItem.Image = ""
   TabItem.SelectedImage = "" 
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = "Bedriftsdata"
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 2
   TabItem.Text = str.Item("strFIELD")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 3
   TabItem.Text = str.Item("strROTATION")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 4
   TabItem.Text = str.Item("strCATTLE")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 5
   TabItem.Text = str.Item("strPIG")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 6
   TabItem.Text = str.Item("strMANURE")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""

   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 7
   TabItem.Text = str.Item("strBALANCE")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 8
   TabItem.Text = str.Item("strRESULT")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   set TabItem = TabView.Tabs.Add()
   TabItem.Key = 9
   TabItem.Text = str.Item("strDOCUMENTATION")
   TabItem.Image = ""
   TabItem.SelectedImage = ""
   TabItem.URL = "data.asp"
   TabItem.Target = "frameData"
   TabItem.DHTML = "onclick=javascript:document.location='?TB=#TBK#'"
   TabItem.ToolTipText = ""
   
   ' Display the TabView
   TabView.Show

   ' Destroy the Component
   TabView.Unload
   set TabView = nothing
end sub
'**************************************************************************************************
%>