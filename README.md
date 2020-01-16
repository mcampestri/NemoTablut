# NemoTablut

## Build ##
`gcc ./NemoTablut/*.c -o Nemo -mcmodel=medium -O3 -pthread -w`

<br/>

## Execution
`./Nemo <color> <seconds> <threads>`

  - `<color>`: black/white
  - `<seconds>`: number of seconds granted to compute the next move (default: 60 seconds)
  - `<threads>`: number of threads (default: 2)
