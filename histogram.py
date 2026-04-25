import pandas as pd #to read the csv files with the distances
import matplotlib.pyplot as plt #to create the histograms

dimensiones = [10, 50, 100, 500, 1000, 2000, 5000] #list of dimensions to analyze

for d in dimensiones: #for each dimension, read the distances from the csv file and create a histogram
    filename = f"distancias_d{d}.csv" #name of the file to read the distances
    distances = pd.read_csv(filename, header=None)[0] #read the distances from the csv file and this distances are in the first column of the csv file, so we select the first column with [0]
    
    plt.figure(figsize=(8, 6)) #set the size of the figure
    plt.hist(distances, bins=50, edgecolor='white', color='#607C8E') #create the histogram
    plt.xlabel('Distancia')
    plt.ylabel('Frecuencia')
    plt.title(f'Distancia entre puntos con dimensión {d}')
    plt.xlim(0, max(distances) + 1)
    plt.grid(True, linestyle='-') #add a grid to the histogram
    
    plt.savefig(f'histograma_d{d}.png', dpi=300, bbox_inches='tight') #save the histogram as a png file with a name that indicates the dimension
    plt.close()

