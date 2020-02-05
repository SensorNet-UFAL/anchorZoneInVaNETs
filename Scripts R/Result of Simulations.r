mobile <- read.table("ResultadoFinalMobile.txt", header=TRUE, sep=",")

tdrive <- read.table("ResultadoFinalTDrive.txt", header=TRUE, sep=",")

#Mobile Century - Time
p <- ggplot(mobile, aes(x=mobile$Tamanho, y=mobile$Tempo, color=mobile$Tecnica)) +
     geom_boxplot() +
     ggtitle("I-880 Freeway: Time alive of Floating Content per Anchor Zones's size") +
     labs(x = "Size of Anchor Zone", y = "Time of alive of Anchor Zone", color = "Strategy")  +
     theme_minimal(base_size = 15)
 
p

#Mobile Century - Quantity of Vehicles
p <- ggplot(mobile, aes(x=mobile$Tamanho, y=mobile$qtd_veh, color=mobile$Tecnica)) +
     geom_boxplot() +
     ggtitle("I-880 Freeway: Quantity of vehicles per Anchor Zones's size") + 
     labs(x = "Size of Anchor Zone", y = "Quantity of Vehicles", color = "Strategy")  +
     theme_minimal(base_size = 15)
 
p

#T-Drive - Time
p <- ggplot(tdrive, aes(x=tdrive$Tamanho, y=tdrive$Tempo, color=tdrive$Tecnica)) +
     geom_boxplot() +
     ggtitle("Beijing: Time alive of Floating Content per Anchor Zones's size") +
     labs(x = "Size of Anchor Zone", y = "Time of alive of Anchor Zone", color = "Strategy")  +
     theme_minimal(base_size = 15)

p

#T-Drive - Quantity of vehicles
p <- ggplot(tdrive, aes(x=tdrive$Tamanho, y=tdrive$qtd_veh, color=tdrive$Tecnica)) +
     geom_boxplot() +
     ggtitle("Beijing: Quantity of vehicles per Anchor Zones's size") + 
     labs(x = "Size of Anchor Zone", y = "Quantity of Vehicles", color = "Strategy")  +
     theme_minimal(base_size = 15)

p