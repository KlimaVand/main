source('readData.R')
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
source("WaterResults1.R")
source("WaterResults2.R")
source("WaterResults3.R")
source("WaterResults4.R")
source("WaterResults5.R")

 #the actual filename is constructed by concatenating a stub to this root

server <- function(input, output,session) {
    getFileUrl <- reactive({
      query <- parseQueryString(session$clientData$url_search)
      file.prefix <-query$file
      if(is.null(file.prefix)){
        urlBase <- '..\\'
        fileroot <- "outputFarm116540ScenarioNr12" #outputFarm117050BaselineScenarioNr5"
      }else{
        urlBase <- 'http://gefionau.dk/animalchange/upload/AnimalChange/ModelOutput/'
        fileroot <-  file.prefix
      }
      
      file.name <- paste0(urlBase,fileroot)
      return(file.name)
    })
    ctooldata <- reactive({
      file.name <- getFileUrl() 
      filename <- paste(file.name,'CtoolFile.xls', sep = '')
      ctooldata <- readData(filename)
    })
    
    plantdata <- reactive({
      file.name <- getFileUrl() 
      filename <- paste(file.name,'plantfile.xls', sep = '')
      plantdata <- readData(filename)
    })
    
    waterdata <- reactive({
      file.name <- getFileUrl() 
      filename <- paste(file.name,'Debug.xls', sep = '')
      waterdata <- readData(filename)
    })
    
    output$ctool <- renderPlot({
      if (is.null(input$Ctool)) {
        return()
      }
      if (input$Ctool %% 2 == 1){
        c.data <- ctooldata()
        CToolResults(c.data,input$obs)  } 
    })
    
    output$ctoolN <- renderPlot({
      if (is.null(input$CtoolN)) {
        return()
      }
      if (input$CtoolN %% 2 == 1){
        c.data <- ctooldata()
        CToolResultsN(c.data,input$obs)  } 
    })
    
    output$Water <- renderPlot({
      if (is.null(input$Water)) {
        return()
      }
      if (input$Water %% 2 == 1){
        w.data <- waterdata()
        WaterResults(w.data,input$obs)  } 
    })
    
    ####### Read crop file ##########
    
    output$plant1 <- renderPlot({
        if (is.null(input$plant1)) {
            return()
        }
        if (input$plant1 %% 2 == 1)
        {
          plant.data <- plantdata()
            PlantResults1(plant.data,input$obs)  
        }
    })
    output$plant2 <- renderPlot({
        if (is.null(input$plant2)) {
            return()
        }
        if (input$plant2 %% 2 == 1)
        {
          plant.data <- plantdata() 
           PlantResults2(plant.data,input$obs)  
        }
    })
    output$plant3 <- renderPlot({
        if (is.null(input$plant3)) {
            return()
        }
        if (input$plant3 %% 2 == 1)
        {
          plant.data <- plantdata()
          PlantResults3(plant.data,input$obs)  
        }
    })
    
    ####### Sequence C data ######
    output$seq1 <- renderPlot({
      if (is.null(input$seq1)) {
        return()
      }
      if (input$seq1 %% 2 == 1)
      {
        plant.data <- plantdata()
        SeqResults1(plant.data,input$obs)  
      }
    })
    
    output$seq2 <- renderPlot({
      if (is.null(input$seq2)) {
        return()
      }
      if (input$seq2 %% 2 == 1)
      {
        plant.data <- plantdata()
        SeqResults2(plant.data,input$obs)  
      }
    })
    
    ######### N data ###############
    output$Ndata1 <- renderPlot({
        if (is.null(input$Ndata1)) {
            return()
        }
        if (input$Ndata1 %% 2 == 1)
        {
           plant.data <- plantdata()
           NdataResults1(plant.data,input$obs) 
        }
    })
    output$Ndata2 <- renderPlot({
      if (is.null(input$Ndata2)) {
        return()
      }
      if (input$Ndata2 %% 2 == 1)
      {
        plant.data <- plantdata()
        NdataResults2(plant.data,input$obs) 
      }
    })
    output$Ndata3 <- renderPlot({
      if (is.null(input$Ndata3)) {
        return()
      }
      if (input$Ndata3 %% 2 == 1)
      {
        plant.data <- plantdata()
        NdataResults3(plant.data,input$obs) 
      }
    })
    output$Ndata4 <- renderPlot({
      if (is.null(input$Ndata4)) {
        return()
      }
      if (input$Ndata4 %% 2 == 1)
      {
        plant.data <- plantdata()
        NdataResults4(plant.data,input$obs) 
      }
    })
    output$Ndata5 <- renderPlot({
      if (is.null(input$Ndata5)) {
        return()
      }
      if (input$Ndata5 %% 2 == 1)
      {
         plant.data <- plantdata()
         NdataResults5(plant.data,input$obs) 
      }
    })
    
    ######## water budgets #############
    output$Waterdata1 <- renderPlot({
      if (is.null(input$Waterdata1)) {
        return()
      }
      if (input$Waterdata1 %% 2 == 1)
      {
        water.data <- waterdata()
        WaterResults1(water.data,input$obs) 
      }
    })
    
    output$Waterdata2 <- renderPlot({
      if (is.null(input$Waterdata2)) {
        return()
      }
      if (input$Waterdata2 %% 2 == 1)
      {
        water.data <- waterdata()
        WaterResults2(water.data,input$obs) 
      }
    })
    
    output$Waterdata3 <- renderPlot({
      if (is.null(input$Waterdata3)) {
        return()
      }
      if (input$Waterdata3 %% 2 == 1)
      {
        water.data <- waterdata()
        WaterResults3(water.data,input$obs) 
      }
    })
    
    output$Waterdata4 <- renderPlot({
      if (is.null(input$Waterdata4)) {
        return()
      }
      if (input$Waterdata4 %% 2 == 1)
      {
        water.data <- waterdata()
        WaterResults4(water.data,input$obs) 
      }
    })
    
    output$Waterdata5 <- renderPlot({
      if (is.null(input$Waterdata5)) {
        return()
      }
      if (input$Waterdata5 %% 2 == 1)
      {
        water.data <- waterdata()
        WaterResults5(water.data,input$obs) 
      }
    })
    
    
    output$numInputui <- renderUI({
      CTool <- ctooldata() 
      CTool1 <- CTool[-c(1),] ##remove second header line
      CTool2 <- mutate(CTool1,year = as.numeric(startDay) / 365)
      Sequences <- select(CTool2,CropSeqID)
      Sequences <- as.numeric(as.character(Sequences$CropSeq))
      i=1
      NumSeq=0
      oldSeq=0
      SeqNumbers <- rep(0, times = 50)
      while (i <= length(Sequences))
      {
        if (Sequences[i]!=oldSeq)
        {
          NumSeq=NumSeq+1
          SeqNumbers[NumSeq]<-Sequences[i]
          oldSeq=Sequences[i]
        }
        i=i+1
      }
      
      numericInput("obs", "Select sequence:", 1,
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
    
    output$Waterdataui1 <- renderUI({
      if(input$Waterdata1 %% 2 == 1)
      {
        plotOutput("Waterdata1")
      }
      
    })
    
    output$Waterdataui2 <- renderUI({
      if(input$Waterdata2 %% 2 == 1)
      {
        plotOutput("Waterdata2")
      }
      
    })
    output$Waterdataui3 <- renderUI({
      if(input$Waterdata3 %% 2 == 1)
      {
        plotOutput("Waterdata3")
      }
      
    })

    output$Waterdataui4 <- renderUI({
      if(input$Waterdata4 %% 2 == 1)
      {
        plotOutput("Waterdata4")
      }
      
    })
    
    output$Waterdataui5 <- renderUI({
      if(input$Waterdata5 %% 2 == 1)
      {
        plotOutput("Waterdata5")
      }
      
    })
    
}