print (getwd())
tempdir <- function() ""
unlockBinding("tempdir", baseenv())
assignInNamespace("tempdir", tempdir, ns="base", envir=baseenv())
assign("tempdir", tempdir, baseenv())
lockBinding("tempdir", baseenv())
print (getwd())
shiny::runApp('app',launch.browser=TRUE)