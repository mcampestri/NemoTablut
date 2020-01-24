# NemoTablut

### Build
```console
gcc ./NemoTablut/*.c -o Nemo -mcmodel=medium -O3 -pthread -w
```

<br/>

### Execution
```console
./Nemo <color> <seconds> <threads>
```

  - `color`: black/white
  - `seconds`: number of seconds given to compute the next move (default: 60 seconds)
  - `threads`: number of threads (default: 2)
