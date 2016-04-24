import sys

try:
	#Constantes
	sobrecosto_temp_media=0.10
	sobrecosto_temp_alta=0.30

	#Sanity check
	if (len(sys.argv)<4):
		print "Ejecutar con la siguiente linea: ./validador <ruta_matriz> <ruta_temporadas> <ruta_solucion>"
		sys.exit(1)

	ruta_instancia=sys.argv[1]
	ruta_temporadas=sys.argv[2]
	ruta_solucion=sys.argv[3]

	matriz=[]

	archivo_instancia=open(ruta_instancia)
	for line in archivo_instancia.readlines():
		matriz.append([int(token) for token in line.strip().split(" ")])

	archivo_solucion=open(ruta_solucion)
	orden_solucion=[int(a) for a in archivo_solucion.readlines()[0].strip().split(" ")]

	archivo_temporadas=open(ruta_temporadas)
	lines=archivo_temporadas.readlines()
	fin_temp_baja=int(lines[0].strip().split(",")[1])
	fin_temp_media=int(lines[1].strip().split(",")[1])

	#Sanity check
	if (len(orden_solucion)!=(len(matriz[0]))):
		print "El largo de la solucion no se corresponde con el de la instancia"
		sys.exit(1)
	if (orden_solucion[0]!=0):
		print "El origen debe ser la ciudad 0"
		sys.exit(1)
	if (set(orden_solucion)!=set(range(0,len(matriz[0])))):
		print "La solucion no es completa, verificar"
		sys.exit(1)

	costo_total=0
	dia=1
	for i in range(1,len(orden_solucion)):
		print "%d-->%d"%(orden_solucion[i-1],orden_solucion[i])
		valor=matriz[orden_solucion[i-1]][orden_solucion[i]]
		if (valor==-1):
			print "Error: las ciudades %d y %d no estan conectadas"%(orden_solucion[i-1],orden_solucion[i])
			sys.exit(1)
		else:
			if (dia<fin_temp_baja):
				print "%d + 0%% = %d"%(valor,valor)
				costo_total+=valor
			else:
				if (dia<fin_temp_media):
					valor_ajustado=valor+(int(round(valor*sobrecosto_temp_media)))
					print "%d + %d%% = %d"%(valor,int(sobrecosto_temp_media*100), valor_ajustado)
					costo_total+=valor_ajustado
				else:
					valor_ajustado=valor+(int(round(valor*sobrecosto_temp_alta)))
					print "%d + %d%% = %d"%(valor,int(sobrecosto_temp_alta*100), valor_ajustado)
					costo_total+=valor_ajustado
			dia+=5
	print "--------"
	print "COSTO_TOTAL: %d"%costo_total
except IOError as error:
	print error


