<CsoundSynthesizer>
<CsOptions>
-odac ; Sortie audio vers vos haut-parleurs
</CsOptions>
<CsInstruments>
sr = 44100        ; Taux d'échantillonnage
ksmps = 32        ; Bloc de contrôle
nchnls = 2        ; Sortie stéréo
0dbfs = 1         ; Amplitude maximale

giwave ftgen 1, 0, 4096, 7, 1, 4096, -1 ;
; Instrument pour une mélodie avec delay
instr 1
    ; Fréquence et durée de la note
    ifreq = cpspch(p4)        ; Convertit la hauteur en fréquence
    idur = p3                 ; Durée de la note
    iamp = 0.9                ; Amplitude du signal

    ; Générer le signal source (onde sinusoïdale)
    asig oscil iamp, ifreq, 1

    ; Application d'une enveloppe ADSR
    aenv linsegr 0, p3/2, 1, idur/2 - 0.02, 1, 0.01, 0
    asig = asig * aenv         ; Appliquer l'enveloppe au signal

    ; Paramètres du delay
    idelayTime = 0.1          ; Temps de delay (en secondes)
    ifeedback = 0.9            ; Niveau de rétroaction (0-1), plus élevé pour plus d'écho
    iwet = 5.0                 ; Niveau du signal traité (wet)
    idry = 0.5                 ; Niveau du signal direct (dry)

    ; Application du delay
    adel delayr idelayTime     ; Initialiser le buffer de delay
    delayw asig + (adel * ifeedback) ; Ajouter le signal avec rétroaction
    asigOut = (adel * iwet) + (asig * idry) ; Mélanger wet et dry

    ; Sortie stéréo
    outs asigOut, asigOut
endin
</CsInstruments>

<CsScore>
; Définition de la table d'onde pour l'oscillateur
;f 1 0 1024 10 1 ; Onde sinusoïdale
;f 1 0 16384 10 1 0 0.333 0 0.2 0 0.142 0 0.111 ; Onde carrée

; Mélodie : Chaque ligne correspond à une note
; Format : i <instr> <début> <durée> <hauteur (octave.pas)>

i 1 0    2 8.07 ; Cmaj/5
i 1 0    2 9.04 ; 
i 1 0    2 10.00 ; 

i 1 2  2 8.07 ; Cdim
i 1 2  2 9.04 
i 1 2  2 10.01

i 1 4    2 8.09 ; Dmin
i 1 4    2 9.05
i 1 4    2 10.02 

i 1 6    2 8.09 ; Ebdim
i 1 6    2 9.05
i 1 6    2 10.03

i 1 8    2 8.10
i 1 8    2 9.07
i 1 8    2 10.05
e
</CsScore>
</CsoundSynthesizer>
