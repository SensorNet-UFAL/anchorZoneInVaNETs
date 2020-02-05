tot_vehicles = read.csv('tot_vehicles.csv',header = TRUE, sep = ',' )
scatterPlot <- ggplot(tot_vehicles,aes(longitude, latitude )) + 
  geom_point() + 
  labs(x="Longitude", y="Latitude") +
  scale_color_manual(values = c('#999999','#E69F00')) + 
  theme(legend.position=c(0,1), legend.justification=c(0,1), text = element_text(size=14)   )
scatterPlot
