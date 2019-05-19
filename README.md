# NemoTablut

Compilazione:

cd NemoTablut-master

gcc ./NemoTablut/*.c -o Nemo -mcmodel=medium -O3 -pthread -w



Esecuzione:

./Nemo colore

Accetta altri due argomenti per l'esecuzione, nell'ordine:

  - Numero di secondi del timer (default: 60 secondi)
  
  - Numero di thread (default: 2)
