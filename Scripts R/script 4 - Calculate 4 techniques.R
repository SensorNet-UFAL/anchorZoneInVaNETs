########################Functions################################
list_veh_bet <- function(x) {
  test <- subset(temp, temp$az_id == x)
  cars <- unique(c(test$vert1,  test$vert2))
  return(sum(subset(between_T, names(between_T) %in% cars )) )
}

list_veh_deg <- function(x) {
  test <- subset(temp, temp$az_id == x)
  cars <- unique(c(test$vert1,  test$vert2))
  return(sum(subset(degree_T, names(degree_T) %in% cars )) )
}

list_maxaz_bet <- function(x) {
  test <- subset(temp, temp$az_id == x)
  cars <- unique(c(test$vert1,  test$vert2))
  bet <- round(max(subset(between_T, names(between_T) %in% cars )), digits = 2 )
  qtd <- round(subset(tot_qtd_hr, tot_qtd_hr$az_id == x)$qtde_veh, digits = 2  )
  return(bet * qtd[1])
}

list_maxaz_deg <- function(x) {
  test <- subset(temp, temp$az_id == x)
  cars <- unique(c(test$vert1,  test$vert2))
  deg <- round(max(subset(degree_T, names(degree_T) %in% cars )), digits = 2 )
  qtd <- round(subset(tot_qtd_hr, tot_qtd_hr$az_id == x)$qtde_veh, digits = 2  )
  return(deg * qtd[1])
}

####################################################################
library("igraph")

########################Calcular tot_ah_veh_T
technique1 <- data.frame("az_id" = 0, "qtde_veh" = 0, "size_az" = 0 )
technique1 <- technique1[-1,]

technique2 <- data.frame("az_id" = 0, "avg_between" = 0, "size_az" = 0 )
technique2 <- technique2[-1,]

technique3 <- data.frame("az_id" = 0, "hybrid" = 0, "size_az" = 0 )
technique3 <- technique3[-1,]

technique2 <- data.frame("az_id" = 0, "avg_degree" = 0, "size_az" = 0 )
technique2 <- technique2[-1,]

vehicles <- read.table(file = "~/simulatedTraffic/vehicles_TDrive.txt", header = TRUE, sep = ",")

