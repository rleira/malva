import sys
import random
import math

try:
	#Constantes
	menor_costo=50

	#Sanity check
	if (len(sys.argv)<4):
		print "Ejecutar con la siguiente linea: ./generador <cant_ciudades> <ratio_no_conectados> <archivo_salida>"
		sys.exit(1)

	cantidad_ciudades=int(sys.argv[1])
	ratio_no_conectadas=float(sys.argv[2])
	archivo_salida=sys.argv[3]

	if (cantidad_ciudades<4):
		print "La cantidad de ciudades debe ser un entero mayor que 4"
		sys.exit(1)

	if (ratio_no_conectadas<0 or ratio_no_conectadas>1):
		print "La tasa de ciudades no conectadas debe ser un real entre 0 y 1."
		sys.exit(1)

	#Sorteo las posiciones de las ciudades en (0,100),(0,100)
	ciudades=[]
	for i in range(0,cantidad_ciudades):
		pos_x=random.randint(0,100)
		pos_y=random.randint(0,100)
		ciudades.append((pos_x,pos_y))


	#Generar la matriz
	matriz=[[0 for m in range(0,cantidad_ciudades)] for n in range(0,cantidad_ciudades)]

	for i in range(0,cantidad_ciudades):
		for j in range(0,cantidad_ciudades):
			if (i==j):
				matriz[i][j]=0
			else:
				distancia=math.sqrt(math.pow((ciudades[i][0]-ciudades[j][0]),2)+math.pow((ciudades[i][1]-ciudades[j][1]),2))
				costo=menor_costo+ int(distancia*random.randint(10,12))
				matriz[i][j]=costo

	#Desconecto ciudades
	cantidad_desconectadas=[0 for m in range(0,cantidad_ciudades)]
	for i in range(0,cantidad_ciudades):
		for j in range(0,cantidad_ciudades):
			if (matriz[i][j]>0):
				if (cantidad_desconectadas[i]<(cantidad_ciudades-3)):
					if (cantidad_desconectadas[j]<(cantidad_ciudades-3)):
						if (random.uniform(0.0,1.0)<ratio_no_conectadas): 
							matriz[i][j]=-1
							matriz[j][i]=-1
							cantidad_desconectadas[i]+=1
							cantidad_desconectadas[j]+=1


	#Sorteo temporadas
	lim1=random.randint(1,cantidad_ciudades-2)
	lim2=random.randint(1,cantidad_ciudades-2)
	while (lim1==lim2):
		lim2=random.randint(2,cantidad_ciudades-2)

	#Escribo limite a archivo de salida
	archivo = open ("%s_temporadas"%archivo_salida, "w")
	archivo.write("1,%d\n"%(5*min(lim1,lim2)-1))
	archivo.write("%d,%d\n"%((5*min(lim1,lim2)),(5*max(lim1,lim2)-1)))
	archivo.write("%d,%d"%((5*max(lim1,lim2)),(5*(cantidad_ciudades-1))))

	#Escribo a archivo de salida
	archivo = open ("%s_matriz"%archivo_salida, "w")
	for i in range(0,cantidad_ciudades):
		if (i!=0):
			archivo.write("\n")
		for j in range (0, cantidad_ciudades):
			if (j!=0):
				archivo.write(" ")
			archivo.write("%d"%matriz[i][j])
except IOError as error:
	print error
