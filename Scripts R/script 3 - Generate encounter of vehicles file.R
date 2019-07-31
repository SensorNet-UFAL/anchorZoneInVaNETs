#Generate encounter of vehicles by sizes of AZs

list_veh <- function(x) {
  temp <- NULL
  for (i in 1:length(x)){
    temp[i] <- x[i]
  }
  temp <- unique(temp)
  temp <- temp[order(temp)]
  return(unique(temp))
}    

sizes <- c("200","400","600","800","1000")

vehicles <- read.table(file = "~/simulatedTraffic/vehicles_TDrive.txt", header = TRUE, sep = ",")

for (j in 1:length(sizes)){
  field <- paste("az_id",sizes[j], sep = "")
  vehicles$az_id <- vehicles[,colnames(vehicles) == field]
  
  tot_vehicles <- aggregate(cbind(vehicle_id) ~ instant + az_id, data = vehicles,FUN=list_veh )
  tot_vehicles <- tot_vehicles[order(tot_vehicles$instant, decreasing = FALSE),]
  
  az_rawdata <- data.frame("instant" = 0, "vert1" = 0, "vert2" = 0, "az_id" = 0)
  az_rawdata <- az_rawdata[-1,]
  
  vehicle_id <- tot_vehicles$vehicle_id
  az_ids <- tot_vehicles$az_id
  instant <- tot_vehicles$instant
  temp <- NULL
  for (ii in 1:length(az_ids)){
    list_vehicle <- vehicle_id[ii][[1]]
    mm <- 1; vert1 <- numeric(0); vert2 <- numeric(0);  vehi <- NULL
    
    if (length(list_vehicle) > 1){
      for (jj in 1:(length(list_vehicle) - 1)){
        
        for (kk in (jj+1):length(list_vehicle) ){
          vert1[mm] <- list_vehicle[jj]
          vert2[mm] <- list_vehicle[kk]
          mm <- mm + 1
        }
      }
      az_id <- az_ids[ii]
      inst <- instant[ii]
      vehi <- cbind(inst, vert1, vert2, az_id)
      temp <- rbind(temp, vehi)      
    }
  }
  az_rawdata <- data.frame(temp)
  file <- paste("az_rawdata_T",sizes[j],".txt", sep = "")
  azResult <- paste("~/simulatedTraffic", file, sep = "/")
  write.table(az_rawdata, azResult, sep = ",")
}#for (j in 1:length(sizes))



