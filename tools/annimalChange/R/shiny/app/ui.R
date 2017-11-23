 library(tidyr)
 library(dplyr)
 library(ggplot2)
 library(readxl)
 library(reshape2)
 library(shiny)

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
    checkboxInput('Waterdata1', 'Crop sequence water inputs'),
    checkboxInput('Waterdata2', 'Crop sequence water status'),
    checkboxInput('Waterdata3', 'Crop sequence drought factors'),
    checkboxInput('Waterdata4', 'Crop sequence water outputs'),
    checkboxInput('Waterdata5', 'Crop sequence LAI'),
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
      uiOutput('Ndataui5'),
      uiOutput('Waterdataui1'),
      uiOutput('Waterdataui2'),
      uiOutput('Waterdataui3'),
      uiOutput('Waterdataui4'),
      uiOutput('Waterdataui5')
    )
)
