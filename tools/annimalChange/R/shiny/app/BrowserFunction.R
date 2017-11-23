source("CToolResults.R")
source("CToolResultsN.R")
source("PlantResults1.R")
source("PlantResults2.R")
source("PlantResults3.R")
source("SeqResults1.R")
source("SeqResults2.R")
source("NdataResults1.R")
source("NdataResults2.R")
source("NdataResults3.R")
source("NdataResults4.R")
source("NdataResults5.R")

BrowserFunction <- function(fileroot) {
  require(shiny)
  shinyApp(
    ui <- fluidPage(
      
      headerPanel('FarmAC field results'),
      sidebarPanel(
        #    h2('Select sequence'),
        uiOutput('numInputui'),
        #),
        #headerPanel('Select Graph'),
        h4('Select Graph(s)'),
        checkboxInput('Ctool', 'Soil C pools'),
        checkboxInput('CtoolN', 'Change in soil N pools'),
        checkboxInput('plant1', 'Crop C fixation'),
        checkboxInput('plant2', 'Crop C outputs'),
        checkboxInput('plant3', 'Crop dry matter budget'),
        checkboxInput('seq1', 'Crop sequence C inputs'),
        checkboxInput('seq2', 'Crop sequence C outputs'),
        checkboxInput('Ndata1', 'Crop N inputs'),
        checkboxInput('Ndata2', 'Crop N outputs'),
        checkboxInput('Ndata3', 'Crop sequence N inputs'),
        checkboxInput('Ndata4', 'Crop sequence N outputs'),
        checkboxInput('Ndata5', 'Crop sequence N losses'),
        tags$head(
          tags$style(type="text/css", "select { max-width: 140px; }"),
          tags$style(type="text/css", ".span4 { max-width: 190px; }"),
          tags$style(type="text/css", ".well { max-width: 180px; }")
        )
      ),
      
      mainPanel(
        uiOutput('ctoolui'),
        uiOutput('ctoolNui'),
        uiOutput('plantui1'),
        uiOutput('plantui2'),
        uiOutput('plantui3'),
        uiOutput('sequi1'),
        uiOutput('sequi2'),
        uiOutput('Ndataui1'),
        uiOutput('Ndataui2'),
        uiOutput('Ndataui3'),
        uiOutput('Ndataui4'),
        uiOutput('Ndataui5')
      )
    )
    , 
    server <- function(input, output) {
      
      output$ctool <- renderPlot({
        if (is.null(input$Ctool)) {
          return()
        }
        if (input$Ctool %% 2 == 1)
          CToolResults(fileroot,input$obs)   
      })
      
      output$ctoolN <- renderPlot({
        if (is.null(input$CtoolN)) {
          return()
        }
        if (input$CtoolN %% 2 == 1)
          CToolResultsN(fileroot,input$obs)   
      })
      
      
      ####### Read crop file ##########
      
      output$plant1 <- renderPlot({
        if (is.null(input$plant1)) {
          return()
        }
        if (input$plant1 %% 2 == 1)
        {
          PlantResults1(fileroot,input$obs)  
        }
      })
      output$plant2 <- renderPlot({
        if (is.null(input$plant2)) {
          return()
        }
        if (input$plant2 %% 2 == 1)
        {
          PlantResults2(fileroot,input$obs)  
        }
      })
      output$plant3 <- renderPlot({
        if (is.null(input$plant3)) {
          return()
        }
        if (input$plant3 %% 2 == 1)
        {
          PlantResults3(fileroot,input$obs)  
        }
      })
      
      ####### Sequence C data ######
      output$seq1 <- renderPlot({
        if (is.null(input$seq1)) {
          return()
        }
        if (input$seq1 %% 2 == 1)
        {
          SeqResults1(fileroot,input$obs)  
        }
      })
      
      output$seq2 <- renderPlot({
        if (is.null(input$seq2)) {
          return()
        }
        if (input$seq2 %% 2 == 1)
        {
          SeqResults2(fileroot,input$obs)  
        }
      })
      
      ######### N data ###############
      output$Ndata1 <- renderPlot({
        if (is.null(input$Ndata1)) {
          return()
        }
        if (input$Ndata1 %% 2 == 1)
        {
          NdataResults1(fileroot,input$obs) 
        }
      })
      output$Ndata2 <- renderPlot({
        if (is.null(input$Ndata2)) {
          return()
        }
        if (input$Ndata2 %% 2 == 1)
        {
          NdataResults2(fileroot,input$obs) 
        }
      })
      output$Ndata3 <- renderPlot({
        if (is.null(input$Ndata3)) {
          return()
        }
        if (input$Ndata3 %% 2 == 1)
        {
          NdataResults3(fileroot,input$obs) 
        }
      })
      output$Ndata4 <- renderPlot({
        if (is.null(input$Ndata4)) {
          return()
        }
        if (input$Ndata4 %% 2 == 1)
        {
          NdataResults4(fileroot,input$obs) 
        }
      })
      output$Ndata5 <- renderPlot({
        if (is.null(input$Ndata5)) {
          return()
        }
        if (input$Ndata5 %% 2 == 1)
        {
          NdataResults5(fileroot,input$obs) 
        }
      })
      
      
      output$numInputui <- renderUI({
        #fileroot = "outputFarm117050BaselineScenarioNr5"
        stub = "CtoolFile.xls"
        filename = paste(fileroot,stub, sep = '')
        CTool <-
          read.delim(filename, header = T, stringsAsFactors = FALSE)
        CTool1 <- CTool[-c(1),] ##remove second header line
        CTool2 <- mutate(CTool1,year = as.numeric(startDay) / 365)
        Sequences <- select(CTool2,CropSeqID)
        Sequences <- as.numeric(as.character(Sequences$CropSeq))
        NumSeq <- max(Sequences)
        
        numericInput("obs", "Select sequence:", 2,
                     min = 1, max = NumSeq,width='120px')
        
      })
      
      output$ctoolui <- renderUI({
        
        
        if(input$Ctool %% 2 == 1)
        {
          plotOutput("ctool")
        }
        
      })
      output$ctoolNui <- renderUI({
        
        
        if(input$CtoolN %% 2 == 1)
        {
          plotOutput("ctoolN")
        }
        
      })
      output$plantui1 <- renderUI({
        if(input$plant1 %% 2 == 1)
        {
          plotOutput("plant1")
        }
        
      })
      output$plantui2 <- renderUI({
        if(input$plant2 %% 2 == 1)
        {
          plotOutput("plant2")
        }
        
      })
      output$plantui3 <- renderUI({
        if(input$plant3 %% 2 == 1)
        {
          plotOutput("plant3")
        }
        
      })
      
      output$sequi1 <- renderUI({
        if(input$seq1 %% 2 == 1)
        {
          plotOutput("seq1")
        }
        
      })
      output$sequi2 <- renderUI({
        if(input$seq2 %% 2 == 1)
        {
          plotOutput("seq2")
        }
        
      })
      output$Ndataui1 <- renderUI({
        if(input$Ndata1 %% 2 == 1)
        {
          plotOutput("Ndata1")
        }
        
      })
      output$Ndataui2 <- renderUI({
        if(input$Ndata2 %% 2 == 1)
        {
          plotOutput("Ndata2")
        }
        
      })
      output$Ndataui3 <- renderUI({
        if(input$Ndata3 %% 2 == 1)
        {
          plotOutput("Ndata3")
        }
        
      })
      output$Ndataui4 <- renderUI({
        if(input$Ndata4 %% 2 == 1)
        {
          plotOutput("Ndata4")
        }
        
      })
      output$Ndataui5 <- renderUI({
        if(input$Ndata5 %% 2 == 1)
        {
          plotOutput("Ndata5")
        }
        
      })
      
      
    }  )
}
