#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

library(shiny)

set.seed(123)
# Define UI for application that draws a histogram
ui <- fluidPage(

    # Application title
    titlePanel("Indexing Test"),

    # Sidebar with a slider input for number of bins 
    sidebarLayout(
        sidebarPanel(
            sliderInput("startage",
                        "start age:",
                        min = 0,
                        max = 10,
                        value = 1),
            sliderInput("endage",
                        "end age:",
                        min = 5,
                        max = 100,
                        value = 20),
            sliderInput("startyear",
                        "start year:",
                        min = 0,
                        max = 100,
                        value = 1),
            sliderInput("endyear",
                        "end year:",
                        min = 20,
                        max = 100,
                        value = 20),
            textInput("season1length",
                      "season 1 length:",
                      value = 0.25),
            textInput("season2length",
                        "season 2 length:",
                        value = 0.25),                 
            textInput("season3length",
                        "season 3 length:",
                        value = 0.25),         
            textInput("season4length",
                        "season 4 length:",
                        value = 0.25)  
        ),

        # Show a plot of the generated distribution
        mainPanel(
           plotOutput("ages")
        )
    )
)

# Define server logic required to draw a histogram
server <- function(input, output) {

    output$ages <- renderPlot({
        # generate bins based on input$bins from ui.R
        ages <- input$startage:input$endage
        # generate data based on input$data from ui.R
        years <- input$startyear:input$endyear
        season_duration <- as.numeric(rep(c(input$season1length,
         input$season2length, input$season3length,
         input$season4length),length(years)))
        print(season_duration)
        population_ages <- rep(0, length(season_duration))

        for(i in seq_len(length(season_duration)*length(years))){
            print(i)
            if(i ==1 ){
                population_ages[i] <- input$startage
            }else{
                population_ages[i] <- population_ages[i-1] + season_duration[i-1]
            }
            if(population_ages[i] > input$endage){
                break
            }
            #if(i%4 == 0){
            #}
        }

        # draw the plot with population ages
     plot(population_ages, col = 'darkgray', ylab= "age", ylim=c(0,input$endage))
    })
}

# Run the application 
shinyApp(ui = ui, server = server)
