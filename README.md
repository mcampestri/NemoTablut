# NemoTablut

Compilazione:\n
cd NemoTablut-master\n
gcc ./NemoTablut/*.c -o Nemo -mcmodel=medium -O3 -pthread -w\n

Esecuzione:\n
./Nemo <colore>\n
Accetta altri due argomenti per l'esecuzione, nell'ordine:\n
  - Numero di secondi del timer (default: 60 secondi)\n
  - Numero di thread (default: 2)\n
