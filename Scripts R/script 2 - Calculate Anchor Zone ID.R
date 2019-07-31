#Insert ID Anchor Zone with 200m, 400, 600m, 800m and 1000m of Size 

vehicles <- read.table(file = "~/simulatedTraffic/vehicles_TDrive.txt", header = TRUE, sep = ",")

sizes <- c("200","400","600","800","1000")

vehicles$hour <- (vehicles$instant  %/% 3600) + 1
for(i in seq(2, 10, 2) ){
  latitudeFactor = (0.0008983153 * i)
  longitudeFactor = (0.0011608537 * i)
  vehicles$line   <- ((vehicles$latitude - 39.3) %/% latitudeFactor)
  vehicles$column <- ((vehicles$longitude - 115.0093 ) %/% longitudeFactor)
  field <- paste("az_id",sizes[i / 2], sep = "")
  if (i == 2){
    vehicles$az_id <- (vehicles$line * 10000) +vehicles$column
  }
  else{
    vehicles$az_id <- (vehicles$line * 1000) +vehicles$column  
  }
  colnames(vehicles)[colnames(vehicles)=="az_id"] <- field
}
vehicles$line <- NULL
vehicles$column <- NULL
write.table(vehicles, file = "~/simulatedTraffic/vehicles_TDrive.txt", sep = ",")
  