for (jj in 1:length(sizes)){
  field <- paste("az_id",sizes[jj], sep = "")
  vehicles$az_id <- vehicles[,colnames(vehicles) == field]
  
  tot_az_veh_T <- aggregate(x=vehicles$vehicle_id, by=list(vehicles$instant, vehicles$az_id), FUN=length)
  tot_az_veh_T$instant <- tot_az_veh_T$Group.1
  tot_az_veh_T$az_id <- tot_az_veh_T$Group.2
  tot_az_veh_T$qtde_veh <- tot_az_veh_T$x
  tot_az_veh_T$Group.1 <- NULL
  tot_az_veh_T$Group.2 <- NULL
  tot_az_veh_T$x <- NULL
  
  #Delete instants that have only one vehicle
  tot_az_veh_T <- subset(tot_az_veh_T, tot_az_veh_T$qtde_veh > 1)
  
  #Insert field "hour"
  tot_az_veh_T$hour <- (tot_az_veh_T$instant  %/% 3600) + 1
  
  #Creating the tot_ah_veh_T - group by hour
  tot_ah_veh_T <- aggregate(cbind(qtde_veh) ~ az_id + hour, data = tot_az_veh_T, mean)
  
  tot_qtd_T <- aggregate(x=tot_ah_veh_T$qtde_veh, by=list(az_id=tot_ah_veh_T$az_id), FUN=mean)
  tot_qtd_T <- tot_qtd_T[order(tot_qtd_T$x,decreasing = TRUE),]
  tot_qtd_T$qtde_veh <- tot_qtd_T$x
  tot_qtd_T$x <- NULL  
  tot_qtd_T$size_az <- sizes[jj]

  
  ###############
  path <- paste("~/simulatedTraffic/az_rawdata_T",sizes[jj],".txt", sep = "")
  T_100m <- read.table(file=path, header = TRUE, sep = ",")
  T_100m$hour <- (T_100m$inst  %/% 3600) + 1
  
  tot_ah_bet_T <- data.frame("az_id" = 0, "avg_between" = 0, "hour" = 0)
  tot_ah_bet_T <- tot_ah_bet_T[-1,]
  
  tot_ah_hib_T <- data.frame("az_id" = 0, "hybrid" = 0, "hour" = 0)
  tot_ah_hib_T <- tot_ah_hib_T[-1,]
  
  tot_ah_deg_T <- data.frame("az_id" = 0, "avg_degree" = 0, "hour" = 0)
  tot_ah_deg_T <- tot_ah_deg_T[-1,]

  for (i in 1:5 ){
    records <- subset(T_100m, hour==i)
    tot_qtd_hr <- subset(tot_ah_veh_T, tot_ah_veh_T$hour == i)
	
    #Join all vertices have crossed and creating edge-eighted
    temp <- aggregate(x=records$inst, by=list(vert1=records$vert1, vert2=records$vert2,az_id=records$az_id ),FUN=length) 
    temp$edge <- temp$x
    temp$x <- NULL
    
    g_T <- graph_from_data_frame(temp[1:2], directed=FALSE, vertices=NULL)%>%
      set_edge_attr("az_id", value = temp$az_id)%>%
      set_edge_attr("weight", value = temp$edge)
    
	comps <- components(g_T)$membership
    colbar <- rainbow(max(comps)+1)
    V(g_T)$color <- colbar[comps+1]
    between_T <- betweenness(g_T, v = V(g_T), directed = FALSE, weights = E(g_T)$weight)
	
	degree_T <- degree(g_T, v = V(g_T))
    
	#Technique 2
    result <- aggregate(x=temp$az_id, by=list(az_id = temp$az_id), FUN=list_veh_bet)
    result$avg_between <- result$x
    result$hour <- i
    result$x <- NULL
    tot_ah_bet_T <- rbind(tot_ah_bet_T, result)	

    #Technique 3
    hybrid <- aggregate(x=temp$az_id, by=list(az_id = temp$az_id), FUN=list_maxaz_bet)
    hybrid$hybrid <- hybrid$x
    hybrid$hour <- i
    hybrid$x <- NULL
    tot_ah_hib_T <- rbind(tot_ah_hib_T, hybrid)
	
	#Technique 4
    result <- aggregate(x=temp$az_id, by=list(az_id = temp$az_id), FUN=list_veh_deg)
    result$avg_degree <- result$x
    result$hour <- i
    result$x <- NULL
    tot_ah_deg_T <- rbind(tot_ah_deg_T, result)
	
  }
  
  tot_bet_T <- aggregate(x=tot_ah_bet_T$avg_between, by=list(az_id=tot_ah_bet_T$az_id), FUN=mean)
  tot_bet_T <- tot_bet_T[order(tot_bet_T$x,decreasing = TRUE),]
  tot_bet_T$avg_between <- tot_bet_T$x
  tot_bet_T$x <- NULL  
  tot_bet_T$size_az <- sizes[jj]
  
  tot_hib_T <- aggregate(x=tot_ah_hib_T$hybrid, by=list(az_id=tot_ah_hib_T$az_id), FUN=mean)
  tot_hib_T <- tot_hib_T[order(tot_hib_T$x,decreasing = TRUE),]
  tot_hib_T$hybrid <- tot_hib_T$x
  tot_hib_T$x <- NULL  
  tot_hib_T$size_az <- sizes[jj]

  tot_deg_T <- aggregate(x=tot_ah_deg_T$avg_degree, by=list(az_id=tot_ah_deg_T$az_id), FUN=mean)
  tot_deg_T <- tot_deg_T[order(tot_deg_T$x,decreasing = TRUE),]
  tot_deg_T$avg_degree <- tot_deg_T$x
  tot_deg_T$x <- NULL  
  tot_deg_T$size_az <- sizes[jj]
  
  #Find the TOP10 of all techniques
  technique1 <- rbind(technique1, tot_qtd_T[1:10,])
  technique2 <- rbind(technique2, tot_bet_T[1:10,])
  technique3 <- rbind(technique3, tot_hib_T[1:10,])
  technique4 <- rbind(technique4, tot_deg_T[1:10,])  
  
  write.table(technique1, "~/simulatedTraffic/technique1.txt", sep = ",")
  write.table(technique2, "~/simulatedTraffic/technique2.txt", sep = ",")
  write.table(technique3, "~/simulatedTraffic/technique3.txt", sep = ",")
  write.table(technique4, "~/simulatedTraffic/technique4.txt", sep = ",")  
  
}