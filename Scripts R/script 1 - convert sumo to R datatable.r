#Xtract the result of Sumo and convert to data.table R

xtract <- function(x,y) {
  cbind(instante = as.numeric(mylist[x]$timestep$.attrs["time"]),
        vehicle_id = mylist[x]$timestep[y]$vehicle["id"],
        latitude = as.numeric(mylist[x]$timestep[y]$vehicle["y"]),
        longitute = as.numeric(mylist[x]$timestep[y]$vehicle["x"]),
        speed = as.numeric(mylist[x]$timestep[y]$vehicle["speed"]))
}


library("XML"); 
library("data.table")
xml_data<- xmlParse("~/veins/examples/tdrive/t-drive.sumo.xml")
mylist <- xmlToList(xml_data)
nr_intervals <- length(unlist(xml_data["//timestep"]))
vehicles <- xtract(100,1)
for (i in 1:nr_intervals){
  nr_vehicles <- xmlSize(mylist[i]$timestep) - 1
  if (nr_vehicles > 0){
    instante<-numeric(0); vehicle_id<-numeric(0); latitude<-numeric(0); longitute<-numeric(0); speed<-numeric(0)
    for (j in 1:nr_vehicles){
      temp <- xtract(i,j)   
      instante[j] <- temp[1]
      vehicle_id[j] <- temp[2]
      latitude[j] <- temp[3]   
      longitute[j] <- temp[4] 
      speed[j] <- temp[5]
    }
    veh <- cbind(instante, vehicle_id, latitude, longitute, speed)
    vehicles <- rbind(vehicles, veh)
  }
}

vehicles <- vehicles[-1,]

write.table(vehicles, file = "~/simulatedTraffic/vehicles_TDrive.txt", sep = ",") 
