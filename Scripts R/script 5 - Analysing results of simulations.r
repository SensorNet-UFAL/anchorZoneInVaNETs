require('omnetpp')

techniques <- c("Flow", "Betweenness", "Degree", "Hybrid")
sizes <- c("200m","400m","600m","800m","1000m")
tdrive <- data.frame("index"=0, "technique"=0, "az_size"=0,  "az_id"=0, "qtd_veh"=0, "time"=0)
tdrive <- tdrive[-1,]
#####################################################33
  
time <- 0
timeFcCreated <- 5

dataSets <- loadDataset(c("General-#0.sca", "General-#0.vec"))
scalar <- dataSets$scalars
vector <- dataSets$vectors
vectorData <- loadVectors(dataSets, NULL)

anchorZone <- vectorData$vectors[vectorData$vectors$name == "anchorZone",]
floatContent <- vectorData$vectors[vectorData$vectors$name == "floatContent",]

if (nrow(anchorZone) > 0){
  AZ <- data.frame("resultkey"=0, "eventno"=0, "x"=0, "y"=0); AZ <- AZ[-1,]
  for (k in 1:length(anchorZone[,1])){
    temp <- vectorData$vectordata[vectorData$vectordata$resultkey==anchorZone[k,1],]
    AZ <- rbind(AZ, temp)
  }
  
  FC <- data.frame("resultkey"=0, "eventno"=0, "x"=0, "y"=0); FC <- FC[-1,]
  for (k in 1:length(floatContent[,1])){
    temp <- vectorData$vectordata[vectorData$vectordata$resultkey==floatContent[k,1],]
    FC <- rbind(FC, temp)
  }
  
  for (q in 1:timeFcCreated){
    FCtemp <- subset(FC, FC$x >= ((18000 / timeFcCreated)* (i-1) ) & FC$x < ((18000 / timeFcCreated)*i) )
    min <- aggregate(x=FCtemp$x, by=list(fc=FCtemp$y ),FUN= min)
    max <- aggregate(x=FCtemp$x, by=list(fc=FCtemp$y ),FUN= max)
    temp2 <- min
    temp2$time <- trunc(max$x) - trunc(min$x)
     
    temp <- aggregate(x=AZ$x, by=list(az_id=AZ$y ),FUN=length)
    temp$time <- temp2$time
    colnames(temp)[2] <- "qtd_veh"
    temp$index <- i*j
    temp$technique <- techniques[i]
    temp$az_size <- sizes[j]      
    nome <- paste("~/", techniques[i],"-", sizes[j],".txt",sep = ""  )
	
    tdrive <- rbind(tdrive, temp)      
  }
}